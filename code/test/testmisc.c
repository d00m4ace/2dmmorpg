//--------------------------------------------------------------------------------------
int FPFRACT_SHIFT = 10;

fp32 fp10 = FP_FRACT(FPFRACT_SHIFT, fp32, 5, 10);
fp32 fp10half = FP_INT(FPFRACT_SHIFT - 1, 1);

fp10 = FP_INT(FPFRACT_SHIFT, 10);
fp10 = FP_MUL_FP(10, fp32, fp10, fp10half);

int i10 = FP_TO_INT(10, fp10);
//--------------------------------------------------------------------------------------
PRINT("int8:%d int16:%d int32:%d int64:%d", sizeof(int8), sizeof(int16), sizeof(int32), sizeof(int64));
PRINT("uint8:%d uint16:%d uint32:%d uint64:%d", sizeof(uint8), sizeof(uint16), sizeof(uint32), sizeof(uint64));
PRINT("bool:%d", sizeof(bool));

PRINT(txt_process("ˇ˜ˇ˜ﬂﬂﬂﬂ˚‚ds ffg2 121!", chr_is_char, chr_to_title));
//--------------------------------------------------------------------------------------
hal_font8_chars_set(FONT8_ZX_RUS)[9] = 127;
PRINT("dos_rus_chars_set[9]=%d", hal_font8_chars_set(FONT8_ZX_RUS)[9]);

char* endptr;
float f1 = txt_str_to_float("-123.435 sdf", &endptr);

char tmp[521];
txt_float_to_str(f1, tmp, 2);

PRINT("text_str_to_float:%f text_float_to_str:%s", f1, tmp);

PRINT("chr_is_char_rus:%d", chr_is_char_rus('ﬂ'));
PRINT("chr_is_char_rus:%d", chr_is_char_rus('Z'));
//--------------------------------------------------------------------------------------
#define INT_EXP 10

{
	float f = 1;
	int32 i = 1;
	for(int e = 0; e < INT_EXP; e++)
	{
		PRINT("%f %d = (int)%d == %d => %s", f, i, (int32)f, i, (((int32)f) == i ? "TRUE" : "FALSE"));

		f = f * 10 + 1;
		i = i * 10 + 1;
	}

	int32 h32dim[] = { 0x3f800000,0x41300000,0x42de0000,0x448ae000,0x462d9c00,0x47d90380,0x4987a238,0x4b298ac7,0x4cd3ed79,0x4e84746c, };

	f = 1;
	i = 1;
	for(int e = 0; e < INT_EXP; e++)
	{
		int32 h;
		(*(float*)(&h)) = f;
		float h32;
		(*(int32*)(&h32)) = h;

		PRINT("%f %d %d => %f %x == %x => %s", f, i, h, h32, h, h32dim[e], (h == h32dim[e] ? "TRUE" : "FALSE"));
		f = f * 10 + 1;
		i = i * 10 + 1;
	}
}

{
	float f = -1;
	int32 i = -1;
	for(int e = 0; e < INT_EXP; e++)
	{
		PRINT("%f %d = (int)%d == %d => %s", f, i, (int32)f, i, (((int32)f) == i ? "TRUE" : "FALSE"));

		f = f * 10 - 1;
		i = i * 10 - 1;
	}

	int32 h32dim[] = { 0xbf800000,0xc1300000,0xc2de0000,0xc48ae000,0xc62d9c00,0xc7d90380,0xc987a238,0xcb298ac7,0xccd3ed79,0xce84746c };

	f = -1;
	i = -1;
	for(int e = 0; e < INT_EXP; e++)
	{
		int32 h;
		(*(float*)(&h)) = f;
		float h32;
		(*(int32*)(&h32)) = h;

		//PRINT("%x", h);
		PRINT("%f %d %d => %f %x == %x => %s", f, i, h, h32, h, h32dim[e], (h == h32dim[e] ? "TRUE" : "FALSE"));
		f = f * 10 - 1;
		i = i * 10 - 1;
	}
}
//--------------------------------------------------------------------------------------
if(hal_mouse_button_pressed(MOUSE_BUTTON_LEFT))
PRINT("hal_mouse_button_pressed:MOUSE_BUTTON_LEFT");
if(hal_mouse_button_pressed(MOUSE_BUTTON_RIGHT))
PRINT("hal_mouse_button_pressed:MOUSE_BUTTON_RIGHT");
if(hal_mouse_button_pressed(MOUSE_BUTTON_MIDDLE))
PRINT("hal_mouse_button_pressed:MOUSE_BUTTON_MIDDLE");

