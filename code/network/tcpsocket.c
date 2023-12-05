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
#pragma comment(lib,"WS2_32.lib")
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

#include "iptools.h"
#include "tcpsocket.h"

//#define USE_TEST_RND_LIMIT

void tcpsocket_init()
{
#if defined(_WIN32)
	// initialization WS2_32.dll
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2, 2);
	WSAStartup(sockVersion, &wsaData); // Asked for a 2.2 Version of socket	
#endif
}

void tcpsocket_free()
{
#if defined(_WIN32)
	// Release WS2_32 library 
	WSACleanup();
#endif
}

bool tcpsocket_recv(NETSOCKET_STATE* socket_state, uint16 max_recv)
{
	if(max_recv == 0)
		max_recv = NETSOCKET_STATE_BUF_SIZE;

	socket_state->recved = 0;
	socket_state->recv_total = max_recv;

	while(socket_state->recv_total - socket_state->recved)
	{
		int recv_len = socket_state->recv_total - socket_state->recved;
#ifdef USE_TEST_RND_LIMIT
		recv_len = rnd(1, recv_len);
#endif // USE_TEST_RND_LIMIT

		int recv_bytes = recv(socket_state->socket, socket_state->data_recv + socket_state->recved, recv_len, 0);

		if(recv_bytes == 0) // If the connection has been gracefully closed, the return value is zero.
		{
			tcpsocket_closesocket(socket_state);
			return false;
		}

		if(recv_bytes == SOCKET_ERROR) // value of SOCKET_ERROR is returned, and a specific error code can be retrieved by calling WSAGetLastError.
		{
			tcpsocket_closesocket(socket_state);
			return false;
		}

		if(socket_state->recved + recv_bytes > 1)
		{
			uint16 packet_size = *((uint16*)socket_state->data_recv);

			if(packet_size > max_recv)
			{
				tcpsocket_closesocket(socket_state);
				return false;
			}

			socket_state->recv_total = packet_size;
		}

		socket_state->recved += recv_bytes;
	}

	if(NETMONITOR_SHOW_NETPACKETS)
	{
		uint16 packet_id = *((uint16*)socket_state->data_recv + 1);
		if(!(NETMONITOR_SKIP_PACKET_COMMON_NOP_ID && packet_id == PACKET_COMMON_NOP_ID))
			PRINT("tcpsocket:%lld recv: %s", socket_state->socket, netpackets_get_enum_string_by_id(packet_id));
	}

	return true;
}

bool tcpsocket_send(NETSOCKET_STATE* socket_state, uint16 max_send)
{
	if(max_send == 0)
		max_send = *((uint16*)socket_state->data_send);

	socket_state->sended = 0;
	socket_state->send_total = max_send;

	while(socket_state->send_total - socket_state->sended)
	{
		int send_len = socket_state->send_total - socket_state->sended;
#ifdef USE_TEST_RND_LIMIT
		send_len = rnd(1, send_len);
#endif // USE_TEST_RND_LIMIT

		int send_bytes = send(socket_state->socket, socket_state->data_send + socket_state->sended, send_len, 0);

		if(send_bytes == SOCKET_ERROR) // value of SOCKET_ERROR is returned, and a specific error code can be retrieved by calling WSAGetLastError.
		{
			tcpsocket_closesocket(socket_state);
			return false;
		}

		socket_state->sended += send_bytes;
	}

	if(NETMONITOR_SHOW_NETPACKETS)
	{
		uint16 packet_id = *((uint16*)socket_state->data_send + 1);
		if(!(NETMONITOR_SKIP_PACKET_COMMON_NOP_ID && packet_id == PACKET_COMMON_NOP_ID))
			PRINT("tcpsocket:%lld send: %s", socket_state->socket, netpackets_get_enum_string_by_id(packet_id));
	}

	return true;
}

uint16 tcpsocket_recv_packet_size(NETSOCKET_STATE* socket_state)
{
	return *((uint16*)socket_state->data_recv);
}

uint16 tcpsocket_send_packet_size(NETSOCKET_STATE* socket_state)
{
	return *((uint16*)socket_state->data_send);
}

void tcpsocket_closesocket(NETSOCKET_STATE* socket_state)
{
	if(socket_state->socket != NETSOCKET_INVALID)
		closesocket(socket_state->socket);
	socket_state->socket = NETSOCKET_INVALID;
	socket_state->state = NETSOCKET_CLOSED;
}
