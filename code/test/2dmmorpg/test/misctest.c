{
	NETPACKET_BLOB* tdst[16] = { 1 };
	NETPACKET_BLOB* tsrc[16] = { 2 };

	for(size_t i = 0; i < 16; i++)
	{
		NETPACKET_BLOB* dst = new_netpacket_blob(2 + 2);
		netpacket_write_packet(dst, PACKET_COMMON_PING_ID, 4, NULL);
		NP_PUSH_TOP_BLOB(&player_char->vec_netblob_send, dst);

		tdst[i] = dst;
	}

	int i; int ic = 16;
	for(i = 0; i < c_vec_count(&player_char->vec_netblob_send); i++)
	{
		NETPACKET_BLOB* dst = NP_POP_BLOB(&player_char->vec_netblob_send);

		ic--;
		tsrc[ic] = dst;

		if(dst == NULL)
			break;

		i--;
	}

	int ikk = c_vec_count(&player_char->vec_netblob_send);
	int kk = 0;

			}