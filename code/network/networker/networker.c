static c_vec_networker g_vec_networkers_client;
static c_vec_networker g_vec_networkers_server;
////////////////////////////////////////////////////
static char g_nt_bufer[NETSESSION_STATE_BUF_SIZE];
static NETPACKET_BLOB g_np_blob = { g_nt_bufer, 0, NETSESSION_STATE_BUF_SIZE };
////////////////////////////////////////////////////
void networkers_init(void)
{
	c_vec_init(&g_vec_networkers_client);
	c_vec_init(&g_vec_networkers_server);
}

void networkers_free(void)
{
	c_vec_foreach_ptr(&g_vec_networkers_client, NETWORKER * networker)
	{
		c_vec_remove(&g_vec_networkers_client, networker);
		networker_deinit(networker);
	}

	c_vec_foreach_ptr(&g_vec_networkers_server, NETWORKER* networker)
	{
		c_vec_remove(&g_vec_networkers_server, networker);
		networker_deinit(networker);
	}

	c_vec_deinit(&g_vec_networkers_client);
	c_vec_deinit(&g_vec_networkers_server);
}
////////////////////////////////////////////////////
int32 networkers_count(NETWORKER_TYPE_ID_ENUM worker_type_id)
{
	if(worker_type_id == NETWORKER_TYPE_ID_CLIENT)
		return g_vec_networkers_client.count;
	else
		return g_vec_networkers_server.count;
}

void networkers_remove(NETWORKER_TYPE_ID_ENUM worker_type_id, NETWORKER* networker)
{
	if(worker_type_id == NETWORKER_TYPE_ID_CLIENT)
		c_vec_remove(&g_vec_networkers_client, networker);
	else
		c_vec_remove(&g_vec_networkers_server, networker);
}

NETWORKER* networkers_get(NETWORKER_TYPE_ID_ENUM worker_type_id, int32 indx)
{
	if(worker_type_id == NETWORKER_TYPE_ID_CLIENT)
		return c_vec_get(&g_vec_networkers_client, indx);
	else
		return c_vec_get(&g_vec_networkers_server, indx);
}

void networkers_remove_at(NETWORKER_TYPE_ID_ENUM worker_type_id, int32 indx)
{
	if(worker_type_id == NETWORKER_TYPE_ID_CLIENT)
		c_vec_remove_at(&g_vec_networkers_client, indx);
	else
		c_vec_remove_at(&g_vec_networkers_server, indx);
}
////////////////////////////////////////////////////
void networker_set(NETWORKER* networker)
{
	networker->worker_job_id = NETWORKER_JOB_ID_NONE;
	networker->worker_state_id = NETWORKER_STATE_ON_CONNECT;

	networker->worker_data = NULL;

	networker->on_request = networker_on_request;
	networker->on_disconnect = networker_on_disconnect;
}

NETWORKER* networker_init(int worker_type_id)
{
	NETWORKER* networker = CALLOC(1, sizeof(NETWORKER));

	networker->user_id = -1;

	networker->worker_type_id = worker_type_id;

	networker->session_id = -1;
	networker->socket_state = NETSOCKET_NONE;

	networker_set(networker);
	return networker;
}
////////////////////////////////////////////////////
void networker_deinit(NETWORKER* networker)
{
	CFREE(networker->worker_data);
	networker->worker_data = NULL;
	CFREE(networker);
}
////////////////////////////////////////////////////
bool networker_on_disconnect(NETWORKER* networker, NETSESSION_STATE* netsession)
{
	networker_deinit(networker);
	return true;
}

bool networker_on_request(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req)
{
	NW_WRITE_PUSH(NETPACKET_ON_IDLE, NULL);
	return true;
}
////////////////////////////////////////////////////
NETWORKER* clnt_networker_get(void)
{
	NETWORKER* networker = networker_init(NETWORKER_TYPE_ID_CLIENT);
	c_vec_push(&g_vec_networkers_client, networker);
	return networker;
}

NETWORKER* serv_networker_get(void)
{
	NETWORKER* networker = networker_init(NETWORKER_TYPE_ID_SERVER);
	c_vec_push(&g_vec_networkers_server, networker);
	return networker;
}
////////////////////////////////////////////////////
#include "client/clnt_download_file.c"
#include "server/serv_download_file.c"
#include "client/clnt_idle.c"
#include "server/serv_idle.c"
////////////////////////////////////////////////////