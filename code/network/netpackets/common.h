#ifndef NETPACKETS_COMMON_H
#define NETPACKETS_COMMON_H

// netpacket: 
// 0 uint32 - packet size
// 4 uint32 - packet id
// 8 *void - packet data

typedef enum
{
	PACKET_COMMON_ERROR_ID = 0,
	PACKET_COMMON_NOP_ID,

	PACKET_COMMON_BLOCK_START_ID,
	PACKET_COMMON_BLOCK_PART_ID,
	PACKET_COMMON_BLOCK_END_ID,

	PACKET_COMMON_CHUNK_ID,

	PACKET_COMMON_PING_ID,
	PACKET_COMMON_PONG_ID,

	NETPACKETS_START_ID = 1024,
} ENUM_PACKET_COMMON_ID;


#define PACKET_COMMON_ERROR_MAXSIZE (4)
#define PACKET_COMMON_NOP_MAXSIZE (4)

#define PACKET_COMMON_BLOCK_MAXSIZE (NETSOCKET_STATE_BUF_SIZE)

#define PACKET_COMMON_CHUNK_MAXSIZE (240)

#define PACKET_COMMON_PING_MAXSIZE (4)
#define PACKET_COMMON_PONG_MAXSIZE (4)

const char* common_netpackets_get_enum_string_by_id(int id);

#endif