typedef struct NETPACKET_CLIENT_HI
{
	uint16 clnt_vers;
	uint16 clnt_platform_id;
	uint16 clnt_send_limit;
	uint16 clnt_job_id;
} NETPACKET_CLIENT_HI;

typedef struct c_vec_of(NETPACKET_CLIENT_HI) C_VEC_NETPACKET_CLIENT_HI;

typedef struct NETPACKET_CLIENT_HANDSHAKE
{
	uint16 clnt_handshake[4];
} NETPACKET_CLIENT_HANDSHAKE;

typedef struct c_vec_of(NETPACKET_CLIENT_HANDSHAKE) C_VEC_NETPACKET_CLIENT_HANDSHAKE;

typedef struct NETPACKET_SERVER_HI
{
	uint16 srvr_vers;
	uint16 srvr_platform_id;
	uint16 srvr_send_limit;
	uint16 srvr_handshake[4];
} NETPACKET_SERVER_HI;

typedef struct c_vec_of(NETPACKET_SERVER_HI) C_VEC_NETPACKET_SERVER_HI;

typedef struct NETPACKET_SERVER_WELCOME
{
	uint32 nothing;
} NETPACKET_SERVER_WELCOME;

typedef struct c_vec_of(NETPACKET_SERVER_WELCOME) C_VEC_NETPACKET_SERVER_WELCOME;

typedef struct NETPACKET_CLIENT_ECHO
{
	uint8 echo_data[256];
} NETPACKET_CLIENT_ECHO;

typedef struct c_vec_of(NETPACKET_CLIENT_ECHO) C_VEC_NETPACKET_CLIENT_ECHO;

typedef struct NETPACKET_SERVER_ECHO
{
	uint8 echo_data[256];
} NETPACKET_SERVER_ECHO;

typedef struct c_vec_of(NETPACKET_SERVER_ECHO) C_VEC_NETPACKET_SERVER_ECHO;

typedef struct NETPACKET_GET_FILE_INFO
{
	char file_name[FILENAME_MAX_SIZE];
} NETPACKET_GET_FILE_INFO;

typedef struct c_vec_of(NETPACKET_GET_FILE_INFO) C_VEC_NETPACKET_GET_FILE_INFO;

typedef struct NETPACKET_FILE_INFO
{
	char file_name[FILENAME_MAX_SIZE];
	int64 modified_timestamp;
} NETPACKET_FILE_INFO;

typedef struct c_vec_of(NETPACKET_FILE_INFO) C_VEC_NETPACKET_FILE_INFO;

typedef struct NETPACKET_DOWNLOAD_FILE
{
	char file_name[FILENAME_MAX_SIZE];
} NETPACKET_DOWNLOAD_FILE;

typedef struct c_vec_of(NETPACKET_DOWNLOAD_FILE) C_VEC_NETPACKET_DOWNLOAD_FILE;

typedef struct NETPACKET_ON_INFO
{
	uint32 nothing;
} NETPACKET_ON_INFO;

typedef struct c_vec_of(NETPACKET_ON_INFO) C_VEC_NETPACKET_ON_INFO;

typedef struct NETPACKET_ON_IDLE
{
	uint32 nothing;
} NETPACKET_ON_IDLE;

typedef struct c_vec_of(NETPACKET_ON_IDLE) C_VEC_NETPACKET_ON_IDLE;

typedef struct NETPACKET_ON_UPDATE
{
	uint32 nothing;
} NETPACKET_ON_UPDATE;

typedef struct c_vec_of(NETPACKET_ON_UPDATE) C_VEC_NETPACKET_ON_UPDATE;

typedef struct NETPACKET_ON_CONTINUE
{
	uint32 nothing;
} NETPACKET_ON_CONTINUE;

typedef struct c_vec_of(NETPACKET_ON_CONTINUE) C_VEC_NETPACKET_ON_CONTINUE;

typedef struct NETPACKET_ON_COMPLETE
{
	uint32 nothing;
} NETPACKET_ON_COMPLETE;

typedef struct c_vec_of(NETPACKET_ON_COMPLETE) C_VEC_NETPACKET_ON_COMPLETE;

typedef struct NETPACKET_DATASET_5000
{
	int32 data_size;
	uint8 data[5000];
} NETPACKET_DATASET_5000;

