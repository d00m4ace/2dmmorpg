void serv_idle_set(NETWORKER* networker, SERV_IDLE* serv)
{
	networker->worker_data = serv;
	networker->worker_job_id = NETWORKER_JOB_ID_IDLE;

	networker->on_request = serv_idle_on_request;
	networker->on_disconnect = serv_idle_on_disconnect;
}

SERV_IDLE* serv_idle_init(void)
{
	SERV_IDLE* serv = CALLOC(1, sizeof(SERV_IDLE));
	return serv;
}

NETWORKER* serv_idle_get(void)
{
	NETWORKER* networker = serv_networker_get();
	SERV_IDLE* serv = serv_idle_init();
	serv_idle_set(networker, serv);

	return networker;
}

bool serv_idle_on_disconnect(NETWORKER* networker, NETSESSION_STATE* netsession)
{
	SERV_IDLE* serv = networker->worker_data;

	CFREE(serv->user_data);
	serv->user_data = NULL;

	networker_on_disconnect(networker, netsession);
	return true;
}

bool serv_idle_on_request(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req)
{
	NW_READ_PACKET_ID(req);

	SERV_IDLE* serv = networker->worker_data;

	//PRINT("session_id:%d SERV_IDLE", netsession->session_id);

	NW_WRITE_PUSH(NETPACKET_ON_IDLE, NULL);

	return true;
}
