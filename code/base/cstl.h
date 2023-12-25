#ifndef CSTL_H
#define CSTL_H

/**
 @file
 Generic vector and hash maps for C. Requires C99 or newer, works best with C11.

 By default, including "cstl" defines the c_lib functions as `static` (that is, private to the
 .c file that included it).

 ## Options - defined before #include "cstl"

 | Option                        | Description                                                     |
 |-------------------------------|-----------------------------------------------------------------|
 | #define CSTL_DEFINE           | Define the functions publicly, so they are available when       |
 |                               | linking.                                                        |
 |-------------------------------|-----------------------------------------------------------------|
 | #define CSTL_DECLARE          | Declare function prototypes without defining the functions      |
 |                               | themselves (that is, use existing public `c_lib` functions).    |
 |-------------------------------|-----------------------------------------------------------------|

 */

#ifndef __STDC_VERSION__
//ISO released a corrigendum yesterday 2012-07-15 Cor 1:2012 for C11: value __STDC_VERSION__ is now fixed to 201112L
#define __STDC_VERSION__ 201112L
#endif

/*
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h> // free, qsort
#include <string.h> // strcmp, memset, memcpy
*/

// @cond configuration
#if defined(CSTL_DEFINE) || defined(CSTL_DECLARE)
#  ifdef __cplusplus
#    define CSTL_API extern "C"
#  else
#    define CSTL_API
#  endif
#else
#  define CSTL_API static
#  define CSTL_DEFINE
#endif
#ifdef __cplusplus
#  define CSTL_MUTABLE mutable
#else
#  define CSTL_MUTABLE
#endif
// @endcond

// MARK: Static assertion

/**
 Static assertion, using the built-in _Static_assert if available.

 @param condition The constant expression to check. If 0, a compile-time error occurs.
 @param msg       The message to display if compilation fails. Only works on C11 or C++11.
 */
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) || (defined(_MSC_VER) && _MSC_VER >= 1800)
#  define c_static_assert _Static_assert
#elif defined(__cplusplus) && (__cplusplus >= 201103L)
#  define c_static_assert static_assert
#else
#  define c_static_assert(condition, msg) ((void) sizeof(char[(condition) ? 1 : -1]))
#endif

/**
 Check if two variables have the same type.
 */
#if defined(__GNUC__)
#  define c_types_compatible(A, B) __builtin_types_compatible_p(__typeof(A), __typeof(B))
#else
#  define c_types_compatible(A, B) (sizeof(A) == sizeof(B))
#endif

// MARK: Vector

/**
 Declares a generic `c_vec` struct or typedef.
 
 For example, and array of `int`s can be declared as a typedef:

     typedef struct c_vec_of(int) vec_int_t;

 or a struct:

     struct vec_int_s c_vec_of(int);

 @tparam value_type The type to contain in the vector.

 @return `{ value_type *values; size_t count; size_t capacity; }`
 */
#define c_vec_of(value_type) \
    { value_type *values; size_t count; size_t capacity; }

/**
 A macro to initialize a vector statically.

     typedef struct c_vec_of(int) vec_int_t;
     vec_int_t int_vec = CSTL_VEC_INIT;

 When finished using the vector, the #c_vec_deinit() function must be called.
 */
#define CSTL_VEC_INIT { NULL, 0, 0 }

/**
 Inits a vector.
 
 When finished using the vector, the #c_vec_deinit() function must be called.

 @param vec Pointer to the vector.
 */
#define c_vec_init(vec) \
    memset((vec), 0, sizeof(*(vec)))

/**
 Deinits the vector. The vector may be used again by calling #c_vec_init().

 @param vec Pointer to the vector.
 */
#define c_vec_deinit(vec) \
    free((void *)(vec)->values)

/**
 Removes all elements from the vector, setting the count to 0. The capacity of the vector is not
 changed.

 @param vec Pointer to the vector.
 */
#define c_vec_clear(vec) \
    (vec)->count = 0

/**
 Gets the number of elements in the vector.

 @param vec Pointer to the vector.
 @return size_t The number of elements in the vector.
 */
#define c_vec_count(vec) \
    (vec)->count

/**
 Gets a pointer to the first element in a vector.

 @param vec Pointer to the vector.
 @return A pointer to the first element. If the vector is empty, the result is undefined.
 */
#define c_vec_begin(vec) \
    ((vec)->values)

/**
 Gets a pointer to the element following the last element in a vector.
 
 The pointer should only be used for iteration, and should not be accessed directly.

 @param vec Pointer to the vector.
 @return A pointer to the element following the last element.
 */
#define c_vec_end(vec) \
    ((vec)->values + (vec)->count)

/**
 Gets a pointer to the last element in a vector.

 @param vec Pointer to the vector.
 @return A pointer to the last element. If the vector is empty, the result is undefined.
 */
#define c_vec_last(vec) \
    ((vec)->values + ((vec)->count - 1))

/**
 Gets an element in a vector.

 @param vec Pointer to the vector.
 @param i The index in the vector, from 0 to `(c_vec_count(vec) - 1)`. If the index is outside of
 the valid range, the result is undefined, and this function may crash.
 @return The element at the index.
 */
#define c_vec_get(vec, i) \
    (*((vec)->values + (i)))

/**
 Gets a pointer to an element in a vector.

 @param vec Pointer to the vector.
 @param i The index in the vector, from 0 to `(c_vec_count(vec) - 1)`. If the index is outside of
 the valid range, the result is undefined.
 @return A pointer to the element at the index.
 */
#define c_vec_get_ptr(vec, i) \
    ((vec)->values + (i))

/**
 Adds a value to the end of a vector.

 @param vec Pointer to the vector.
 @param value The value to add.
 @return `true` if the value was successfully added to the vector, `false` otherwise (out of memory
 error).
 */
#define c_vec_push(vec, value) \
    (c_vec_ensure_capacity(vec, 1) ? ((vec)->values[((vec)->count++)] = (value), 1) : 0)

/**
 Adds space for a value at the end of a vector, returning a pointer to the newly added location.

 @param vec Pointer to the vector.
 @return The pointer to the new location, or `NULL` if failure (out of memory error).
 */
#define c_vec_push_new(vec) \
    (c_vec_ensure_capacity(vec, 1) ? ((vec)->values + ((vec)->count++)) : NULL)

/**
 Adds all the elements from one vector into another.

 @param vec Pointer to the vector to add elements to.
 @param vec2 Pointer to a vector to get elements from.
 @return `true` if the values in `vec2` were successfully added to the vector, `false` otherwise 
 (out of memory error).
 */
#define c_vec_push_all(vec, vec2) \
    (c_vec_ensure_capacity(vec, (vec2)->count) ? \
    (memcpy((vec)->values + (vec)->count, (vec2)->values, sizeof(*(vec)->values) * (vec2)->count), \
    ((vec)->count += (vec2)->count)) : 0)

