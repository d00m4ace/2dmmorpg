typedef struct CLNT_IDLE
{
	void* user_data;
} CLNT_IDLE;

CLNT_IDLE* clnt_idle_init(void);
void clnt_idle_set(NETWORKER* networker, CLNT_IDLE* serv);
NETWORKER* clnt_idle_get(void);

bool clnt_idle_on_request(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);
bool clnt_idle_on_disconnect(NETWORKER* networker, NETSESSION_STATE* netsession);