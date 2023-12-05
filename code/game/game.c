static int MAX_SCR_WIDTH = 384; // render texture max size

// low end android 1280x720
static int SCR_WIDTH_MIN = 320;
static int SCR_HEIGHT_MIN = 180;
// hi end android 1080+
//static int SCR_WIDTH_MIN = 422;
//static int SCR_HEIGHT_MIN = 216;

static int SCR_WIDTH = 0;
static int SCR_HEIGHT = 0;

static int SCR_W_BORDER_MAX = 5; // 6
static int SCR_H_BORDER_MAX = 0; // 6
static int SCR_W_BORDER = 0;
static int SCR_H_BORDER = 0;

static int SCR_SCALE = 2;

static char game_window_title[256] = "GAME";

void game_init(void)
{
	game_font_init();
	game_sound_init();
	game_imgs_init();
	game_txt_init();

	gui_init();
}

void game_shutdown(void)
{
	game_sound_unload(); // Unload sound data
}

int32 game_scr_width(void)
{
	return SCR_WIDTH;
}

int32 game_scr_height(void)
{
	return SCR_HEIGHT;
}

int32 game_scr_scale(void)
{
	return SCR_SCALE;
}

int32 game_scr_border_width(void)
{
	return SCR_W_BORDER;
}

int32 game_scr_border_height(void)
{
	return SCR_H_BORDER;
}

void game_scr_init(int32 scr_w_min, int32 scr_h_min, int32 scr_w_border_max, int32 scr_h_border_max)
{
	int scr_width = 0;
	int scr_height = 0;

	SCR_WIDTH_MIN = scr_w_min;
	SCR_HEIGHT_MIN = scr_h_min;
	SCR_W_BORDER_MAX = scr_w_border_max; //6
	SCR_H_BORDER_MAX = scr_h_border_max; //6
	SCR_WIDTH = scr_w_min;
	SCR_HEIGHT = scr_h_min;
	SCR_SCALE = 3;

#ifdef _WIN32	
	scr_width = SCR_WIDTH * SCR_SCALE + SCR_W_BORDER_MAX * SCR_SCALE * 2;
	scr_height = SCR_HEIGHT * SCR_SCALE + SCR_H_BORDER_MAX * SCR_SCALE * 2;
	//SetConfigFlags(FLAG_FULLSCREEN_MODE);
#endif // _WIN32

	//----------------------------------------------------------------------------------
	hal_window_init(scr_width, scr_height, game_window_title);
	hal_window_set_exit_key(0);
	hal_set_target_fps(60); // Set our game to run at 60 frames-per-second
	//----------------------------------------------------------------------------------

	scr_width = hal_window_width();
	scr_height = hal_window_height();

#ifdef PLATFORM_ANDROID
	if (scr_height < 1000)
	{
		SCR_WIDTH_MIN = 320;
		SCR_HEIGHT_MIN = 180;
	}

	SCR_SCALE = scr_height / SCR_HEIGHT_MIN;
#endif // PLATFORM_ANDROID

	SCR_WIDTH = scr_width / SCR_SCALE;
	SCR_HEIGHT = scr_height / SCR_SCALE;

	SCR_W_BORDER = (SCR_WIDTH - SCR_WIDTH_MIN) / 2;

	if (SCR_W_BORDER < 0)
		SCR_W_BORDER = 0;

	if (SCR_W_BORDER > SCR_W_BORDER_MAX)
		SCR_W_BORDER = SCR_W_BORDER_MAX;

	SCR_WIDTH -= SCR_W_BORDER * 2;

	SCR_H_BORDER = (SCR_HEIGHT - SCR_HEIGHT_MIN) / 2;

	if (SCR_H_BORDER < 0)
		SCR_H_BORDER = 0;

	if (SCR_H_BORDER > SCR_H_BORDER_MAX)
		SCR_H_BORDER = SCR_H_BORDER_MAX;

	SCR_HEIGHT -= SCR_H_BORDER * 2;

	while (SCR_WIDTH > MAX_SCR_WIDTH)
		MAX_SCR_WIDTH += 128;

	PRINT("MAX_SCR_WIDTH:%d SCR_WIDTH:%d SCR_HEIGHT:%d", MAX_SCR_WIDTH, SCR_WIDTH, SCR_HEIGHT);

	//----------------------------------------------------------------------------------
	hal_screen_init(MAX_SCR_WIDTH);
	//----------------------------------------------------------------------------------
	RD2IMG iscr = rd2_img_init(SCR_WIDTH, SCR_HEIGHT);
	rd2_scr_set(&iscr);

	RD2PIXELSHADER ps = rd2_shader_init();
	//ps.state |= PIXELSHADER_FORCE_PLOT;
	//ps.tint = ARGB32(255, 155, 255, 155);
	rd2_shader_set(&ps);
	//----------------------------------------------------------------------------------
	rd2_scr_stack_init(); // call only after we setup game screen width&height
	//----------------------------------------------------------------------------------
	gui_kb_init();
	//----------------------------------------------------------------------------------
}