/**
 Inserts a value at the specified location in the vector. The element currently at that
 location, and all subsequent elements, are moved to the right by one position.

 @param vec Pointer to the vector.
 @param index `size_t` The index at which to insert the element. If the index is greater than or
 equal to number of elements in the vector, the value is added to the end, like #c_vec_push().
 @param value The value to insert.
 */

/*
FIXED BUG BY D00M4ACE! NOT COPY LAST ELEMENT!
ORIGINAL CODE:
#define c_vec_insert_at(vec, index, value) \
    do { \
        if (c_vec_ensure_capacity(vec, 1)) { \
            size_t _i1 = (index); \
            if (_i1 + 1 < (vec)->count) { \
                memmove((vec)->values + _i1 + 1, (vec)->values + _i1, \
                        ((vec)->count - _i1 - 1) * sizeof(*(vec)->values)); \
            } \
            (vec)->values[_i1] = (value); \
            (vec)->count++; \
        } \
    } while (0)
*/

#define c_vec_insert_at(vec, index, value) \
    do { \
        if (c_vec_ensure_capacity(vec, 1)) { \
            size_t _i1 = (index); \
            if (_i1 < (vec)->count) { \
                memmove((vec)->values + _i1 + 1, (vec)->values + _i1, \
                        ((vec)->count - _i1) * sizeof(*(vec)->values)); \
            } \
            (vec)->values[_i1] = (value); \
            (vec)->count++; \
        } \
    } while (0)

/**
 Removes an element at the specified location in the vector.

 @param vec Pointer to the vector.
 @param index `size_t` The index of the element to remove. If the index is greater than or
 equal to number of elements in the vector, the size of the vector is reduced by one.
 */
#define c_vec_remove_at(vec, index) \
    do { \
        size_t _i2 = (index); \
        if (_i2 + 1 < (vec)->count) { \
            memmove((vec)->values + _i2, (vec)->values + _i2 + 1, \
                    ((vec)->count - _i2 - 1) * sizeof(*(vec)->values)); \
        } \
        if ((vec)->count > 0) { \
            (vec)->count--; \
        } \
    } while (0)

/**
 Removes the first element in the vector that equals the specified value.

 This function uses the equality operator `==` to test for equal values, which will fail to 
 compile if values are structs.

 @param vec Pointer to the vector.
 @param value The value to find and remove.
 */
#define c_vec_remove(vec, value) \
    do { \
        for (size_t _i3 = 0; _i3 < (vec)->count; _i3++) { \
            if ((vec)->values[_i3] == (value)) { \
                c_vec_remove_at(vec, _i3); \
                break; \
            } \
        } \
    } while (0)

/**
 Foreach macro that iterates over the values in the vector.

 The vector should not be modified during iteration. A call to #c_vec_push()
 during iteration could cause as crash.

 The `break` and `continue` keywords are supported during iteration.

 Example:

 c_vec_foreach(vec, char *value) {
     printf("Value: %s\n", value);
 }

 @param vec   Pointer to the vector.
 @param var   The value type and name.
 */
#define c_vec_foreach(vec, var) \
    for (size_t _keep = 1, _i = 0, _len = (vec)->count; _keep && _i < _len; _keep = 1 - _keep, _i++) \
    for (var = *((vec)->values + _i); _keep; _keep = 1 - _keep)

/**
 Foreach macro that iterates over the values in the vector, in reverse order.

 The vector should not be modified during iteration. A call to #c_vec_push()
 during iteration could cause as crash.

 The `break` and `continue` keywords are supported during iteration.

 Example:

 c_vec_foreach_rev(vec, char *value) {
     printf("Value: %s\n", value);
 }

 @param vec   Pointer to the vector.
 @param var   The value type and name.
 */
#define c_vec_foreach_rev(vec, var) \
    for (size_t _keep = 1, _i = 0, _len = (vec)->count; _keep && _i < _len; _keep = 1 - _keep, _i++) \
    for (var = *((vec)->values + (_len - _i - 1)); _keep; _keep = 1 - _keep)

/**
 Foreach macro that iterates over pointers to the values in the vector.

 The vector should not be modified during iteration. A call to #c_vec_push()
 during iteration could cause as crash.

 The `break` and `continue` keywords are supported during iteration.

 Example:

 c_vec_foreach_ptr(vec, char **value) {
     printf("Value: %s\n", *value);
 }

 @param vec   Pointer to the vector.
 @param var   The value type and name.
 */
#define c_vec_foreach_ptr(vec, var) \
    for (size_t _keep = 1, _i = 0, _len = (vec)->count; _keep && _i < _len; _keep = 1 - _keep, _i++) \
    for (var = (vec)->values + _i; _keep; _keep = 1 - _keep)

/**
 Foreach macro that iterates over pointers to the values in the vector.

 The vector should not be modified during iteration. A call to #c_vec_push()
 during iteration could cause as crash.

 The `break` and `continue` keywords are supported during iteration.

 Example:

 c_vec_foreach_ptr_rev(vec, char **value) {
     printf("Value: %s\n", *value);
 }

 @param vec   Pointer to the vector.
 @param var   The value type and name.
 */
#define c_vec_foreach_ptr_rev(vec, var) \
    for (size_t _keep = 1, _i = 0, _len = (vec)->count; _keep && _i < _len; _keep = 1 - _keep, _i++) \
    for (var = (vec)->values + (_len - _i - 1); _keep; _keep = 1 - _keep)

/**
 Applies a function to each element in a vector.

 @param vec Pointer to the vector.
 @param func The function.
 */
#define c_vec_apply(vec, func) \
    for (size_t _i = 0, _len = (vec)->count; _i < _len; _i++) func(*((vec)->values + _i))

/**
 Applies a function to a pointer to each element in a vector.

 @param vec Pointer to the vector.
 @param func The function.
 */
#define c_vec_apply_ptr(vec, func) \
    for (size_t _i = 0, _len = (vec)->count; _i < _len; _i++) func(((vec)->values + _i))

/**
 Sorts elements in a vector

 @param vec Pointer to the vector.
 @param compare_func Pointer to a function that compares two elements. Uses the same syntax as
 `qsort`.
 */
#define c_vec_sort(vec, compare_func) \
    qsort((void *)(vec)->values, (vec)->count, sizeof(*((vec)->values)), compare_func)

/**
 Ensures that a vector has enough space for additional elements.

 @param vec Pointer to the vector.
 @param additional_count The number of addtional elements to make room for.
 @return `true` if successful, `false` otherwise (out of memory error).
 */
#define c_vec_ensure_capacity(vec, additional_count) \
    (((vec)->count + (size_t)(additional_count) <= (vec)->capacity) ? true : \
    _c_vec_realloc((void **)&(vec)->values, (vec)->count + (size_t)(additional_count), \
                    sizeof(*(vec)->values), &(vec)->capacity))

// MARK: Map

/**
 Declares a generic `c_map` struct or typedef.

 For example, a map of string keys with `int` values can be declared as a typedef:

     typedef struct c_map_of(const char *, int) my_map_t;

 or a struct:

     struct my_map_s c_map_of(const char *, int);

 @tparam key_type   The key type.
 @tparam value_type The value type.

 @return Internal structure members in curly braces.
 */
