///////////////////////////////////////////////////////////////
#define TXT_TMP_STR_BUFFERS 16
#define TXT_TMP_STR_MAX_SIZE (1024*4)

static char* txt_tmp_str_buffers;
static int txt_tmp_str_buffer_index = 0;
///////////////////////////////////////////////////////////////
void txt_init(void)
{
	txt_tmp_str_buffers = CALLOC(1, TXT_TMP_STR_BUFFERS * TXT_TMP_STR_MAX_SIZE);
}
///////////////////////////////////////////////////////////////
int32 txt_str_tmp_length(void)
{
	return TXT_TMP_STR_MAX_SIZE;
}

char* txt_str_tmp(void)
{
	char* tmp_str = txt_tmp_str_buffers + txt_tmp_str_buffer_index * TXT_TMP_STR_MAX_SIZE;
	memset(tmp_str, 0, TXT_TMP_STR_MAX_SIZE);
	txt_tmp_str_buffer_index = (txt_tmp_str_buffer_index + 1) % TXT_TMP_STR_BUFFERS;
	return tmp_str;
}
///////////////////////////////////////////////////////////////
const char* txt_format(const char* text, ...)
{
	char* tmp_str = txt_str_tmp();

	va_list args;
	va_start(args, text);
	vsnprintf(tmp_str, TXT_TMP_STR_MAX_SIZE, text, args);
	va_end(args);

	return tmp_str;
}

const char* txt_process(const char* text, bool (*chr_condition)(char), char(*chr_process)(char))
{
	char* tmp_str = txt_str_tmp();

	char* cbuf = tmp_str;
	while(*text)
	{
		if((*chr_condition)(*text))
			*cbuf = (*chr_process)(*text);
		else
			*cbuf = *text;

		text++;
		cbuf++;
	}

	return tmp_str;
}
///////////////////////////////////////////////////////////////
char* txt_win1251_to_oem866(char* text)
{
	uint8* str = (uint8*)text;

	for(int i = 0; str[i]; i++)
	{
		if(str[i] >= 0xc0U && str[i] < 0xe0U)
			str[i] = 0x80U + (str[i] - 0xc0U);
		else if(str[i] >= 0xe0U && str[i] < 0xf0U)
			str[i] = 0xa0 + (str[i] - 0xe0U);
		else if(str[i] >= 0xf0U) // && str[i] <= 0xffU)
			str[i] = 0xe0U + (str[i] - 0xf0U);
		else if(str[i] == 0xa8U)
			str[i] = 0xf0U;
		else if(str[i] == 0xb8U)
			str[i] = 0xf1U;
	}

	return text;
}

bool chr_is_digit(char c)
{
	return c >= '0' && c <= '9';
}

bool chr_is_char(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= 'À' && c <= 'ß') || (c >= 'à' && c <= 'ÿ') || (c == '¨' || c == '¸');
}

bool chr_is_alphanumeric(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= 'À' && c <= 'ß') || (c >= 'à' && c <= 'ÿ') || (c == '¨' || c == '¸');
}

bool chr_is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r');
}

bool chr_is_char_eng(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool chr_is_char_rus(char c)
{
	return (c >= 'À' && c <= 'ß') || (c >= 'à' && c <= 'ÿ') || (c == '¨' || c == '¸');
}

bool chr_is_title(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'À' && c <= 'ß') || (c == '¨');
}

bool chr_is_small(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'à' && c <= 'ÿ') || (c == '¸');
}

char chr_to_title(char c)
{
	if(c >= 'a' && c <= 'z')
		return 'A' + (c - 'a');
	if(c >= 'à' && c <= 'ÿ')
		return 'À' + (c - 'à');
	if(c == '¸')
		return '¨';
	return c;
}

char chr_to_small(char c)
{
	if(c >= 'A' && c <= 'Z')
		return 'a' + (c - 'A');
	if(c >= 'À' && c <= 'ß')
		return 'à' + (c - 'À');
	if(c == '¨')
		return '¸';
	return c;
}

