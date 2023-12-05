#define SPR_ANIM_FLG_NONE (0) // at the end set anim frame_id = 0
#define SPR_ANIM_FLG_ONCE (1UL<<1) // at the end set anim frame_id = last_frame_id
#define SPR_ANIM_FLG_LOOP (1UL<<2)
#define SPR_ANIM_FLG_LOOP_BACK (1UL<<3)

typedef struct RD2SPR_ANIM
{
	c_vec_spr_frame vec_spr_frames;
	uint32 flags;
} RD2SPR_ANIM;

typedef struct c_vec_of(RD2SPR_ANIM) c_vec_spr_anim;

RD2SPR_ANIM spr_anim_init(uint32 flags, int32 frames_count, RD2SPR_FRAME frame[]);
void spr_anim_free(RD2SPR_ANIM* spr_anim);

void spr_anim_frame_push(RD2SPR_ANIM* spr_anim, RD2SPR_FRAME frame);
int32 spr_anim_frame_count(RD2SPR_ANIM* spr_anim);
RD2SPR_FRAME* spr_anim_frame_get(RD2SPR_ANIM* spr_anim, int32 frame_id);