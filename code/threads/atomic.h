///////////////////////////////////////////////////////////////
//https://en.cppreference.com/w/c/thread
///////////////////////////////////////////////////////////////
#if defined(_WIN32)
#ifndef __STDC_VERSION__
//ISO released a corrigendum yesterday 2012-07-15 Cor 1:2012 for C11: value __STDC_VERSION__ is now fixed to 201112L
#define __STDC_VERSION__ 201112L
#endif
#endif
///////////////////////////////////////////////////////////////
#include <stdatomic.h>

typedef atomic_uchar atom_uint8;
typedef atomic_ushort atom_uint16;
typedef atomic_uint atom_uint32;
typedef atomic_ullong atom_uint64;
typedef atomic_char atom_int8;
typedef atomic_short atom_int16;
typedef atomic_int atom_int32;
typedef atomic_llong atom_int64;

#define atom_store(obj,arg) atomic_store(obj,arg)
#define atom_load(obj) atomic_load(obj)

//!!!ALL func return the value held previously by the atomic object pointed to by obj.
#define atom_or(obj,arg) atomic_fetch_or(obj,arg)
#define atom_xor(obj,arg) atomic_fetch_xor(obj,arg)
#define atom_and(obj,arg) atomic_fetch_and(obj,arg)

#define atom_add(obj,arg) atomic_fetch_add(obj,arg)
#define atom_sub(obj,arg) atomic_fetch_sub(obj,arg)

#define atom_inc(obj) atomic_fetch_add(obj,1)
#define atom_dec(obj) atomic_fetch_add(obj,-1)

#define atom_cmp(obj,expected) (atom_load(obj) == (expected))