if(hal_mouse_button_down(MOUSE_BUTTON_LEFT))
PRINT("hal_mouse_button_down:MOUSE_BUTTON_LEFT");
if(hal_mouse_button_down(MOUSE_BUTTON_RIGHT))
PRINT("hal_mouse_button_down:MOUSE_BUTTON_RIGHT");
if(hal_mouse_button_down(MOUSE_BUTTON_MIDDLE))
PRINT("hal_mouse_button_down:MOUSE_BUTTON_MIDDLE");
//--------------------------------------------------------------------------------------
if(hal_touch_point_count() > 0)
PRINT("hal_touch_point_count:%d", hal_touch_point_count());
//--------------------------------------------------------------------------------------
if(hal_mouse_button_pressed(MOUSE_BUTTON_LEFT) || hal_mouse_button_down(MOUSE_BUTTON_LEFT))
//if(hal_gesture_detected(GESTURE_TAP))
{
	//vec2i xy = hal_mouse_pos();

	for(int i = 0; i < hal_touch_point_count(); i++)
	{
		vec2f xy = hal_touch_id_pos(i);
		PRINT("hal_touch_id_pos:%d %f %f", i, xy.x, xy.y);

		PRINT("hal_touch_point_id:%d ", hal_touch_point_id(i));
	}

	{
		vec2i xy = hal_touch_pos();
		PRINT("hal_touch_pos: %d %d", xy.x, xy.y);
	}

	{
		vec2f xy = hal_mouse_pos();
		PRINT("hal_mouse_pos: %f %f", xy.x, xy.y);
	}

	{
		vec2f xy = hal_mouse_pos();

		last_touch_pos.x = xy.x;
		last_touch_pos.y = xy.y;
	}
}
//--------------------------------------------------------------------------------------
if(hal_gesture_last_detected())
{
	PRINT("hal_gesture_last_detected: %d", hal_gesture_last_detected());
	PRINT("hal_gesture_hold_duration: %d", hal_gesture_hold_duration());

	vec2f drag = hal_gesture_drag_vector();
	PRINT("hal_gesture_drag_vector: %d %f angle %f", drag.x, drag.y, hal_gesture_drag_angle());

	vec2f pinch = hal_gesture_pinch_vector();
	PRINT("hal_gesture_pinch_vector: %d %f angle %f", pinch.x, pinch.y, hal_gesture_pinch_angle());
}
//--------------------------------------------------------------------------------------
{
	vec2i c = last_touch_pos;

	c.x++; c.y++;
	hal_draw_line((vec2i) { c.x - 10, c.y }, (vec2i) { c.x + 10, c.y }, ARGB32(0, 0, 0, 255));
	hal_draw_line((vec2i) { c.x, c.y - 10 }, (vec2i) { c.x, c.y + 10 }, ARGB32(0, 0, 0, 255));

	c.x--; c.y--;
	hal_draw_line((vec2i) { c.x - 10, c.y }, (vec2i) { c.x + 10, c.y }, ARGB32(200, 200, 200, 255));
	hal_draw_line((vec2i) { c.x, c.y - 10 }, (vec2i) { c.x, c.y + 10 }, ARGB32(200, 200, 200, 255));

	hal_draw_text(txt_format("HERE:%d,%d last_touch_pos ﬂ¡¿ƒ¿¡”!", c.x, c.y), (vec2i) { c.x + 100, c.y }, 20, ARGB32(200, 200, 200, 255));
}
//--------------------------------------------------------------------------------------
{
	uint32* scr_pixels = hal_screen_pixels();

	for(int y = 0; y < SCR_HEIGHT; y++)
		for(int x = 0; x < SCR_WIDTH; x++)
		{
			scr_pixels[y * MAX_SCR_WIDTH + x] = ARGB32(rnd_u8(), rnd_u8(), rnd_u8(), 255);

			if(y == 0 || x == 0 || y == SCR_HEIGHT - 1 || x == SCR_WIDTH - 1)
				scr_pixels[y * MAX_SCR_WIDTH + x] = ARGB32(200, 200, 200, 255);

		}
}
//--------------------------------------------------------------------------------------
{
	uint32* scr_pixels = hal_screen_pixels();

	for(int y = 0; y < SCR_HEIGHT; y++)
		for(int x = 0; x < SCR_WIDTH; x++)
		{
			//scr_pixels[y * MAX_SCR_WIDTH + x] = ARGB32(rnd_u8(), rnd_u8(), rnd_u8(), 255);
			scr_pixels[y * MAX_SCR_WIDTH + x] = ARGB32(255, 255, 0, 255);

			if(y == 0 || x == 0 || y == SCR_HEIGHT - 1 || x == SCR_WIDTH - 1)
				scr_pixels[y * MAX_SCR_WIDTH + x] = ARGB32(200, 200, 200, 255);

		}
}