///////////////////////////////////////////////////////////////
#ifdef USE_STDLIB
int32 txt_str_to_int(const char* nptr, char** endptr)
{
	int32 x = atol(nptr);

	if(endptr != 0)
	{
		if(*nptr == '-' || *nptr == '+')
			nptr++;

		while(chr_is_digit(*nptr))
			nptr++;

		*endptr = (char*)nptr;
	}

	return x;
}
#else
int32 text_str_to_int(const char* nptr, char** endptr)
{
	static const char* s;
	static int32 x;
	static uint8 c, neg = 0;

	s = nptr;

	while(chr_is_space(c = *s++));

	if(c == '-')
	{
		neg = 1;
		c = *s++;
	}
	else if(c == '+')
		c = *s++;

	for(x = 0; chr_is_digit(c); c = *s++)
	{
		c -= '0';
		//x *= 10;
		x = (x << 3) + (x << 1);
		x += c;
	}

	if(neg)
		x = -x;

	if(endptr != 0)
		*endptr = (char*)(s != nptr ? s - 1 : nptr);

	return x;
}
#endif

uint8 txt_int_to_str(int32 x, char* str, uint8 digits_required)
{
	if(x < 0)
	{
		*str = '-';
		x = -x;
		str++;
	}

	uint8 i = 0;
	while(x)
	{
		str[i++] = (x % 10) + '0';
		x = x / 10;
	}

	while(i < digits_required)
		str[i++] = '0';

	txt_str_reverse(str, i);
	str[i] = '\0';
	return i;
}

#ifdef USE_FLOAT32
uint8 txt_float_to_str(float n, char* res, uint8 afterpoint)
{
	uint8 i = 0;

	if(n < 0)
	{
		res[0] = '-';
		n = -n;
		i++;
	}

	int32 ipart = (int32)n;
	float fpart = n - (float)ipart;

	i += txt_int_to_str(ipart, res + i, 1);

	if(afterpoint != 0)
	{
#ifdef USE_STDLIB
		float pow10 = powf(10, afterpoint);
#else
		float pow10 = 10;
		uint8 ap = afterpoint;
		while(--ap > 0)
			pow10 *= 10;
#endif

		res[i] = '.';
		i++;
		fpart = fpart * pow10;
		i += txt_int_to_str((int32)fpart, res + i, afterpoint);
	}
	return i;
}

float txt_str_to_float(const char* str, char** endptr)
{
#if defined(USE_STDLIB) && !(defined(__ZX80__) || defined(__ZX81__))//&&0
	return strtof(str, endptr);
#else
	static float number;
	static int exponent;
	static char negative;
	static char* p;
	static float p10;
	static int n;
	static int num_digits;
	static int num_decimals;

	p = (char*)str;

	// Skip leading whitespace
	while(chr_is_space(*p)) p++;

	// Handle optional sign
	negative = 0;
	switch(*p)
	{
		case '-': negative = 1; // Fall through to increment position
		case '+': p++;
	}

	number = 0.;
	exponent = 0;
	num_digits = 0;
	num_decimals = 0;

	// Process string of digits
	while(chr_is_digit(*p))
	{
		number = number * 10. + (*p - '0');
		p++;
		num_digits++;
	}

	// Process decimal part
	if(*p == '.')
	{
		p++;

		while(chr_is_digit(*p))
		{
			number = number * 10. + (*p - '0');
			p++;
			num_digits++;
			num_decimals++;
		}

		exponent -= num_decimals;
	}

	if(num_digits == 0)
	{
		//errno = ERANGE;
		return 0.0;
	}

	// Correct for sign
	if(negative) number = -number;

	// Process an exponent string
	if(*p == 'e' || *p == 'E')
	{
		// Handle optional sign
		negative = 0;
		switch(*++p)
		{
			case '-': negative = 1;   // Fall through to increment pos
			case '+': p++;
		}

		// Process string of digits
		n = 0;
		while(chr_is_digit(*p))
		{
			n = n * 10 + (*p - '0');
			p++;
		}

		if(negative)
			exponent -= n;
		else
			exponent += n;
	}

	if(exponent < FLT_MIN_EXP || exponent > FLT_MAX_EXP)
	{
		//errno = ERANGE;
		return HUGE_VAL;
	}

	// Scale the result
	p10 = 10.;
	n = exponent;
	if(n < 0) n = -n;
	while(n)
	{
		if(n & 1)
		{
			if(exponent < 0)
				number /= p10;
			else
				number *= p10;
		}
		n >>= 1;
		p10 *= p10;
	}

	//if (number == HUGE_VAL) errno = ERANGE;
	if(endptr) *endptr = p;

	return number;
#endif
}
#endif //  USE_FLOAT32

