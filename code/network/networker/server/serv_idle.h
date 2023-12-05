typedef struct SERV_IDLE
{
	void* user_data;
} SERV_IDLE;

SERV_IDLE* serv_idle_init(void);
void serv_idle_set(NETWORKER* networker, SERV_IDLE* serv);
NETWORKER* serv_idle_get(void);

bool serv_idle_on_request(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);
bool serv_idle_on_disconnect(NETWORKER* networker, NETSESSION_STATE* netsession);