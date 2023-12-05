#ifdef _WIN64
typedef uint64 netsocket;
#define NETSOCKET_INVALID (netsocket)(~0)
#endif

#if defined(_WIN32) && !defined(_WIN64)
typedef uint32 netsocket;
#define NETSOCKET_INVALID (netsocket)(~0)
#endif

#ifdef USE_LINUX
typedef int32 netsocket;
#define NETSOCKET_INVALID -1
#endif

#define NETSOCKET_NONE 0
#define NETSOCKET_CONNECTING 1
#define NETSOCKET_CONNECTED 2
#define NETSOCKET_CLOSED 3

typedef struct
{
	netsocket socket;
	int32 state;

	char data_recv[NETSOCKET_STATE_BUF_SIZE];
	uint16 recved;
	uint16 recv_total;

	char data_send[NETSOCKET_STATE_BUF_SIZE];
	uint16 sended;
	uint16 send_total;

} NETSOCKET_STATE;

void netsocket_handshake_func(uint16* digit_handshake, uint16* digit_handshake_out);
bool netsocket_handshake_cmp(uint16* digit_handshake, uint16* digit_handshake_cmp);