typedef struct c_vec_of(NETPACKET_DATASET_5000) C_VEC_NETPACKET_DATASET_5000;

typedef struct NETPACKET_DATASET_1000
{
	int32 data_size;
	uint8 data[1000];
} NETPACKET_DATASET_1000;

typedef struct c_vec_of(NETPACKET_DATASET_1000) C_VEC_NETPACKET_DATASET_1000;

typedef struct NETPACKET_DATASET_256
{
	int32 data_size;
	uint8 data[256];
} NETPACKET_DATASET_256;

typedef struct c_vec_of(NETPACKET_DATASET_256) C_VEC_NETPACKET_DATASET_256;

typedef struct NETPACKET_DATASET_128
{
	int32 data_size;
	uint8 data[128];
} NETPACKET_DATASET_128;

typedef struct c_vec_of(NETPACKET_DATASET_128) C_VEC_NETPACKET_DATASET_128;

typedef struct NETPACKET_DATASET_64
{
	int32 data_size;
	uint8 data[64];
} NETPACKET_DATASET_64;

typedef struct c_vec_of(NETPACKET_DATASET_64) C_VEC_NETPACKET_DATASET_64;

typedef struct NETPACKET_DATASET_32
{
	int32 data_size;
	uint8 data[32];
} NETPACKET_DATASET_32;

typedef struct c_vec_of(NETPACKET_DATASET_32) C_VEC_NETPACKET_DATASET_32;

typedef struct NETPACKET_DATASET_16
{
	int32 data_size;
	uint8 data[16];
} NETPACKET_DATASET_16;

typedef struct c_vec_of(NETPACKET_DATASET_16) C_VEC_NETPACKET_DATASET_16;

typedef struct NETPACKET_USER_LOGIN
{
	char user_name[USER_NAME_MAX];
	char user_password_hash[USER_PASSWORD_MAX];
} NETPACKET_USER_LOGIN;

typedef struct c_vec_of(NETPACKET_USER_LOGIN) C_VEC_NETPACKET_USER_LOGIN;

bool NETPACKET_CLIENT_HI_WRITE(NETPACKET_BLOB* dst, NETPACKET_CLIENT_HI* pack);
bool NETPACKET_CLIENT_HI_READ(NETPACKET_BLOB* dst, NETPACKET_CLIENT_HI* pack);
int32 NETPACKET_CLIENT_HI_SIZE(void);

int32 C_VEC_NETPACKET_CLIENT_HI_SIZE(C_VEC_NETPACKET_CLIENT_HI* vec);
bool C_VEC_NETPACKET_CLIENT_HI_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_CLIENT_HI* vec);
bool C_VEC_NETPACKET_CLIENT_HI_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_CLIENT_HI* vec);

bool NETPACKET_CLIENT_HANDSHAKE_WRITE(NETPACKET_BLOB* dst, NETPACKET_CLIENT_HANDSHAKE* pack);
bool NETPACKET_CLIENT_HANDSHAKE_READ(NETPACKET_BLOB* dst, NETPACKET_CLIENT_HANDSHAKE* pack);
int32 NETPACKET_CLIENT_HANDSHAKE_SIZE(void);

int32 C_VEC_NETPACKET_CLIENT_HANDSHAKE_SIZE(C_VEC_NETPACKET_CLIENT_HANDSHAKE* vec);
bool C_VEC_NETPACKET_CLIENT_HANDSHAKE_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_CLIENT_HANDSHAKE* vec);
bool C_VEC_NETPACKET_CLIENT_HANDSHAKE_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_CLIENT_HANDSHAKE* vec);

bool NETPACKET_SERVER_HI_WRITE(NETPACKET_BLOB* dst, NETPACKET_SERVER_HI* pack);
bool NETPACKET_SERVER_HI_READ(NETPACKET_BLOB* dst, NETPACKET_SERVER_HI* pack);
int32 NETPACKET_SERVER_HI_SIZE(void);

int32 C_VEC_NETPACKET_SERVER_HI_SIZE(C_VEC_NETPACKET_SERVER_HI* vec);
bool C_VEC_NETPACKET_SERVER_HI_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_SERVER_HI* vec);
bool C_VEC_NETPACKET_SERVER_HI_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_SERVER_HI* vec);

