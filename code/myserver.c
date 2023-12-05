#if defined(_WIN32)
//#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_NONSTDC_NO_DEPRECATE
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif
///////////////////////////////////////////////////////////////
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
///////////////////////////////////////////////////////////////
#include "platform/android/tools.h"
///////////////////////////////////////////////////////////////
#include "hardware/hal.h"
///////////////////////////////////////////////////////////////
#if defined(_WIN32)
#include "platform/win32/win32.h"
#endif
///////////////////////////////////////////////////////////////
#include "game/game01/img/img.h"
#include "game/game01/txt/txt_enums.h"
#include "game/game01/img/img.c"
#include "game/game01/txt/txt_en.c"
#include "game/game01/txt/txt_ru.c"
///////////////////////////////////////////////////////////////

#define USE_ONE_KERNEL_LOCK

int main(int argc, char** argv)
{
	//--------------------------------------------------------------------------------------
	// Initialization
	//--------------------------------------------------------------------------------------
	hal_system_init();
	game_init();

	server_init();

	server_startup("localhost", 7979);
	//server_startup("127.0.0.1", 7979);
	//server_startup("192.168.2.5", 7979);

	uint32 time_start = clock();
	bool is_finished = true;

	char packet_out[NETSESSION_STATE_BUF_SIZE];

	int32 srvr_sended_nop_total = 0, srvr_recved_nop_total = 0;
	uint32 srvr_nop_time = clock();

	int clnt_count = -1;

	while(true)
	{
		if(kbhit())
		{
			int c = getch();
			PRINT("getch() %d : %c", c, c);

			if(c == 27) //Esc then exit
				break;

			if(c == 'i') //Esc then exit
			{
				PRINT("netthreads_count:%d netsessions_count:%d netthreads_count_max:%d netsessions_count_max:%d", get_server_netthreads_count(), get_server_netsessions_count(), get_server_netthreads_count_max(), get_server_netsessions_count_max());
			}

			if(c == '-') //Esc then exit
			{
				SERVER_STATE* srvr = server_get_state();
				srvr->nop_sleep_time_ms -= 10;

				if(srvr->nop_sleep_time_ms < 0)
					srvr->nop_sleep_time_ms = 0;

				PRINT("nop_sleep_time_ms:%d", srvr->nop_sleep_time_ms);
			}

			if(c == '+') //Esc then exit
			{
				SERVER_STATE* srvr = server_get_state();
				srvr->nop_sleep_time_ms += 10;

				PRINT("nop_sleep_time_ms:%d", srvr->nop_sleep_time_ms);
			}

			if(c == '3') //Esc then exit
			{
				NETSESSION_STATE* netsession = get_server_netsession(0); //hack ping to first netsession only
				if(netsession->session_id != -1 && netsession->socket_state == NETSOCKET_CONNECTED)
				{
					NETPACKET_BLOB dst = { packet_out, 0, NETSESSION_STATE_BUF_SIZE };

					if(!netpacket_write_packet(&dst, PACKET_COMMON_PING_ID, 4, NULL)) break;
					PRINT("test ping to %s:%d", netsession->host, netsession->host_port);
					time_start = clock();
					netsession_push_packet(netsession, &dst, true);
				}
			}

			if(c == '2')
			{
#ifdef USE_ONE_KERNEL_LOCK
				kernel_lock();
#endif
				int netsessions_count_max = get_server_netsessions_count_max();

				{
					time_start = clock();
					SERVER_STATE* srvr = server_get_state();
					srvr->send_total = 0;
					srvr->recv_total = 0;
					is_finished = false;
					clnt_count = 0;

					PRINT("start ECHO test to:%d", netsessions_count_max);
				}

				for(int i = 0; i < netsessions_count_max; i++)
				{
					NETSESSION_STATE* netsession = get_server_netsession(i);

					if(netsession->session_id != -1 && netsession->socket_state == NETSOCKET_CONNECTED)
					{
						NETPACKET_BLOB dt_out = { packet_out, 0, NETSESSION_STATE_BUF_SIZE };

						uint16 packet_size = 0, packet_id = PACKET_SERVER_ECHO_ID;

						if(!netpacket_write_uint16(&dt_out, &packet_size)) continue;
						if(!netpacket_write_uint16(&dt_out, &packet_id)) continue;

						for(uint16 ui16 = 0; ui16 < (256); ui16++)
							//for(uint16 ui16 = 0; ui16 < (10000); ui16++)
							netpacket_write_uint16(&dt_out, &ui16);

						netpacket_finish(&dt_out, 0);

						PRINT("send PACKET_SERVER_ECHO_ID to %s:%d", netsession->host, netsession->host_port);

#ifdef USE_ONE_KERNEL_LOCK
						netsession_push_packet(netsession, &dt_out, false);
						netsession_push_packet(netsession, &dt_out, false);
#else
						netsession_push_packet(netsession, &dt_out, true);
						netsession_push_packet(netsession, &dt_out, true);
#endif						
						clnt_count++;
					}
				}

#ifdef USE_ONE_KERNEL_LOCK
				kernel_free();
#endif				
			}

			{
				SERVER_STATE* srvr = server_get_state();
				PRINT("send_total: %ld recv_total: %ld send_nop_total: %ld recv_nop_total: %ld", srvr->send_total, srvr->recv_total, srvr->send_nop_total, srvr->recv_nop_total);
			}

			{
				SERVER_STATE* srvr = server_get_state();
				uint32 time_now = clock();
				double time_elapsed = ((double)(time_now - srvr_nop_time)) / CLOCKS_PER_SEC;

				double send_mb = (srvr->send_nop_total - srvr_sended_nop_total) / (1024. * 1024);
				double recv_mb = (srvr->recv_nop_total - srvr_recved_nop_total) / (1024. * 1024);

				PRINT("nop_sleep:%dms elapsed:%2.4lfs send_nop: %2.4lfMb recv_nop: %2.4lfMb send_nop: %2.4lfMb/s recv_nop: %2.4lfMb/s", srvr->nop_sleep_time_ms, time_elapsed, send_mb, recv_mb, send_mb / time_elapsed, recv_mb / time_elapsed);
				srvr_sended_nop_total = srvr->send_nop_total;
				srvr_recved_nop_total = srvr->recv_nop_total;
				srvr_nop_time = time_now;
			}
		}

		if(!is_finished)
		{
			SERVER_STATE* srvr = server_get_state();

			if(srvr->send_total == (1056 * clnt_count) && srvr->recv_total == (1032 * clnt_count)) //2 test echo packets client limit 250
			//if(srvr->send_total == (432 * clnt_count) && srvr->recv_total == (432 * clnt_count)) //2 test echo packets client limit 90
			//if(srvr->send_total == (408 * clnt_count) && srvr->recv_total == (408 * clnt_count)) //2 test echo packets
			//if(srvr->send_total == (20332 * clnt_count) && srvr->recv_total == (20084 * clnt_count)) //1 test echo packet
			//if(srvr->send_total == (40664 * clnt_count) && srvr->recv_total == (40168 * clnt_count)) //2 test echo packets
			{
				uint32 time_stop = clock();

				double sec_elapsed = ((double)(time_stop - time_start)) / CLOCKS_PER_SEC;

				double send_mb = srvr->send_total / (1024. * 1024);
				double recv_mb = srvr->recv_total / (1024. * 1024);

				double send_mb_speed = send_mb / sec_elapsed;
				double recv_mb_speed = recv_mb / sec_elapsed;

				PRINT("clnt_count:%d time:%2.4lfs send: %2.4lfMb recv: %2.4lfMb send speed: %2.4lfMb/s recv speed: %2.4lfMb/s", clnt_count, sec_elapsed, send_mb, recv_mb, send_mb_speed, recv_mb_speed);

				is_finished = true;
			}
		}

		{
#ifdef USE_ONE_KERNEL_LOCK
			kernel_lock();
#endif

			server_threads_clean();
			server_netsession_clean();

			for(int i = 0; i < get_server_netsessions_count_max(); i++)
			{
				NETSESSION_STATE* netsession = get_server_netsession(i);

				if(netsession->session_id != -1 && netsession->socket_state == NETSOCKET_CONNECTED)
				{
					NETPACKET_BLOB dt_out = { packet_out, 0, NETSESSION_STATE_BUF_SIZE };

#ifdef USE_ONE_KERNEL_LOCK
					if(netsession_pop_packet(netsession, &dt_out, false))
#else
					if(netsession_pop_packet(netsession, &dt_out, true))
#endif
					{
						uint16 packet_size = 0, packet_id;

						dt_out.pos = 0;
						if(!netpacket_read_uint16(&dt_out, &packet_size)) continue;
						if(!netpacket_read_uint16(&dt_out, &packet_id)) continue;

						if(packet_id == PACKET_CLIENT_ECHO_ID)
						{
							PRINT("clnt:%d recv PACKET_CLIENT_ECHO_ID from %s:%d", i, netsession->host, netsession->host_port);

							PRINT("clnt:%d send back PACKET_CLIENT_ECHO_ID to %s:%d", i, netsession->host, netsession->host_port);

#ifdef USE_ONE_KERNEL_LOCK
							netsession_push_packet(netsession, &dt_out, false);
#else
							netsession_push_packet(netsession, &dt_out, true);
#endif
						}
						else if(packet_id == PACKET_SERVER_ECHO_ID)
						{
							PRINT("clnt:%d recv back PACKET_SERVER_ECHO_ID from %s:%d", i, netsession->host, netsession->host_port);
						}
						else if(packet_id == PACKET_COMMON_PONG_ID)
						{
							double sec_elapsed = ((double)(clock() - time_start)) / CLOCKS_PER_SEC;
							PRINT("ping to %s:%d time:%2.3lfs", netsession->host, netsession->host_port, sec_elapsed);
						}
					}
				}
			}

			//server_threads_timeout();
#ifdef USE_ONE_KERNEL_LOCK
			kernel_free();
#endif	
		}

		//kernel_sleep(100);
	}

	tcpsocket_free();
}