#define c_map_of(key_type, value_type) { \
    /* The `entry` struct has two purposes:
    1) Let the compiler determine member alignment and stride for the bucket array.
    2) Provide temp space for arguments and return values.
    The hash member must be first. */ \
    CSTL_MUTABLE struct { \
        c_hash_t hash; \
        key_type k; \
        value_type v; \
    } entry; \
    CSTL_MUTABLE value_type *v_ptr; \
    struct _c_map *m; \
    c_hash_t (*key_hash_func)(key_type); \
}

/**
 Inits a map, automatically chooising hash and equals functions if possible. If not possible,
 a compile-time error occurs.
 
 When using C11, this function works when keys are integers, floats, or strings. When using C99,
 this function only works with const string keys (that is, `const char *`).
 
 To init a map with a custom key, use #c_map_init_custom() instead.

 When finished using the map, the #c_map_deinit() function must be called.

 @param map Pointer to the map.

 @return bool `true` if success, `false` otherwise (out of memory error).
 */
#define c_map_init(map) \
    c_map_init_with_capacity(map, 0)

/**
 Inits a map.

 When finished using the map, the #c_map_deinit() function must be called.

 @param map         Pointer to the map.
 @param hash_func   The function to calculate the hash of the key. The signature of the function
                    is `c_hash_t hash_func(key_type)`. For example, see #c_const_str_hash().
 @param equals_func The function to determine if two keys are equal. The signature of the function
                    is `bool equals_func(void *, void *)`, where the parameters are pointers to the 
                    key. For example, see #c_str_equals().
 
 @return bool `true` if success, `false` otherwise (out of memory error).
 */
#define c_map_init_custom(map, hash_func, equals_func) \
    c_map_init_custom_with_capacity(map, hash_func, equals_func, 0)

/**
 Inits a map with the specified initial capacity, automatically chooising hash and equals functions
 if possible. If not possible, a compile-time error occurs.

 When using C11, this function works when keys are integers, floats, or strings. When using C99,
 this function only works with const string keys (that is, `const char *`).

 To init a map with a custom key, use #c_map_init_custom_with_capacity() instead.

 When finished using the map, the #c_map_deinit() function must be called.

 @param map      Pointer to the map.
 @param capacity The initial capacity. If 0, the default capacity is used. The actual capacity will
                 be a power-of-two integer greater than or equal to the requested capacity.

 @return bool `true` if success, `false` otherwise (out of memory error).
 */
#define c_map_init_with_capacity(map, capacity) \
    c_map_init_custom_with_capacity(map, c_default_hash((map)->entry.k), \
                                     c_default_equals((map)->entry.k), capacity)

/**
 Inits a map.

 When finished using the map, the #c_map_deinit() function must be called.

 @param map         Pointer to the map.
 @param hash_func   The function to calculate the hash of the key. The signature of the function
                    is `c_hash_t hash_func(key_type)`. For example, see #c_str_hash().
 @param equals_func The function to determine if two keys are equal. The signature of the function
                    is `bool equals_func(void *, void *)`, where the parameters are pointers to the
                    key. For example, see #c_str_equals().
 @param capacity    The initial capacity. If 0, the default capacity is used. The actual capacity
                    may be a power-of-two integer greater than or equal to the requested capacity.

 @return bool `true` if success, `false` otherwise (out of memory error).
 */
#define c_map_init_custom_with_capacity(map, hash_func, equals_func, capacity) ( \
    memset((map), 0, sizeof(*(map))), \
    (map)->key_hash_func = hash_func, \
    (((map)->m = _c_map_create(capacity, equals_func, \
                                CSTL_OFFSETOF(&(map)->entry, &(map)->entry.k), \
                                CSTL_OFFSETOF(&(map)->entry, &(map)->entry.v), \
                                sizeof((map)->entry))) != NULL) \
)

/**
 Deinits the map. The map may be used again by calling #c_map_init().

 @param map Pointer to the map.
 */
#define c_map_deinit(map) \
    _c_map_free((map)->m)

/**
 Gets the number of elements in the map.

 @param map Pointer to the map.

 @return size_t The number of elements in the map.
 */
#define c_map_count(map) \
    _c_map_count((map)->m)

/**
 Gets the capacity of a hash map. The capacity never shrinks.

 @param map Pointer to the map. If `NULL`, the default capacity is returned.

 @return size_t The capacity.
 */
#define c_map_capacity(map) \
    _c_map_capacity((map) ? (map)->m : NULL)

/**
 Puts a key-value pair into the map. If the key already exists in the map, it is replaced with
 the new value.

 @param map   Pointer to the map.
 @param key   The key.
 @param value The value.

 @return `true` if the operation was successful, `false` otherwise (out of memory).
 */
#define c_map_put(map, key, value) ( \
    (map)->entry.k = (key), \
    (map)->entry.v = (value), \
    _c_map_put(&(map)->m, &(map)->entry.k, sizeof((map)->entry.k), \
                (map)->key_hash_func((map)->entry.k), \
                &(map)->entry.v, sizeof((map)->entry.v)) \
)

/**
 Get a pointer to the value associated with a key, creating a new mapping if the key does not exist
 in the map.
 
 If the key didn't previously exist in the map, the value is undefined.
 
 Example:
 
 *    *c_map_put_and_get_ptr(map, "jenny") = "867-5309";

 The returned pointer should be considered temporary. It may be invalid, and should not be used,
 after any modification to the map (like a call to #c_map_put() or #c_map_remove().)

 @param map Pointer to the map.
 @param key The key.

 @return A pointer to the value, or `NULL` for out-of-memory error.
 */
#define c_map_put_and_get_ptr(map, key) ( \
    (map)->entry.k = (key), \
    _c_map_put_and_get_ptr(&(map)->m, &(map)->entry.k, sizeof((map)->entry.k), \
                            (map)->key_hash_func((map)->entry.k), \
                            (void **)&(map)->v_ptr, sizeof((map)->entry.v)), \
    (map)->v_ptr \
)

/**
 Copies mappings from one map to another. The hash maps must have the same types, hash functions,
 amd equals functions.

 @param map      Pointer to the map.
 @param from_map Pointer to the map to copy mappings from.

 @return `true` if all of the mappings from `from_map` were copied to the map. If `false`, 
 (out of memory error), some of the mappings may have been copied before failure.
 */
#define c_map_put_all(map, from_map) (\
    ((sizeof((map)->entry) == sizeof((from_map)->entry) && \
    (map)->key_hash_func == (from_map)->key_hash_func) ? \
    _c_map_put_all(&(map)->m, (from_map)->m, sizeof((map)->entry.k), \
                    sizeof((map)->entry.v)) : \
    false) \
)

/**
 Gets a value from the map. If the key doesn't exist in the map, returns a zeroed-out value
 (`0`, `0.0`, `{0}`, `NULL`, etc.).

 @param map Pointer to the map.
 @param key The key.

 @return The value, or zero if the key doesn't exist in the map.
 */
