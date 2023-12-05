//////////////////////////////////////////////////////////////////
#define SOCKET_REMOTE_ADDR_SIZE 16

typedef struct
{
	atom_int32 session_id;

	netsocket socket;

	uint8 remote_addr[SOCKET_REMOTE_ADDR_SIZE];

	atom_int32 time_accept;
	atom_int32 time_last_io;
} SERVER_NETTHREAD_STATE;
//////////////////////////////////////////////////////////////////
NETSESSION_STATE* server_netsessions;
atom_int32 server_netsessions_count_max = 0;
atom_int32 server_netsessions_count = 0;

SERVER_NETTHREAD_STATE* server_netthreads;
atom_int32 server_netthreads_count_max = 0;
atom_int32 server_netthreads_count = 0;

static SERVER_STATE server;
//////////////////////////////////////////////////////////////////
int32 get_server_netsessions_count_max() { return atom_or(&server_netsessions_count_max, 0); }
int32 get_server_netsessions_count() { return atom_or(&server_netsessions_count, 0); }

int32 get_server_netthreads_count_max() { return atom_or(&server_netthreads_count_max, 0); }
int32 get_server_netthreads_count() { return atom_or(&server_netthreads_count, 0); }

NETSESSION_STATE* get_server_netsession(int indx) { return &server_netsessions[indx]; }
//////////////////////////////////////////////////////////////////
bool server_bind_netthread_netsession(SERVER_NETTHREAD_STATE* netthread);
bool server_get_netthread_netsession(SERVER_NETTHREAD_STATE* netthread, NETSESSION_STATE** netsession);
int32 server_next_netsession_id();
int32 server_next_netthread_id();
void server_netsession_clean();
void server_threads_clean();
void server_threads_timeout();
void server_shutdown();
void server_error_handling(const char* msg);

void server_error_handling(const char* msg);
void netsession_clean_packets(NETSESSION_STATE* netsession);
//////////////////////////////////////////////////////////////////
SERVER_STATE* server_get_state()
{
	return &server;
}

void netthread_init(SERVER_NETTHREAD_STATE* netthread)
{
	memset(netthread, 0, sizeof(SERVER_NETTHREAD_STATE));
	netthread->session_id = -1;
	netthread->socket = NETSOCKET_INVALID;
}

void server_init()
{
	server_netsessions = malloc(sizeof(NETSESSION_STATE) * SERVER_MAX_NETSESSION);
	server_netthreads = malloc(sizeof(SERVER_NETTHREAD_STATE) * SERVER_MAX_NETTHREADS);

	for(int i = 0; i < SERVER_MAX_NETSESSION; i++)
		netsession_init(&server_netsessions[i]);

	for(int i = 0; i < SERVER_MAX_NETTHREADS; i++)
		netthread_init(&server_netthreads[i]);
}

// has kernel_lock() 
bool server_bind_netthread_netsession(SERVER_NETTHREAD_STATE* netthread)
{
	kernel_lock();

	int32 netsession_id = server_next_netsession_id();
	netthread->session_id = netsession_id;

	if(netsession_id == -1)
	{
		kernel_free();
		return false;
	}

	NETSESSION_STATE* netsession = &server_netsessions[netsession_id];

	netsession->session_id = netsession_id;
	netsession->socket_state = NETSOCKET_CONNECTING;

	netsession_clean_packets(netsession);

	struct sockaddr_in* remote_addr = (struct sockaddr_in*)netthread->remote_addr;
	strcpy(netsession->host, inet_ntoa(remote_addr->sin_addr));
	netsession->host_port = remote_addr->sin_port;

	kernel_free();
	return true;
}

bool server_get_netthread_netsession(SERVER_NETTHREAD_STATE* netthread, NETSESSION_STATE** netsession)
{
	*netsession = NULL;

	int32 netsession_id = atom_or(&netthread->session_id, 0);

	if(netsession_id == -1)
		return false;

	*netsession = &server_netsessions[netsession_id];

	if((*netsession)->session_id != netthread->session_id)
		return false;

	return true;
}

