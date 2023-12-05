//--------------------------------------------------------------------------------------
static uint32 random32_seed = 0;
uint32 random32(void);

uint8 rnd_u8() { return (uint8)(random32() & 0xFF); }
uint16 rnd_u16() { return (uint16)(random32() & 0xFFFF); }
uint32 rnd_u32() { return (uint32)(random32()); }
uint64 rnd_u64() { return (((uint64)random32()) << 32) | random32(); }

int32 rnd(int32 min, int32 max)
{
	return (random32() % (max - min + 1) + min);
}

void rnd_seed(uint32 seed)
{
	if(!seed)
		random32_seed = ((uint32)time(NULL));
	else
		random32_seed = seed;
}

uint32 random32(void)
{
	const uint32 a = 1664525u;
	const uint32 c = 1013904223u;
	random32_seed = (a * random32_seed) + c;
	return random32_seed;
}
//--------------------------------------------------------------------------------------
//This code implements a simple linear feedback shift register (LFSR)algorithm to generate random numbers.
// The algorithm uses four 32 - bit unsigned integers as state variables(x, y, z, and w) 
// that are updated every time the function is called.The algorithm is similar to 
// the LCG algorithm used in the previous examples, but with a different set of constantsand bit - shifting operations.
// Note that the quality of the generated random numbers may depend on the initial values 
// of the state variables and the quality of the algorithm.This implementation is relatively simple and fast, 
// but it may not be suitable for all applications that require true randomness.
// For more secureand reliable random number generation, 
// you may need to use a third - party library or a hardware generator.
//--------------------------------------------------------------------------------------
static LFSR_STATE lfsr_state;

void lfsr_random_set_seed(uint32 seed)
{
	lfsr_state.x = seed;
	lfsr_state.y = 362436069;
	lfsr_state.z = 521288629;
	lfsr_state.w = 88675123;
}

LFSR_STATE lfsr_random_get_state(void)
{
	return lfsr_state;
}

uint32 lfsr_random(void)
{
	uint32 t = lfsr_state.x ^ (lfsr_state.x << 11);
	lfsr_state.x = lfsr_state.y; lfsr_state.y = lfsr_state.z; lfsr_state.z = lfsr_state.w;
	lfsr_state.w = (lfsr_state.w ^ (lfsr_state.w >> 19)) ^ (t ^ (t >> 8));

	return lfsr_state.w;
}
//--------------------------------------------------------------------------------------

/*
 * prng.c - Portable, ISO C90 and C99 compliant high-quality
 * pseudo-random number generator based on the alleged RC4
 * cipher.  This PRNG should be suitable for most general-purpose
 * uses.  Not recommended for cryptographic or financial
 * purposes.  Not thread-safe.
 */

 /* RC4-based pseudo-random state. */
static unsigned char s[256];
static int s_i, s_j;

/* Nonzero if PRNG has been seeded. */
static int seeded;

/* Swap bytes that A and B point to. */
#define SWAP_BYTE(A, B)                         \
        do {                                    \
                unsigned char swap_temp = *(A); \
                *(A) = *(B);                    \
                *(B) = swap_temp;               \
        } while (0)

void prng_seed_bytes(const void* key, size_t size);

/* Seeds the pseudo-random number generator based on the current
   time.

   If the user calls neither this function nor prng_seed_bytes()
   before any prng_get*() function, this function is called
   automatically to obtain a time-based seed. */
void
prng_seed_time(void)
{
	static time_t t;
	if(t == 0)
		t = time(NULL);
	else
		t++;

	prng_seed_bytes(&t, sizeof t);
}

/* Retrieves one octet from the array BYTES, which is N_BYTES in
   size, starting at an offset of OCTET_IDX octets.  BYTES is
   treated as a circular array, so that accesses past the first
   N_BYTES bytes wrap around to the beginning. */
static unsigned char
get_octet(const void* bytes_, size_t n_bytes, size_t octet_idx)
{
	const unsigned char* bytes = bytes_;
	if(CHAR_BIT == 8)
		return bytes[octet_idx % n_bytes];
	else
	{
		size_t first_byte = octet_idx * 8 / CHAR_BIT % n_bytes;
		size_t start_bit = octet_idx * 8 % CHAR_BIT;
		unsigned char c = (bytes[first_byte] >> start_bit) & 255;

		size_t bits_filled = CHAR_BIT - start_bit;
		if(CHAR_BIT % 8 != 0 && bits_filled < 8)
		{
			size_t bits_left = 8 - bits_filled;
			unsigned char bits_left_mask = (1u << bits_left) - 1;
			size_t second_byte = first_byte + 1 < n_bytes ? first_byte + 1 : 0;

			c |= (bytes[second_byte] & bits_left_mask) << bits_filled;
		}

		return c;
	}
}