#define c_map_get(map, key) ( \
    (map)->entry.k = (key), \
    _c_map_get((map)->m, &(map)->entry.k, (map)->key_hash_func((map)->entry.k), \
                (void *)&(map)->entry.v, sizeof((map)->entry.v)), \
    (map)->entry.v \
)

/**
 Get a pointer to the value associated with a key, or `NULL` if the key does not exist in the map.
 
 The returned pointer should be considered temporary. It may be invalid, and should not be used,
 after any modification to the map (like a call to #c_map_put() or #c_map_remove().)

 @param map Pointer to the map.
 @param key The key.

 @return A pointer to the value, or `NULL` if the key does not exist in the map.
 */
#define c_map_get_ptr(map, key) ( \
    (map)->entry.k = (key), \
    _c_map_get_ptr((map)->m, &(map)->entry.k, (map)->key_hash_func((map)->entry.k), \
                    (void **)&(map)->v_ptr), \
    (map)->v_ptr \
)

/**
 Checks if a key exists in the map.

 @param map Pointer to the map.
 @param key The key.

 @return bool `true` if the key exists, `false` otherwise.
 */
#define c_map_contains(map, key) ( \
    (map)->entry.k = (key), \
    _c_map_contains((map)->m, &(map)->entry.k, (map)->key_hash_func((map)->entry.k)) \
)

/**
 Removes a key from the map.

 @param map Pointer to the map.
 @param key The key to remove.

 @return `true` if the key was in the map (thus removed), `false` otherwise.
 */
#define c_map_remove(map, key) ( \
    (map)->entry.k = (key), \
    _c_map_remove((map)->m, &(map)->entry.k, (map)->key_hash_func((map)->entry.k)) \
)

/**
 Foreach macro that iterates over the keys and values in the map. The mappings are not returned in
 any particular order, and the order may change as the map is modified.
 
 The map should not be modified during iteration. A call to #c_map_put() or #c_map_remove()
 during iteration could cause as crash.
 
 The `break` and `continue` keywords are supported during iteration.
 
 Example:
 
     c_map_foreach(map, const char *key, char *value) {
         printf("Name: %s  Phone: %s\n", key, value);
     }

 @param map       Pointer to the map.
 @param key_var   The key type and name.
 @param value_var The value type and name.
 */
#define c_map_foreach(map, key_var, value_var) \
    for (size_t _keep = 1, _keep2 = 1, *_i = NULL; _keep && \
        ((_i = (size_t *)_c_map_next((map)->m, _i, (void *)&(map)->entry.k, \
                                      sizeof((map)->entry.k), (void *)&(map)->entry.v, \
                                      sizeof((map)->entry.v))) != NULL); \
        _keep = 1 - _keep, _keep2 = 1 - _keep2) \
    for (key_var = (map)->entry.k; _keep && _keep2; _keep2 = 1 - _keep2) \
    for (value_var = (map)->entry.v; _keep; _keep = 1 - _keep)

// MARK: Concurrent queue

/**
 The default capacity of a queue. This is the minimum capacity; queues can grow in size.
 */
#define CSTL_QUEUE_DEFAULT_CAPACITY 16

/**
 A macro to initialize a queue statically.

     typedef struct c_queue_of(int) my_queue_t;
     my_queue_t queue = CSTL_QUEUE_INIT;

 When finished using the queue, the #c_queue_deinit() function must be called.
 */
#define CSTL_QUEUE_INIT { { NULL, NULL, NULL, false, false, CSTL_QUEUE_DEFAULT_CAPACITY }, NULL }

/**
 Declares a generic `c_queue` struct or typedef.

 For example, a c_queue with `int` values can be declared as a typedef:

     typedef struct c_queue_of(int) my_queue_t;

 or a struct:

     struct my_queue_s c_queue_of(int);

 @tparam value_type The value type.

 @return Internal structure members in curly braces.
 */
#define c_queue_of(value_type) { \
    struct _c_queue q; \
    value_type *v; \
}

/**
 Inits a queue with the default minimum capacity.

 To init a queue with a custom minimum capacity, use #c_queue_init_with_capacity() instead.

 This function is not thread safe. If two threads attempt to init a queue at the same time, the
 result is undefined.

 When finished using the queue, the #c_queue_deinit() function must be called.

 @tparam queue Pointer to the queue.
 */
#define c_queue_init(queue) \
    c_queue_init_with_capacity(queue, CSTL_QUEUE_DEFAULT_CAPACITY)

/**
 Inits a queue with a custom minimum capacity. The queue may grow in size larger than the specified
 capacity.

 This function is not thread safe. If two threads attempt to init a queue at the same time, the
 result is undefined.

 When finished using the queue, the #c_queue_deinit() function must be called.

 @tparam queue Pointer to the queue.
 @tparam capacity The minimum number of values the queue can hold.
 */
#define c_queue_init_with_capacity(queue, capacity) \
    _c_queue_init(&(queue)->q, sizeof(*(queue)->v), capacity)

/**
 Deinits the queue. The queue may be used again by calling #c_queue_init().

 @tparam queue Pointer to the queue.
 */
#define c_queue_deinit(queue) \
    c_queue_deinit_with_deallocator(queue, NULL)

/**
 Deinits the queue and deallocates its remaining values. Each remaining value is
 removed from the queue and sent as a param to the deallocator function.

 The queue may be used again by calling #c_queue_init().

 @tparam queue Pointer to the queue.
 @tparam deallocator The deallocator function, declared as `void (*deallocator)(void *)`.
 */
#define c_queue_deinit_with_deallocator(queue, deallocator) \
    _c_queue_deinit(&(queue)->q, sizeof(*(queue)->v), (deallocator))

/**
 Adds a value to the back of the queue.

 @tparam queue Pointer to the queue.
 @tparam value The value to add. Must be an addressable rvalue.
 */
#define c_queue_push(queue, value) do { \
    c_static_assert(c_types_compatible(*(queue)->v, value), "Incompatible types"); \
    _c_queue_push(&(queue)->q, sizeof(*(queue)->v), &(value)); \
} while (0)

/**
 Attempts to remove a value from the front of the queue.

 @tparam queue Pointer to the queue.
 @tparam value_ptr The address to store the removed value.
 @return true on success, false if the queue is empty.
 */
#define c_queue_pop(queue, value_ptr) (\
    sizeof(char[c_types_compatible(*(queue)->v, *(value_ptr)) ? 1 : -1]) && /* Type check */ \
    _c_queue_pop(&(queue)->q, sizeof(*(queue)->v), (value_ptr)) \
)

// MARK: Declarations: Hash functions

/// The hash type, which is returned from hash functions.
typedef uint32_t c_hash_t;