hal_update_screen(SCR_HEIGHT);

hal_draw_screen(SCR_BORDER, SCR_BORDER, SCR_WIDTH, SCR_HEIGHT, SCR_SCALE, SCR_SCALE);
//--------------------------------------------------------------------------------------

{
	for(int i = 0; i < 3; i++)
	{
		{
			//char* str_prnt = "ﬂ·‡! -.+-+.- -123. -23.4 -.02 +123. +23.4 +.02 #123.# #23.4# %.02% ‰‡·‡ ‰”ÛÛÛ Yba daba dUyuu ffg2 20456121!-.";
			//char* str_prnt = "ﬂ·‡ ‰‡·‡ ‰”ÛÛÛ Yba daba dUyuu ffg2 20456121!";
			char* str_prnt = "ﬂ·‡ #!#01%##\n[#*]#%#! #!%‰‡·‡%#! \n#!#2%[#10*]#%#!#!#0&#!‰”ÛÛÛ#1&!!#!#2& #!\n#!#3%[#-10*]#%#! ?Yba! {daba} <dUyuu> #!~ffg2#! *20456121!";

			rd2_font_print_arg_add_float(10.12345f, 2);
			rd2_font_print_color_add(ARGB32(155, 0, 155, 255), ARGB32(200, 0, 0, 255));

			rd2_font_print_arg_add(txt_format("%dx%d X%d", game_scr_width(), game_scr_height(), game_scr_scale()));
			rd2_font_print_color_add(ARGB32(0, 155, 155, 255), ARGB32(0, 200, 0, 255));

			rd2_font_print_arg_add_int(100001, 8);
			rd2_font_print_color_add(ARGB32(155, 155, 0, 255), ARGB32(0, 0, 200, 255));

			//rd2_font_print_img_add(&heart_ico, VALIGN_TOP);
			//rd2_font_print_img_add(&heart_ico, VALIGN_MIDDLE);
			//rd2_font_print_img_add(&heart_ico, VALIGN_BOTTOM);

			rd2_font_print_set_max_size((point32) { 100, 100 });

			RD2FONT* fnt = game_font_get(GAME_FONT_ID_ZX_RUS + i);
			rd2_font_print(fnt, str_prnt, i * 100 + 5, 0, 1, ARGB32(255, 255, 255, 255), ARGB32(200, 155, 0, 255), true, false);

			fnt = game_font_get(GAME_FONT_ID_ZX_RUS_BORDER + i);
			rd2_font_print(fnt, str_prnt, i * 100 + 5, 100 + 10, 1, ARGB32(255, 255, 255, 255), ARGB32(200, 155, 0, 255), true, true);
		}
	}

	{
		//char* str_prnt = "ﬂ·‡! -.+-+.- -123. -23.4 -.02 +123. +23.4 +.02 #123.# #23.4# %.02% ‰‡·‡ ‰”ÛÛÛ Yba daba dUyuu ffg2 20456121!-.";
		//char* str_prnt = "ﬂ·‡ ‰‡·‡ ‰”ÛÛÛ Yba daba dUyuu ffg2 20456121!";
		char* str_prnt = "ﬂ·‡ #!#01%##\n[#*]#%#! #!%‰‡·‡%#! \n#!#2%[#10*]#%#!#!#0&#!‰”ÛÛÛ#1&!!#!#2& #!\n#!#3%[#-10*]#%#! ?Yba! {daba} <dUyuu> #!~ffg2#! *20456121!";

		rd2_font_print_arg_add_float(10.12345f, 2);
		rd2_font_print_color_add(ARGB32(155, 0, 155, 255), ARGB32(200, 0, 0, 255));

		rd2_font_print_arg_add(txt_format("%dx%d X%d", game_scr_width(), game_scr_height(), game_scr_scale()));
		rd2_font_print_color_add(ARGB32(0, 155, 155, 255), ARGB32(0, 200, 0, 255));

		rd2_font_print_arg_add_int(100001, 8);
		rd2_font_print_color_add(ARGB32(155, 155, 0, 255), ARGB32(0, 0, 200, 255));

		//rd2_font_print_img_add(&heart_ico, VALIGN_TOP);
		//rd2_font_print_img_add(&heart_ico, VALIGN_MIDDLE);
		//rd2_font_print_img_add(&heart_ico, VALIGN_BOTTOM);

		rd2_font_print_set_max_size((point32) { 100, 100 });

		int i = 3;
		RD2FONT* fnt = game_font_get(GAME_FONT_ID_ZX_RUS_THIN);
		rd2_font_print(fnt, str_prnt, i * 100 + 5, 0, 1, ARGB32(255, 255, 255, 255), ARGB32(200, 155, 0, 255), true, false);

		fnt = game_font_get(GAME_FONT_ID_ZX_RUS_THIN_BORDER);
		rd2_font_print(fnt, str_prnt, i * 100 + 5, 100 + 10, 1, ARGB32(255, 255, 255, 255), ARGB32(200, 155, 0, 255), true, true);
	}
}

