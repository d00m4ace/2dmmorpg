#define DIR_FLG_SCAN_SUBDIRS (1U<<0)
#define DIR_FLG_ONLY_DIRS (1U<<1)

void win32_dir_read(c_vec_str* vs, const char* path, const char* filter, uint32 flags);

int64 win32_get_file_last_modified_timestamp(char* filepath);
