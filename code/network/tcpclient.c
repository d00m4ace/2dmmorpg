////////////////////////////////////////////////////
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NONSTDC_NO_DEPRECATE
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <winsock2.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#include <strsafe.h>

#include <windows.h>
#endif
////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include <stdint.h>

#include "../config.h"

////////////////////////////////////////////////////
#ifdef USE_LINUX
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#endif
////////////////////////////////////////////////////

#include "../base/cstl.h"
#include "../base/types.h"
#include "../base/rnd.h"

#include "../threads/atomic.h"
#include "../threads/kernel.h"

#include "../network/netpackets.h"
#include "../network/netsocket.h"
#include "../network/netsession.h"

#include "../network/netpackets/common.h"
#include "../network/netpackets/netpackets.h"
#include "../network/netpackets/netpackets_enums.h"

#include "../network/networker/networker.h"

#include "iptools.h"
#include "tcpsocket.h"
#include "tcpclient.h"

#ifndef CLIENT_VERSION
#define CLIENT_VERSION 0x0100
#endif

#ifndef CLIENT_PLATFORM_ID
#define CLIENT_PLATFORM_ID 0x01
#endif

#ifndef CLIENT_SEND_LIMIT
#define CLIENT_SEND_LIMIT 250
#endif

KERNEL_THREAD_FUNC(thread_proc_tcpclient_listening);

void netsession_clean_packets(NETSESSION_STATE* netsession);

// need to call netsession_init(netsession) for init! 
void tcpclient_connect(NETSESSION_STATE* netsession, NETWORKER* net_worker, char* ipaddr, int port)
{
	strcpy(netsession->host, ipaddr);
	netsession->host_port = port;
	netsession->socket_state = NETSOCKET_NONE;
	netsession_clean_packets(netsession);

	netsession->user_data = net_worker;
	net_worker->socket_state = NETSOCKET_NONE;
	netsession->host_job_id = net_worker->worker_job_id;

	PRINT("Connecting to %s:%d", ipaddr, port);
	kernel_thread(thread_proc_tcpclient_listening, netsession);
}

void tcpclient_recv_send(NETSESSION_STATE* netsession, NETSOCKET_STATE* socket_state);

