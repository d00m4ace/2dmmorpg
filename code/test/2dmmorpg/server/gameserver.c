#include <libpq-fe.h>

#define USE_ONE_KERNEL_LOCK

//--------------------------------------------------------------------------------------
#include "../common/common.h"
//--------------------------------------------------------------------------------------

NETWORKER* server_networker_get(NETSESSION_STATE* netsession)
{
	switch(netsession->host_job_id)
	{
		case NETWORKER_JOB_ID_DOWNLOAD_FILE: return serv_download_file_get(); break;
		case NETWORKER_JOB_ID_PCHAR: return serv_pchar_networker_get(); break;
	}

	return serv_networker_get();
}

int main(int argc, char** argv)
{
	//--------------------------------------------------------------------------------------
	// Initialization
	//--------------------------------------------------------------------------------------
	hal_system_init();
	game_init();
	server_init();
	networkers_init();
	//--------------------------------------------------------------------------------------
	c_vec_init(&vec_player_chars);
	//--------------------------------------------------------------------------------------
	server_startup("localhost", 7979, server_networker_get);
	//server_startup("127.0.0.1", 7979);
	//server_startup("192.168.2.5", 7979);
	//--------------------------------------------------------------------------------------
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

			if(c == 'i')
			{
				PRINT("netthreads_count:%d netsessions_count:%d netthreads_count_max:%d netsessions_count_max:%d", get_server_netthreads_count(), get_server_netsessions_count(), get_server_netthreads_count_max(), get_server_netsessions_count_max());
			}

			if(c == '-')
			{
				SERVER_STATE* srvr = server_get_state();
				srvr->nop_sleep_time_ms -= 10;

				if(srvr->nop_sleep_time_ms < 0)
					srvr->nop_sleep_time_ms = 0;

				PRINT("nop_sleep_time_ms:%d", srvr->nop_sleep_time_ms);
			}

			if(c == '+')
			{
				SERVER_STATE* srvr = server_get_state();
				srvr->nop_sleep_time_ms += 10;

				PRINT("nop_sleep_time_ms:%d", srvr->nop_sleep_time_ms);
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

		{
#ifdef USE_ONE_KERNEL_LOCK
			kernel_lock();
#endif
			server_threads_clean();
			server_netsession_clean();

			//server_threads_timeout();
#ifdef USE_ONE_KERNEL_LOCK
			kernel_free();
#endif	
		}

		//kernel_sleep(100);
	}
	//--------------------------------------------------------------------------------------
	// De-Initialization
	//--------------------------------------------------------------------------------------
	networkers_free();

	hal_system_shutdown();
	game_shutdown();
	return 0;
}
