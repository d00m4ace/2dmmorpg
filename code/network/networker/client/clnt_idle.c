void clnt_idle_set(NETWORKER* networker, CLNT_IDLE* clnt)
{
	networker->worker_data = clnt;
	networker->worker_job_id = NETWORKER_JOB_ID_IDLE;

	networker->on_request = clnt_idle_on_request;
	networker->on_disconnect = clnt_idle_on_disconnect;
}

CLNT_IDLE* clnt_idle_init(void)
{
	CLNT_IDLE* clnt = CALLOC(1, sizeof(CLNT_IDLE));
	return clnt;
}

NETWORKER* clnt_idle_get(void)
{
	NETWORKER* networker = clnt_networker_get();
	CLNT_IDLE* clnt = clnt_idle_init();
	clnt_idle_set(networker, clnt);

	return networker;
}

bool clnt_idle_on_disconnect(NETWORKER* networker, NETSESSION_STATE* netsession)
{
	CLNT_IDLE* clnt = networker->worker_data;

	CFREE(clnt->user_data);
	clnt->user_data = NULL;

	networker_on_disconnect(networker, netsession);
	return true;
}

bool clnt_idle_on_request(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req)
{
	NW_READ_PACKET_ID(req);
	CLNT_IDLE* clnt = networker->worker_data;

	//PRINT("session_id:%d CLNT_IDLE", netsession->session_id);

	NW_WRITE_PUSH(NETPACKET_ON_IDLE, NULL);

	return true;
}
