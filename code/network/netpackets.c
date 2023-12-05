uint16 netpacket_read_array_uint8(NETPACKET_BLOB* blob, uint8* dst, uint16 size)
{
	if(blob->pos + size <= blob->size)
	{
		uint8* dst_end = dst + size;
		while(dst != dst_end)
			*dst++ = *(blob->data + blob->pos++);
		return size;
	}
	return 0;
}

uint16 netpacket_read_array_uint16(NETPACKET_BLOB* blob, uint16* dst, uint16 size) { return netpacket_read_array_uint8(blob, (uint8*)dst, 2 * size); }
uint16 netpacket_read_array_uint32(NETPACKET_BLOB* blob, uint32* dst, uint16 size) { return netpacket_read_array_uint8(blob, (uint8*)dst, 4 * size); }
uint16 netpacket_read_array_uint64(NETPACKET_BLOB* blob, uint64* dst, uint16 size) { return netpacket_read_array_uint8(blob, (uint8*)dst, 8 * size); }

uint16 netpacket_write_array_uint8(NETPACKET_BLOB* blob, uint8* src, uint16 size)
{
	if(blob->pos + size <= blob->size)
	{
		uint8* src_end = src + size;
		while(src != src_end)
			*(blob->data + blob->pos++) = *src++;
		return size;
	}
	return 0;
}

uint16 netpacket_write_array_uint16(NETPACKET_BLOB* blob, uint16* src, uint16 size) { return netpacket_write_array_uint8(blob, (uint8*)src, 2 * size); }
uint16 netpacket_write_array_uint32(NETPACKET_BLOB* blob, uint32* src, uint16 size) { return netpacket_write_array_uint8(blob, (uint8*)src, 4 * size); }
uint16 netpacket_write_array_uint64(NETPACKET_BLOB* blob, uint64* src, uint16 size) { return netpacket_write_array_uint8(blob, (uint8*)src, 8 * size); }

uint16 netpacket_read_uint8(NETPACKET_BLOB* blob, uint8* val) { return netpacket_read_array_uint8(blob, val, 1); }
uint16 netpacket_read_uint16(NETPACKET_BLOB* blob, uint16* val) { return netpacket_read_array_uint8(blob, (uint8*)val, 2); }
uint16 netpacket_read_uint32(NETPACKET_BLOB* blob, uint32* val) { return netpacket_read_array_uint8(blob, (uint8*)val, 4); }
uint16 netpacket_read_uint64(NETPACKET_BLOB* blob, uint64* val) { return netpacket_read_array_uint8(blob, (uint8*)val, 8); }

uint16 netpacket_write_uint8(NETPACKET_BLOB* blob, uint8* val) { return netpacket_write_array_uint8(blob, val, 1); }
uint16 netpacket_write_uint16(NETPACKET_BLOB* blob, uint16* val) { return netpacket_write_array_uint8(blob, (uint8*)val, 2); }
uint16 netpacket_write_uint32(NETPACKET_BLOB* blob, uint32* val) { return netpacket_write_array_uint8(blob, (uint8*)val, 4); }
uint16 netpacket_write_uint64(NETPACKET_BLOB* blob, uint64* val) { return netpacket_write_array_uint8(blob, (uint8*)val, 8); }

uint16 netpacket_skip_bytes(NETPACKET_BLOB* blob, uint16 skip_bytes)
{
	if(blob->pos + skip_bytes <= blob->size)
	{
		blob->pos += skip_bytes;
		return skip_bytes;
	}
	return 0;
}

bool netpacket_split(NETPACKET_BLOB* src, NETPACKET_BLOB* dst, uint16 size_limit)
{
	uint16 psize, pid = PACKET_COMMON_ERROR_ID;
	uint32 srcpos = src->pos;

	if(size_limit > PACKET_COMMON_BLOCK_MAXSIZE) return false;

	if(netpacket_read_uint16(src, &psize) == 0) return false;
	if(netpacket_read_uint16(src, &pid) == 0) return false;
	src->pos = srcpos;

	if(src->pos + psize > src->size)
		return false;

	if(dst->pos + psize > dst->size)
		return false;

	if(psize < size_limit)
	{
		memcpy(dst->data, src->data + srcpos, psize);
		src->pos += psize;
		dst->pos += psize;
		return true;
	}

	if(dst->pos + psize > dst->size)
		return false;

	if(NETMONITOR_SHOW_NETPACKETS)
	{
		uint16 packet_id = pid;
		PRINT("netpacket_split send: %s", netpackets_get_enum_string_by_id(packet_id));
	}

	uint16 block_size, block_id = PACKET_COMMON_BLOCK_START_ID;
	uint32 srcpos_end = srcpos + psize;

	while(src->pos < srcpos_end)
	{
		block_size = srcpos_end - src->pos;

		if(block_size > size_limit - 4)
		{
			block_size = size_limit - 4;

			if(src->pos != srcpos)
				block_id = PACKET_COMMON_BLOCK_PART_ID;
		}
		else
		{
			block_id = PACKET_COMMON_BLOCK_END_ID;
		}

		block_size += 4;

		if(src->pos + block_size > src->size)
			return false;

		if(dst->pos + block_size > dst->size)
			return false;

		netpacket_write_uint16(dst, &block_size);
		netpacket_write_uint16(dst, &block_id);

		netpacket_write_array_uint8(dst, src->data + src->pos, block_size - 4);

		src->pos += block_size - 4;

		if(block_id == PACKET_COMMON_BLOCK_END_ID)
			break;
	}

	return true;
}

