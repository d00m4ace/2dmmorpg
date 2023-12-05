//--------------------------------------------------------------------------------------
typedef struct RD2SPR_SET
{
	c_vec_img vec_spr_imgs;
	c_vec_spr_anim vec_spr_anims;
} RD2SPR_SET;

typedef struct c_vec_of(RD2SPR_SET) c_vec_spr_sets;
//--------------------------------------------------------------------------------------
RD2SPR_SET spr_set_init(void);
void spr_set_free(RD2SPR_SET* spr_set);

void spr_set_img_push(RD2SPR_SET* spr_set, RD2IMG img);
int32 spr_set_img_count(RD2SPR_SET* spr_set);
RD2IMG* spr_set_img_get(RD2SPR_SET* spr_set, int32 img_id);

void spr_set_anim_push(RD2SPR_SET* spr_set, RD2SPR_ANIM spr_anim);
int32 spr_set_anim_count(RD2SPR_SET* spr_set);
RD2SPR_ANIM* spr_set_anim_get(RD2SPR_SET* spr_set, int32 anim_id);
//--------------------------------------------------------------------------------------