/// Gets the default hash function for the specified key type. Uses _Generic, so it requires C11.
#ifndef c_default_hash
#  if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) || (defined(_MSC_VER) && _MSC_VER >= 1800)
#    define c_default_hash(key) _Generic(key, \
       uint8_t      : c_uint8_hash, \
       int8_t       : c_int8_hash, \
       uint16_t     : c_uint16_hash, \
       int16_t      : c_int16_hash, \
       uint32_t     : c_uint32_hash, \
       int32_t      : c_int32_hash, \
       uint64_t     : c_uint64_hash, \
       int64_t      : c_int64_hash, \
       float        : c_float_hash, \
       double       : c_double_hash, \
       char *       : c_str_hash, \
       const char * : c_const_str_hash, \
       void *       : c_ptr_hash, \
       const void * : c_const_ptr_hash)
#  else
// Force compile error if type is not 'char *`
#    define c_default_hash(key) (c_static_assert(sizeof(_c_is_char(*key)) == sizeof(bool) && \
                                                   sizeof(*key) == sizeof(char), \
                                                   "Only works with `char *` type"), \
                                                   c_const_str_hash)
#  endif
#endif

static inline bool _c_is_char(char);

/// Gets the hash for a uint8_t.
CSTL_API c_hash_t c_uint8_hash(uint8_t key);

/// Gets the hash for a int8_t.
CSTL_API c_hash_t c_int8_hash(int8_t key);

/// Gets the hash for a uint16_t.
CSTL_API c_hash_t c_uint16_hash(uint16_t key);

/// Gets the hash for a int16_t.
CSTL_API c_hash_t c_int16_hash(int16_t key);

/// Gets the hash for a uint32_t.
CSTL_API c_hash_t c_uint32_hash(uint32_t key);

/// Gets the hash for a int32_t.
CSTL_API c_hash_t c_int32_hash(int32_t key);

/// Gets the hash for a uint64_t.
CSTL_API c_hash_t c_uint64_hash(uint64_t key);

/// Gets the hash for a int64_t.
CSTL_API c_hash_t c_int64_hash(int64_t key);

/// Gets the hash for a float.
CSTL_API c_hash_t c_float_hash(float key);

/// Gets the hash for a double.
CSTL_API c_hash_t c_double_hash(double key);

/// Gets the hash for a string.
CSTL_API c_hash_t c_str_hash(char *key);

/// Gets the hash for a string.
CSTL_API c_hash_t c_const_str_hash(const char *key);

/// Combines two hashes into one.
CSTL_API c_hash_t c_hash_combine(c_hash_t hash_a, c_hash_t hash_b);

// MARK: Declarations: Equals functions

/// Gets the default equals function for the specified key type. Uses _Generic, so it requires C11.
#ifndef c_default_equals
#  if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 201112L) || (defined(_MSC_VER) && _MSC_VER >= 1800)
#    define c_default_equals(key) _Generic(key, \
       uint8_t      : c_8bit_equals, \
       int8_t       : c_8bit_equals, \
       uint16_t     : c_16bit_equals, \
       int16_t      : c_16bit_equals, \
       uint32_t     : c_32bit_equals, \
       int32_t      : c_32bit_equals, \
       uint64_t     : c_64bit_equals, \
       int64_t      : c_64bit_equals, \
       float        : c_32bit_equals, \
       double       : c_64bit_equals, \
       char *       : c_str_equals, \
       const char * : c_str_equals)
#  else
#    define c_default_equals(key) c_str_equals
#  endif
#endif

/// Checks if two 8-bit values are equal.
CSTL_API bool c_8bit_equals(const void *v1, const void *v2);

/// Checks if two 16-bit values are equal.
CSTL_API bool c_16bit_equals(const void *v1, const void *v2);

/// Checks if two 32-bit values are equal.
CSTL_API bool c_32bit_equals(const void *v1, const void *v2);

/// Checks if two 64-bit values are equal.
CSTL_API bool c_64bit_equals(const void *v1, const void *v2);

/// Checks if two strings are equal.
CSTL_API bool c_str_equals(const void *a, const void *b);

// MARK: Declarations: Private functions

// @cond private

#define CSTL_PTR_INC(ptr, offset) ((uint8_t *)(ptr) + (offset))
#define CSTL_OFFSETOF(base_ptr, ptr) ((size_t)((uint8_t *)(ptr) - (uint8_t *)(base_ptr)))

struct _c_map;
struct _c_queue_block;
struct _c_queue;

CSTL_API bool _c_vec_realloc(void **values, size_t min_capacity, size_t element_size,
                                size_t *capacity);

CSTL_API struct _c_map *_c_map_create(size_t initial_capacity,
                                          bool (*key_equals_func)(const void *key1,
                                                                  const void *key2),
                                          size_t key_offset, size_t value_offset,
                                          size_t bucket_stride);

CSTL_API void _c_map_free(struct _c_map *map);

CSTL_API size_t _c_map_count(const struct _c_map *map);

CSTL_API size_t _c_map_capacity(const struct _c_map *map);

CSTL_API bool _c_map_contains(const struct _c_map *map, const void *key,
                                  c_hash_t key_hash);

CSTL_API bool _c_map_put(struct _c_map **map, const void *key,
                            size_t key_size, c_hash_t key_hash,
                            const void *value, size_t value_size);

CSTL_API void _c_map_put_and_get_ptr(struct _c_map **map, const void *key,
                                        size_t key_size, c_hash_t key_hash,
                                        void **value_ptr, size_t value_size);

CSTL_API bool _c_map_put_all(struct _c_map **map,
                                const struct _c_map *from_map,
                                size_t key_size, size_t value_size);

CSTL_API void _c_map_get(const struct _c_map *map, const void *key,
                            c_hash_t key_hash, void *value, size_t value_size);

CSTL_API void _c_map_get_ptr(const struct _c_map *map, const void *key,
                                c_hash_t key_hash, void **value_ptr);

CSTL_API bool _c_map_remove(struct _c_map *map, const void *key,
                               c_hash_t key_hash);

CSTL_API void *_c_map_next(const struct _c_map *map, void *iterator, void *key,
                              size_t key_size, void *value, size_t value_size);

CSTL_API struct _c_queue_block *_c_queue_new_block(const struct _c_queue *queue,
                                                       size_t value_size);

CSTL_API void _c_queue_free_block(struct _c_queue_block *block);

CSTL_API struct _c_queue_block *_c_queue_get_free_block(struct _c_queue *queue,
                                                            size_t value_size);

CSTL_API void _c_queue_release_free_block(struct _c_queue *queue,
                                             struct _c_queue_block *block);

CSTL_API void _c_queue_set_value(void *values, size_t value_size, size_t index, void *value);

CSTL_API void _c_queue_get_value(void *values, size_t value_size, size_t index, void *value);

CSTL_API bool _c_queue_pop(struct _c_queue *queue, size_t value_size, void *value);

CSTL_API void _c_queue_push(struct _c_queue *queue, size_t value_size, void *value);

CSTL_API void _c_queue_init(struct _c_queue *queue, size_t value_size, size_t capacity);

CSTL_API void _c_queue_deinit(struct _c_queue *queue, size_t value_size,
                                 void (*deallocator)(void *));

// MARK: Implementation: Hash functions