void game_scr_draw(void)
{
	rd2_scr_draw(SCR_W_BORDER * SCR_SCALE, SCR_H_BORDER * SCR_SCALE, SCR_SCALE, SCR_SCALE);
}

//----------------------------------------------------------------------------------

RD2FONT game_font[GAME_FONTS_MAX_COUNT];

RD2FONT* game_font_get(int32 font_id)
{
	ASSERT(font_id >= 0 && font_id < GAME_FONTS_MAX_COUNT);
	return &game_font[font_id];
}

void game_font_init(void)
{
	{
		for (int i = 0; i < 3; i++)
		{
			RD2FONT* fnt;
			game_font[i] = rd2_font_init(8, 8);
			fnt = &game_font[i];

			rd2_font8_set(fnt, hal_font8_chars_set(FONT8_ZX_RUS + i), 256);

			rd2_font_trim_chars(&game_font[i]);

			fnt->padding_width = 1;
			fnt->space_width = 8;

			game_font[3 + i] = rd2_font_border(fnt, 1, ARGB32(0, 0, 0, 155));
			rd2_font_trim_chars(&game_font[3 + i]);
		}
	}

	{
		RD2FONT* fnt;
		game_font[GAME_FONT_ID_ZX_RUS_THIN] = rd2_font_init(8, 8);
		fnt = &game_font[GAME_FONT_ID_ZX_RUS_THIN];

		rd2_font8_set(fnt, hal_font8_chars_set(FONT8_ZX_RUS), 256);

		fnt->padding_width = 1;
		fnt->space_width = 4;

		rd2_font_thin_chars(fnt, 1);
	}

	{
		RD2FONT* fnt;
		game_font[GAME_FONT_ID_ZX_RUS_THIN_BORDER] = rd2_font_init(8, 8);
		fnt = &game_font[GAME_FONT_ID_ZX_RUS_THIN_BORDER];

		rd2_font8_set(fnt, hal_font8_chars_set(FONT8_ZX_RUS), 256);

		fnt->padding_width = 0;
		fnt->space_width = 5;

		rd2_font_thin_chars(fnt, 1);

		RD2FONT fntbld = rd2_font_border(fnt, 1, ARGB32(0, 0, 0, 155));
		rd2_font_free(fnt);
		game_font[GAME_FONT_ID_ZX_RUS_THIN_BORDER] = fntbld;

		rd2_font_trim_chars(fnt);
	}

	{
		RD2FONT* fnt;
		game_font[GAME_FONT_ID_MICROFONT] = rd2_font_init(8, 8);
		fnt = &game_font[GAME_FONT_ID_MICROFONT];

		rd2_font_load_image(fnt, "microfont.png");

		fnt->padding_width = 1;
		fnt->space_width = 3;

		fnt->char_height = 7;

		rd2_font_trim_chars(fnt);
	}

	{
		RD2FONT* fnt;
		game_font[GAME_FONT_ID_MICROFONT_BORDER] = rd2_font_init(8, 8);
		fnt = &game_font[GAME_FONT_ID_MICROFONT_BORDER];

		rd2_font_load_image(fnt, "microfont.png");

		fnt->padding_width = 0;
		fnt->space_width = 3;

		RD2FONT fntbld = rd2_font_border(fnt, 1, ARGB32(0, 0, 0, 155));
		rd2_font_free(fnt);
		game_font[GAME_FONT_ID_MICROFONT_BORDER] = fntbld;

		fntbld.padding_width = 0;
		fntbld.space_width = 3;

		rd2_font_trim_chars(fnt);
	}
}

