bool clnt_dwnld_fl_req_get_fl_info(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);
bool clnt_dwnld_fl_req_on_fl_info(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);
bool clnt_dwnld_fl_req_on_dl_fl(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);

void clnt_download_file_set(NETWORKER* networker, CLNT_DOWNLOAD_FILE* clnt)
{
	networker->worker_data = clnt;
	networker->worker_job_id = NETWORKER_JOB_ID_DOWNLOAD_FILE;

	networker->on_request = clnt_dwnld_fl_req_get_fl_info;
	networker->on_disconnect = clnt_download_file_on_disconnect;

	return networker;
}

CLNT_DOWNLOAD_FILE* clnt_download_file_init(void)
{
	CLNT_DOWNLOAD_FILE* clnt = CALLOC(1, sizeof(CLNT_DOWNLOAD_FILE));
	return clnt;
}

NETWORKER* clnt_download_file_get(char clnt_file_name[], char serv_file_name[])
{
	NETWORKER* networker = clnt_networker_get();
	CLNT_DOWNLOAD_FILE* clnt = clnt_download_file_init();

	strcpy(clnt->clnt_file_name, clnt_file_name);
	strcpy(clnt->serv_file_name, serv_file_name);

	clnt_download_file_set(networker, clnt);
	return networker;
}

bool clnt_download_file_on_disconnect(NETWORKER* networker, NETSESSION_STATE* netsession)
{
	CLNT_DOWNLOAD_FILE* clnt = networker->worker_data;

	if(clnt->fp != NULL)
		fclose(clnt->fp);
	clnt->fp = NULL;

	networker_on_disconnect(networker, netsession);
	return true;
}

bool clnt_dwnld_fl_req_get_fl_info(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);
bool clnt_dwnld_fl_req_on_fl_info(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);
bool clnt_dwnld_fl_req_on_dl_fl(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);

bool clnt_dwnld_fl_req_get_fl_info(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req)
{
	NW_READ_PACKET_ID(req);
	CLNT_DOWNLOAD_FILE* clnt = networker->worker_data;

	NW_SET(NETPACKET_GET_FILE_INFO, pack);
	strcpy(pack.file_name, clnt->serv_file_name);
	NW_WRITE_PUSH(NETPACKET_GET_FILE_INFO, &pack);

	networker->on_request = clnt_dwnld_fl_req_on_fl_info;

	return true;
}

bool clnt_dwnld_fl_req_on_fl_info(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req)
{
	NW_READ_PACKET_ID(req);
	CLNT_DOWNLOAD_FILE* clnt = networker->worker_data;

	NW_IF_PACKET(NETPACKET_FILE_INFO)
	{
		NW_SET_READ(NETPACKET_FILE_INFO, req_pack, req);

		clnt->modified_timestamp = req_pack.modified_timestamp;

		NW_SET(NETPACKET_DOWNLOAD_FILE, res_pack);
		strcpy(res_pack.file_name, req_pack.file_name);
		NW_WRITE_PUSH(NETPACKET_DOWNLOAD_FILE, &res_pack);

		networker->on_request = clnt_dwnld_fl_req_on_dl_fl;

		return true;
	}

	return true;
}

bool clnt_dwnld_fl_req_on_dl_fl(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req)
{
	NW_READ_PACKET_ID(req);
	CLNT_DOWNLOAD_FILE* clnt = networker->worker_data;

	NW_IF_PACKET(NETPACKET_DATASET_5000)
	{
		if(clnt->fp == NULL)
			clnt->fp = fopen(txt_format(clnt->clnt_file_name, netsession->session_id), "wb");  // r for read, b for binary

		NW_SET_READ(NETPACKET_DATASET_5000, req_pack, req);
		{
			int32 bytes_written = fwrite(req_pack.data, 1, req_pack.data_size, clnt->fp);
			clnt->fp_size += bytes_written;
		}
		NW_WRITE_PUSH(NETPACKET_ON_CONTINUE, NULL);

		return true;
	}

	NW_IF_PACKET(NETPACKET_ON_COMPLETE)
	{
		if(clnt->fp != NULL)
			fclose(clnt->fp);
		clnt->fp = NULL;

		NW_WRITE_PUSH(NETPACKET_ON_COMPLETE, NULL);
		networker->worker_state_id = NETWORKER_STATE_ON_COMPLETE;
		PRINT("NETWORKER_STATE_ON_COMPLETE");

		networker->on_request = clnt_download_file_on_request;

		return true;
	}

	return true;
}

bool clnt_download_file_on_request(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req)
{
	NW_READ_PACKET_ID(req);
	CLNT_DOWNLOAD_FILE* clnt = networker->worker_data;

	{
		CFREE(networker->worker_data);
		networker->worker_data = NULL;

		CLNT_IDLE* clnt = CALLOC(1, sizeof(CLNT_IDLE));
		clnt_idle_set(networker, clnt);
	}

	return true;
}
