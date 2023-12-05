void txt_init(void);

int32 txt_str_tmp_max_size(void);
char* txt_str_tmp(void);

const char* txt_format(const char* text, ...);
const char* txt_process(const char* text, bool (*chr_condition)(char), char(*chr_process)(char));

char* txt_win1251_to_oem866(char* text);

void txt_str_reverse(char* str, int len);

uint8 txt_int_to_str(int32 x, char* str, uint8 digits_required);
int32 txt_str_to_int(const char* nptr, char** endptr);

#ifdef USE_FLOAT32
uint8 txt_float_to_str(float n, char* res, uint8 afterpoint);
float txt_str_to_float(const char* str, char** endptr);
#endif

char* txt_str_space_field(const char* str, int32 space_count); // +X add space before string, -X add after string

bool chr_is_digit(char c);
bool chr_is_char(char c);
bool chr_is_alphanumeric(char c);
bool chr_is_space(char c);
bool chr_is_char_eng(char c);
bool chr_is_char_rus(char c);
bool chr_is_title(char c);
bool chr_is_small(char c);

char chr_to_title(char c);
char chr_to_small(char c);

void txt_str_to_small(char* str);
void txt_str_to_title(char* str);

int32 txt_str_len(char* str, int32 max_len);

char* txt_str_alloc(char* str, int32 max_len); // return allocated buffer with copy of str, if max_len == -1, buffer size equals str length + 1
char* txt_str_new(char* str); // return allocated buffer with copy of str
void txt_str_free(char* str); // frees str allocated buffer from txt_str_alloc or txt_str_new

char* txt_str_cat(const char* str0, const char* str1);
char* txt_str_part(const char* str, int32 part_start, int32 part_len); // if part_len == -1 copy rest part of str

int32 chr_find_in_str(char c, const char* str);

int32 txt_str_find_str(const char* str, const char* substr, int32 start);
int32 txt_str_find_str_reverse(const char* str, const char* substr, int32 start);

char* txt_str_replace_str(const char* str, const char* str_search, const char* str_replace, int32 start);
char* txt_str_replace_str_all(const char* str, const char* str_search, const char* str_replace, int32 start);

char* txt_remove_double_quotes(const char* str, int32 str_len);

int32 chr_str_replace_chr(char* str, const char* chr_search, char chr_replace, int32 start);
int32 chr_str_replace_chr_all(char* str, const char* chr_search, char chr_replace, int32 start);

int32 txt_str_check_filename(const char* str);

//if(parse_opt_str(argv[i], "-s=", i_src_dir))
bool parse_opt_str(char* arg, char* opt_name, char* opt_str);
bool parse_opt_int(char* arg, char* opt_name, int32* opt_int);
bool parse_opt_float(char* arg, char* opt_name, float* opt_float);