//----------------------------------------------------------------------------------
RD2IMG* game_imgs;

RD2IMG* game_imgs_get(int32 game_img_id)
{
	ASSERT(game_img_id >= 0 && game_img_id < GAME_IMGS_MAX_COUNT);
	return &game_imgs[game_img_id];
}

bool game_imgs_load(const char* file_name, int32 tile_size, int32 imgs_offset, int32 tiles_count)
{
	int32 img_map_width = 16 * (tile_size + 1) + 1;
	int32 img_map_height = (tiles_count / 16 + 1) * (tile_size + 1) + 1;

	RD2IMG img_map = hal_load_image(file_name);

	if (img_map.pixels32 == NULL)
		return false;

	int32 img_x = 1, img_y = 1;

	for (int32 i = 0; i < tiles_count; i++)
	{
		if (i != 0 && (i % 16) == 0)
		{
			img_x = 1;
			img_y += tile_size + 1;
		}

		RD2IMG img = rd2_img_init(tile_size, tile_size);

		rd2_img_blit(&img, &img_map, 0, 0, img_x, img_y, tile_size, tile_size);

		game_imgs[imgs_offset + i] = img;

		img_x += tile_size + 1;
	}

	return true;
}

void game_imgs_init(void)
{
	game_imgs = CALLOC(1, GAME_IMGS_MAX_COUNT * sizeof(RD2IMG));

	game_imgs_load_all();
}

//----------------------------------------------------------------------------------
const char** game_txts;
int32 game_lang_id;

void game_txt_en(const char* txt[]);
void game_txt_ru(const char* txt[]);

void game_txt_init(void)
{
	game_txts = CALLOC(1, GAME_TXT_MAX_COUNT * sizeof(const char*));

	game_txt_set_lang(TXT_LANG_ID_EN);
}

const char* game_txt_get(int32 game_txt_id)
{
	ASSERT(game_txt_id >= 0 && game_txt_id < GAME_TXT_MAX_COUNT);

	return game_txts[game_txt_id];
}

void game_txt_set_lang(int32 game_txt_lang_id)
{
	ASSERT(game_txt_lang_id >= 0 && game_txt_lang_id < TXT_LANG_ID_ENUM_COUNT);

	game_lang_id = game_txt_lang_id;

	switch (game_txt_lang_id)
	{
	case TXT_LANG_ID_EN: game_txt_en(game_txts); break;
	case TXT_LANG_ID_RU: game_txt_ru(game_txts); break;
	}
}

//----------------------------------------------------------------------------------
hal_handler game_sound[GAME_SOUNDS_MAX_COUNT] = { 0 };
float game_sound_volume = 0.1f;

hal_handler game_sound_get(int32 game_sound_id)
{
	ASSERT(game_sound_id >= 0 && game_sound_id < GAME_SOUNDS_MAX_COUNT);
	return game_sound[game_sound_id];
}

float game_sound_get_volume(void)
{
	return game_sound_volume;
}

void game_sound_set_volume(float volume)
{
	game_sound_volume = volume;

	for (int i = 0; i < GAME_SOUND_ID_ENUM_COUNT; i++)
		hal_sound_set_volume(game_sound[i], game_sound_volume);
}

void game_sound_unload(void)
{
	for (int i = 0; i < GAME_SOUND_ID_ENUM_COUNT; i++)
		hal_sound_unload(game_sound[i]);
}

void game_sound_play(int32 game_sound_id)
{
	hal_sound_play(game_sound_get(game_sound_id));
}

void game_sound_init(void)
{
	game_sound[GAME_SOUND_ID_CLICK] = hal_sound_load("sounds/click.wav");
	game_sound[GAME_SOUND_ID_BIPBIP] = hal_sound_load("sounds/biubiu.ogg");
	game_sound[GAME_SOUND_ID_BIUBIU] = hal_sound_load("sounds/bipbip.wav");

	game_sound_set_volume(0.1f);
}
