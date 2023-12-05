////////////////////////////////////////////////////
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <strsafe.h>

#include <windows.h>
#endif
////////////////////////////////////////////////////
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

#include "../../config.h"

#include "../../base/cstl.h"
#include "../../base/types.h"
#include "../../base/macros.h"
#include "../../base/rnd.h"
#include "../../base/fixpt.h"
#include "../../base/text.h"
#include "../../base/memory.h"

#include "win32.h"
////////////////////////////////////////////////////
void win32_dir_read(c_vec_str* vs, const char* path, const char* filter, uint32 flags)
{
	WIN32_FIND_DATAA findData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	char* dir;

	int32 path_len = strlen(path);
	int32 path_end = txt_str_find_str_reverse(path, "/", -1);

	if(path_end != path_len - 1)
		dir = txt_str_alloc(txt_str_cat(path, "/"), -1);
	else
		dir = txt_str_alloc(path, -1);

	hFind = FindFirstFileA(txt_str_cat(dir, "*"), &findData);

	if(hFind == INVALID_HANDLE_VALUE)
	{
		PRINT("Invalid handle value! Please check your path: %s", dir);
		return;
	}

	while(FindNextFileA(hFind, &findData) != 0)
	{
		if(strcmp(findData.cFileName, ".") == 0 || strcmp(findData.cFileName, "..") == 0)
			continue;

		//PRINT("%s", findData.cFileName);

		bool is_found_dir = findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;

		if(*filter == '*' || txt_str_find_str_reverse(findData.cFileName, filter, -1) != -1)
		{
			if((!is_found_dir && !(flags & DIR_FLG_ONLY_DIRS)) ||
				(is_found_dir && (flags & DIR_FLG_ONLY_DIRS)))
			{
				c_vec_push(vs, txt_str_alloc(txt_str_cat(dir, findData.cFileName), -1));
			}
		}

		if(is_found_dir && (flags & DIR_FLG_SCAN_SUBDIRS))
		{
			const char* subdir = txt_str_alloc(txt_str_cat(dir, findData.cFileName), -1);
			win32_dir_read(vs, subdir, filter, flags);
			txt_str_free(subdir);
		}
	}

	FindClose(hFind);
	txt_str_free(dir);
}
////////////////////////////////////////////////////

int64 win32_get_file_last_modified_timestamp(char* filepath)
{
	// Specify the file path
	//char* filepath = "C:\\Users\\User\\Desktop\\example.txt";

	// Get the file's last modified time
	FILETIME ftModified;
	HANDLE hFile = CreateFile(filepath, GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	int64 timestamp = 0;

	if(hFile != INVALID_HANDLE_VALUE)
	{
		if(GetFileTime(hFile, NULL, NULL, &ftModified))
		{
			// Convert filetime to system time
			SYSTEMTIME stModified;
			FileTimeToSystemTime(&ftModified, &stModified);

			//Get the number of seconds since January 1, 1970 12:00am UTC
			//Code released into public domain; no attribution required.

			const int64 UNIX_TIME_START = 0x019DB1DED53E8000; //January 1, 1970 (start of Unix epoch) in "ticks"
			const int64 TICKS_PER_SECOND = 10000000; //a tick is 100ns

			LARGE_INTEGER li;
			li.LowPart = ftModified.dwLowDateTime;
			li.HighPart = ftModified.dwHighDateTime;

			//Convert ticks since 1/1/1970 into seconds
			timestamp = (li.QuadPart - UNIX_TIME_START) / TICKS_PER_SECOND;

			// Print the file's last modified time
			//printf("The file was last modified on %02d/%02d/%04d at %02d:%02d:%02d\n",
				//stModified.wDay, stModified.wMonth, stModified.wYear,
				//stModified.wHour, stModified.wMinute, stModified.wSecond);
		}
		CloseHandle(hFile);
	}

	return timestamp;
}
