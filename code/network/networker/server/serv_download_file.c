bool serv_dwnld_fl_req_on_get_fl_info(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);
bool serv_dwnld_fl_send_data(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);
bool serv_dwnld_fl_req_strt_dl_fl(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);

void serv_download_file_set(NETWORKER* networker, SERV_DOWNLOAD_FILE* serv)
{
	networker->worker_data = serv;
	networker->worker_job_id = NETWORKER_JOB_ID_DOWNLOAD_FILE;

	networker->on_request = serv_dwnld_fl_req_on_get_fl_info;
	networker->on_disconnect = serv_download_file_on_disconnect;
}

SERV_DOWNLOAD_FILE* serv_download_file_init(void)
{
	SERV_DOWNLOAD_FILE* serv = CALLOC(1, sizeof(SERV_DOWNLOAD_FILE));
	return serv;
}

NETWORKER* serv_download_file_get(void)
{
	NETWORKER* networker = serv_networker_get();
	SERV_DOWNLOAD_FILE* serv = serv_download_file_init();
	serv_download_file_set(networker, serv);
	return networker;
}

bool serv_download_file_on_disconnect(NETWORKER* networker, NETSESSION_STATE* netsession)
{
	SERV_DOWNLOAD_FILE* serv = networker->worker_data;

	if(serv->fp != NULL)
		fclose(serv->fp);
	serv->fp = NULL;

	networker_on_disconnect(networker, netsession);
	return true;
}


bool serv_dwnld_fl_req_on_get_fl_info(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req)
{
	NW_READ_PACKET_ID(req);
	SERV_DOWNLOAD_FILE* serv = networker->worker_data;

	NW_IF_PACKET(NETPACKET_GET_FILE_INFO)
	{
		NW_SET_READ(NETPACKET_GET_FILE_INFO, req_pack, req);

		int64 timestamp = win32_get_file_last_modified_timestamp(req_pack.file_name);

		NW_SET(NETPACKET_FILE_INFO, res_pack);
		strcpy(res_pack.file_name, req_pack.file_name);
		res_pack.modified_timestamp = timestamp;
		NW_WRITE_PUSH(NETPACKET_FILE_INFO, &res_pack);

		networker->on_request = serv_dwnld_fl_req_strt_dl_fl;
		return true;
	}

	return true;
}

bool serv_dwnld_fl_send_data(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req)
{
	NW_READ_PACKET_ID(req);
	SERV_DOWNLOAD_FILE* serv = networker->worker_data;

	NW_SET(NETPACKET_DATASET_5000, res_pack);
	res_pack.data_size = fread(res_pack.data, 1, sizeof(res_pack.data), serv->fp);
	serv->fp_size += res_pack.data_size;

	if(res_pack.data_size)
	{
		NW_WRITE_PUSH(NETPACKET_DATASET_5000, &res_pack);
		return true;
	}
	else
	{
		if(serv->fp != NULL)
			fclose(serv->fp);
		serv->fp = NULL;

		NW_WRITE_PUSH(NETPACKET_ON_COMPLETE, NULL);
		networker->worker_state_id = NETWORKER_STATE_ON_COMPLETE;
		PRINT("NETWORKER_STATE_ON_COMPLETE");

		networker->on_request = serv_download_file_on_request;
	}

	return true;
}

bool serv_dwnld_fl_req_strt_dl_fl(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req)
{
	NW_READ_PACKET_ID(req);
	SERV_DOWNLOAD_FILE* serv = networker->worker_data;

	NW_IF_PACKET(NETPACKET_DOWNLOAD_FILE)
	{
		NW_SET_READ(NETPACKET_DOWNLOAD_FILE, req_pack, req);
		strcpy(serv->serv_file_name, req_pack.file_name);
		serv->fp = fopen(serv->serv_file_name, "rb");  // r for read, b for binary
		serv->fp_size = 0;

		return serv_dwnld_fl_send_data(networker, netsession, req);
	}

	NW_IF_PACKET(NETPACKET_ON_CONTINUE)
	{
		return serv_dwnld_fl_send_data(networker, netsession, req);
	}

	return true;
}

bool serv_download_file_on_request(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req)
{
	NW_READ_PACKET_ID(req);
	SERV_DOWNLOAD_FILE* serv = networker->worker_data;

	{
		CFREE(networker->worker_data);
		networker->worker_data = NULL;

		SERV_IDLE* serv = CALLOC(1, sizeof(SERV_IDLE));
		serv_idle_set(networker, serv);
	}

	return true;
}
