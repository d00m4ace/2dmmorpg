PCHAR_NETWORKER* pchar_networker_init(void)
{
	PCHAR_NETWORKER* pchar_networker = (PCHAR_NETWORKER*)CALLOC(1, sizeof(PCHAR_NETWORKER));
	return pchar_networker;
}

NETWORKER* clnt_pchar_networker_get(void)
{
	NETWORKER* networker = clnt_networker_get();
	PCHAR_NETWORKER* pchar_networker = pchar_networker_init();
	pchar_networker_set(networker, pchar_networker);

	return networker;
}

NETWORKER* serv_pchar_networker_get(void)
{
	NETWORKER* networker = serv_networker_get();
	PCHAR_NETWORKER* pchar_networker = pchar_networker_init();
	pchar_networker_set(networker, pchar_networker);

	return networker;
}

void pchar_networker_set(NETWORKER* networker, PCHAR_NETWORKER* pchar_networker)
{
	networker->worker_data = pchar_networker;
	networker->worker_job_id = NETWORKER_JOB_ID_PCHAR;

	networker->on_request = pchar_networker_on_request;
	networker->on_disconnect = pchar_networker_on_disconnect;

	PLAYER_CHAR* player_char = CALLOC(1, sizeof(PLAYER_CHAR));
	player_char_init(player_char);

	pchar_networker->user_data = player_char;

	c_vec_push(&vec_player_chars, player_char);
}

bool pchar_networker_on_disconnect(NETWORKER* networker, NETSESSION_STATE* netsession)
{
	PCHAR_NETWORKER* pchar_networker = networker->worker_data;
	PLAYER_CHAR* player_char = pchar_networker->user_data;

	player_char->user_network_state = PLAYER_CHAR_NETWORK_STATE_DISCONNECTED;

	pchar_networker->user_data = NULL;

	networker_on_disconnect(networker, netsession);
	return true;
}

bool pchar_networker_on_request(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req)
{
	NW_READ_PACKET_ID(req);

	PCHAR_NETWORKER* pchar_networker = networker->worker_data;
	PLAYER_CHAR* player_char = pchar_networker->user_data;

	if(player_char->user_network_state == PLAYER_CHAR_NETWORK_STATE_NONE)
		player_char->user_network_state = PLAYER_CHAR_NETWORK_STATE_CONNECTED;

	if(player_char->user_network_state == PLAYER_CHAR_NETWORK_STATE_DISCONNECT)
		return false;

	NW_IF_NOT_PACKET(NETPACKET_ON_IDLE)
	{
		NETPACKET_BLOB* blb = new_netpacket_blob(netpacket_read_packet_size(req));

		if(!netpacket_write_packet_from_blob(blb, req))
		{
			NP_BLOB_FREE(blb);
			return false;
		}

		NP_PUSH_BLOB(&player_char->vec_netblob_recv, blb);
	}

	if(c_vec_count(&player_char->vec_netblob_send))
	{
		NETPACKET_BLOB* blb = netpacket_pop(&player_char->vec_netblob_send);

		if(!netsession_push_packet(netsession, blb, false))
		{
			NP_BLOB_FREE(blb);
			return false;
		}

		NP_BLOB_FREE(blb);
	}
	else
	{
		NW_WRITE_PUSH(NETPACKET_ON_IDLE, NULL);
	}

	//PRINT("session_id:%d PCHAR_NETWORKER", netsession->session_id);

	return true;
}