bool NETPACKET_SERVER_WELCOME_WRITE(NETPACKET_BLOB* dst, NETPACKET_SERVER_WELCOME* pack);
bool NETPACKET_SERVER_WELCOME_READ(NETPACKET_BLOB* dst, NETPACKET_SERVER_WELCOME* pack);
int32 NETPACKET_SERVER_WELCOME_SIZE(void);

int32 C_VEC_NETPACKET_SERVER_WELCOME_SIZE(C_VEC_NETPACKET_SERVER_WELCOME* vec);
bool C_VEC_NETPACKET_SERVER_WELCOME_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_SERVER_WELCOME* vec);
bool C_VEC_NETPACKET_SERVER_WELCOME_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_SERVER_WELCOME* vec);

bool NETPACKET_CLIENT_ECHO_WRITE(NETPACKET_BLOB* dst, NETPACKET_CLIENT_ECHO* pack);
bool NETPACKET_CLIENT_ECHO_READ(NETPACKET_BLOB* dst, NETPACKET_CLIENT_ECHO* pack);
int32 NETPACKET_CLIENT_ECHO_SIZE(void);

int32 C_VEC_NETPACKET_CLIENT_ECHO_SIZE(C_VEC_NETPACKET_CLIENT_ECHO* vec);
bool C_VEC_NETPACKET_CLIENT_ECHO_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_CLIENT_ECHO* vec);
bool C_VEC_NETPACKET_CLIENT_ECHO_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_CLIENT_ECHO* vec);

bool NETPACKET_SERVER_ECHO_WRITE(NETPACKET_BLOB* dst, NETPACKET_SERVER_ECHO* pack);
bool NETPACKET_SERVER_ECHO_READ(NETPACKET_BLOB* dst, NETPACKET_SERVER_ECHO* pack);
int32 NETPACKET_SERVER_ECHO_SIZE(void);

int32 C_VEC_NETPACKET_SERVER_ECHO_SIZE(C_VEC_NETPACKET_SERVER_ECHO* vec);
bool C_VEC_NETPACKET_SERVER_ECHO_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_SERVER_ECHO* vec);
bool C_VEC_NETPACKET_SERVER_ECHO_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_SERVER_ECHO* vec);

bool NETPACKET_GET_FILE_INFO_WRITE(NETPACKET_BLOB* dst, NETPACKET_GET_FILE_INFO* pack);
bool NETPACKET_GET_FILE_INFO_READ(NETPACKET_BLOB* dst, NETPACKET_GET_FILE_INFO* pack);
int32 NETPACKET_GET_FILE_INFO_SIZE(void);

int32 C_VEC_NETPACKET_GET_FILE_INFO_SIZE(C_VEC_NETPACKET_GET_FILE_INFO* vec);
bool C_VEC_NETPACKET_GET_FILE_INFO_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_GET_FILE_INFO* vec);
bool C_VEC_NETPACKET_GET_FILE_INFO_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_GET_FILE_INFO* vec);

bool NETPACKET_FILE_INFO_WRITE(NETPACKET_BLOB* dst, NETPACKET_FILE_INFO* pack);
bool NETPACKET_FILE_INFO_READ(NETPACKET_BLOB* dst, NETPACKET_FILE_INFO* pack);
int32 NETPACKET_FILE_INFO_SIZE(void);

int32 C_VEC_NETPACKET_FILE_INFO_SIZE(C_VEC_NETPACKET_FILE_INFO* vec);
bool C_VEC_NETPACKET_FILE_INFO_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_FILE_INFO* vec);
bool C_VEC_NETPACKET_FILE_INFO_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_FILE_INFO* vec);

bool NETPACKET_DOWNLOAD_FILE_WRITE(NETPACKET_BLOB* dst, NETPACKET_DOWNLOAD_FILE* pack);
bool NETPACKET_DOWNLOAD_FILE_READ(NETPACKET_BLOB* dst, NETPACKET_DOWNLOAD_FILE* pack);
int32 NETPACKET_DOWNLOAD_FILE_SIZE(void);

int32 C_VEC_NETPACKET_DOWNLOAD_FILE_SIZE(C_VEC_NETPACKET_DOWNLOAD_FILE* vec);
bool C_VEC_NETPACKET_DOWNLOAD_FILE_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DOWNLOAD_FILE* vec);
bool C_VEC_NETPACKET_DOWNLOAD_FILE_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DOWNLOAD_FILE* vec);

