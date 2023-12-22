NETPACKET_BLOB* new_netpacket_blob(uint32 size)
{
	NETPACKET_BLOB* blob = CALLOC(1, sizeof(NETPACKET_BLOB));
	blob->data = CALLOC(1, size);
	blob->size = size;
	return blob;
}

void free_netpacket_blob(NETPACKET_BLOB* blob)
{
	if(blob)
	{
		CFREE(blob->data);
		CFREE(blob);
	}
}

NETPACKET_BLOB* netpacket_pop(netblob_vec_t* vec)
{
	if(c_vec_count(vec) == 0)
		return NULL;

	NETPACKET_BLOB* blob = (NETPACKET_BLOB*)c_vec_get(vec, 0);
	c_vec_remove_at(vec, 0);

	return blob;
}

void netpacket_push(netblob_vec_t* vec, NETPACKET_BLOB* blob)
{
	c_vec_push(vec, blob);
}

bool netpacket_write_packet_from_blob(NETPACKET_BLOB* dst, NETPACKET_BLOB* src)
{
	int packet_id = netpacket_read_packet_id(src);
	int packet_size = netpacket_read_packet_size(src);

	uint32 dst_pos = dst->pos;
	if(!netpacket_write_packet(dst, packet_id, packet_size, src->data + src->pos + 4))
		return false;
	dst->pos = dst_pos;

	return true;
}