///////////////////////////////////////////////////////////////

char* txt_str_space_field(const char* str, int32 space_count) // +X add space before string, -X add after string
{
	char* str_spc = txt_str_tmp();

	int32 spc_len = ABS(space_count);

	ASSERT(spc_len < 1024);

	memset(str_spc, ' ', spc_len);
	str_spc[spc_len] = 0;

	int32 tstr_len = (int32)strlen(str);

	ASSERT(spc_len + tstr_len < 1024);

	if(space_count > 0)
	{
		spc_len = space_count - tstr_len;
		if(spc_len < 0)
			spc_len = 0;
		memcpy(str_spc + spc_len, str, tstr_len + 1);
	}
	else
	{
		memcpy(str_spc, str, tstr_len);
		str_spc[MAX(tstr_len, spc_len)] = 0;
	}

	return str_spc;
}

char* txt_str_cat(const char* str0, const char* str1)
{
	char* str_tmp = txt_str_tmp();

	int32 str0_len = strlen(str0);

	strcpy(str_tmp, str0);
	strcpy(str_tmp + str0_len, str1);

	return str_tmp;
}

char* txt_str_part(const char* str, int32 part_start, int32 part_len)
{
	char* str_tmp = txt_str_tmp();

	int32 str_len = strlen(str);

	ASSERT(part_start <= str_len);

	if(part_len == -1 || part_len > str_len - part_start)
		part_len = str_len - part_start;

	memcpy(str_tmp, str + part_start, part_len);

	return str_tmp;
}

char* txt_remove_double_quotes(const char* str, int32 str_len)
{
	char* str_tmp = txt_str_tmp();

	if(str_len == -1)
		str_len = strlen(str);

	int32 j = 0;

	for(int32 i = 0; i < str_len; i++)
	{
		if(str[i] == '\\' && str[i + 1] == '"')
		{
			str_tmp[j++] = '"';
			i++;
			continue;
		}

		if(str[i] != '"')
			str_tmp[j++] = str[i];
	}

	str_tmp[j] = 0;

	return str_tmp;
}

int32 chr_find_in_str(char c, const char* str)
{
	for(int32 i = 0; str[i] != 0; i++)
		if(c == str[i])
			return i;

	return -1;
}

char* txt_str_replace_str(const char* str, const char* str_search, const char* str_replace, int32 start)
{
	int32 str_found_at = txt_str_find_str(str, str_search, start);

	if(str_found_at != -1)
	{
		char* str_tmp = txt_str_tmp();

		int32 str_len = strlen(str);
		int32 str_search_len = strlen(str_search);
		int32 str_replace_len = strlen(str_replace);

		memcpy(str_tmp, str, str_found_at);
		memcpy(str_tmp + str_found_at, str_replace, str_replace_len);
		memcpy(str_tmp + str_found_at + str_replace_len, str + str_found_at + str_search_len, str_len - str_found_at - str_search_len);

		return str_tmp;
	}

	return NULL;
}

int32 chr_str_replace_chr(char* str, const char* chr_search, char chr_replace, int32 start)
{
	for(int32 i = start; str[i] != 0; i++)
	{
		int32 found_at = chr_find_in_str(str[i], chr_search);

		if(found_at != -1)
		{
			str[i] = chr_replace;
			return i;
		}
	}

	return -1;
}

int32 chr_str_replace_chr_all(char* str, const char* chr_search, char chr_replace, int32 start)
{
	int32 found_at = -1;

	while(true)
	{
		start = chr_str_replace_chr(str, chr_search, chr_replace, start);

		if(start != -1)
			found_at = start, start++;
		else
			break;
	}

	return found_at;
}

