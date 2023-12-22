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
	for(int i = 0; i < c_vec_count(&pchar->vec_netblob_send); i++)
		NP_BLOB_FREE(c_vec_get(&pchar->vec_netblob_send, i));

	for(int i = 0; i < c_vec_count(&pchar->vec_netblob_recv); i++)
		NP_BLOB_FREE(c_vec_get(&pchar->vec_netblob_recv, i));

	c_vec_clear(&pchar->vec_netblob_send);
	c_vec_clear(&pchar->vec_netblob_recv);

	c_vec_deinit(&pchar->vec_netblob_send);
	c_vec_deinit(&pchar->vec_netblob_recv);
}