//--------------------------------------------------------------------------------------
{
	int32 i_arr[20];
	for(size_t i = 0; i < 20; i++)
		i_arr[i] = i;

	VPTR vpi = vptr_init(10);

	int32 i_arr2[] = { -1,-10,-100,-1000,-10000, };

	for(size_t i = 0; i < 20; i++)
		vptr_elm_add(&vpi, &i_arr[i]);

	vptr_elm_delete(&vpi, 0, 1);

	vptr_elm_insert(&vpi, 10, 5);
	vptr_elm_delete(&vpi, 10, 5);

	vptr_elm_set_at(&vpi, 9, &i_arr2[4]);
	vptr_elm_add_at(&vpi, 9, &i_arr2[3]);

	for(size_t i = 0; i < vpi.count; i++)
	{
		int32* p_i = (int32*)vptr_elm(&vpi, i);

		if(p_i)
			PRINT(txt_format("vpi[%d]=%d", i, *p_i));
		else
			PRINT(txt_format("vpi[%d]=NULL", i));
	}

	PRINT(txt_format("vpi size:%d count:%d", vpi.max_size, vpi.count));

	int kkk = 0;
}
//--------------------------------------------------------------------------------------
if(0)
for(int h = 0; h < GAME_ICON_ID_I24_COIN_GOLD; h++)
{
	rd2_spr_fast(game_icon_get(h), 24 + 26 * h, 24, 1.f); // support only scaling no rotation
	rd2_spr_fast(game_icon_get(GAME_ICON_ID_I24_COIN_GOLD + h), 24 + 26 * h, 24 + 26, 1.f); // support only scaling no rotation
}

