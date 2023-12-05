#ifdef USE_FIXPT

typedef USE_FIXPT_TYPE fixpt;

#define FIXPT_INT(i) (((fixpt)(i)) << USE_FIXPT_FRACT_SHIFT)
#define FIXPT_MUL_INT(n,i) ((fixpt)(((USE_FIXPT_OVERFLOW_TYPE) (n)) * (i)))
#define FIXPT_DIV_INT(n,i) ((fixpt)(((USE_FIXPT_OVERFLOW_TYPE) (n)) / (i)))

#define FIXPT_TO_INT(i) ((i) >> USE_FIXPT_FRACT_SHIFT)
#define FIXPT_MUL_FP(n,i) ((fixpt)((((USE_FIXPT_OVERFLOW_TYPE) (n)) * (i)) >> USE_FIXPT_FRACT_SHIFT))
#define FIXPT_DIV_FP(n,i) ((fixpt)((((USE_FIXPT_OVERFLOW_TYPE) (n)) << USE_FIXPT_FRACT_SHIFT) / (i)))

// 3.14 = FIXPT_FRACT(314,100)
#define FIXPT_FRACT(i,d) ((fixpt)((((USE_FIXPT_OVERFLOW_TYPE) (i)) << USE_FIXPT_FRACT_SHIFT) / (d)))

static inline fixpt fixpt_byte(uint8 i) { return FIXPT_INT(i); }
static inline fixpt fixpt_int16(int16 i) { return FIXPT_INT(i); }
static inline fixpt fixpt_fract(int32 i, int32 d) { return FIXPT_FRACT(i, d); }

static inline fixpt fixpt_to_int(fixpt f) { return (f >> USE_FIXPT_FRACT_SHIFT); }

fixpt fixpt_acc();
fixpt fixpt_acc_to_int();

void fixpt_lda(fixpt n);
void fixpt_lda_byte(uint8 n);
void fixpt_lda_int(fixpt n);

void fixpt_add(fixpt n);
void fixpt_add_byte(uint8 n);
void fixpt_add_int(fixpt n);

void fixpt_mul(fixpt n);
void fixpt_mul_byte(uint8 n);
void fixpt_mul_int(fixpt n);

void fixpt_div(fixpt n);
void fixpt_div_byte(uint8 n);
void fixpt_div_int(fixpt n);

uint8 txt_fixpt_to_str(fixpt n, char* res, uint8 afterpoint);
fixpt txt_str_to_fixpt(const char* str, char** endptr);

// CONST int32 FP MATH
typedef int32 fp32;
typedef int64 fp64;

fp32 fp_sqrt(int FPFRACT_SHIFT, fp32 fp);

#define FP_INT(FPFRACT_SHIFT,i) (((fp32)(i)) << (FPFRACT_SHIFT))
#define FP_MUL_INT(n,i) ((n)*(i))
#define FP_DIV_INT(n,i) ((n)/(i))
#define FP_TO_INT(FPFRACT_SHIFT,i) ((i) >> (FPFRACT_SHIFT))
#define FP_MUL_FP(FPFRACT_SHIFT,FPOVERFLOW_TYPE,n,i) ((fp32)((((FPOVERFLOW_TYPE) (n)) * (i)) >> (FPFRACT_SHIFT)))
#define FP_DIV_FP(FPFRACT_SHIFT,FPOVERFLOW_TYPE,n,i) ((fp32)((((FPOVERFLOW_TYPE) (n)) << (FPFRACT_SHIFT)) / (i)))
// 3.14 = FP_FRACT(314,100)
#define FP_FRACT(FPFRACT_SHIFT,FPOVERFLOW_TYPE,i,d) ((fp32)((((FPOVERFLOW_TYPE) (i)) << (FPFRACT_SHIFT)) / (d)))
#define FP_HALF(FPFRACT_SHIFT) FP_INT((FPFRACT_SHIFT - 1), 1)
#define FP_SQRT(FPFRACT_SHIFT,n) fp_sqrt(FPFRACT_SHIFT,n)

#define FP10_INT(i) FP_INT(10,i)
#define FP10_MUL_INT(n,i) FP_MUL_INT(n,i)
#define FP10_DIV_INT(n,i) FP_DIV_INT(n,i)
#define FP10_TO_INT(i) FP_TO_INT(10,i)
#define FP10_MUL_FP(FPOVERFLOW_TYPE,n,i) FP_MUL_FP(10,FPOVERFLOW_TYPE,n,i)
#define FP10_DIV_FP(FPOVERFLOW_TYPE,n,i) FP_DIV_FP(10,FPOVERFLOW_TYPE,n,i)
#define FP10_FRACT(FPOVERFLOW_TYPE,i,d) FP_FRACT(10,FPOVERFLOW_TYPE,i,d)
#define FP10_HALF() FP_HALF(10)
#define FP10_SQRT(n) FP_SQRT(10,n)

#define FP12_INT(i) FP_INT(12,i)
#define FP12_MUL_INT(n,i) FP_MUL_INT(n,i)
#define FP12_DIV_INT(n,i) FP_DIV_INT(n,i)
#define FP12_TO_INT(i) FP_TO_INT(12,i)
#define FP12_MUL_FP(FPOVERFLOW_TYPE,n,i) FP_MUL_FP(12,FPOVERFLOW_TYPE,n,i)
#define FP12_DIV_FP(FPOVERFLOW_TYPE,n,i) FP_DIV_FP(12,FPOVERFLOW_TYPE,n,i)
#define FP12_FRACT(FPOVERFLOW_TYPE,i,d) FP_FRACT(12,FPOVERFLOW_TYPE,i,d)
#define FP12_HALF() FP_HALF(12)
#define FP12_SQRT(n) FP_SQRT(12,n)

#define FP16_INT(i) FP_INT(16,i)
#define FP16_MUL_INT(n,i) FP_MUL_INT(n,i)
#define FP16_DIV_INT(n,i) FP_DIV_INT(n,i)
#define FP16_TO_INT(i) FP_TO_INT(16,i)
#define FP16_MUL_FP(FPOVERFLOW_TYPE,n,i) FP_MUL_FP(16,FPOVERFLOW_TYPE,n,i)
#define FP16_DIV_FP(FPOVERFLOW_TYPE,n,i) FP_DIV_FP(16,FPOVERFLOW_TYPE,n,i)
#define FP16_FRACT(FPOVERFLOW_TYPE,i,d) FP_FRACT(16,FPOVERFLOW_TYPE,i,d)
#define FP16_HALF() FP_HALF(16)
#define FP16_SQRT(n) FP_SQRT(16,n)

#endif // USE_FIXPT