/* Seeds the pseudo-random number based on the SIZE bytes in
   KEY.  At most the first 2048 bits in KEY are used. */
void prng_seed_bytes(const void* key, size_t size)
{
	int i, j;

	assert(key != NULL && size > 0);

	for(i = 0; i < 256; i++)
		s[i] = i;
	for(i = j = 0; i < 256; i++)
	{
		j = (j + s[i] + get_octet(key, size, i)) & 255;
		SWAP_BYTE(s + i, s + j);
	}

	s_i = s_j = 0;
	seeded = 1;
}

/* Returns a pseudo-random integer in the range [0, 255]. */
unsigned char
prng_get_octet(void)
{
	if(!seeded)
		prng_seed_time();

	s_i = (s_i + 1) & 255;
	s_j = (s_j + s[s_i]) & 255;
	SWAP_BYTE(s + s_i, s + s_j);

	return s[(s[s_i] + s[s_j]) & 255];
}

/* Returns a pseudo-random integer in the range [0, UCHAR_MAX]. */
unsigned char
prng_get_byte(void)
{
	unsigned byte;
	int bits;

	byte = prng_get_octet();
	for(bits = 8; bits < CHAR_BIT; bits += 8)
		byte = (byte << 8) | prng_get_octet();
	return byte;
}

/* Fills BUF with SIZE pseudo-random bytes. */
void
prng_get_bytes(void* buf_, size_t size)
{
	unsigned char* buf;

	for(buf = buf_; size-- > 0; buf++)
		*buf = prng_get_byte();
}

/* Returns a pseudo-random unsigned long in the range [0,
   ULONG_MAX]. */
unsigned long
prng_get_ulong(void)
{
	unsigned long ulng;
	size_t bits;

	ulng = prng_get_octet();
	for(bits = 8; bits < CHAR_BIT * sizeof ulng; bits += 8)
		ulng = (ulng << 8) | prng_get_octet();
	return ulng;
}

/* Returns a pseudo-random long in the range [0, LONG_MAX]. */
long
prng_get_long(void)
{
	return prng_get_ulong() & LONG_MAX;
}

/* Returns a pseudo-random unsigned int in the range [0,
   UINT_MAX]. */
unsigned
prng_get_uint(void)
{
	unsigned uint;
	size_t bits;

	uint = prng_get_octet();
	for(bits = 8; bits < CHAR_BIT * sizeof uint; bits += 8)
		uint = (uint << 8) | prng_get_octet();
	return uint;
}

/* Returns a pseudo-random int in the range [0, INT_MAX]. */
int
prng_get_int(void)
{
	return prng_get_uint() & INT_MAX;
}

/* Returns a pseudo-random floating-point number from the uniform
   distribution with range [0,1). */
double
prng_get_double(void)
{
	for(;;)
	{
		double dbl = prng_get_ulong() / (ULONG_MAX + 1.0);
		if(dbl >= 0.0 && dbl < 1.0)
			return dbl;
	}
}

/* Returns a pseudo-random floating-point number from the
   distribution with mean 0 and standard deviation 1.  (Multiply
   the result by the desired standard deviation, then add the
   desired mean.) */
double
prng_get_double_normal(void)
{
	/* Knuth, _The Art of Computer Programming_, Vol. 2, 3.4.1C,
	   Algorithm P. */
	static int has_next = 0;
	static double next_normal;
	double this_normal;

	if(has_next)
	{
		this_normal = next_normal;
		has_next = 0;
	}
	else
	{
		static double limit;
		double v1, v2, s;

		if(limit == 0.0)
			limit = log(DBL_MAX / 2) / (DBL_MAX / 2);

		for(;;)
		{
			double u1 = prng_get_double();
			double u2 = prng_get_double();
			v1 = 2.0 * u1 - 1.0;
			v2 = 2.0 * u2 - 1.0;
			s = v1 * v1 + v2 * v2;
			if(s > limit && s < 1)
				break;
		}

		this_normal = v1 * sqrt(-2. * log(s) / s);
		next_normal = v2 * sqrt(-2. * log(s) / s);
		has_next = 1;
	}

	return this_normal;
}

///////////////////////////////////////////////////////////////