#define NETSESSION_HOST_MAXNAME 32

#define NETSESSION_FLAG_RECV_OVERFLOW (1<<0)
#define NETSESSION_FLAG_SHUTDOWN (1<<1)

typedef struct
{
	atom_int32 session_id;

	atom_int32 socket_state;

	char host[NETSESSION_HOST_MAXNAME];
	int host_port;
	uint16 host_vers;
	uint16 host_platform_id;
	uint16 host_job_id;
	uint16 send_limit;

	char data_recv[NETSESSION_STATE_BUF_SIZE];
	atom_int32 recved;
	atom_int32 recv_total;

	char data_send[NETSESSION_STATE_BUF_SIZE];
	atom_int32 sended;
	atom_int32 send_total;

	void* user_data;

	atom_int32 flags;
} NETSESSION_STATE;

void netsession_init(NETSESSION_STATE* netsession);
void netsession_shutdown(NETSESSION_STATE* netsession);

bool netsession_push_packet(NETSESSION_STATE* netsession, NETPACKET_BLOB* src, bool do_lock);
bool netsession_pop_packet(NETSESSION_STATE* netsession, NETPACKET_BLOB* dst, bool do_lock);

bool netsession_send_packet(NETSESSION_STATE* netsession, NETPACKET_BLOB* ext);
bool netsession_recv_packet(NETSESSION_STATE* netsession, NETPACKET_BLOB* src);
