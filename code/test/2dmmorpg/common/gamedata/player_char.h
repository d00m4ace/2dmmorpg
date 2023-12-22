typedef struct c_vec_of(NETPACKET_BLOB*) netblob_vec_t;

enum PLAYER_CHAR_NETWORK_STATE
{
	PLAYER_CHAR_NETWORK_STATE_NONE,
	PLAYER_CHAR_NETWORK_STATE_CONNECTED,
	PLAYER_CHAR_NETWORK_STATE_DISCONNECTED,
	PLAYER_CHAR_NETWORK_STATE_DISCONNECT,
};

typedef struct PLAYER_CHAR
{
	int32 user_id;
	char user_name[USER_NAME_MAX];
	char user_password_hash[USER_PASSWORD_MAX];

	//PLAYER_CHAR_NETWORK_STATE
	int32 user_network_state;

	int32 user_level;
	int32 user_exp;
	int32 user_hp;
	int32 user_mp;

	int32 user_gold;

	int32 user_str;
	int32 user_sp;

	int32 user_atk;
	int32 user_def;

	p32 user_pos;

	netblob_vec_t vec_netblob_send;
	netblob_vec_t vec_netblob_recv;

} PLAYER_CHAR;

typedef struct c_vec_of(PLAYER_CHAR*) player_char_vec_t;
player_char_vec_t vec_player_chars;