if(0)
for(int h = 0; h < GAME_TXT_ID_ENUM_COUNT; h++)
{
	RD2FONT* fnt = game_font_get(GAME_FONT_ID_ZX_RUS_THIN_BORDER);
	rd2_font_print(fnt, game_txt_get(h), h * 100 + 5, 100 + 10, 1, ARGB32(255, 255, 255, 255), ARGB32(200, 155, 0, 255), true, true);
}

gui_draw();

if(0)
{
	int x = 64;
	int y = 64;
	int sx = 16;
	int sy = 16;
	int c = 0;
	for(int h = 0; h < 6; h++)
		for(int w = 0; w < 6; w++)
			rd2_rect_fill(x + w * (sx + 1), y + h * (sy + 1), sx, sy, PAL_CLRS_32[(c++) % 32]);
}

//--------------------------------------------------------------------------------------
typedef struct
{
	float x;
	float y;
} point_t;

static c_hash_t point_hash(point_t p)
{
	c_hash_t hash_x = c_float_hash(p.x);
	c_hash_t hash_y = c_float_hash(p.y);
	return c_hash_combine(hash_x, hash_y);
}

static bool point_equals(const void* v1, const void* v2)
{
	const point_t* p1 = (const point_t*)v1;
	const point_t* p2 = (const point_t*)v2;
	return (p1->x == p2->x && p1->y == p2->y);
}

static int c_str_cmp(const void* a, const void* b)
{
	const char* str1 = *(const char* const*)a;
	const char* str2 = *(const char* const*)b;

	return strcmp(str1, str2);
}

typedef struct c_vec_of(const char*) str_vec_t;
str_vec_t vec_str;

void vec_str_add_vals(void)
{
	c_vec_push(&vec_str, "dave");
	c_vec_push(&vec_str, "mary");
	c_vec_push(&vec_str, "steve");
	c_vec_push(&vec_str, "adel");
	c_vec_push(&vec_str, "ˇÌ");
	c_vec_push(&vec_str, "‡Ì‰ÂÈ");
}

typedef struct c_map_of(const char*, char*) str_map_t;
str_map_t map_str;

void map_str_add_vals(void)
{
	c_map_put(&map_str, "dave", "bananas");
	c_map_put(&map_str, "mary", "grapes");
	c_map_put(&map_str, "steve", "pineapples");

	// Put and get pointer to value
	*c_map_put_and_get_ptr(&map_str, "sue") = "beans";
	*c_map_put_and_get_ptr(&map_str, "john") = "fries";
}

{
	c_vec_init(&vec_str);

	vec_str_add_vals();
	vec_str_add_vals();
	vec_str_add_vals();

	const char* value = c_vec_get(&vec_str, 2);
	PRINT("A person: %s", value);

	{
		PRINT("Names unsorted:%d", vec_str.count);

		int32 cc = vec_str.count;

		for(size_t i = 0; i < cc; i++)
		{
			PRINT("> Name: %s", c_vec_get(&vec_str, i));
		}

		PRINT("Names unsorted:%d", vec_str.count);
	}

	{
		c_vec_foreach(&vec_str, const char* value)
		{
			PRINT("> Name: %s", value);
		}
	}

	c_vec_sort(&vec_str, c_str_cmp);

	{
		PRINT("Names sorted:%d", vec_str.count);

		int32 cc = vec_str.count;

		for(size_t i = 0; i < cc; i++)
		{
			PRINT("> Name: %d, %s", i, c_vec_get(&vec_str, i));
		}

		PRINT("Names sorted:%d", vec_str.count);
	}

	{
		int32 cc = 0;
		c_vec_foreach(&vec_str, const char* value)
		{
			//PRINT("> Name: %s", value);
			PRINT("> Name: cc=%d, %s", cc++, value);
		}
	}

	c_vec_deinit(&vec_str);
}

