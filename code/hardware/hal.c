#if defined(_WIN32)
//#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
//#define _CRT_NONSTDC_NO_DEPRECATE
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

///////////////////////////////////////////////////////////////
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdint.h>

#include <float.h>
#include <limits.h>
///////////////////////////////////////////////////////////////

#include "raylib.h"

///////////////////////////////////////////////////////////////

#include "hal.h"

///////////////////////////////////////////////////////////////

#include "../data/data.h"

#include "../base/rnd.c"
#include "../base/text.c"
#include "../base/fixpt.c"
#include "../base/memory.c"

#include "../math/d2math.c"

#include "../render/rd2assist.c"
#include "../render/rd2pixelshaders.c"
#include "../render/rd2image.c"
#include "../render/rd2graphics.c"

#include "../render/rd2replacecolor.c"

#include "../render/rd2.c"

#include "../render/rd2sprite.c"

#include "../render/rd2spriteframe.c"
#include "../render/rd2spriteanim.c"
#include "../render/rd2spriteset.c"
#include "../render/rd2spriteobject.c"
#include "../render/rd2spriteman.c"

#include "../render/rd2font.c"

#include "../render/gui/gui.c"

#include "../game/game.c"

#include "../tools/rd2font.c"

#include "../network/netpackets.c"
#include "../network/netsocket.c"
#include "../network/netsession.c"

#include "../network/netpackets/common.c"
#include "../network/netpackets/netpackets.c"

#include "../network/networker/networker.c"
///////////////////////////////////////////////////////////////

#define ARGB32TOCOLOR(rgba32) (Color){(rgba32)&0xff,(rgba32>>8)&0xff,(rgba32>>16)&0xff,(rgba32>>24)&0xff}

///////////////////////////////////////////////////////////////

static int hal_target_fps = 0;

static Image hal_scr_image;
static Texture2D hal_scr_texture;
static int hal_scr_max_size;
static uint32 hal_draw_frame_count = 0;
static bool is_draw_flash_time = false;

static bool is_hal_window_init = false;

///////////////////////////////////////////////////////////////

void hal_system_init(void)
{
#if defined(PLATFORM_ANDROID)
	AutoHideNavBarOnGainedFocus();
#endif
	//--------------------------------------------------------------------------------------	
	// System Initialize 
	//--------------------------------------------------------------------------------------	
	mem_init();
	txt_init();
	kernel_init();
	hal_audio_init();
	tcpsocket_init();
	iptools_init();
	rd2_img_stack_init();
	spr_man_init();
}

void hal_system_shutdown(void)
{
	//--------------------------------------------------------------------------------------	
	// System De-Initialization
	//--------------------------------------------------------------------------------------
	hal_audio_close(); // Close audio device
	hal_window_close(); // Close window and OpenGL context
	tcpsocket_free();
}

void hal_window_init(int width, int height, const char* title)
{
	InitWindow(width, height, title);
	is_hal_window_init = true;
}

int hal_window_width(void)
{
	return GetScreenWidth();
}

int hal_window_height(void)
{
	return GetScreenHeight();
}

void hal_audio_init(void)
{
	InitAudioDevice();
}

bool hal_audio_is_ready(void) // Check if audio device has been initialized successfully
{
	return IsAudioDeviceReady();
}


RD2IMG hal_load_image(const char* file_name)
{
	Image img = LoadImage(txt_format("resources/%s", file_name));

	ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

	RD2IMG r2img = rd2_img_init(img.width, img.height);

	rd2_img_raw_copy(&r2img, img.data, img.width * 4, img.width, img.height);

	UnloadImage(img);

	return r2img;
}

bool hal_save_image(RD2IMG r2img, const char* file_name)
{
	Image img = GenImageColor(r2img.width, r2img.height, BLACK);
	ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);

	rd2_raw_img_copy(&r2img, img.data, img.width * 4, img.width, img.height);

	bool res = ExportImage(img, txt_format("resources/%s", file_name));

	UnloadImage(img);

	return res;
}


void hal_audio_master_volume(float volume)
{
	SetMasterVolume(volume);
}

hal_handler hal_sound_load(const char* file_name)
{
	Sound sound = LoadSound(txt_format("resources/%s", file_name));

	hal_handler hh = CALLOC(1, sizeof(sound));
	memcpy(hh, &sound, sizeof(sound));
	return hh;
}

void hal_sound_play(hal_handler hh) // Play a sound
{
	PlaySound(*((Sound*)hh));
}

void hal_sound_stop(hal_handler hh) // Stop playing a sound
{
	StopSound(*((Sound*)hh));
}

void hal_sound_pause(hal_handler hh) // Pause a sound
{
	PauseSound(*((Sound*)hh));
}

