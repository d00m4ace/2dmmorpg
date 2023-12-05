//--------------------------------------------------------------------------------------
#define GAME_FONTS_MAX_COUNT 16
#define GAME_SOUNDS_MAX_COUNT 1024
#define GAME_IMGS_MAX_COUNT (64*1024)
#define GAME_TXT_MAX_COUNT (64*1024)
//--------------------------------------------------------------------------------------
void game_init(void);
void game_shutdown(void);
//--------------------------------------------------------------------------------------
int32 game_scr_border_width(void);
int32 game_scr_border_height(void);
int32 game_scr_width(void);
int32 game_scr_height(void);
int32 game_scr_scale(void);
//--------------------------------------------------------------------------------------
void game_scr_init(int32 scr_w_min, int32 scr_h_min, int32 scr_w_border_max, int32 scr_h_border_max);
void game_scr_draw(void);
//--------------------------------------------------------------------------------------
void game_font_init(void);
RD2FONT* game_font_get(int32 font_id);
//--------------------------------------------------------------------------------------
void game_imgs_init(void);
bool game_imgs_load(const char* file_name, int32 tile_size, int32 imgs_offset, int32 tiles_count);
RD2IMG* game_imgs_get(int32 game_img_id);
//--------------------------------------------------------------------------------------
void game_txt_init(void);
void game_txt_set_lang(int32 game_txt_lang_id_);
const char* game_txt_get(int32 game_txt_id);
//--------------------------------------------------------------------------------------
void game_sound_init(void);
void game_sound_unload(void);
hal_handler game_sound_get(int32 game_sound_id);

void game_sound_set_volume(float volume);
float game_sound_get_volume(void);

void game_sound_play(int32 game_sound_id);
//--------------------------------------------------------------------------------------
void game_imgs_load_all(void);
//--------------------------------------------------------------------------------------
#include "game_enums.h"
//--------------------------------------------------------------------------------------
