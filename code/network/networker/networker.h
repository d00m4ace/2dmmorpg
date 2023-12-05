typedef enum NETWORKER_TYPE_ID_ENUM
{
	NETWORKER_TYPE_ID_NONE,

	NETWORKER_TYPE_ID_CLIENT,
	NETWORKER_TYPE_ID_SERVER,
} NETWORKER_TYPE_ID_ENUM;

typedef enum NETWORKER_JOB_ID_ENUM
{
	NETWORKER_JOB_ID_NONE,

	NETWORKER_JOB_ID_IDLE,

	NETWORKER_JOB_ID_DOWNLOAD_FILE,
	NETWORKER_JOB_ID_UPLOAD_FILE,
} NETWORKER_JOB_ID_ENUM;

typedef struct NETWORKER NETWORKER;

typedef struct NETWORKER
{
	atom_int32 user_id;

	atom_int32 worker_type_id; // NETWORKER_TYPE_ID_ENUM
	atom_int32 worker_job_id; // NETWORKER_JOB_ID_ENUM
	atom_int32 worker_state_id;

	atom_int32 session_id;
	atom_int32 socket_state;

	void* worker_data;

	bool (*on_request)(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);
	bool (*on_disconnect)(NETWORKER* networker, NETSESSION_STATE* netsession);
} NETWORKER;

typedef struct c_vec_of(NETWORKER*) c_vec_networker;

////////////////////////////////////////////////////
void networkers_init(void);
void networkers_free(void);
////////////////////////////////////////////////////
int32 networkers_count(NETWORKER_TYPE_ID_ENUM worker_type_id);
NETWORKER* networkers_get(NETWORKER_TYPE_ID_ENUM worker_type_id, int32 indx);
void networkers_remove(NETWORKER_TYPE_ID_ENUM worker_type_id, NETWORKER* networker);
void networkers_remove_at(NETWORKER_TYPE_ID_ENUM worker_type_id, int32 indx);
////////////////////////////////////////////////////
void networker_set(NETWORKER* networker);

NETWORKER* networker_init(int worker_type_id);
void networker_deinit(NETWORKER* networker);
////////////////////////////////////////////////////
NETWORKER* clnt_networker_get(void);
NETWORKER* serv_networker_get(void);
////////////////////////////////////////////////////
// always call with kernel_lock outside!
bool networker_on_disconnect(NETWORKER* networker, NETSESSION_STATE* netsession);
bool networker_on_request(NETWORKER* networker, NETSESSION_STATE* netsession, NETPACKET_BLOB* req);
////////////////////////////////////////////////////
typedef enum NETWORKER_STATE_ENUM
{
	NETWORKER_STATE_ON_CONNECT,
	NETWORKER_STATE_ON_DISCONNECT,
	NETWORKER_STATE_ON_REQUEST,

	NETWORKER_STATE_ON_INFO,
	NETWORKER_STATE_ON_IDLE,
	NETWORKER_STATE_ON_WORK,
	NETWORKER_STATE_ON_UPDATE,
	NETWORKER_STATE_ON_CONTINUE,
	NETWORKER_STATE_ON_COMPLETE,
} NETWORKER_STATE_ENUM;
////////////////////////////////////////////////////
#define NW_READ_PACKET_ID(BLOB_PTR) uint16 packet_id = netpacket_read_packet_id(BLOB_PTR); if(packet_id == -1)	return false;

#define G_NP_BLOB_REST() g_np_blob.pos = 0; g_np_blob.size = NETSESSION_STATE_BUF_SIZE;

#define NW_WRITE(PACKET,PACKET_STRUCT_NAME_PTR) { G_NP_BLOB_REST() if(!PACKET##_WRITE(&g_np_blob, PACKET_STRUCT_NAME_PTR)) return false; }
#define NW_PUSH(NETSESSION_PTR) netsession_push_packet(NETSESSION_PTR, &g_np_blob, false);

#define NW_WRITE_PUSH(PACKET,PACKET_STRUCT_NAME_PTR) { \
NW_WRITE(PACKET,PACKET_STRUCT_NAME_PTR) \
NW_PUSH(netsession) \
}

#define NW_SET(PACKET,PACKET_STRUCT_NAME) PACKET PACKET_STRUCT_NAME = {0};
#define NW_READ(PACKET,PACKET_STRUCT_NAME_PTR,BLOB_PTR) { if(!PACKET##_READ(BLOB_PTR, PACKET_STRUCT_NAME_PTR)) return false; }

#define NW_SET_READ(PACKET,PACKET_STRUCT_NAME,BLOB_PTR) NW_SET(PACKET,PACKET_STRUCT_NAME) \
NW_READ(PACKET,&PACKET_STRUCT_NAME,BLOB_PTR)

#define NW_IF_PACKET(PACKET) if(packet_id == PACKET##_ID)
#define NW_IF_PACKET_OR(PACKET,OR) if((packet_id == PACKET##_ID) || (OR))
////////////////////////////////////////////////////
#include "client/clnt_download_file.h"
#include "client/clnt_idle.h"
#include "server/serv_download_file.h"
#include "server/serv_idle.h"
////////////////////////////////////////////////////