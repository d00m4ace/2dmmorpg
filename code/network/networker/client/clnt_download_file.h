typedef struct CLNT_DOWNLOAD_FILE
{
	FILE* fp;
	int32 fp_size;

	int64 modified_timestamp;

	char clnt_file_name[FILENAME_MAX_SIZE];
	char serv_file_name[FILENAME_MAX_SIZE];
} CLNT_DOWNLOAD_FILE;

CLNT_DOWNLOAD_FILE* clnt_download_file_init(void);
void clnt_download_file_set(NETWORKER* networker, CLNT_DOWNLOAD_FILE* clnt);
NETWORKER* clnt_download_file_get(char clnt_file_name[], char serv_file_name[]);

bool clnt_download_file_on_request(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);
bool clnt_download_file_on_disconnect(NETWORKER* networker, NETSESSION_STATE* netsession);