{
	//typedef struct c_map_of(const char*, char*) str_map_t;
	//str_map_t map;
	c_map_init(&map_str);

	map_str_add_vals();
	map_str_add_vals();
	map_str_add_vals();

	// Overwrite existing value
	c_map_put(&map_str, "john", "salad");

	// Get
	PRINT("john: %s.", c_map_get(&map_str, "john"));
	PRINT("mary: %s.", c_map_get(&map_str, "mary"));

	char* value = c_map_get(&map_str, "dave");
	PRINT("dave likes %s.", value);

	// Overwrite via c_map_get_ptr
	*c_map_get_ptr(&map_str, "john") = "fries";
	PRINT("john: no, wait, %s.", c_map_get(&map_str, "john"));

	// Examples when key doesn't exist
	PRINT("Map size: %zu", c_map_count(&map_str));
	c_map_get_ptr(&map_str, "cyrus"); // Returns NULL
	PRINT("c_map_get_ptr(&map, \"cyrus\") == %s", c_map_get_ptr(&map_str, "cyrus") == NULL ? "NULL" : "NOT NULL");

	PRINT("Map size before c_map_put_and_get_ptr(&map, \"cyrus\"): %zu", c_map_count(&map_str));
	c_map_put_and_get_ptr(&map_str, "cyrus"); // Mapping created - but value undefined!
	PRINT("Map size after c_map_put_and_get_ptr(&map, \"cyrus\"): %zu", c_map_count(&map_str));
	*c_map_put_and_get_ptr(&map_str, "cyrus") = "(who knows)"; // Define the value so we don't crash

	c_map_foreach(&map_str, const char* key, char* value)
	{
		PRINT("> % s likes % s.", key, value);
	}

	c_map_deinit(&map_str);
}

{
	// Map with integer keys
	typedef struct c_map_of(int, char*) int_map_t;

	int_map_t int_map;
	// On C11, you can use `c_map_init(&map)` instead
	c_map_init_custom(&int_map, c_int32_hash, c_32bit_equals);

	c_map_put(&int_map, 10, "ten");
	c_map_put(&int_map, 20, "twenty");
	c_map_put(&int_map, 30, "thirty");

	PRINT("20 is `%s`.", c_map_get(&int_map, 20));

	c_map_deinit(&int_map);
}

{
	typedef struct c_map_of(point_t, char*) point_map_t;

	point_map_t map;
	c_map_init_custom(&map, point_hash, point_equals);

	point_t key = { 100.0, 200.0 };
	c_map_put(&map, key, "Buried treasure");

	PRINT("Value at (%f, %f): %s", key.x, key.y, c_map_get(&map, key));

	c_map_deinit(&map);
}

//--------------------------------------------------------------------------------------
{
#if defined(__STDC_VERSION__) 
	PRINT("__STDC_VERSION__ = %d", __STDC_VERSION__);
#endif
}
//--------------------------------------------------------------------------------------