#ifdef CSTL_DEFINE

#if defined(__GNUC__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-function"
#elif defined (_MSC_VER)
#  pragma warning(push)
#  pragma warning(disable:4505)
#endif

// Hash functions from Wang http://www.cris.com/~Ttwang/tech/inthash.htm
// and Jenkins http://www.burtleburtle.net/bob/hash/doobs.html

CSTL_API c_hash_t c_uint8_hash(uint8_t key) {
    return c_uint32_hash(key);
}

CSTL_API c_hash_t c_int8_hash(int8_t key) {
    return c_uint32_hash((uint32_t)key);
}

CSTL_API c_hash_t c_uint16_hash(uint16_t key) {
    return c_uint32_hash(key);
}

CSTL_API c_hash_t c_int16_hash(int16_t key) {
    return c_uint32_hash((uint32_t)key);
}

CSTL_API c_hash_t c_uint32_hash(uint32_t key) {
    key += ~(key << 16);
    key ^=  (key >> 5);
    key +=  (key << 3);
    key ^=  (key >> 13);
    key += ~(key << 9);
    key ^=  (key >> 17);
    return key;
}

CSTL_API c_hash_t c_int32_hash(int32_t key) {
    return c_uint32_hash((uint32_t)key);
}

CSTL_API c_hash_t c_float_hash(float key) {
    uint32_t int_key;
    c_static_assert(sizeof(key) == sizeof(int_key), "float must be be 32-bit");
    memcpy(&int_key, &key, sizeof(int_key));
    return c_uint32_hash(int_key);
}

CSTL_API c_hash_t c_uint64_hash(uint64_t key) {
    key += ~(key << 34);
    key ^=  (key >> 29);
    key += ~(key << 11);
    key ^=  (key >> 14);
    key += ~(key <<  7);
    key ^=  (key >> 28);
    key += ~(key << 26);
    return (c_hash_t)key;
}

CSTL_API c_hash_t c_int64_hash(int64_t key) {
    return c_uint64_hash((uint64_t)key);
}

CSTL_API c_hash_t c_double_hash(double key) {
    uint64_t int_key;
    c_static_assert(sizeof(key) == sizeof(int_key), "double must be 64-bit");
    memcpy(&int_key, &key, sizeof(int_key));
    return c_uint64_hash(int_key);
}

CSTL_API c_hash_t c_const_str_hash(const char *key) {
    c_hash_t hash = 0;
    char ch;
    while ((ch = *key++) != 0) {
        hash += (c_hash_t)ch;
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);
    return hash;
}

CSTL_API c_hash_t c_str_hash(char *key) {
    return c_const_str_hash(key);
}

CSTL_API c_hash_t c_const_ptr_hash(const void *key) {
#if UINTPTR_MAX == UINT32_MAX
    uint32_t int_key;
    memcpy(&int_key, &key, sizeof(int_key));
    return c_uint32_hash(int_key);
#elif UINTPTR_MAX == UINT64_MAX
    uint64_t int_key;
    memcpy(&int_key, &key, sizeof(int_key));
    return c_uint64_hash(int_key);
#else
    c_static_assert(false, "Unknown pointer size");
    return 0;
#endif
}

CSTL_API c_hash_t c_ptr_hash(void *key) {
    return c_const_ptr_hash(key);
}

CSTL_API c_hash_t c_hash_combine(c_hash_t hash_a, c_hash_t hash_b) {
    return hash_a ^ (hash_b + 0x9e3779b9 + (hash_a << 6) + (hash_a >> 2));
}

// MARK: Implementation: Equals functions

CSTL_API bool c_8bit_equals(const void *v1, const void *v2) {
    uint8_t a = *(const uint8_t *)v1;
    uint8_t b = *(const uint8_t *)v2;
    return a == b;
}

CSTL_API bool c_16bit_equals(const void *v1, const void *v2) {
    uint16_t a = *(const uint16_t *)v1;
    uint16_t b = *(const uint16_t *)v2;
    return a == b;
}

CSTL_API bool c_32bit_equals(const void *v1, const void *v2) {
    uint32_t a = *(const uint32_t *)v1;
    uint32_t b = *(const uint32_t *)v2;
    return a == b;
}

CSTL_API bool c_64bit_equals(const void *v1, const void *v2) {
    uint64_t a = *(const uint64_t *)v1;
    uint64_t b = *(const uint64_t *)v2;
    return a == b;
}

CSTL_API bool c_str_equals(const void *a, const void *b) {
    const char *str1 = *(const char * const *)a;
    const char *str2 = *(const char * const *)b;

    return strcmp(str1, str2) == 0;
}

CSTL_API bool c_ptr_equals(const void *v1, const void *v2) {
    const void *a = *(const void * const *)v1;
    const void *b = *(const void * const *)v2;
    return a == b;
}

// MARK: Implementation: Private vector functions

CSTL_API bool _c_vec_realloc(void **values, size_t min_capacity,
                                size_t element_size, size_t *capacity) {
    const size_t capacity_2 = *capacity << 1;
    min_capacity = min_capacity < 8 ? 8 : min_capacity;
    const size_t new_capacity = capacity_2 > min_capacity ? capacity_2 : min_capacity;
    void *new_values = realloc(*values, element_size * new_capacity);
    if (new_values) {
        *values = new_values;
        *capacity = new_capacity;
        return true;
    } else {
        return false;
    }
}

// MARK: Implementation: Private map functions

/*
 Hashtable implementation notes:
 1) Open addressing
 2) Linear probing
 3) Cleanup on deletion (no lazy deletion)

 Only the lower 31 bits of the hash are used. The upper bit is the "occupied" flag.

 References:
 * https://en.wikipedia.org/wiki/Open_addressing
 * http://research.cs.vt.edu/AVresearch/hashing/index.php
 */

static const c_hash_t CSTL_MAP_OCCUPIED_FLAG = 0x80000000;
static const size_t CSTL_MAP_MIN_CAPACITY = 32;
static const float CSTL_MAP_DEFAULT_MAX_LOAD = 0.75f;

struct _c_map {
    void *buckets;

    size_t key_offset;
    size_t value_offset;
    size_t bucket_stride;

    size_t capacity_n;
    size_t capacity_mask;
    size_t max_count;
    size_t count; // The count is the only member that is mutated after _c_map_init()

    bool (*key_equals_func)(const void *key1, const void *key2);

    float max_load_factor;
};

static struct _c_map *_c_map_init(struct _c_map *map, size_t initial_capacity) {
    map->count = 0;
    if (initial_capacity < CSTL_MAP_MIN_CAPACITY) {
        initial_capacity = CSTL_MAP_MIN_CAPACITY;
    }
    size_t capacity_n = 0;
    while ((1u << capacity_n) < initial_capacity) {
        capacity_n++;
    }
    size_t capacity = (1u << capacity_n);

