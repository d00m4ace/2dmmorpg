#define IMG_I16_ENUMS_OFFSET 0

#define IMG_CHRS_HEAD_ENUMS_OFFSET  (5000+256*0)
#define IMG_CHRS_FACE_ENUMS_OFFSET  (5000+256*1)
#define IMG_CHRS_HAIR_ENUMS_OFFSET  (5000+256*2)
#define IMG_CHRS_BEARD_ENUMS_OFFSET (5000+256*3)

#define IMG_CHRS_HELM_ENUMS_OFFSET  (5000+256*4)
#define IMG_CHRS_BODY_ENUMS_OFFSET  (5000+256*4+1024)

#define IMG_MOBS_16_ENUMS_OFFSET  (10000+1024*0)
#define IMG_MOBS_24_ENUMS_OFFSET  (10000+1024*2)
#define IMG_MOBS_32_ENUMS_OFFSET  (10000+1024*4)
#define IMG_MOBS_40_ENUMS_OFFSET  (10000+1024*6)
#define IMG_MOBS_48_ENUMS_OFFSET  (10000+1024*8)

#include "img_i16_enums.h"

#include "img_chrs_beard_enums.h"
#include "img_chrs_body_enums.h"
#include "img_chrs_face_enums.h"
#include "img_chrs_hair_enums.h"
#include "img_chrs_head_enums.h"
#include "img_chrs_helm_enums.h"

#include "img_mobs_16_enums.h"
#include "img_mobs_24_enums.h"
#include "img_mobs_32_enums.h"
#include "img_mobs_40_enums.h"
#include "img_mobs_48_enums.h"

void game_imgs_load_all(void);
