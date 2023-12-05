typedef struct
{
	uint8* data;
	uint32 pos;
	uint32 size;
} NETPACKET_BLOB;

uint16 netpacket_read_array_uint8(NETPACKET_BLOB* blob, uint8* dst, uint16 size);
uint16 netpacket_read_array_uint16(NETPACKET_BLOB* blob, uint16* dst, uint16 size);
uint16 netpacket_read_array_uint32(NETPACKET_BLOB* blob, uint32* dst, uint16 size);
uint16 netpacket_read_array_uint64(NETPACKET_BLOB* blob, uint64* dst, uint16 size);

uint16 netpacket_write_array_uint8(NETPACKET_BLOB* blob, uint8* src, uint16 size);
uint16 netpacket_write_array_uint16(NETPACKET_BLOB* blob, uint16* src, uint16 size);
uint16 netpacket_write_array_uint32(NETPACKET_BLOB* blob, uint32* src, uint16 size);
uint16 netpacket_write_array_uint64(NETPACKET_BLOB* blob, uint64* src, uint16 size);

uint16 netpacket_read_uint8(NETPACKET_BLOB* blob, uint8* val);
uint16 netpacket_read_uint16(NETPACKET_BLOB* blob, uint16* val);
uint16 netpacket_read_uint32(NETPACKET_BLOB* blob, uint32* val);
uint16 netpacket_read_uint64(NETPACKET_BLOB* blob, uint64* val);

uint16 netpacket_write_uint8(NETPACKET_BLOB* blob, uint8* val);
uint16 netpacket_write_uint16(NETPACKET_BLOB* blob, uint16* val);
uint16 netpacket_write_uint32(NETPACKET_BLOB* blob, uint32* val);
uint16 netpacket_write_uint64(NETPACKET_BLOB* blob, uint64* val);

uint16 netpacket_skip_bytes(NETPACKET_BLOB* blob, uint16 skip_bytes);

bool netpacket_split(NETPACKET_BLOB* src, NETPACKET_BLOB* dst, uint16 size_limit);
bool netpacket_unsplit(NETPACKET_BLOB* src, NETPACKET_BLOB* dst);

bool netpacket_unsplit_ready(NETPACKET_BLOB* src);

bool netpacket_write_packet(NETPACKET_BLOB* dst, uint16 packet_id, uint16 packet_size, uint8* data);
bool netpacket_read_packet(NETPACKET_BLOB* src, uint16* packet_id, uint16* packet_size, uint8* data);

int netpacket_read_packet_id(NETPACKET_BLOB* src);
int netpacket_read_packet_size(NETPACKET_BLOB* src);

uint16 netpacket_calc_bits(uint8* src, uint32 src_len);