    map->buckets = calloc(capacity, map->bucket_stride);
    if (map->buckets) {
        map->capacity_n = capacity_n;
        map->capacity_mask = capacity - 1;
        map->max_count = (size_t)(capacity * map->max_load_factor);
        // Make sure there is always at least one free entry (for _c_map_find_entry)
        if (map->max_count >= capacity) {
            map->max_count = capacity - 1;
        }
    } else {
        free(map);
        map = NULL;
    }
    return map;
}

static struct _c_map *_c_map_copy(const struct _c_map *from_map,
                                    size_t initial_capacity,
                                    size_t key_size, size_t value_size) {
    struct _c_map *map = (struct _c_map *)calloc(1, sizeof(struct _c_map));
    if (map) {
        map->key_equals_func = from_map->key_equals_func;
        map->key_offset = from_map->key_offset;
        map->value_offset = from_map->value_offset;
        map->bucket_stride = from_map->bucket_stride;
        map->max_load_factor = from_map->max_load_factor;
        map = _c_map_init(map, initial_capacity);
        if (map) {
            bool success = _c_map_put_all(&map, from_map, key_size, value_size);
            if (!success) {
                _c_map_free(map);
                map = NULL;
            }
        }
    }

    return map;
}

static void *_c_map_find_entry(const struct _c_map *map, const void *key,
                                c_hash_t key_hash, void **empty_entry) {
    c_hash_t hash = key_hash | CSTL_MAP_OCCUPIED_FLAG;
    size_t bucket_index = (size_t)(hash & map->capacity_mask);
    while (true) {
        void *bucket = CSTL_PTR_INC(map->buckets, bucket_index * map->bucket_stride);
        c_hash_t flags_hash = *(c_hash_t *)(bucket);
        if (hash == flags_hash &&
            map->key_equals_func(CSTL_PTR_INC(bucket, map->key_offset), key)) {
            return bucket;
        } else if ((flags_hash & CSTL_MAP_OCCUPIED_FLAG) == 0) {
            if (empty_entry) {
                *empty_entry = bucket;
            }
            return NULL;
        }
        bucket_index = (bucket_index + 1) & map->capacity_mask; // Linear probing.
    }
}

static void *_c_map_find_or_put_entry(struct _c_map **map, const void *key,
                                       size_t key_size, c_hash_t key_hash, size_t value_size) {
    void *new_entry = NULL;
    void *entry = _c_map_find_entry(*map, key, key_hash, &new_entry);
    if (!entry) {
        // Grow
        if ((*map)->count >= (*map)->max_count) {
            struct _c_map *new_map = _c_map_copy(*map, (1u << ((*map)->capacity_n + 1)),
                                                     key_size, value_size);
            if (!new_map) {
                return NULL;
            }
            _c_map_free(*map);
            *map = new_map;
            new_entry = NULL;
            _c_map_find_entry(*map, key, key_hash, &new_entry);
        }
        if (new_entry) {
            entry = new_entry;
            key_hash |= CSTL_MAP_OCCUPIED_FLAG;
            memcpy(entry, &key_hash, sizeof(c_hash_t));
            memcpy(CSTL_PTR_INC(entry, (*map)->key_offset), key, key_size);
            (*map)->count++;
        }
    }
    return entry;
}

CSTL_API struct _c_map *_c_map_create(size_t initial_capacity,
                                          bool (*key_equals_func)(const void *key1,
                                                                  const void *key2),
                                          size_t key_offset, size_t value_offset,
                                          size_t bucket_stride) {
    struct _c_map *map = (struct _c_map *)calloc(1, sizeof(struct _c_map));
    if (map) {
        map->key_equals_func = key_equals_func;
        map->key_offset = key_offset;
        map->value_offset = value_offset;
        map->bucket_stride = bucket_stride;
        map->max_load_factor = CSTL_MAP_DEFAULT_MAX_LOAD;
        map = _c_map_init(map, initial_capacity);
    }
    return map;
}

CSTL_API void _c_map_free(struct _c_map *map) {
    if (map) {
        free(map->buckets);
        free(map);
    }
}

CSTL_API size_t _c_map_count(const struct _c_map *map) {
    return map->count;
}

CSTL_API size_t _c_map_capacity(const struct _c_map *map) {
    return map ? (1u << map->capacity_n) : CSTL_MAP_MIN_CAPACITY;
}

CSTL_API bool _c_map_contains(const struct _c_map *map, const void *key,
                                 c_hash_t key_hash) {
    return (_c_map_find_entry(map, key, key_hash, NULL) != NULL);
}

CSTL_API bool _c_map_put(struct _c_map **map, const void *key,
                            size_t key_size, c_hash_t key_hash,
                            const void *value, size_t value_size) {
    void *entry = _c_map_find_or_put_entry(map, key, key_size, key_hash, value_size);
    if (entry) {
        memcpy(CSTL_PTR_INC(entry, (*map)->value_offset), value, value_size);
        return true;
    } else {
        return false;
    }
}

CSTL_API void _c_map_put_and_get_ptr(struct _c_map **map, const void *key,
                                        size_t key_size, c_hash_t key_hash,
                                        void **value_ptr, size_t value_size) {
    void *entry = _c_map_find_or_put_entry(map, key, key_size, key_hash, value_size);
    if (entry) {
        *value_ptr = CSTL_PTR_INC(entry, (*map)->value_offset);
    } else {
        *value_ptr = NULL;
    }
}

CSTL_API bool _c_map_put_all(struct _c_map **map,
                                const struct _c_map *from_map,
                                size_t key_size, size_t value_size) {
    if ((*map)->key_equals_func != from_map->key_equals_func) {
        return false;
    }

    void *iterator = from_map->buckets;
    void *end = CSTL_PTR_INC(from_map->buckets, (from_map->bucket_stride << from_map->capacity_n));
    while (iterator < end) {
        c_hash_t flags_hash = *(c_hash_t *)(iterator);
        if (flags_hash & CSTL_MAP_OCCUPIED_FLAG) {
            void *key = CSTL_PTR_INC(iterator, from_map->key_offset);
            void *value = CSTL_PTR_INC(iterator, from_map->value_offset);
            bool success = _c_map_put(map, key, key_size, flags_hash, value, value_size);
            if (!success) {
                return false;
            }
        }
        iterator = CSTL_PTR_INC(iterator, from_map->bucket_stride);
    }
    return true;
}

CSTL_API void _c_map_get(const struct _c_map *map, const void *key,
                            c_hash_t key_hash, void *value, size_t value_size) {
    void *entry = _c_map_find_entry(map, key, key_hash, NULL);
    if (entry) {
        memcpy(value, CSTL_PTR_INC(entry, map->value_offset), value_size);
    } else {
        memset(value, 0, value_size);
    }
}

CSTL_API void _c_map_get_ptr(const struct _c_map *map, const void *key,
                                c_hash_t key_hash, void **value_ptr) {
    void *entry = _c_map_find_entry(map, key, key_hash, NULL);
    if (entry) {
        *value_ptr = CSTL_PTR_INC(entry, map->value_offset);
    } else {
        *value_ptr = NULL;
    }
}

