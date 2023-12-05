typedef struct SERV_DOWNLOAD_FILE
{
	FILE* fp;
	int32 fp_size;

	char serv_file_name[FILENAME_MAX_SIZE];

	int64 modified_timestamp;
} SERV_DOWNLOAD_FILE;

NETWORKER* serv_download_file_get(void);
SERV_DOWNLOAD_FILE* serv_download_file_init(void);
void serv_download_file_set(NETWORKER* networker, SERV_DOWNLOAD_FILE* serv);

bool serv_download_file_on_request(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);
bool serv_download_file_on_disconnect(NETWORKER* networker, NETSESSION_STATE* netsession);