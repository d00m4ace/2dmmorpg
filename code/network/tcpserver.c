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
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <errno.h>
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
#include "tcpserver.h"

////////////////////////////////////////////////////
#include "tcpserverman.c"
////////////////////////////////////////////////////

////////////////////////////////////////////////////
KERNEL_THREAD_FUNC(thread_proc_server_listening);
KERNEL_THREAD_FUNC(thread_proc_client_listening);
////////////////////////////////////////////////////

void server_startup(char* ipaddr, int port, NETWORKER* (*net_worker_get)(NETSESSION_STATE* netsession))
{
	memset(&server, 0, sizeof(SERVER_STATE));

	server.nop_sleep_time_ms = DEFAULT_LOOP_BACK_NOP_TIME_MS;
	server.threads_timeout = DEFAULT_THREADS_TIMEOUT;

	server.net_worker_get = net_worker_get;

	if(ipaddr == NULL) ipaddr = "localhost";

	strcpy(server.host, ipaddr);
	server.port = port;

	if(strcmp(ipaddr, "localhost") == 0) strcpy(server.host, iptools_host_ip());

	struct addrinfo hints;
	struct addrinfo* local = NULL;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = ((ipaddr) ? 0 : AI_PASSIVE);

	char port_str[10];
	sprintf(port_str, "%d", server.port);

	// Resolve the server address and port
	if(getaddrinfo(ipaddr, port_str, &hints, &local) != 0)
	{
		server_error_handling("getaddrinfo failed");
		return;
	}

	// Create a SOCKET for connecting to server
	if((server.listen_socket = socket(local->ai_family, local->ai_socktype, local->ai_protocol)) == INVALID_SOCKET)
	{
		freeaddrinfo(local);
		server_error_handling("build listen socket failed");
		return;
	}

	{

#ifdef USE_LINUX
		//linux way
		struct timeval timeout;
		timeout.tv_sec = DEFAULT_SOCKET_TIMEOUT_MS / 1000;
		timeout.tv_usec = 0;
#endif

#ifdef _WIN32
		int timeout = DEFAULT_SOCKET_TIMEOUT_MS; //ms
#endif // _WIN32		

		if(setsockopt(server.listen_socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)))
			PRINT("setsockopt failed");
		if(setsockopt(server.listen_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)))
			PRINT("setsockopt failed");
	}

	{
		// Disable send buffering on the socket. Setting SO_SNDBUF
		// to 0 causes winsock to stop buffering sends and perform
		// sends directly from our buffers, thereby save one memory copy.
		//
		// However, this does prevent the socket from ever filling the
		// send pipeline. This can lead to packets being sent that are
		// not full (i.e. the overhead of the IP and TCP headers is 
		// great compared to the amount of data being carried).
		//
		// Disabling the send buffer has less serious repercussions 
		// than disabling the receive buffer.
		int nRet = 0;
		int nZero = 0;
		nRet = setsockopt(server.listen_socket, SOL_SOCKET, SO_SNDBUF, (char*)&nZero, sizeof(nZero));
		if(nRet == SOCKET_ERROR)
		{
			PRINT("setsockopt(SNDBUF) failed");
			freeaddrinfo(local);
			server_error_handling("build listen socket failed");
			return;
		}
	}

	if(strcmp(ipaddr, "localhost") == 0)
	{
		struct sockaddr_in serv_addr = { 0 };
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(server.port);

		// Setup the TCP listening socket
		if(bind(server.listen_socket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR)
		{
			freeaddrinfo(local);
			server_error_handling("listen socket bind failed");
			return;
		}
	}
	else
	{
		// Setup the TCP listening socket
		if(bind(server.listen_socket, local->ai_addr, (int)local->ai_addrlen) == SOCKET_ERROR)
		{
			freeaddrinfo(local);
			server_error_handling("listen socket bind failed");
			return;
		}
	}

	if(listen(server.listen_socket, DEFAULT_BACKLOG) == SOCKET_ERROR)
	{
		freeaddrinfo(local);
		server_error_handling("socket listening failed");
		return;
	}

	// Don't need the local interface anymore
	if(local != NULL)
		freeaddrinfo(local);

	PRINT("Server listening on %s:%d", server.host, server.port);
	kernel_thread(thread_proc_server_listening, &server.listen_socket);
}

#ifdef USE_LINUX
#define SOCKADDR struct sockaddr
#endif // USE_LINUX

