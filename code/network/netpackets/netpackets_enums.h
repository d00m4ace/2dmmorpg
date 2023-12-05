typedef enum
{
	NETPACKET_CLIENT_HI_ID = NETPACKETS_START_ID,
	NETPACKET_CLIENT_HANDSHAKE_ID,
	NETPACKET_SERVER_HI_ID,
	NETPACKET_SERVER_WELCOME_ID,
	NETPACKET_CLIENT_ECHO_ID,
	NETPACKET_SERVER_ECHO_ID,
	NETPACKET_GET_FILE_INFO_ID,
	NETPACKET_FILE_INFO_ID,
	NETPACKET_DOWNLOAD_FILE_ID,
	NETPACKET_ON_INFO_ID,
	NETPACKET_ON_IDLE_ID,
	NETPACKET_ON_UPDATE_ID,
	NETPACKET_ON_CONTINUE_ID,
	NETPACKET_ON_COMPLETE_ID,
	NETPACKET_DATASET_5000_ID,
	NETPACKET_DATASET_1000_ID,
	NETPACKET_DATASET_256_ID,
	NETPACKET_DATASET_128_ID,
	NETPACKET_DATASET_64_ID,
	NETPACKET_DATASET_32_ID,
	NETPACKET_DATASET_16_ID,
} ENUM_NETPACKETS;
