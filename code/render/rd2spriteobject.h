//--------------------------------------------------------------------------------------
#define SPR_FLG_NONE (0)
#define SPR_FLG_FLIP_H (1UL<<1)
#define SPR_FLG_FLIP_V (1UL<<2)
#define SPR_FLG_ROT_90 (1UL<<3)
#define SPR_FLG_ROT_180 (1UL<<4)

#define SPR_FLG_ANIM_END (1UL<<5)
#define SPR_FLG_ANIM_LOOP_BACK (1UL<<6)
//--------------------------------------------------------------------------------------
typedef struct RD2SPR_OBJ
{
	int16 spr_set_id;
	int16 spr_anim_id;
	int16 frame_id;
	uint16 frames;
	int16 replace_colors_id;
	uint32 flags;
} RD2SPR_OBJ;

typedef struct c_vec_of(RD2SPR_OBJ) c_vec_spr_objs;

RD2SPR_OBJ spr_obj_init(int16 spr_set_id, int16 spr_anim_id);

RD2SPR_SET* spr_obj_spr_set_get(RD2SPR_OBJ* spr_obj);
RD2SPR_ANIM* spr_obj_spr_anim_get(RD2SPR_OBJ* spr_obj);
RD2SPR_FRAME* spr_obj_spr_frame_get(RD2SPR_OBJ* spr_obj);
int32 spr_obj_frame_id_get(RD2SPR_OBJ* spr_obj);
RD2IMG* spr_obj_spr_frame_img_get(RD2SPR_OBJ* spr_obj);

void spr_obj_update(RD2SPR_OBJ* spr_obj);
RD2IMG spr_obj_render(RD2SPR_OBJ* spr_obj);
void spr_obj_draw(RD2SPR_OBJ* spr_obj, p32 pos);

void spr_obj_spr_anim_set(RD2SPR_OBJ* spr_obj, int32 anim_id);
//--------------------------------------------------------------------------------------
