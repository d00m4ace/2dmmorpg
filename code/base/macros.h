#define ARGB32(r,g,b,a) ( (((uint32)(a))<<(24)) | (((uint32)(b))<<(16)) | (((uint32)(g))<<(8)) | (((uint32)(r))) )

#define ARGB32_GETA(argb32) (((argb32)>>24))
#define ARGB32_GETR(argb32) ((argb32)&0xFF)
#define ARGB32_GETB(argb32) (((argb32)>>16)&0xFF)
#define ARGB32_GETG(argb32) (((argb32)>>8)&0xFF)

#define ARGB32_SET_ALPHA(c, a) ( (((uint32)(c)) & 0x00FFFFFF) | (((uint32)(a)) << 24) )

// Expands 0-255 into 0-256
#define EXPAND256(x) ((x) + ((x) > 0))
#define UNEXPAND256(x) ( (x) > 0 ? ((x) - 1) : 0)

#define ABS(a) (((a) < 0) ? -(a) : (a))
#define MAX(a,b) (((a) > (b)) ? (a) : (b))
#define MIN(a,b) (((a) < (b)) ? (a) : (b))

#define SWAP(a,b,c) {(c) = (a); (a) = (b); (b) = (c);}

static const int32 POW10_INT32[] = { 1, 10, 100, 1000, 10000, 100000L, 1000000L, 10000000L, 100000000L, };
#define CONST_POW10(times) (POW10_INT32[(times)])

#ifndef D2_INLINE
#ifdef _MSC_VER
#define D2_INLINE __forceinline
#else
#define D2_INLINE inline __attribute__((always_inline))
#endif
#endif // !D2_INLINE