// has kernel_lock() 
KERNEL_THREAD_FUNC(thread_proc_tcpclient_listening)
{
	NETSOCKET_STATE socket_state = { 0 };
	NETSESSION_STATE* netsession = ((NETSESSION_STATE*)p_data);

	for(;;)
	{
		socket_state.socket = NETSOCKET_INVALID;

		netsocket client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if(client_socket == NETSOCKET_INVALID) break;

		socket_state.socket = client_socket;
		socket_state.state = NETSOCKET_CONNECTING;

		rnd_seed(netsession->session_id);

		{
			struct sockaddr_in serv_addr = { 0 };
			serv_addr.sin_family = AF_INET;
			kernel_lock();
			serv_addr.sin_port = htons(netsession->host_port);

#ifdef _WIN32
			serv_addr.sin_addr.S_un.S_addr = inet_addr(netsession->host);
#endif // _WIN32

#ifdef USE_LINUX
			serv_addr.sin_addr.s_addr = inet_addr(netsession->host);
#endif // _WIN32

			netsession->socket_state = NETSOCKET_CONNECTING;
			kernel_free();

			if(connect(socket_state.socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
			{
				PRINT("Failed connect to %s:%d", netsession->host, netsession->host_port);
				break;
			}

			PRINT("Connected to %s:%d", netsession->host, netsession->host_port);
			socket_state.state = NETSOCKET_CONNECTED;
		}

		if(0)
		{
			int rnd_sleep = rnd(0, 6000);
			PRINT("netsession:%d rnd_sleep:%d", netsession->session_id, rnd_sleep);
			kernel_sleep(rnd_sleep);
		}

		{
			NETPACKET_CLIENT_HI clnt_pack_hi = { CLIENT_VERSION, CLIENT_PLATFORM_ID, CLIENT_SEND_LIMIT, netsession->host_job_id };
			{
				NETPACKET_BLOB dst = { socket_state.data_send, 0, NETSOCKET_STATE_BUF_SIZE };
				if(!NETPACKET_CLIENT_HI_WRITE(&dst, &clnt_pack_hi)) break;
				if(!tcpsocket_send(&socket_state, 0)) break;
			}

			NETPACKET_SERVER_HI serv_pack_hi;
			{
				if(!tcpsocket_recv(&socket_state, 0)) break;
				NETPACKET_BLOB dst = { socket_state.data_recv, 0, NETSOCKET_STATE_BUF_SIZE };
				if(!NETPACKET_SERVER_HI_READ(&dst, &serv_pack_hi)) break;

				netsession->host_vers = serv_pack_hi.srvr_vers;
				netsession->host_platform_id = serv_pack_hi.srvr_platform_id;
				netsession->send_limit = serv_pack_hi.srvr_send_limit;
			}

			NETPACKET_CLIENT_HANDSHAKE clnt_pack_handshake = { 0 };
			{
				netsocket_handshake_func(serv_pack_hi.srvr_handshake, clnt_pack_handshake.clnt_handshake);
				NETPACKET_BLOB dst = { socket_state.data_send, 0, NETSOCKET_STATE_BUF_SIZE };
				if(!NETPACKET_CLIENT_HANDSHAKE_WRITE(&dst, &clnt_pack_handshake)) break;
				if(!tcpsocket_send(&socket_state, 0)) break;
			}

			/*
			{
				if(!tcpsocket_recv(&socket_state, 0)) break;
				NETPACKET_BLOB dst = { socket_state.data_recv, 0, NETSOCKET_STATE_BUF_SIZE };
				uint16 packet_id, packet_size;
				if(!netpacket_read_packet(&dst, &packet_id, &packet_size, NULL)) break;

				if(packet_id != NETPACKET_SERVER_WELCOME_ID)
				{
					PRINT("Server did not send PACKET_SERVER_WELCOME_ID packet_id:%d", packet_id);
					break;
				}
			}
			*/

			if(0)
			{
				int rnd_sleep = rnd(0, 6000);
				PRINT("netsession:%d rnd_sleep:%d", netsession->session_id, rnd_sleep);
				kernel_sleep(rnd_sleep);
			}

			atom_store(&netsession->socket_state, NETSOCKET_CONNECTED);

			kernel_lock();
			{
				NETWORKER* networker = netsession->user_data;
				networker->socket_state = NETSOCKET_CONNECTED;
				networker->session_id = netsession->session_id;
			}
			kernel_free();

			tcpclient_recv_send(netsession, &socket_state);

			break;
		}
	}

	//close_socket:
	PRINT("Connection closed to %s:%d", netsession->host, netsession->host_port);

	kernel_lock();
	netsession->socket_state = NETSOCKET_CLOSED;

	if(netsession->user_data)
	{
		NETWORKER* net_worker = netsession->user_data;
		net_worker->socket_state = NETSOCKET_CLOSED;

		net_worker->on_disconnect(net_worker, netsession);

		net_worker->session_id = -1;
		netsession->user_data = NULL;
	}
	kernel_free();

	tcpsocket_closesocket(&socket_state);

	return 0;
}

// has kernel_lock()
void tcpclient_recv_send(NETSESSION_STATE* netsession, NETSOCKET_STATE* socket_state)
{
	char packet_out[NETSESSION_STATE_BUF_SIZE];
	NETPACKET_BLOB dst = { socket_state->data_send, 0, NETSOCKET_STATE_BUF_SIZE };
	NETPACKET_BLOB src = { socket_state->data_recv, 0, NETSOCKET_STATE_BUF_SIZE };
	uint16 packet_id = 0, packet_size;

	for(;;)
	{
		if(!tcpsocket_recv(socket_state, 0)) break;

		{
			src.pos = 0;

			if(!netpacket_read_packet(&src, &packet_id, &packet_size, NULL)) break;

			if(packet_id != PACKET_COMMON_NOP_ID && packet_id != PACKET_COMMON_PING_ID)
			{
				src.pos = 0;
				if(!netsession_recv_packet(netsession, &src)) break;

				kernel_lock();
				NETPACKET_BLOB dt_out = { packet_out, 0, NETSESSION_STATE_BUF_SIZE };
				if(netsession_pop_packet(netsession, &dt_out, false))
				{
					if(netsession->user_data)
					{
						NETWORKER* net_worker = netsession->user_data;
						dt_out.pos = 0;

						if(!net_worker->on_request(net_worker, netsession, &dt_out)) break;
					}
				}
				kernel_free();
			}
		}

		{
			dst.pos = 0;

			if(packet_id == PACKET_COMMON_PING_ID)
			{
				if(!netpacket_write_packet(&dst, PACKET_COMMON_PONG_ID, 4, NULL)) break;
			}
			else if(!netsession_send_packet(netsession, &dst))
			{
				// no packets send OK
				if(netsession->flags & NETSESSION_FLAG_SHUTDOWN) break;

				if(!netpacket_write_packet(&dst, PACKET_COMMON_NOP_ID, 4, NULL)) break;
			}
		}

		if(!tcpsocket_send(socket_state, 0)) break;
	}
}

