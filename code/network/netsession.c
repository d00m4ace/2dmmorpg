//netsession

#ifndef NETSESSION_STATE_BUF_SIZE
#define NETSESSION_STATE_BUF_SIZE (1024*63)
#endif

bool tcpsocket_recv(NETSOCKET_STATE* socket_state, uint16 max_recv);
bool tcpsocket_send(NETSOCKET_STATE* socket_state, uint16 max_send);

uint16 tcpsocket_recv_packet_size(NETSOCKET_STATE* socket_state);
uint16 tcpsocket_send_packet_size(NETSOCKET_STATE* socket_state);

#define USE_ATOMIC_CMP

void netsession_init(NETSESSION_STATE* netsession)
{
	memset(netsession, 0, sizeof(NETSESSION_STATE));
	netsession->session_id = -1;
	netsession->socket_state = NETSOCKET_NONE;
}

void netsession_clean_packets(NETSESSION_STATE* netsession)
{
	atom_store(&netsession->recved, 0);
	atom_store(&netsession->recv_total, 0);
	atom_store(&netsession->sended, 0);
	atom_store(&netsession->send_total, 0);
}

void netsession_shutdown(NETSESSION_STATE* netsession)
{
	atom_or(&netsession->flags, NETSESSION_FLAG_SHUTDOWN);
}

// has kernel_lock() if do_lock
bool netsession_push_packet(NETSESSION_STATE* netsession, NETPACKET_BLOB* src, bool do_lock)
{
	if(do_lock)
		kernel_lock();

	NETPACKET_BLOB dst = { netsession->data_send + netsession->sended, 0, (NETSESSION_STATE_BUF_SIZE - netsession->sended) };

	src->pos = 0;
	bool rb = netpacket_split(src, &dst, netsession->send_limit);

	if(rb)
		netsession->sended += dst.pos;

	if(do_lock)
		kernel_free();
	return rb;
}

// has kernel_lock()
bool netsession_send_packet(NETSESSION_STATE* netsession, NETPACKET_BLOB* dst)
{
#ifdef USE_ATOMIC_CMP
	if(atom_cmp(&netsession->sended, 0))
		return false;
#endif

	kernel_lock();

	NETPACKET_BLOB src = { netsession->data_send, 0, netsession->sended };

	uint16 packet_id, packet_size;

	if(!netpacket_read_packet(&src, &packet_id, &packet_size, NULL))
	{
		kernel_free();
		return false;
	}

	if(packet_id == PACKET_COMMON_PING_ID)
		netsession->time_ping_send = hal_time_ms();

	src.pos = 0;

	assert(packet_size <= dst->size);

	memcpy(dst->data, src.data, packet_size);
	dst->pos = packet_size;

	netsession->sended -= packet_size;

	if(netsession->sended)
		memmove(netsession->data_send, netsession->data_send + packet_size, netsession->sended);

	kernel_free();
	return true;
}

// has kernel_lock()
bool netsession_recv_packet(NETSESSION_STATE* netsession, NETPACKET_BLOB* src)
{
	kernel_lock();

	NETPACKET_BLOB dst = { netsession->data_recv + netsession->recved, 0, (NETSESSION_STATE_BUF_SIZE - netsession->recved) };

	uint16 packet_id, packet_size;

	if(!netpacket_read_packet(src, &packet_id, &packet_size, NULL))
	{
		kernel_free();
		return false;
	}

	if(packet_id == PACKET_COMMON_PONG_ID)
		netsession->time_ping = hal_time_ms() - netsession->time_ping_send;

	src->pos = 0;

	if(packet_size > dst.size)
	{
		atom_or(&netsession->flags, NETSESSION_FLAG_RECV_OVERFLOW);
		kernel_free();
		return false;
	}

	assert(packet_size <= dst.size);

	memcpy(dst.data, src->data, packet_size);
	src->pos = packet_size;

	netsession->recved += packet_size;

	kernel_free();
	return true;
}

// has kernel_lock() if do_lock
bool netsession_pop_packet(NETSESSION_STATE* netsession, NETPACKET_BLOB* dst, bool do_lock)
{
#ifdef USE_ATOMIC_CMP
	if(atom_cmp(&netsession->recved, 0))
		return false;
#endif

	if(do_lock)
		kernel_lock();

	NETPACKET_BLOB src = { netsession->data_recv, 0, netsession->recved };

	uint16 packet_id, packet_size;

	if(!netpacket_read_packet(&src, &packet_id, &packet_size, NULL))
	{
		if(do_lock)
			kernel_free();
		return false;
	}

	if(packet_id == PACKET_COMMON_BLOCK_START_ID)
	{
		src.pos = 0;
		if(!netpacket_unsplit_ready(&src))
		{
			if(do_lock)
				kernel_free();
			return false;
		}
	}

	dst->pos = 0;
	src.pos = 0;
	bool rb = netpacket_unsplit(&src, dst);

	if(rb)
	{
		netsession->recved -= src.pos;

		if(netsession->recved)
			memmove(netsession->data_recv, netsession->data_recv + src.pos, netsession->recved);
	}

	if(do_lock)
		kernel_free();
	return rb;
}