void hal_sound_resume(hal_handler hh) // Resume a paused sound
{
	ResumeSound(*((Sound*)hh));
}

bool hal_sound_is_playing(hal_handler hh) // Check if a sound is currently playing
{
	return IsSoundPlaying(*((Sound*)hh));
}

void hal_sound_set_volume(hal_handler hh, float volume) // Set volume for a sound (1.0 is max level)
{
	SetSoundVolume(*((Sound*)hh), volume);
}

void hal_sound_set_pitch(hal_handler hh, float pitch) // Set pitch for a sound (1.0 is base level)
{
	SetSoundPitch(*((Sound*)hh), pitch);
}

void hal_sound_set_pan(hal_handler hh, float pan) // Set pan for a sound (0.5 is center)
{
	SetSoundPan(*((Sound*)hh), pan);
}

void hal_sound_set_update(hal_handler hh, const void* data, int sample_count) // Update sound buffer with new data
{
	UpdateSound(*((Sound*)hh), data, sample_count);
}


void hal_set_target_fps(int fps)
{
	SetTargetFPS(hal_target_fps = fps);
}


void hal_screen_init(int max_size)
{
	hal_scr_max_size = max_size;
	hal_scr_image = GenImageColor(max_size, max_size, BLACK); // Loaded in CPU memory (RAM)
	ImageFormat(&hal_scr_image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8); // Format image to RGBA 32bit (required for texture update) <-- ISSUE
	hal_scr_texture = LoadTextureFromImage(hal_scr_image); // Image converted to texture, GPU memory (VRAM)
}

void hal_update_screen(int max_height)
{
	UpdateTextureRec(hal_scr_texture, (Rectangle) { 0, 0, hal_scr_max_size, max_height }, hal_scr_image.data);
}

bool hal_window_is_close(void)
{
	return WindowShouldClose();
}


bool hal_key_pressed(int key)
{
	return IsKeyPressed(key);
}

int hal_key_pressed_last(void)
{
	return GetKeyPressed();
}

int hal_char_pressed_last(void)
{
	return GetCharPressed();
}

bool hal_key_released(int key)
{
	return IsKeyReleased(key);
}

bool hal_key_down(int key)
{
	return IsKeyDown(key);
}

bool hal_key_up(int key)
{
	return IsKeyUp(key);
}


void hal_window_set_exit_key(int key)
{
	SetExitKey(key);
}


bool hal_mouse_button_pressed(int button)
{
	return IsMouseButtonPressed(button);
}

bool hal_mouse_button_down(int button)
{
	return IsMouseButtonDown(button);
}

bool hal_mouse_button_released(int button)
{
	return IsMouseButtonReleased(button);
}

bool hal_mouse_button_up(int button)
{
	return IsMouseButtonUp(button);
}

p32 hal_mouse_pos(void)
{
	Vector2 xy = GetMousePosition();

#ifdef PLATFORM_ANDROID
	xy.x = xy.x * GetScreenWidth();
	xy.y = xy.y * GetScreenHeight();
#endif // PLATFORM_ANDROID

	return (p32) { xy.x, xy.y };
}

p32 hal_mouse_delta(void)
{
	Vector2 xy = GetMouseDelta();
	return (p32) { xy.x, xy.y };
}

void hal_mouse_set_pos(int x, int y)
{
	SetMousePosition(x, y);
}

void hal_mouse_set_offset(int x, int y)
{
	SetMouseOffset(x, y);
}

void hal_mouse_set_scale(float x, float y)
{
	SetMouseScale(x, y);
}

float hal_mouse_wheel(void)
{
	return GetMouseWheelMove();
}

void hal_mouse_set_cursor(int cursor)
{
	SetMouseCursor(cursor);
}


void hal_gesture_enabled(unsigned int flags) // Enable a set of gestures using flags
{
	SetGesturesEnabled(flags);
}

bool hal_gesture_detected(int gesture)
{
	return IsGestureDetected(gesture);
}

int hal_gesture_last_detected(void)
{
	return GetGestureDetected();
}

int hal_gesture_hold_duration(void) // Get gesture hold time in milliseconds
{
	return GetGestureHoldDuration();
}

d2v hal_gesture_drag_vector(void) // Get gesture drag vector
{
	Vector2 xy = GetGestureDragVector();
	return (d2v) { xy.x, xy.y };
}

float hal_gesture_drag_angle(void) // Get gesture drag angle
{
	return GetGestureDragAngle();
}

d2v hal_gesture_pinch_vector(void) // Get gesture pinch delta
{
	Vector2 xy = GetGesturePinchVector();
	return (d2v) { xy.x, xy.y };
}