char* txt_str_replace_str_all(const char* str, const char* str_search, const char* str_replace, int32 start)
{
	while(true)
	{
		int32 found_at = txt_str_find_str(str, str_search, start);

		if(found_at != -1)
			str = txt_str_replace_str(str, str_search, str_replace, found_at);
		else
			break;
	}

	return str;
}

int32 txt_str_find_str(const char* str, const char* substr, int32 start)
{
	int32 str_len = strlen(str);
	int32 substr_len = strlen(substr);

	for(int32 i = start; str_len - i >= substr_len; i++)
	{
		const char* s0 = str + i;
		const char* s1 = substr;

		while(*s0 && *s1)
		{
			if(*s0 != *s1)
				break;

			s0++;
			s1++;
		}

		if(!*s1)
			return i;
	}

	return -1;
}

int32 txt_str_find_str_reverse(const char* str, const char* substr, int32 start)
{
	int32 str_len = strlen(str);
	int32 substr_len = strlen(substr);

	if(start == -1)
		start = str_len - substr_len;

	if(str_len - start < substr_len)
		return -1;

	for(int32 i = start; i >= 0; i--)
	{
		const char* s0 = str + i;
		const char* s1 = substr;

		while(*s0 && *s1)
		{
			if(*s0 != *s1)
				break;

			s0++;
			s1++;
		}

		if(!*s1)
			return i;
	}

	return -1;
}

void txt_str_to_small(char* str)
{
	for(; *str; str++)
		*str = chr_to_small(*str);
}

void txt_str_to_title(char* str)
{
	for(; *str; str++)
		*str = chr_to_title(*str);
}

void txt_str_reverse(char* str, int len)
{
	int i = 0, j = len - 1;
	char temp;
	while(i < j)
	{
		temp = str[i];
		str[i] = str[j];
		str[j] = temp;
		i++;
		j--;
	}
}

int32 txt_str_len(char* str, int32 max_len)
{
	int32 str_len = 0;

	while(*(str + str_len) != 0 && (max_len == -1 || str_len < max_len))
		str_len++;

	return str_len;
}

char* txt_str_alloc(char* str, int32 max_len)
{
	int32 str_len = txt_str_len(str, max_len);

	if(max_len == -1)
		max_len = str_len;

	ASSERT(str_len <= max_len);

	char* str_alloc = CALLOC(1, max_len + 1);

	memcpy(str_alloc, str, MIN(str_len, max_len));

	return str_alloc;
}

char* txt_str_new(char* str)
{
	return txt_str_alloc(str, -1);
}

void txt_str_free(char* str)
{
	CFREE(str);
}

int32 txt_str_check_filename(const char* str)
{
	for(int32 i = 0; *str != 0; *str++, i++)
	{
		if(*str == '_' || *str == '/' || *str == '.')
			continue;

		if(chr_is_digit(*str))
			continue;

		if(chr_is_char_eng(*str) && chr_is_small(*str))
			continue;

		return i;
	}

	return -1;
}

///////////////////////////////////////////////////////////////
bool parse_opt_str(char* arg, char* opt_name, char* opt_str)
{
	int32 opt_name_len = strlen(opt_name);

	if(strncmp(arg, opt_name, opt_name_len) == 0)
	{
		strcpy(opt_str, arg + opt_name_len);
		PRINT("%s%s", opt_name, opt_str);
		return true;
	}

	return false;
}

bool parse_opt_int(char* arg, char* opt_name, int32* opt_int)
{
	int32 opt_name_len = strlen(opt_name);

	if(strncmp(arg, opt_name, opt_name_len) == 0)
	{
		*opt_int = txt_str_to_int(arg + opt_name_len, NULL);
		PRINT("%s%d", opt_name, *opt_int);
		return true;
	}

	return false;
}

bool parse_opt_float(char* arg, char* opt_name, float* opt_float)
{
	int32 opt_name_len = strlen(opt_name);

	if(strncmp(arg, opt_name, opt_name_len) == 0)
	{
		*opt_float = txt_str_to_float(arg + opt_name_len, NULL);
		PRINT("%s%f", opt_name, *opt_float);
		return true;
	}

	return false;
}
///////////////////////////////////////////////////////////////
