#define STRS_FLG_SKIP_EMPTY_STRINGS (1U<<0)
#define STRS_FLG_REMOVE_DOUBLE_QUOTES (1U<<1)
#define STRS_FLG_SKIP_DELIMITERS (1U<<2)

void strs_from_bytes(c_vec_str* vs, uint8* bytes, int32 bytes_size, uint32 flags)
{
	int32 str_i_start = 0;
	int32 str_i_end = -1;

	for(int32 i = 0; i < bytes_size; i++)
	{
		if(i == bytes_size - 1)
			str_i_end = bytes_size;
		else if(bytes[i] == '\r' && bytes[i + 1] == '\n')
		{
			str_i_end = i;
			i++;
		}

		if(str_i_end != -1)
		{
			int32 str_len = str_i_end - str_i_start;

			if(str_len > 0)
				c_vec_push(vs, txt_str_alloc(bytes + str_i_start, str_len));
			else if(!(flags & STRS_FLG_SKIP_EMPTY_STRINGS))
				c_vec_push(vs, txt_str_alloc("", 1));

			str_i_start = i + 1;
			str_i_end = -1;
		}
	}
}

void strs_split_str(c_vec_str* vs, const char* str, const char* str_delimiters, uint32 flags)
{
	int32 str_i_start = -1;
	int32 str_i_end = -1;

	bool is_double_quotes = false;

	for(int32 i = 0; str[i] != 0; i++)
	{
		if(str_i_start == -1)
		{
			if(str[i] == '"')
			{
				is_double_quotes = true;
				str_i_start = i;
			}
			else if(str[i] > ' ' && chr_find_in_str(str[i], str_delimiters) == -1)
				str_i_start = i;
		}
		else if(str_i_end == -1)
		{
			if(is_double_quotes && str[i] == '\\' && str[i + 1] == '"')
			{
				i++;
				continue;
			}

			if(is_double_quotes && str[i] == '"')
			{
				i++;
				is_double_quotes = false;
				str_i_end = i;
			}

			if(!is_double_quotes)
			{
				if(str[i] <= ' ' || chr_find_in_str(str[i], str_delimiters) != -1)
					str_i_end = i;
			}

			if(str_i_end == -1 && str[i + 1] == 0)
				str_i_end = i + 1;
		}

		if(str_i_start != -1 && str_i_end != -1)
		{
			int32 str_len = str_i_end - str_i_start;

			if(str_len > 0)
			{
				if(str[str_i_start] == '"' && flags & STRS_FLG_REMOVE_DOUBLE_QUOTES)
					c_vec_push(vs, txt_str_new(txt_remove_double_quotes(str + str_i_start, str_len)));
				else
					c_vec_push(vs, txt_str_alloc(str + str_i_start, str_len));
			}

			str_i_start = -1;
			str_i_end = -1;
		}

		if(str_i_start == -1)
		{
			if(chr_find_in_str(str[i], str_delimiters) != -1)
			{
				if(!(flags & STRS_FLG_SKIP_DELIMITERS))
					c_vec_push(vs, txt_str_alloc(str + i, 1));
			}
			else if(!(flags & STRS_FLG_SKIP_EMPTY_STRINGS))
				c_vec_push(vs, txt_str_alloc(str + i, 1));
		}
	}
}

void strs_from_file(c_vec_str* vec_strs, const char* file_name, uint32 flags)
{
	uint32 bytes_read = 0;
	uint8* bytes = LoadFileData(file_name, &bytes_read);
	strs_from_bytes(vec_strs, bytes, bytes_read, flags);
	UnloadFileData(bytes);
}