{
	GUI_DISPLAY* dsp = gui_display(0);
	gui_add_display(dsp);

	int32 w = 320, h = 180;
	dsp->pos = (p32){ (game_scr_width() - w) / 2,(game_scr_height() - h) / 2 };

	GUI_PLANE* pln = gui_plane(-1, (p32) { w, h });
	gui_display_add_plane(dsp, pln);

	pln->flags |= GUI_FLG_INPUTLOCK | GUI_FLG_SHADOWSCREEN;
	//pln->flags |= GUI_FLG_UNSCROLLABLE;

	if(1)
	{
		int cc = 0;
		for(size_t j = 0; j < 36; j++) //36
			for(size_t i = 0; i < 16; i++) //16
			{
				if(1)
				{
					/*
											GUI_ELEM* cmps = gui_composite_combo(txt_format("#%d@#%d&%d#%d&", (i * 3 + j) % PAL_TRANSPARENT, GAME_ICON_ID_I16_COIN_GOLD + i * 3 + j, cc * 500, GAME_ICON_ID_I16_DIAMOND_AMBER + i * 3),
												//"012", "345", "678", "901");
												NULL, "[a ˇ 5]", NULL, txt_format("#%d@%d", (cc) % (PAL_TRANSPARENT + 1), cc));
											//"012", "345", "678", "901");
											//"012", "345", "678", "901");

											gui_composite_set_fixed_size(cmps, (p32) { 24 * 2, 24 });
					//*/

					//GUI_ELEM* cmps = gui_composite_tr(txt_format("#%d@#%d&%d#%d&", (i * 3 + j) % PAL_TRANSPARENT, GAME_ICON_ID_I16_COIN_GOLD + i * 3 + j, cc * 500, GAME_ICON_ID_I16_DIAMOND_AMBER + i * 3), "[a ˇ 5]", gui_style_size_min(GUI_STYLE_BUTTON));
					//GUI_ELEM* cmps = gui_composite_br(txt_format("#%d@#%d&%d#%d&", (i * 3 + j) % PAL_TRANSPARENT, GAME_ICON_ID_I16_COIN_GOLD + i * 3 + j, cc * 500, GAME_ICON_ID_I16_DIAMOND_AMBER + i * 3), txt_format("#%d@%d", (cc) % (PAL_TRANSPARENT + 1), cc), gui_style_size_min(GUI_STYLE_BUTTON));
					GUI_ELEM* cmps = gui_composite_tr_br(txt_format("#%d@#%d&%d#%d&", (i * 3 + j) % PAL_TRANSPARENT, GAME_ICON_ID_I16_COIN_GOLD + i * 3 + j, cc * 500, GAME_ICON_ID_I16_DIAMOND_AMBER + i * 3), "[a ˇ 5]", txt_format("#%d@%d", (cc) % (PAL_TRANSPARENT + 1), cc), gui_style_size_min(GUI_STYLE_BUTTON));

					gui_plane_add_elm(pln, cmps);
				}
				else if(1)
				{
					GUI_ELEM* lst = gui_list();

					if(rnd_u8() >> 7)
						gui_list_set_horizontal(lst);

					int32 selected_max = 1 + rnd_u8() % 36;
					int32 selected = rnd_u8() % selected_max;

					for(int32 l = 0; l < selected_max; l++)
					{
						GUI_ELEM* elm = gui_button(txt_format("#%d@#%d&%d#%d&", (l) % PAL_TRANSPARENT,
							GAME_ICON_ID_I16_COIN_GOLD + l, l, GAME_ICON_ID_I16_DIAMOND_AMBER + l));

						gui_list_add_elm(lst, elm);

						if(selected == l)
							gui_list_set_selected(lst, l);
					}

					gui_plane_add_elm(pln, lst);
				}
				else
					//if((cc % 6) == 0)
					//if(cc == 5)
				{
					//GUI_ELEM* elm = gui_progressbar("%d%%", rnd(0,100), 0, 100);
					//GUI_ELEM* elm = gui_progressbar("%d%%", cc, 0, 100);
					GUI_ELEM* elm = gui_spinner("%d%%", cc, 0, 100);
					gui_plane_add_elm(pln, elm);
				}

				if((cc % 6) > 10)
				{
					GUI_ELEM* elm = gui_button(txt_format("#%d@#%d&%d#%d&", (i * 3 + j) % PAL_TRANSPARENT,
						GAME_ICON_ID_I16_COIN_GOLD + i * 3 + j, cc * 500, GAME_ICON_ID_I16_DIAMOND_AMBER + i * 3));

					//GUI_ELEM* elm = gui_button_fixed_size(txt_format("#%d@#%d&%d#%d&", (i * 3 + j) % PAL_TRANSPARENT,
						//GAME_ICON_ID_I16_COIN_GOLD + i * 3 + j, cc * 500, GAME_ICON_ID_I16_DIAMOND_AMBER + i * 3), (p32) { 16 * 4, 16 * 1 });

					//GUI_ELEM* elm = gui_button_size(txt_format("#%d&%d#%d&", 
						//GAME_ICON_ID_I16_COIN_GOLD + i * 3 + j, cc * 500, GAME_ICON_ID_I16_DIAMOND_AMBER + i * 3), (p32) { 16 * 4, 16 * 1 });

					if(rnd_u8() >> 7)
						elm->style = GUI_STYLE_TILE_BUTTON;

					if((cc % 6) == 1)
						elm->flags |= GUI_FLG_DISABLED;

					if((cc % 6) == 2)
						elm->flags |= GUI_FLG_LOCKED;

					if((cc % 6) == 3)
						elm->flags |= GUI_FLG_FOCUSABLE;

					if((cc % 6) == 4)
						elm->flags |= GUI_FLG_SELECTABLE;

					if((cc % 6) == 5)
						elm->flags |= GUI_FLG_SELECTABLE | GUI_FLG_SELECTED;

					if(elm->flags & (GUI_FLG_FOCUSABLE))
						elm->flags |= GUI_FLG_MOUSELOCK;

					int32 offs = 0;

					//elm->origin = (p32){ i * 24 * 3.6 - offs,j * 24 * 1.5 - offs };

					gui_plane_add_elm(pln, elm);
				}

				if(0)
				{
					if((cc % 5) == 0 && cc != 0)
						gui_plane_add_elm(pln, gui_space_new_line());
					else if((cc % 3) == 0 && cc != 0)
						gui_plane_add_elm(pln, gui_space((p32) { 24, 24 }));
				}

				cc++;
			}
	}

	layout_tile_pack(&pln->vptr_elems, w * 1.7, (p32) { 1, 1 });
	//layout_tile_print(&pln->vptr_elems, (2 * w) / 3, -1, (p32) { 1, 1 });
}

