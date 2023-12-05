void vecm_print_strs(VECM* vs)
{
	for(int32 i = 0; i < vecm_count(vs); i++)
		PRINT("%d:[%s]", i, vecm_elm(vs, i));
}

void vecm_strs_from_bytes(VECM* vs, uint8* bytes, int32 bytes_size, uint32 flags)
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
				vecm_elm_add_str(vs, bytes + str_i_start, str_len);
			else if(!(flags & STRS_FLG_SKIP_EMPTY_STRINGS))
				vecm_elm_add_str(vs, "", 0);

			str_i_start = i + 1;
			str_i_end = -1;
		}
	}
}

void vecm_str_split(VECM* vs, const char* str, const char* str_delimiters, uint32 flags)
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
		}

		if(str_i_start != -1 && str_i_end != -1)
		{
			int32 str_len = str_i_end - str_i_start;

			if(str_len > 0)
			{
				if(str[str_i_start] == '"' && flags & STRS_FLG_REMOVE_DOUBLE_QUOTES)
					vecm_elm_add_str(vs, txt_remove_double_quotes(str + str_i_start, str_len), -1);
				else
					vecm_elm_add_str(vs, str + str_i_start, str_len);
			}

			str_i_start = -1;
			str_i_end = -1;
		}

		if(str_i_start == -1)
		{
			if(chr_find_in_str(str[i], str_delimiters) != -1)
				vecm_elm_add_str(vs, str + i, 1);
			else if(!(flags & STRS_FLG_SKIP_EMPTY_STRINGS))
				vecm_elm_add_str(vs, str + i, 1);
		}
	}
}

void vecm_dir_files(VECM* vs, const char* dir_path, const char* extension_filter, bool is_only_names) // extension_filter = ".png" for only one type of files or NULL for any type of files 
{
	FilePathList dir;

	if(extension_filter != NULL)
		dir = LoadDirectoryFilesEx(dir_path, extension_filter, false);
	else
		dir = LoadDirectoryFiles(dir_path);

	for(int32 i = 0; i < dir.count; i++)
	{
		//PRINT("%s %s", dir.paths[i], (IsPathFile(dir.paths[i]) ? "file" : "dir"));
		if(IsPathFile(dir.paths[i]))
		{
			const char* file_name;

			if(is_only_names)
				file_name = GetFileNameWithoutExt(dir.paths[i]);
			else
				file_name = GetFileName(dir.paths[i]);

			//PRINT("%s", file_name);
			vecm_elm_add_str(vs, file_name, -1);
		}
	}

	UnloadDirectoryFiles(dir);
}

void vecm_dir_dirs(VECM* vs, const char* dir_path)
{
	FilePathList dir = LoadDirectoryFiles(dir_path);

	for(int32 i = 0; i < dir.count; i++)
	{
		if(!IsPathFile(dir.paths[i]))
		{
			const char* file_name = GetFileName(dir.paths[i]);

			//PRINT("%s", file_name);
			vecm_elm_add_str(vs, file_name, -1);
		}
	}

	UnloadDirectoryFiles(dir);
}

int32 vecm_str_find(VECM* vs, const char* str)
{
	for(int32 j = 0; j < vs->vp.count; j++)
	{
		const char* str_j = vecm_elm(vs, j);

		if(strcmp(str, str_j) == 0)
			return j;
	}

	return -1;
}

#define VECM_MAX_BUFFER_SIZE (1024*1024)
#define VECM_DEFAULT_STR_COUNT (1024*1024)

VECM i_enums = vecm_init(VECM_MAX_BUFFER_SIZE, VECM_DEFAULT_STR_COUNT);
VECM i_enums_old = vecm_init(VECM_MAX_BUFFER_SIZE, VECM_DEFAULT_STR_COUNT);