CSTL_API void *_c_map_next(const struct _c_map *map, void *iterator, void *key,
                              size_t key_size, void *value, size_t value_size) {
    if (map->count == 0) {
        return NULL;
    }
    void *begin = map->buckets;
    void *end = CSTL_PTR_INC(map->buckets, (map->bucket_stride << map->capacity_n));
    if (!iterator) {
        iterator = map->buckets;
    }
    while (iterator >= begin && iterator < end) {
        c_hash_t flags_hash = *(c_hash_t *)(iterator);
        void *next_iterator = CSTL_PTR_INC(iterator, map->bucket_stride);
        if (flags_hash & CSTL_MAP_OCCUPIED_FLAG) {
            if (key) {
                memcpy(key, CSTL_PTR_INC(iterator, map->key_offset), key_size);
            }
            if (value) {
                memcpy(value, CSTL_PTR_INC(iterator, map->value_offset), value_size);
            }
            return next_iterator;
        }
        iterator = next_iterator;
    }
    return NULL;
}

CSTL_API bool _c_map_remove(struct _c_map *map, const void *key, c_hash_t key_hash) {
    void *removed_entry = _c_map_find_entry(map, key, key_hash, NULL);
    if (!removed_entry) {
        return false;
    }
    memset(removed_entry, 0, sizeof(c_hash_t));
    map->count--;

    size_t i = CSTL_OFFSETOF(map->buckets, removed_entry) / map->bucket_stride;
    size_t j = i;

    // NOTE: This only works with linear probing
    const size_t mask = (1u << map->capacity_n) - 1u;
    while (true) {
        j = (j + 1) & mask;
        void *entry = CSTL_PTR_INC(map->buckets, j * map->bucket_stride);

        c_hash_t flags_hash = *(c_hash_t *)(entry);
        if ((flags_hash & CSTL_MAP_OCCUPIED_FLAG) == 0) {
            break;
        }
        size_t k = flags_hash & mask;
        if ((i <= j) ? ((k <= i) || (k > j)) : ((k <= i) && (k > j))) {
            memcpy(removed_entry, entry, map->bucket_stride);
            i = j;
            removed_entry = entry;
            memset(removed_entry, 0, sizeof(c_hash_t));
        }
    }
    return true;
}

#if defined(__GNUC__)
#  pragma GCC diagnostic pop
#elif defined (_MSC_VER)
#  pragma warning(pop)
#endif

#endif // CSTL_DEFINE

// @endcond

#undef CSTL_API
#undef CSTL_DEFINE
#undef CSTL_DECLARE

#endif

/*
//--------------------------------------------------------------------------------------
typedef struct
{
    float x;
    float y;
} point_t;

static c_hash_t point_hash(point_t p)
{
    c_hash_t hash_x = c_float_hash(p.x);
    c_hash_t hash_y = c_float_hash(p.y);
    return c_hash_combine(hash_x, hash_y);
}

static bool point_equals(const void* v1, const void* v2)
{
    const point_t* p1 = (const point_t*)v1;
    const point_t* p2 = (const point_t*)v2;
    return (p1->x == p2->x && p1->y == p2->y);
}

static int c_str_cmp(const void* a, const void* b)
{
    const char* str1 = *(const char* const*)a;
    const char* str2 = *(const char* const*)b;

    return strcmp(str1, str2);
}
//--------------------------------------------------------------------------------------
    {
        typedef struct c_vec_of(const char*) str_vec_t;

        str_vec_t vec;
        c_vec_init(&vec);

        c_vec_push(&vec, "dave");
        c_vec_push(&vec, "mary");
        c_vec_push(&vec, "steve");
        c_vec_push(&vec, "adel");
        c_vec_push(&vec, "ян");
        c_vec_push(&vec, "андрей");

        const char* value = c_vec_get(&vec, 2);
        PRINT("A person: %s", value);

        PRINT("Names unsorted");
        c_vec_foreach(&vec, const char* value)
        {
            PRINT("> Name: %s", value);
        }

        PRINT("Names sorted");
        c_vec_sort(&vec, c_str_cmp);
        c_vec_foreach(&vec, const char* value)
        {
            PRINT("> Name: %s", value);
        }
        c_vec_deinit(&vec);
    }

    {
        typedef struct c_map_of(const char*, char*) str_map_t;

        str_map_t map;
        c_map_init(&map);

        c_map_put(&map, "dave", "bananas");
        c_map_put(&map, "mary", "grapes");
        c_map_put(&map, "steve", "pineapples");

        // Put and get pointer to value
        *c_map_put_and_get_ptr(&map, "sue") = "beans";
        *c_map_put_and_get_ptr(&map, "john") = "fries";

        // Overwrite existing value
        c_map_put(&map, "john", "salad");

        // Get
        PRINT("john: %s.", c_map_get(&map, "john"));
        PRINT("mary: %s.", c_map_get(&map, "mary"));

        char* value = c_map_get(&map, "dave");
        PRINT("dave likes %s.", value);

        // Overwrite via ok_map_get_ptr
        *c_map_get_ptr(&map, "john") = "fries";
        PRINT("john: no, wait, %s.", c_map_get(&map, "john"));

        // Examples when key doesn't exist
        PRINT("Map size: %zu", c_map_count(&map));
        c_map_get_ptr(&map, "cyrus"); // Returns NULL
        PRINT("c_map_get_ptr(&map, \"cyrus\") == %s", c_map_get_ptr(&map, "cyrus") == NULL ? "NULL" : "NOT NULL");

        PRINT("Map size before c_map_put_and_get_ptr(&map, \"cyrus\"): %zu", c_map_count(&map));
        c_map_put_and_get_ptr(&map, "cyrus"); // Mapping created - but value undefined!
        PRINT("Map size after c_map_put_and_get_ptr(&map, \"cyrus\"): %zu", c_map_count(&map));
        *c_map_put_and_get_ptr(&map, "cyrus") = "(who knows)"; // Define the value so we don't crash

        c_map_foreach(&map, const char* key, char* value)
        {
            PRINT("> % s likes % s.", key, value);
        }

        c_map_deinit(&map);
    }

    {
        // Map with integer keys
        typedef struct c_map_of(int, char*) int_map_t;

        int_map_t int_map;
        // On C11, you can use `c_map_init(&map)` instead
        c_map_init_custom(&int_map, c_int32_hash, c_32bit_equals);

        c_map_put(&int_map, 10, "ten");
        c_map_put(&int_map, 20, "twenty");
        c_map_put(&int_map, 30, "thirty");

        PRINT("20 is `%s`.", c_map_get(&int_map, 20));

        c_map_deinit(&int_map);
    }

    {
        typedef struct c_map_of(point_t, char*) point_map_t;

        point_map_t map;
        c_map_init_custom(&map, point_hash, point_equals);

        point_t key = { 100.0, 200.0 };
        c_map_put(&map, key, "Buried treasure");

        PRINT("Value at (%f, %f): %s", key.x, key.y, c_map_get(&map, key));

        c_map_deinit(&map);
    }
//--------------------------------------------------------------------------------------
*/