float hal_gesture_pinch_angle(void) // Get gesture pinch angle
{
	return GetGesturePinchAngle();
}


p32 hal_touch_pos(void)
{
	return (p32) { GetTouchX(), GetTouchY() };
}

d2v hal_touch_id_pos(int index)
{
	Vector2 xy = GetTouchPosition(index);

#ifdef PLATFORM_ANDROID
	xy.x = xy.x * GetScreenWidth();
	xy.y = xy.y * GetScreenHeight();
#endif // PLATFORM_ANDROID

	return (d2v) { xy.x, xy.y };
}

int hal_touch_point_id(int index)
{
	return GetTouchPointId(index);
}

int hal_touch_point_count(void)
{
	return GetTouchPointCount();
}




bool hal_gamepad_available(int gamepad) // Check if a gamepad is available
{
	return IsGamepadAvailable(gamepad);
}

const char* hal_gamepad_name(int gamepad) // Get gamepad internal name id
{
	return GetGamepadName(gamepad);
}

bool hal_gamepad_button_pressed(int gamepad, int button)
{
	return IsGamepadButtonPressed(gamepad, button);
}

bool hal_gamepad_button_down(int gamepad, int button)
{
	return IsGamepadButtonDown(gamepad, button);
}

bool hal_gamepad_button_released(int gamepad, int button)
{
	return IsGamepadButtonReleased(gamepad, button);
}

bool hal_gamepad_button_up(int gamepad, int button)
{
	return IsGamepadButtonUp(gamepad, button);
}

int hal_gamepad_button_pressed_last(void)
{
	return GetGamepadButtonPressed();
}

int hal_gamepad_axis_count(int gamepad) // Check if a gamepad is available
{
	return GetGamepadAxisCount(gamepad);
}

float hal_gamepad_axis_movement(int gamepad, int axis) // Check if a gamepad is available
{
	return GetGamepadAxisMovement(gamepad, axis);
}

int hal_gamepad_mappings(const char* mappings) // Set internal gamepad mappings (SDL_GameControllerDB)
{
	return SetGamepadMappings(mappings);
}



uint32* hal_screen_pixels(void)
{
	return hal_scr_image.data;
}

int hal_screen_max_size(void)
{
	return hal_scr_max_size;
}


void hal_draw_begin(void)
{
	BeginDrawing();
}

void hal_draw_end(void)
{
	EndDrawing();

	hal_draw_frame_count++;

	int32 flash_frames = hal_target_fps * 0.33f + 0.5f;
	is_draw_flash_time = (hal_draw_frame_count % (flash_frames * 2)) > flash_frames;
}

uint32 hal_get_draw_frame_count(void)
{
	return hal_draw_frame_count;
}

bool hal_is_draw_flash_time(void)
{
	return is_draw_flash_time;
}

uint32 hal_time_ms(void)
{
	return (1000ULL * clock()) / CLOCKS_PER_SEC;
}

void hal_draw_fps(int x, int y)
{
	DrawFPS(x, y);
}

void hal_draw_text(const char* text, point32 pos, int size, uint32 color) // Draw text (using default font)
{
	DrawText(text, pos.x, pos.y, size, ARGB32TOCOLOR(color));
}


void hal_draw_line(point32 a, point32 b, uint32 color)
{
	DrawLine(a.x, a.y, b.x, b.y, ARGB32TOCOLOR(color));
}

void hal_cls(uint32 color)
{
	ClearBackground(ARGB32TOCOLOR(color));
}

void hal_draw_screen(int x, int y, int width, int height, int width_scale, int height_scale)
{
	DrawTexturePro(hal_scr_texture, (Rectangle) { 0, 0, width, height }, (Rectangle) { x, y, width* width_scale, height* height_scale }, (Vector2) { 0, 0 }, 0, WHITE);
}



void hal_sound_unload(hal_handler hh)
{
	UnloadSound(*((Sound*)hh));
	hal_handler_free(hh);
}

void hal_audio_close(void)
{
	CloseAudioDevice();
}

void hal_window_close(void)
{
	if(is_hal_window_init)
		CloseWindow();

	is_hal_window_init = false;
}

////////////////////////////////////////////
void hal_handler_free(hal_handler hh)
{
	CFREE(hh);
}

////////////////////////////////////////////

char* hal_font8_chars_set(int font8)
{
	switch(font8)
	{
		case FONT8_ZX_RUS: return zx_win1251_chars_set;
		case FONT8_BOLD_RUS: return bold8_rus_chars_set;
		case FONT8_DOS_RUS: return dos_rus_chars_set;
		default:
		break;
	}
	return NULL;
}
