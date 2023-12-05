#define MAX_CLIENTS 10
NETSESSION_STATE* netsessions;
int netsessions_count = 0;

int main(int argc, char** argv)
{
	//--------------------------------------------------------------------------------------
	// Initialization
	//--------------------------------------------------------------------------------------
	hal_system_init();
	game_init();
	networkers_init();

	netsessions = CALLOC(MAX_CLIENTS, sizeof(NETSESSION_STATE));

	//server_startup(NULL, 7979);
	//server_startup("localhost", 7979);
	//server_startup("127.0.0.1", 7979);
	//server_startup("192.168.2.5", 7979);

	for(int i = 0; i < MAX_CLIENTS; i++)
	{
		netsession_init(&netsessions[i]);
		netsessions[i].session_id = i;

		//if(i && !(i%10)) kernel_sleep(100);

		//tcpclient_connect(&netsessions[i], iptools_host_ip(), 7979);
		//tcpclient_connect(&netsessions[i], "192.168.2.5", 7979);
		//tcpclient_connect(&netsessions[i], "192.168.2.33", 7979);
		//192.168.2.15:7979
		tcpclient_connect(&netsessions[i], clnt_download_file_get("output/download/hugosite_%d.7z", "output/download/hugosite.7z"), "192.168.2.6", 7979);
		//tcpclient_connect(&netsessions[i], clnt_download_file_get("output/download/hugosite_%d.7z", "output/download/hugosite.7z"), "192.168.2.35", 7979);
		//tcpclient_connect(&netsessions[i], clnt_download_file_get("output/download/hugosite_%d.7z", "output/download/hugosite.7z"), "192.168.2.6", 7979);
		
		//tcpclient_connect(&netsessions[i], "127.0.0.1", 7979);

		netsessions_count++;
	}

	while(true)
	{
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
