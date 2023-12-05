typedef struct NETWORKER NETWORKER;

typedef struct
{
	char host[NETSESSION_HOST_MAXNAME];
	int port;

	netsocket listen_socket;

	atom_int32 recv_total;
	atom_int32 send_total;

	atom_int32 recv_nop_total;
	atom_int32 send_nop_total;

	atom_int32 nop_sleep_time_ms;
	atom_int32 threads_timeout;

	NETWORKER* (*net_worker_get)(NETSESSION_STATE* netsession);
} SERVER_STATE;

void server_init();
SERVER_STATE* server_get_state();

void server_startup(char* ipaddr, int port, NETWORKER* (*net_worker_get)(NETSESSION_STATE* netsession));
void server_shutdown();

int32 server_next_netthread_id();
int32 server_next_netsession_id();

void server_threads_timeout();
void server_threads_clean();

int32 get_server_netsessions_count_max();
int32 get_server_netsessions_count();

int32 get_server_netthreads_count_max();
int32 get_server_netthreads_count();

void server_netsession_clean();

NETSESSION_STATE* get_server_netsession(int indx);
