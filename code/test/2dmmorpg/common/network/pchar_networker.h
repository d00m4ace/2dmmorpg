typedef struct PCHAR_NETWORKER
{
	void* user_data;
} PCHAR_NETWORKER;

PCHAR_NETWORKER* pchar_networker_init(void);
void pchar_networker_set(NETWORKER* networker, PCHAR_NETWORKER* serv);

bool pchar_networker_on_request(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);
bool pchar_networker_on_disconnect(NETWORKER* networker, NETSESSION_STATE* netsession);

NETWORKER* clnt_pchar_networker_get(void);
NETWORKER* serv_pchar_networker_get(void);