bool netpacket_unsplit_ready(NETPACKET_BLOB* src)
{
	uint16 packet_id, packet_size;
	uint32 srcpos = src->pos;
	uint32 src_end_pos = 0;

	for(;;)
	{
		if(netpacket_read_packet(src, &packet_id, &packet_size, NULL))
		{
			if(packet_id == PACKET_COMMON_BLOCK_END_ID)
			{
				src_end_pos = src->pos;
				break;
			}
		}
		else
			break;
	}

	src->pos = srcpos;
	return (src_end_pos != 0);
}

bool netpacket_unsplit(NETPACKET_BLOB* src, NETPACKET_BLOB* dst)
{
	uint16 block_size, block_id = PACKET_COMMON_ERROR_ID;
	uint32 srcpos = src->pos;

	if(netpacket_read_uint16(src, &block_size) == 0) return false;
	if(netpacket_read_uint16(src, &block_id) == 0) return false;
	src->pos = srcpos;

	if(block_size > PACKET_COMMON_BLOCK_MAXSIZE) return false;

	if(src->pos + block_size > src->size)
		return false;

	if(dst->pos + block_size > dst->size)
		return false;

	if(block_id != PACKET_COMMON_BLOCK_START_ID)
	{
		memcpy(dst->data, src->data + srcpos, block_size);
		src->pos += block_size;
		dst->pos += block_size;
		return true;
	}

	src->pos += 4;

	while(block_id >= PACKET_COMMON_BLOCK_START_ID && block_id <= PACKET_COMMON_BLOCK_END_ID)
	{
		netpacket_write_array_uint8(dst, src->data + src->pos, block_size - 4);

		src->pos += block_size - 4;

		if(block_id == PACKET_COMMON_BLOCK_END_ID)
			break;

		if(netpacket_read_uint16(src, &block_size) == 0) return false;
		if(netpacket_read_uint16(src, &block_id) == 0) return false;

		if(block_size > PACKET_COMMON_BLOCK_MAXSIZE) return false;

		if(src->pos - 4 + block_size > src->size)
			return false;

		if(dst->pos - 4 + block_size > dst->size)
			return false;
	}

	if(NETMONITOR_SHOW_NETPACKETS)
	{
		uint16 packet_id = *((uint16*)dst->data + 1);
		PRINT("netpacket_split recv: %s", netpackets_get_enum_string_by_id(packet_id));
	}

	return true;
}

bool netpacket_write_packet(NETPACKET_BLOB* dst, uint16 packet_id, uint16 packet_size, uint8* data)
{
	if(dst->pos + packet_size > dst->size)
		return false;

	uint32 srcpos = dst->pos;

	if(netpacket_write_uint16(dst, &packet_size) == 0) return false;
	if(netpacket_write_uint16(dst, &packet_id) == 0) return false;

	if(data != NULL)
		memcpy(dst->data + dst->pos, data, packet_size - 4);

	dst->pos = srcpos + packet_size;

	return true;
}

bool netpacket_read_packet(NETPACKET_BLOB* src, uint16* packet_id, uint16* packet_size, uint8* data)
{
	uint32 srcpos = src->pos;

	if(netpacket_read_uint16(src, packet_size) == 0) return false;
	if(netpacket_read_uint16(src, packet_id) == 0) return false;

	if(srcpos + *packet_size > src->size)
		return false;

	if(data != NULL)
		memcpy(data, src->data + src->pos, (*packet_size) - 4);

	src->pos = srcpos + *packet_size;

	return true;
}

int netpacket_read_packet_id(NETPACKET_BLOB* src)
{
	uint16 packet_id;

	uint32 srcpos = src->pos;
	src->pos += 2;
	if(netpacket_read_uint16(src, &packet_id) == 0) return -1;
	src->pos = srcpos;

	return packet_id;
}

int netpacket_read_packet_size(NETPACKET_BLOB* src)
{
	uint16 packet_size;

	uint32 srcpos = src->pos;
	if(netpacket_read_uint16(src, &packet_size) == 0) return -1;
	src->pos = srcpos;

	return packet_size;
}

uint16 netpacket_calc_bits(uint8* src, uint32 src_len)
{
	uint16 bits = 0;

	for(uint16 i = 0; i < src_len; i++, src++)
		for(uint8 c = 0; c < 8; c++)
			if(*src & (1U << c))
				bits++;

	return bits;
}