int32 server_next_netsession_id()
{
	int netsession_id = -1;

	for(int32 i = 0; i < get_server_netsessions_count_max(); i++)
	{
		if(server_netsessions[i].session_id == -1)
		{
			netsession_id = i;
			break;
		}
	}

	if(netsession_id == -1)
	{
		if(get_server_netsessions_count_max() == SERVER_MAX_NETSESSION)
			return -1;

		netsession_id = get_server_netsessions_count_max();
		atom_inc(&server_netsessions_count_max);
	}

	atom_inc(&server_netsessions_count);

	return netsession_id;
}

int32 server_next_netthread_id()
{
	int thread_id = -1;

	for(int32 i = 0; i < get_server_netthreads_count_max(); i++)
	{
		if(server_netthreads[i].socket == NETSOCKET_INVALID && server_netthreads[i].session_id == -1)
		{
			thread_id = i;
			break;
		}
	}

	if(thread_id == -1)
	{
		if(get_server_netthreads_count_max() == SERVER_MAX_NETTHREADS)
			return -1;

		thread_id = get_server_netthreads_count_max();
		atom_inc(&server_netthreads_count_max);
	}

	atom_inc(&server_netthreads_count);

	return thread_id;
}

void server_netsession_clean()
{
	for(int32 i = 0; i < get_server_netsessions_count_max(); i++)
	{
		if(server_netsessions[i].session_id != -1)
		{
			if(server_netsessions[i].socket_state == NETSOCKET_CLOSED && server_netsessions[i].user_data == NULL)
			{
				server_netsessions[i].session_id = -1;
				atom_dec(&server_netsessions_count);
			}
		}
	}

	for(;;)
	{
		int32 max_netsession_id = get_server_netsessions_count_max();
		if(max_netsession_id > 0 && server_netsessions[max_netsession_id - 1].session_id == -1)
		{
			atom_dec(&server_netsessions_count_max);
			continue;
		}

		break;
	}
}

void server_threads_clean()
{
	for(;;)
	{
		int32 max_netthread_id = get_server_netthreads_count_max();
		if(max_netthread_id > 0 && server_netthreads[max_netthread_id - 1].socket == NETSOCKET_INVALID)
		{
			atom_dec(&server_netthreads_count_max);
			continue;
		}

		break;
	}
}

void server_threads_timeout()
{
	int32 time_now = clock();
	for(int32 i = 0; i < get_server_netthreads_count_max(); i++)
	{
		if(server_netthreads[i].session_id != -1 && server_netthreads[i].socket != NETSOCKET_INVALID)
		{
			if(time_now > server_netthreads[i].time_last_io && (time_now - server_netthreads[i].time_last_io) > server.threads_timeout)
			{
				PRINT("timeout for netthreads[%d] %2.4f", i, ((double)time_now - server_netthreads[i].time_last_io) / CLOCKS_PER_SEC);
				closesocket(server_netthreads[i].socket);
				server_netthreads[i].socket = NETSOCKET_INVALID;
			}
		}
	}
}

void server_shutdown()
{
	/*
	kernel_threads_wait_all_end(server.client_threads, server.clients_count, INFINITE);

	for(int i = 0; i < server.clients_count; i++)
		kernel_thread_free(server.client_threads[i]);
	*/

	// Turn off the monitor socket 
	if(server.listen_socket != INVALID_SOCKET)
		closesocket(server.listen_socket);
	server.listen_socket = INVALID_SOCKET;
}

#define SOCKET_ERROR_TXT(err) case err: strcpy(msgbuf, #err); break;

void server_error_handling(const char* msg)
{
	char msgbuf[256];   // for a message up to 255 bytes.

#if defined(_WIN32)
	int	err;
	msgbuf[0] = '\0';    // Microsoft doesn't guarantee this on man page.

	err = WSAGetLastError();

	FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, // flags
		NULL,                // lpsource
		err,                 // message id
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),    // languageid
		msgbuf,              // output buffer
		sizeof(msgbuf),     // size of msgbuf, bytes
		NULL);               // va_list of arguments

	if(!*msgbuf)
		sprintf(msgbuf, "%d", err);  // provide error # if no string available	
#endif

#ifdef USE_LINUX
	strcpy(msgbuf, strerror(errno));
#endif

	PRINT("server error: %s \nsocket last error: %s", msg, msgbuf);
}
