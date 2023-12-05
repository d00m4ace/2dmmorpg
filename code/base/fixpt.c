#ifdef USE_FIXPT

//define USE_FIXPT_FRACT_SHIFT in config.h
static fixpt g_FIXPT_ACC;//fixed point accumulator

fixpt fixpt_acc() { return g_FIXPT_ACC; }

fixpt fixpt_acc_to_int() { return (g_FIXPT_ACC >> USE_FIXPT_FRACT_SHIFT); }

void fixpt_lda(fixpt n) { g_FIXPT_ACC = n; }
void fixpt_lda_byte(uint8 n) { g_FIXPT_ACC = FIXPT_INT(n); }
void fixpt_lda_int(fixpt n) { g_FIXPT_ACC = FIXPT_INT(n); }

void fixpt_add(fixpt n) { g_FIXPT_ACC += n; }
void fixpt_add_byte(uint8 n) { g_FIXPT_ACC += FIXPT_INT(n); }
void fixpt_add_int(fixpt n) { g_FIXPT_ACC += FIXPT_INT(n); }

void fixpt_mul(fixpt n) { g_FIXPT_ACC = FIXPT_MUL_FP(g_FIXPT_ACC, n); }
void fixpt_mul_byte(uint8 n) { g_FIXPT_ACC = FIXPT_MUL_INT(g_FIXPT_ACC, n); }
void fixpt_mul_int(fixpt n) { g_FIXPT_ACC = FIXPT_MUL_INT(g_FIXPT_ACC, n); }

void fixpt_div(fixpt n) { g_FIXPT_ACC = FIXPT_DIV_FP(g_FIXPT_ACC, n); }
void fixpt_div_byte(uint8 n) { g_FIXPT_ACC = FIXPT_DIV_INT(g_FIXPT_ACC, n); }
void fixpt_div_int(fixpt n) { g_FIXPT_ACC = FIXPT_DIV_INT(g_FIXPT_ACC, n); }

uint8 txt_fixpt_to_str(fixpt n, char* res, uint8 afterpoint)
{
	uint8 i = 0;

	if(n < 0)
	{
		res[0] = '-';
		n = -n;
		i++;
	}

	fixpt ipart = (fixpt)(n >> USE_FIXPT_FRACT_SHIFT);

	i += txt_int_to_str(ipart, res + i, 1);

	if(afterpoint != 0)
	{
		fixpt fpart = ((fixpt)n) - (ipart << USE_FIXPT_FRACT_SHIFT);

		int32 pow10 = CONST_POW10(afterpoint);

		fpart = (((USE_FIXPT_OVERFLOW_TYPE)fpart) * pow10) >> USE_FIXPT_FRACT_SHIFT;

		res[i] = '.';
		i++;

		i += txt_int_to_str(fpart, res + i, afterpoint);
	}
	return i;
}

fixpt txt_str_to_fixpt(const char* str, char** endptr)
{
	static char* p;
	static char* p_endptr;
	static int32 p10;

	p = (char*)str;

	// Skip leading whitespace
	while(chr_is_space(*p)) p++;

	// Process string of digits
	fixpt ipart = txt_str_to_int(p, &p_endptr) << USE_FIXPT_FRACT_SHIFT;
	p = p_endptr;

	// Process decimal part
	if(*p == '.')
	{
		p++;

		p10 = 0;
		fixpt fpart = txt_str_to_int(p, &p_endptr);
		uint8 dp = (uint8)(p_endptr - p);
		p = p_endptr;

		if(dp > 8) dp = 8;
		if(dp) p10 = CONST_POW10(dp);

		fpart = (p10) ? fixpt_fract(fpart, p10) : 0;
		ipart += (ipart < 0) ? -fpart : fpart;
	}

	if(endptr) *endptr = p;

	return ipart;
}

fp32 fp_sqrt(int FPFRACT_SHIFT, fp32 fp)
{
	/*/
	fp = fp << USE_FIXPT_FRACT_SHIFT;

	uint32 b = 1 << 30, q = 0, r = fp;

	while(b > r)
		b >>= 2;

	while(b > 0)
	{
		uint32 t = q + b;
		q >>= 1;
		if(r >= t)
		{
			r -= t;
			q += b;
		}
		b >>= 2;
	}

	return q;
	//*/

	/*/
	fp = fp << USE_FIXPT_FRACT_SHIFT;

	uint64 b = ((uint64)1) << 62, q = 0, r = fp;

	while(b > r)
		b >>= 2;

	while(b > 0)
	{
		uint64 t = q + b;

		q >>= 1;

		if(r >= t)
		{
			r -= t;
			q += b;
		}

		b >>= 2;
	}

	return q;
	//*/

	//*/
	static uint32 num;
	static uint32 result;
	static uint32 bit;
	static uint8 n;

	num = fp;
	result = 0;

	// Many numbers will be less than 15, so
	// this gives a good balance between time spent
	// in if vs. time spent in the while loop
	// when searching for the starting value.
	if(num & 0xFFF00000UL)
		bit = 1UL << 30;
	else
		bit = 1UL << 18;

	while(bit > num)
		bit >>= 2;

	// The main part is executed twice, in order to avoid
	// using 64 bit values in computations.
	for(n = 0; n < 2; n++)
	{
		// First we get the top 24 bits of the answer.
		while(bit)
		{
			if(num >= result + bit)
			{
				num -= result + bit;
				result = (result >> 1) + bit;
			}
			else
			{
				result = (result >> 1);
			}
			bit >>= 2;
		}

		if(n == 0)
		{
			// Then process it again to get the lowest 8 bits.
			if(num > (1UL << FPFRACT_SHIFT))
			{
				// The remainder 'num' is too large to be shifted left
				// by 16, so we have to add 1 to result manually and
				// adjust 'num' accordingly.
				// num = a - (result + 0.5)^2
				//	 = num + result^2 - (result + 0.5)^2
				//	 = num - result - 0.5
				num -= result;
				num = (num << FPFRACT_SHIFT) - FP_HALF(FPFRACT_SHIFT);
				result = (result << FPFRACT_SHIFT) + FP_HALF(FPFRACT_SHIFT);
			}
			else
			{
				num <<= FPFRACT_SHIFT;
				result <<= FPFRACT_SHIFT;
			}

			bit = 1 << (FPFRACT_SHIFT - 2);
		}
	}

#ifndef FIXMATH_NO_ROUNDING
	// Finally, if next bit would have been 1, round the result upwards.
	//if(num > result) result++;
#endif

	return (fp32)result;
	//*/
}

#endif // USE_FIXPT

/*
float invSqrt(float x)
{
	float xhalf = 0.5f * x;
	union
	{
		float x;
		int i;
	} u;
	u.x = x;
	u.i = 0x5f375a86 - (u.i >> 1);
	// The next line can be repeated any number of times to increase accuracy
	u.x = u.x * (1.5f - xhalf * u.x * u.x);
	return u.x;
}
*/
