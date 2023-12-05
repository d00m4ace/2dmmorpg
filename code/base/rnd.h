//--------------------------------------------------------------------------------------
uint8 rnd_u8();
uint16 rnd_u16();
uint32 rnd_u32();
uint64 rnd_u64();

int32 rnd(int32 min, int32 max);

void rnd_seed(uint32 seed);
//--------------------------------------------------------------------------------------
typedef struct LFSR_STATE
{
	uint32 x;
	uint32 y;
	uint32 z;
	uint32 w;
} LFSR_STATE;

uint32 lfsr_random(void);

void lfsr_random_set_seed(uint32 seed);

LFSR_STATE lfsr_random_get_state(void);
//--------------------------------------------------------------------------------------