//--------------------------------------------------------------------------------------

{
	GUI_ELEM* elm = gui_input_elm_released();

	if(elm != NULL && elm->pln)
	{
		if(elm->elem_type == GUI_ELEM_COMPOSITE)
			gui_composite_set_txt(elm, 2, txt_format("id:%d", gui_input_pressed_frames()));
	}
}

//--------------------------------------------------------------------------------------
//Instead of va_list, using __VA_ARGS__ on array let you achieve the desired result :

#include<stdio.h>

#define count(...)                              \
do {                                            \
    int parameters[] = {__VA_ARGS__};           \
    int count = sizeof(parameters)/sizeof(int); \
                                                \
    for (int i = 0; i < count; i++)             \
        printf("%d", parameters[i]);            \
} while(0)

int main()
{
	count(1, 2, 3, 4, 5, 6);
	count(7);
	count(8, 9, 10);
}
//12345678910
//--------------------------------------------------------------------------------------
//In this code it is possible when you pass only pointer

# include <unistd.h>
# include <stdarg.h>
# include <string.h>
# include <errno.h>

size_t __print__(char* str1, ...);
# define print(...) __print__(NULL, __VA_ARGS__, NULL)
# define ENDL "\n"

int main()
{

	print("1", ENDL, "2", ENDL, "3", ENDL);

	return 0;
}

size_t __print__(char* str1, ...)
{
	va_list args;
	va_start(args, str1);
	size_t out_char = 0;
	char* tmp_str;
	while((tmp_str = va_arg(args, char*)) != NULL)
		out_char = out_char + write(1, tmp_str, strlen(tmp_str));
	va_end(args);
	return out_char;
}
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------


void test_c_vec_netpackets(void)
{
	C_VEC_NETPACKET_CLIENT_ECHO vec_echo;
	c_vec_init(&vec_echo);

	for(size_t i = 0; i < 1000; i++)
	{
		NETPACKET_CLIENT_ECHO echo = { 0 };
		sprintf(echo.echo_data, "ECHO %d", i);
		c_vec_push(&vec_echo, echo);
	}

	char* big_buf = CALLOC(1000, 300);
	NETPACKET_BLOB dst = { big_buf,0,1000 * 300 };

	bool is_w_ok = C_VEC_NETPACKET_SERVER_ECHO_WRITE(&dst, &vec_echo);

	c_vec_clear(&vec_echo);

	dst.pos = 0;
	bool is_r_ok = C_VEC_NETPACKET_SERVER_ECHO_READ(&dst, &vec_echo);

	int32 i = 0;
	c_vec_foreach_ptr(&vec_echo, NETPACKET_SERVER_ECHO * pack)
		PRINT("%d:%s", i++, pack->echo_data);
}

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------