bool NETPACKET_ON_INFO_WRITE(NETPACKET_BLOB* dst, NETPACKET_ON_INFO* pack);
bool NETPACKET_ON_INFO_READ(NETPACKET_BLOB* dst, NETPACKET_ON_INFO* pack);
int32 NETPACKET_ON_INFO_SIZE(void);

int32 C_VEC_NETPACKET_ON_INFO_SIZE(C_VEC_NETPACKET_ON_INFO* vec);
bool C_VEC_NETPACKET_ON_INFO_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_INFO* vec);
bool C_VEC_NETPACKET_ON_INFO_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_INFO* vec);

bool NETPACKET_ON_IDLE_WRITE(NETPACKET_BLOB* dst, NETPACKET_ON_IDLE* pack);
bool NETPACKET_ON_IDLE_READ(NETPACKET_BLOB* dst, NETPACKET_ON_IDLE* pack);
int32 NETPACKET_ON_IDLE_SIZE(void);

int32 C_VEC_NETPACKET_ON_IDLE_SIZE(C_VEC_NETPACKET_ON_IDLE* vec);
bool C_VEC_NETPACKET_ON_IDLE_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_IDLE* vec);
bool C_VEC_NETPACKET_ON_IDLE_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_IDLE* vec);

bool NETPACKET_ON_UPDATE_WRITE(NETPACKET_BLOB* dst, NETPACKET_ON_UPDATE* pack);
bool NETPACKET_ON_UPDATE_READ(NETPACKET_BLOB* dst, NETPACKET_ON_UPDATE* pack);
int32 NETPACKET_ON_UPDATE_SIZE(void);

int32 C_VEC_NETPACKET_ON_UPDATE_SIZE(C_VEC_NETPACKET_ON_UPDATE* vec);
bool C_VEC_NETPACKET_ON_UPDATE_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_UPDATE* vec);
bool C_VEC_NETPACKET_ON_UPDATE_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_UPDATE* vec);

bool NETPACKET_ON_CONTINUE_WRITE(NETPACKET_BLOB* dst, NETPACKET_ON_CONTINUE* pack);
bool NETPACKET_ON_CONTINUE_READ(NETPACKET_BLOB* dst, NETPACKET_ON_CONTINUE* pack);
int32 NETPACKET_ON_CONTINUE_SIZE(void);

int32 C_VEC_NETPACKET_ON_CONTINUE_SIZE(C_VEC_NETPACKET_ON_CONTINUE* vec);
bool C_VEC_NETPACKET_ON_CONTINUE_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_CONTINUE* vec);
bool C_VEC_NETPACKET_ON_CONTINUE_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_CONTINUE* vec);

bool NETPACKET_ON_COMPLETE_WRITE(NETPACKET_BLOB* dst, NETPACKET_ON_COMPLETE* pack);
bool NETPACKET_ON_COMPLETE_READ(NETPACKET_BLOB* dst, NETPACKET_ON_COMPLETE* pack);
int32 NETPACKET_ON_COMPLETE_SIZE(void);

int32 C_VEC_NETPACKET_ON_COMPLETE_SIZE(C_VEC_NETPACKET_ON_COMPLETE* vec);
bool C_VEC_NETPACKET_ON_COMPLETE_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_COMPLETE* vec);
bool C_VEC_NETPACKET_ON_COMPLETE_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_ON_COMPLETE* vec);

bool NETPACKET_DATASET_5000_WRITE(NETPACKET_BLOB* dst, NETPACKET_DATASET_5000* pack);
bool NETPACKET_DATASET_5000_READ(NETPACKET_BLOB* dst, NETPACKET_DATASET_5000* pack);
int32 NETPACKET_DATASET_5000_SIZE(void);

int32 C_VEC_NETPACKET_DATASET_5000_SIZE(C_VEC_NETPACKET_DATASET_5000* vec);
bool C_VEC_NETPACKET_DATASET_5000_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_5000* vec);
bool C_VEC_NETPACKET_DATASET_5000_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_5000* vec);

bool NETPACKET_DATASET_1000_WRITE(NETPACKET_BLOB* dst, NETPACKET_DATASET_1000* pack);
bool NETPACKET_DATASET_1000_READ(NETPACKET_BLOB* dst, NETPACKET_DATASET_1000* pack);
int32 NETPACKET_DATASET_1000_SIZE(void);

