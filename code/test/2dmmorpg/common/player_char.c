void player_char_init(PLAYER_CHAR* pchar)
{
	pchar->user_id = -1;
	pchar->user_name[0] = '\0';
	pchar->user_password_hash[0] = '\0';

	pchar->user_level = 0;
	pchar->user_exp = 0;
	pchar->user_hp = 0;
	pchar->user_mp = 0;

	pchar->user_gold = 0;

	pchar->user_str = 0;
	pchar->user_sp = 0;

	pchar->user_atk = 0;
	pchar->user_def = 0;

	pchar->user_pos = (p32) { 0,0 };

	c_vec_init(&pchar->vec_netblob_send);
	c_vec_init(&pchar->vec_netblob_recv);
}

void player_char_deinit(PLAYER_CHAR* pchar)
{
	c_vec_foreach_ptr(&pchar->vec_netblob_send, NETPACKET_BLOB * np_blob)
	{
		c_vec_remove(&pchar->vec_netblob_send, np_blob);
		//netpacket_blob_deinit(np_blob);
	}

	c_vec_foreach_ptr(&pchar->vec_netblob_recv, NETPACKET_BLOB * np_blob)
	{
		c_vec_remove(&pchar->vec_netblob_recv, np_blob);
		//netpacket_blob_deinit(np_blob);
	}

	c_vec_deinit(&pchar->vec_netblob_send);
	c_vec_deinit(&pchar->vec_netblob_recv);
}

