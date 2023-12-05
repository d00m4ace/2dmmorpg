const char* common_netpackets_get_enum_string_by_id(int id)
{
	switch(id)
	{
		case PACKET_COMMON_ERROR_ID:
		return "PACKET_COMMON_ERROR_ID";
		case PACKET_COMMON_NOP_ID:
		return "PACKET_COMMON_NOP_ID";
		case PACKET_COMMON_BLOCK_START_ID:
		return "PACKET_COMMON_BLOCK_START_ID";
		case PACKET_COMMON_BLOCK_PART_ID:
		return "PACKET_COMMON_BLOCK_PART_ID";
		case PACKET_COMMON_BLOCK_END_ID:
		return "PACKET_COMMON_BLOCK_END_ID";
		case PACKET_COMMON_CHUNK_ID:
		return "PACKET_COMMON_CHUNK_ID";
		case PACKET_COMMON_PING_ID:
		return "PACKET_COMMON_PING_ID";
		case PACKET_COMMON_PONG_ID:
		return "PACKET_COMMON_PONG_ID";
		default:
		return "Unknown packet id";
	}
}