int32 C_VEC_NETPACKET_DATASET_1000_SIZE(C_VEC_NETPACKET_DATASET_1000* vec);
bool C_VEC_NETPACKET_DATASET_1000_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_1000* vec);
bool C_VEC_NETPACKET_DATASET_1000_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_1000* vec);

bool NETPACKET_DATASET_256_WRITE(NETPACKET_BLOB* dst, NETPACKET_DATASET_256* pack);
bool NETPACKET_DATASET_256_READ(NETPACKET_BLOB* dst, NETPACKET_DATASET_256* pack);
int32 NETPACKET_DATASET_256_SIZE(void);

int32 C_VEC_NETPACKET_DATASET_256_SIZE(C_VEC_NETPACKET_DATASET_256* vec);
bool C_VEC_NETPACKET_DATASET_256_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_256* vec);
bool C_VEC_NETPACKET_DATASET_256_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_256* vec);

bool NETPACKET_DATASET_128_WRITE(NETPACKET_BLOB* dst, NETPACKET_DATASET_128* pack);
bool NETPACKET_DATASET_128_READ(NETPACKET_BLOB* dst, NETPACKET_DATASET_128* pack);
int32 NETPACKET_DATASET_128_SIZE(void);

int32 C_VEC_NETPACKET_DATASET_128_SIZE(C_VEC_NETPACKET_DATASET_128* vec);
bool C_VEC_NETPACKET_DATASET_128_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_128* vec);
bool C_VEC_NETPACKET_DATASET_128_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_128* vec);

bool NETPACKET_DATASET_64_WRITE(NETPACKET_BLOB* dst, NETPACKET_DATASET_64* pack);
bool NETPACKET_DATASET_64_READ(NETPACKET_BLOB* dst, NETPACKET_DATASET_64* pack);
int32 NETPACKET_DATASET_64_SIZE(void);

int32 C_VEC_NETPACKET_DATASET_64_SIZE(C_VEC_NETPACKET_DATASET_64* vec);
bool C_VEC_NETPACKET_DATASET_64_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_64* vec);
bool C_VEC_NETPACKET_DATASET_64_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_64* vec);

bool NETPACKET_DATASET_32_WRITE(NETPACKET_BLOB* dst, NETPACKET_DATASET_32* pack);
bool NETPACKET_DATASET_32_READ(NETPACKET_BLOB* dst, NETPACKET_DATASET_32* pack);
int32 NETPACKET_DATASET_32_SIZE(void);

int32 C_VEC_NETPACKET_DATASET_32_SIZE(C_VEC_NETPACKET_DATASET_32* vec);
bool C_VEC_NETPACKET_DATASET_32_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_32* vec);
bool C_VEC_NETPACKET_DATASET_32_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_32* vec);

bool NETPACKET_DATASET_16_WRITE(NETPACKET_BLOB* dst, NETPACKET_DATASET_16* pack);
bool NETPACKET_DATASET_16_READ(NETPACKET_BLOB* dst, NETPACKET_DATASET_16* pack);
int32 NETPACKET_DATASET_16_SIZE(void);

int32 C_VEC_NETPACKET_DATASET_16_SIZE(C_VEC_NETPACKET_DATASET_16* vec);
bool C_VEC_NETPACKET_DATASET_16_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_16* vec);
bool C_VEC_NETPACKET_DATASET_16_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_DATASET_16* vec);

bool NETPACKET_USER_LOGIN_WRITE(NETPACKET_BLOB* dst, NETPACKET_USER_LOGIN* pack);
bool NETPACKET_USER_LOGIN_READ(NETPACKET_BLOB* dst, NETPACKET_USER_LOGIN* pack);
int32 NETPACKET_USER_LOGIN_SIZE(void);

int32 C_VEC_NETPACKET_USER_LOGIN_SIZE(C_VEC_NETPACKET_USER_LOGIN* vec);
bool C_VEC_NETPACKET_USER_LOGIN_WRITE(NETPACKET_BLOB* dst, C_VEC_NETPACKET_USER_LOGIN* vec);
bool C_VEC_NETPACKET_USER_LOGIN_READ(NETPACKET_BLOB* dst, C_VEC_NETPACKET_USER_LOGIN* vec);

const char* netpackets_get_enum_string_by_id(int id);