// has kernel_lock() 
KERNEL_THREAD_FUNC(thread_proc_server_listening)
{
	netsocket listen_socket = *((netsocket*)p_data);

	struct sockaddr_in remote_addr;
	int remote_addr_size = sizeof(remote_addr);

	netsocket client_socket;

	//set opt timeout here

	while(true)
	{
		// Accept a new connection 
		client_socket = accept(listen_socket, (struct sockaddr*)&remote_addr, &remote_addr_size);

		if(client_socket == INVALID_SOCKET)
		{
			//PRINT("Failed accept()");
			continue;
		}

		{
			// Disable send buffering on the socket. Setting SO_SNDBUF
			// to 0 causes winsock to stop buffering sends and perform
			// sends directly from our buffers, thereby save one memory copy.
			//
			// However, this does prevent the socket from ever filling the
			// send pipeline. This can lead to packets being sent that are
			// not full (i.e. the overhead of the IP and TCP headers is 
			// great compared to the amount of data being carried).
			//
			// Disabling the send buffer has less serious repercussions 
			// than disabling the receive buffer.
			int nRet = 0;
			int nZero = 0;
			nRet = setsockopt(client_socket, SOL_SOCKET, SO_SNDBUF, (char*)&nZero, sizeof(nZero));
			if(nRet == SOCKET_ERROR)
			{
				PRINT("setsockopt(SNDBUF) failed");
			}
		}

		kernel_lock();

		// TODO: SEARCH FOR FREE SLOT
		SERVER_NETTHREAD_STATE* netthread;

		int thread_id = server_next_netthread_id();

		if(thread_id == -1)
		{
			PRINT("No free netthread for new connection.");
			kernel_free();
			closesocket(client_socket);
			continue;
		}

		netthread = &server_netthreads[thread_id];
		netthread->time_accept = netthread->time_last_io = clock();

		netthread->socket = client_socket;
		netthread->session_id = -1;

		assert(remote_addr_size == SOCKET_REMOTE_ADDR_SIZE);
		memcpy(&netthread->remote_addr, &remote_addr, remote_addr_size);

		PRINT("New connection %s:%d server_threads_count: %ld", inet_ntoa(remote_addr.sin_addr), remote_addr.sin_port, get_server_netthreads_count());

		kernel_free();

		kernel_thread(thread_proc_client_listening, netthread);
	}

	return 0;
}

