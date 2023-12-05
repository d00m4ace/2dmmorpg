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

#define MAX_CLIENTS 1
NETSESSION_STATE* netsessions;
int netsessions_count = 0;

int main(int argc, char** argv)
{
	//--------------------------------------------------------------------------------------
	// Initialization
	//--------------------------------------------------------------------------------------
	hal_system_init();
	game_init();

	netsessions = malloc(sizeof(NETSESSION_STATE) * MAX_CLIENTS);

	for(int i = 0; i < MAX_CLIENTS; i++)
		netsession_init(&netsessions[i]);
		
	//server_startup(NULL, 7979);
	//server_startup("localhost", 7979);
	//server_startup("127.0.0.1", 7979);
	//server_startup("192.168.2.5", 7979);

	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		//if(i && !(i%10)) kernel_sleep(100);

		netsessions[i].session_id = i;

		//tcpclient_connect(&netsessions[i], iptools_host_ip(), 7979);
		//tcpclient_connect(&netsessions[i], "192.168.2.5", 7979);
		//tcpclient_connect(&netsessions[i], "192.168.2.33", 7979);
		tcpclient_connect(&netsessions[i], "192.168.2.35", 7979);
		//tcpclient_connect(&netsessions[i], "127.0.0.1", 7979);

		netsessions_count++;
	}

	char packet_out[NETSESSION_STATE_BUF_SIZE];

	while(true)
	{
		if(kbhit())
		{
			int c = getch();
			PRINT("getch() %d : %c", c, c);

			if(c == 27) //Esc then exit
				break;

			if(c == '1')
			{
				kernel_lock();

				int clnt_count = 0;

				for(int i = 0; i < MAX_CLIENTS; i++)
				{
					if(netsessions[i].socket_state == NETSOCKET_CONNECTED)
					{
						NETPACKET_BLOB dt_out = { packet_out, 0, NETSESSION_STATE_BUF_SIZE };

						uint16 packet_size = 1000*2 + 4, packet_id = PACKET_CLIENT_ECHO_ID;

						if(!netpacket_write_uint16(&dt_out, &packet_size)) continue;
						if(!netpacket_write_uint16(&dt_out, &packet_id)) continue;

						for(uint16 ui16 = 0; ui16 < 1000; ui16++)
							netpacket_write_uint16(&dt_out, &ui16);

						PRINT("send PACKET_CLIENT_ECHO_ID to %s:%d", netsessions[i].host, netsessions[i].host_port);
						netsession_push_packet(&netsessions[i], &dt_out, false);

						clnt_count++;
					}
				}

				PRINT("start ECHO test from clients count:%d\n", clnt_count);

				kernel_free();
			}
		}

		//kernel_sleep(100);

		{
			kernel_lock();

			for(int i = 0; i < MAX_CLIENTS; i++)
			{
				NETPACKET_BLOB dt_out = { packet_out, 0, NETSESSION_STATE_BUF_SIZE };

				if(netsession_pop_packet(&netsessions[i], &dt_out, false))
				{
					uint16 packet_size = 0, packet_id;

					dt_out.pos = 0;
					if(!netpacket_read_uint16(&dt_out, &packet_size)) continue;
					if(!netpacket_read_uint16(&dt_out, &packet_id)) continue;

					if(packet_id == PACKET_SERVER_ECHO_ID)
					{
						PRINT("clnt:%d recv PACKET_SERVER_ECHO_ID from %s:%d", i, netsessions[i].host, netsessions[i].host_port);

						PRINT("clnt:%d send back PACKET_SERVER_ECHO_ID to %s:%d", i, netsessions[i].host, netsessions[i].host_port);
						netsession_push_packet(&netsessions[i], &dt_out, false);
					}
					else if(packet_id == PACKET_CLIENT_ECHO_ID)
					{
						PRINT("clnt:%d recv back PACKET_CLIENT_ECHO_ID from %s:%d", i, netsessions[i].host, netsessions[i].host_port);
					}
				}
			}

			kernel_free();
		}
	}

	tcpsocket_free();
}
