typedef struct NETWORKER NETWORKER;

void tcpclient_connect(NETSESSION_STATE* netsession, NETWORKER* net_worker, char* ipaddr, int port);