// has kernel_lock() 
KERNEL_THREAD_FUNC(thread_proc_client_listening)
{
	NETSOCKET_STATE socket_state = { 0 };
	SERVER_NETTHREAD_STATE* netthread;

	kernel_lock();
	netthread = ((SERVER_NETTHREAD_STATE*)p_data);
	socket_state.socket = netthread->socket;
	socket_state.state = NETSOCKET_CONNECTED;
	kernel_free();

	rnd_seed(0);

	for(;;)
	{
		{
			NETPACKET_CLIENT_HI clnt_pack_hi;
			{
				if(!tcpsocket_recv(&socket_state, NETPACKET_CLIENT_HI_SIZE())) break;
				NETPACKET_BLOB dst = { socket_state.data_recv, 0, NETSOCKET_STATE_BUF_SIZE };
				if(!NETPACKET_CLIENT_HI_READ(&dst, &clnt_pack_hi)) break;
			}

			NETPACKET_SERVER_HI serv_pack_hi = { SERVER_VERSION, SERVER_PLATFORM_ID, SERVER_SEND_LIMIT, {rnd_u16(),rnd_u16(),rnd_u16(),rnd_u16(),} };
			{
				NETPACKET_BLOB dst = { socket_state.data_send, 0, NETSOCKET_STATE_BUF_SIZE };
				if(!NETPACKET_SERVER_HI_WRITE(&dst, &serv_pack_hi)) break;
				if(!tcpsocket_send(&socket_state, 0)) break;
			}

			NETPACKET_CLIENT_HANDSHAKE clnt_pack_handshake;
			{
				if(!tcpsocket_recv(&socket_state, NETPACKET_CLIENT_HANDSHAKE_SIZE())) break;
				NETPACKET_BLOB dst = { socket_state.data_recv, 0, NETSOCKET_STATE_BUF_SIZE };
				if(!NETPACKET_CLIENT_HANDSHAKE_READ(&dst, &clnt_pack_handshake)) break;
			}

			{
				uint16 digit_handshake[4];
				netsocket_handshake_func(serv_pack_hi.srvr_handshake, digit_handshake);
				if(!netsocket_handshake_cmp(digit_handshake, clnt_pack_handshake.clnt_handshake)) break;
			}

			{
				NETPACKET_BLOB dst = { socket_state.data_send, 0, NETSOCKET_STATE_BUF_SIZE };
				if(!NETPACKET_SERVER_WELCOME_WRITE(&dst, &serv_pack_hi)) break;
				if(!tcpsocket_send(&socket_state, 0)) break;
			}

			if(!server_bind_netthread_netsession(netthread)) break;

			NETSESSION_STATE* netsession;
			if(!server_get_netthread_netsession(netthread, &netsession)) break;

			{
				netsession->send_limit = clnt_pack_hi.clnt_send_limit;
				netsession->host_platform_id = clnt_pack_hi.clnt_platform_id;
				netsession->host_vers = clnt_pack_hi.clnt_vers;
				netsession->host_job_id = clnt_pack_hi.clnt_job_id;
				atom_store(&netsession->socket_state, NETSOCKET_CONNECTED);

				kernel_lock();
				{
					NETWORKER* networker = server.net_worker_get(netsession);
					netsession->user_data = networker;
					networker->socket_state = NETSOCKET_CONNECTED;
					networker->session_id = netsession->session_id;
				}
				kernel_free();

				//PRINT("clnt_phys_addr:%s", iptools_phys_addr_to_str(netsession->host_phys_addr));
			}

			atom_store(&netthread->time_last_io, clock());

			{
				char packet_out[NETSESSION_STATE_BUF_SIZE];
				NETPACKET_BLOB src = { socket_state.data_recv, 0, NETSOCKET_STATE_BUF_SIZE };
				NETPACKET_BLOB dst = { socket_state.data_send, 0, NETSOCKET_STATE_BUF_SIZE };
				uint16 packet_id, packet_size;

				for(;;)
				{
					if(!tcpsocket_recv(&socket_state, 0)) break;

					atom_store(&netthread->time_last_io, clock());

					{
						src.pos = 0;
						if(!netpacket_read_packet(&src, &packet_id, &packet_size, NULL)) break;

						if(packet_id == PACKET_COMMON_PING_ID)
						{
							if(!(netsession->flags & NETSESSION_FLAG_SHUTDOWN))
							{
								dst.pos = 0;
								if(!netpacket_write_packet(&dst, PACKET_COMMON_PONG_ID, 4, NULL)) break;
								if(!tcpsocket_send(&socket_state, 0)) break;
								continue;
							}
						}
						else if(packet_id != PACKET_COMMON_NOP_ID)
						{
							src.pos = 0;
							if(!netsession_recv_packet(netsession, &src)) break;

							atom_add(&server.recv_total, src.pos);
							atom_add(&netsession->recv_total, src.pos);

							kernel_lock();
							NETPACKET_BLOB dt_out = { packet_out, 0, NETSESSION_STATE_BUF_SIZE };
							if(netsession_pop_packet(netsession, &dt_out, false))
							{
								if(netsession->user_data)
								{
									NETWORKER* networker = netsession->user_data;
									dt_out.pos = 0;

									if(!networker->on_request(networker, netsession, &dt_out)) break;
								}
							}
							kernel_free();
						}
						else
						{
							atom_add(&server.recv_nop_total, src.pos);
						}
					}

					{
						dst.pos = 0;
						if(!netsession_send_packet(netsession, &dst))
						{
							// no packets send OK
							if(netsession->flags & NETSESSION_FLAG_SHUTDOWN) break;

							if(!netpacket_write_packet(&dst, PACKET_COMMON_NOP_ID, 4, NULL)) break;

							if(server.nop_sleep_time_ms)
								kernel_sleep(server.nop_sleep_time_ms);

							atom_add(&server.send_nop_total, dst.pos);
						}
						else
						{
							atom_add(&server.send_total, dst.pos);
							atom_add(&netsession->send_total, dst.pos);
						}
					}

					if(!tcpsocket_send(&socket_state, 0)) break;
				}
			}
		}

		break;
	}

	//close_socket:

	kernel_lock();

	{
		struct sockaddr_in* remote_addr = (struct sockaddr_in*)netthread->remote_addr;
		PRINT("Connection closed to %s:%d", inet_ntoa(remote_addr->sin_addr), remote_addr->sin_port);
	}

	if(netthread->socket != NETSOCKET_INVALID)
	{
		// Close the connection to the client 
		tcpsocket_closesocket(&socket_state);
	}

	netthread->socket = NETSOCKET_INVALID;

	{
		NETSESSION_STATE* netsession;
		if(server_get_netthread_netsession(netthread, &netsession))
		{
			netsession->socket_state = NETSOCKET_CLOSED;

			if(netsession->user_data)
			{
				NETWORKER* net_worker = netsession->user_data;
				net_worker->socket_state = NETSOCKET_CLOSED;

				net_worker->on_disconnect(net_worker, netsession);

				net_worker->session_id = -1;
				netsession->user_data = NULL;
			}
		}
	}

	netthread->session_id = -1;

	atom_dec(&server_netthreads_count);

	kernel_free();

	return 0;
}

