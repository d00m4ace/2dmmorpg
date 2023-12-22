static char g_nt_bufer[NETSESSION_STATE_BUF_SIZE];
static NETPACKET_BLOB g_np_blob = { g_nt_bufer, 0, NETSESSION_STATE_BUF_SIZE };

NETPACKET_BLOB* new_netpacket_blob(uint32 size);
void free_netpacket_blob(NETPACKET_BLOB* blob);

void netpacket_push(netblob_vec_t* vec, NETPACKET_BLOB* blob);
NETPACKET_BLOB* netpacket_pop(netblob_vec_t* vec);

bool netpacket_write_packet_from_blob(NETPACKET_BLOB* dst, NETPACKET_BLOB* src);

#define NP_PUSH(VEC_PTR,PACKET_STRUCT_NAME_PTR,PACKET_PTR) { \
NETPACKET_BLOB* blb = new_netpacket_blob(PACKET_STRUCT_NAME_PTR##_SIZE()); \
PACKET_STRUCT_NAME_PTR##_WRITE(blb, PACKET_PTR); \
blb->pos = 0; \
netpacket_push(VEC_PTR, blb); \
}

#define NP_PUSH_BLOB(VEC_PTR,BLOB_PTR) netpacket_push(VEC_PTR, BLOB_PTR)

#define NP_POP_BLOB(VEC_PTR) netpacket_pop(VEC_PTR)
#define NP_BLOB_FREE(BLOB_PTR) free_netpacket_blob(BLOB_PTR)

#define NP_PACKET_SIZE(BLOB_PTR) (*((uint16*)(BLOB_PTR)->data))
#define NP_PACKET_ID(BLOB_PTR) (*(((uint16*)(BLOB_PTR)->data)+1))
#define NP_PACKET_DATA(BLOB_PTR) ((BLOB_PTR)->data+4)
#define NP_PACKET_DATA_SIZE(BLOB_PTR) (NP_PACKET_SIZE(BLOB_PTR)-4)

#define NP_IF_PACKET(BLOB_PTR,PACKET_STRUCT_NAME) if(NP_PACKET_ID(BLOB_PTR) == PACKET_STRUCT_NAME##_ID)
#define NP_IF_NOT_PACKET(BLOB_PTR,PACKET_STRUCT_NAME) if(NP_PACKET_ID(BLOB_PTR) != PACKET_STRUCT_NAME##_ID)

#define NP_READ_PACKET(BLOB_PTR,PACKET_STRUCT_NAME,PACKET_PTR) PACKET_STRUCT_NAME##_READ(BLOB_PTR, PACKET_PTR)
#define NP_WRITE_PACKET(BLOB_PTR,PACKET_STRUCT_NAME,PACKET_PTR) PACKET_STRUCT_NAME##_WRITE(BLOB_PTR, PACKET_PTR)

#define NP_CREATE_READ_PACKET(BLOB_PTR,PACKET_STRUCT_NAME,PACKET) PACKET_STRUCT_NAME PACKET = {0}; bool PACKET##_is_read = NP_READ_PACKET(BLOB_PTR,PACKET_STRUCT_NAME,&PACKET);
