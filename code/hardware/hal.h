#include "../config.h"

#include "../base/cstl.h"
#include "../base/types.h"
#include "../base/macros.h"
#include "../base/rnd.h"
#include "../base/fixpt.h"
#include "../base/text.h"
#include "../base/memory.h"

#include "../math/d2math.h"

#include "../render/rd2assist.h"
#include "../render/palette.h"
#include "../render/rd2pixelshaders.h"
#include "../render/rd2image.h"
#include "../render/rd2graphics.h"

#include "../render/rd2replacecolor.h"

#include "../render/rd2.h"

#include "../render/rd2sprite.h"

#include "../render/rd2spriteframe.h"
#include "../render/rd2spriteanim.h"
#include "../render/rd2spriteset.h"
#include "../render/rd2spriteobject.h"
#include "../render/rd2spriteman.h"

#include "../render/rd2font.h"

#include "../render/gui/gui.h"

#include "../game/game.h"

#include "../tools/rd2font.h"

#include "../threads/atomic.h"
#include "../threads/kernel.h"

#include "../network/netpackets.h"
#include "../network/netsocket.h"
#include "../network/netsession.h"

#include "../network/netpackets/common.h"
#include "../network/netpackets/netpackets.h"
#include "../network/netpackets/netpackets_enums.h"

#include "../network/iptools.h"
#include "../network/tcpsocket.h"
#include "../network/tcpclient.h"
#include "../network/tcpserver.h"

#include "../network/networker/networker.h"
///////////////////////////////////////////////////////////////

void hal_system_init(void);
void hal_system_shutdown(void);

void hal_window_init(int width, int height, const char* title);
void hal_window_close(void);

bool hal_window_is_close(void);
void hal_window_set_exit_key(int key); // Set a custom key to exit program (default is ESC)

int hal_window_width(void);
int hal_window_height(void);

void hal_audio_init(void);
bool hal_audio_is_ready(void); // Check if audio device has been initialized successfully
void hal_audio_master_volume(float volume);
void hal_audio_close(void);

RD2IMG hal_load_image(const char* file_name);
bool hal_save_image(RD2IMG r2img, const char* file_name);

hal_handler hal_sound_load(const char* file_name);

void hal_sound_play(hal_handler hh); // Play a sound
void hal_sound_stop(hal_handler hh); // Stop playing a sound
void hal_sound_pause(hal_handler hh); // Pause a sound
void hal_sound_resume(hal_handler hh); // Resume a paused sound

bool hal_sound_is_playing(hal_handler hh); // Check if a sound is currently playing

void hal_sound_set_volume(hal_handler hh, float volume); // Set volume for a sound (1.0 is max level)
void hal_sound_set_pitch(hal_handler hh, float pitch); // Set pitch for a sound (1.0 is base level)
void hal_sound_set_pan(hal_handler hh, float pan); // Set pan for a sound (0.5 is center)

void hal_sound_unload(hal_handler hh);
void hal_sound_set_update(hal_handler hh, const void* data, int sample_count); // Update sound buffer with new data

void hal_set_target_fps(int fps);

void hal_screen_init(int max_size);

uint32* hal_screen_pixels(void);
void hal_update_screen(int max_height);
int hal_screen_max_size(void);

void hal_draw_begin(void);
void hal_draw_end(void);

uint32 hal_get_draw_frame_count(void);
bool hal_is_draw_flash_time(void);

uint32 hal_time_ms(void);

void hal_cls(uint32 color);
void hal_draw_fps(int x, int y);
void hal_draw_text(const char* text, point32 pos, int size, uint32 color); // Draw text (using default font)
void hal_draw_line(point32 a, point32 b, uint32 color);

void hal_draw_screen(int x, int y, int width, int height, int width_scale, int height_scale);

bool hal_key_pressed(int key); // Check if a key has been pressed once
int hal_key_pressed_last(void); // See KEY_A etc or returns 0 when the key pressed queue is empty
int hal_char_pressed_last(void); // Get char pressed (unicode), returns 0 when the queue is empty
bool hal_key_released(int key); // Check if a key has been released once
bool hal_key_down(int key); // Check if a key is being pressed
bool hal_key_up(int key); // Check if a key is NOT being pressed

bool hal_mouse_button_pressed(int button);
bool hal_mouse_button_down(int button);
bool hal_mouse_button_released(int button);
bool hal_mouse_button_up(int button);
p32 hal_mouse_pos(void);
p32 hal_mouse_delta(void);
void hal_mouse_set_pos(int x, int y);
void hal_mouse_set_offset(int x, int y);
void hal_mouse_set_scale(float x, float y);
float hal_mouse_wheel(void);
void hal_mouse_set_cursor(int cursor);

p32 hal_touch_pos(void);
d2v hal_touch_id_pos(int index);
int hal_touch_point_id(int index);
int hal_touch_point_count(void);

void hal_gesture_enabled(unsigned int flags); // Enable a set of gestures using flags
bool hal_gesture_detected(int gesture); // See GESTURE_DOUBLETAP etc
int hal_gesture_last_detected(void); // Get latest detected gesture
int hal_gesture_hold_duration(void); // Get gesture hold time in milliseconds
d2v hal_gesture_drag_vector(void); // Get gesture drag vector
float hal_gesture_drag_angle(void); // Get gesture drag angle
d2v hal_gesture_pinch_vector(void); // Get gesture pinch delta
float hal_gesture_pinch_angle(void); // Get gesture pinch angle

bool hal_gamepad_available(int gamepad); // Check if a gamepad is available
const char* hal_gamepad_name(int gamepad); // Get gamepad internal name id
bool hal_gamepad_button_pressed(int gamepad, int button);
bool hal_gamepad_button_down(int gamepad, int button);
bool hal_gamepad_button_released(int gamepad, int button);
bool hal_gamepad_button_up(int gamepad, int button);
int hal_gamepad_button_pressed_last(void);
int hal_gamepad_axis_count(int gamepad); // Check if a gamepad is available
float hal_gamepad_axis_movement(int gamepad, int axis); // Check if a gamepad is available
int hal_gamepad_mappings(const char* mappings); // Set internal gamepad mappings (SDL_GameControllerDB)

void hal_handler_free(hal_handler hh);

///////////////////////////////////////////////////////////////

#define FONT8_ZX_RUS 1
#define FONT8_BOLD_RUS 2
#define FONT8_DOS_RUS 3

char* hal_font8_chars_set(int font8);

///////////////////////////////////////////////////////////////

#if !defined(RAYLIB_VERSION)

// Keyboard keys (US keyboard layout)
// NOTE: Use GetKeyPressed() to allow redefining
// required keys for alternative layouts
typedef enum
{
	KEY_NULL = 0,        // Key: NULL, used for no key pressed
	// Alphanumeric keys
	KEY_APOSTROPHE = 39,       // Key: '
	KEY_COMMA = 44,       // Key: ,
	KEY_MINUS = 45,       // Key: -
	KEY_PERIOD = 46,       // Key: .
	KEY_SLASH = 47,       // Key: /
	KEY_ZERO = 48,       // Key: 0
	KEY_ONE = 49,       // Key: 1
	KEY_TWO = 50,       // Key: 2
	KEY_THREE = 51,       // Key: 3
	KEY_FOUR = 52,       // Key: 4
	KEY_FIVE = 53,       // Key: 5
	KEY_SIX = 54,       // Key: 6
	KEY_SEVEN = 55,       // Key: 7
	KEY_EIGHT = 56,       // Key: 8
	KEY_NINE = 57,       // Key: 9
	KEY_SEMICOLON = 59,       // Key: ;
	KEY_EQUAL = 61,       // Key: =
	KEY_A = 65,       // Key: A | a
	KEY_B = 66,       // Key: B | b
	KEY_C = 67,       // Key: C | c
	KEY_D = 68,       // Key: D | d
	KEY_E = 69,       // Key: E | e
	KEY_F = 70,       // Key: F | f
	KEY_G = 71,       // Key: G | g
	KEY_H = 72,       // Key: H | h
	KEY_I = 73,       // Key: I | i
	KEY_J = 74,       // Key: J | j
	KEY_K = 75,       // Key: K | k
	KEY_L = 76,       // Key: L | l
	KEY_M = 77,       // Key: M | m
	KEY_N = 78,       // Key: N | n
	KEY_O = 79,       // Key: O | o
	KEY_P = 80,       // Key: P | p
	KEY_Q = 81,       // Key: Q | q
	KEY_R = 82,       // Key: R | r
	KEY_S = 83,       // Key: S | s
	KEY_T = 84,       // Key: T | t
	KEY_U = 85,       // Key: U | u
	KEY_V = 86,       // Key: V | v
	KEY_W = 87,       // Key: W | w
	KEY_X = 88,       // Key: X | x
	KEY_Y = 89,       // Key: Y | y
	KEY_Z = 90,       // Key: Z | z
	KEY_LEFT_BRACKET = 91,       // Key: [
	KEY_BACKSLASH = 92,       // Key: '\'
	KEY_RIGHT_BRACKET = 93,       // Key: ]
	KEY_GRAVE = 96,       // Key: `
	// Function keys
	KEY_SPACE = 32,       // Key: Space
	KEY_ESCAPE = 256,      // Key: Esc
	KEY_ENTER = 257,      // Key: Enter
	KEY_TAB = 258,      // Key: Tab
	KEY_BACKSPACE = 259,      // Key: Backspace
	KEY_INSERT = 260,      // Key: Ins
	KEY_DELETE = 261,      // Key: Del
	KEY_RIGHT = 262,      // Key: Cursor right
	KEY_LEFT = 263,      // Key: Cursor left
	KEY_DOWN = 264,      // Key: Cursor down
	KEY_UP = 265,      // Key: Cursor up
	KEY_PAGE_UP = 266,      // Key: Page up
	KEY_PAGE_DOWN = 267,      // Key: Page down
	KEY_HOME = 268,      // Key: Home
	KEY_END = 269,      // Key: End
	KEY_CAPS_LOCK = 280,      // Key: Caps lock
	KEY_SCROLL_LOCK = 281,      // Key: Scroll down
	KEY_NUM_LOCK = 282,      // Key: Num lock
	KEY_PRINT_SCREEN = 283,      // Key: Print screen
	KEY_PAUSE = 284,      // Key: Pause
	KEY_F1 = 290,      // Key: F1
	KEY_F2 = 291,      // Key: F2
	KEY_F3 = 292,      // Key: F3
	KEY_F4 = 293,      // Key: F4
	KEY_F5 = 294,      // Key: F5
	KEY_F6 = 295,      // Key: F6
	KEY_F7 = 296,      // Key: F7
	KEY_F8 = 297,      // Key: F8
	KEY_F9 = 298,      // Key: F9
	KEY_F10 = 299,      // Key: F10
	KEY_F11 = 300,      // Key: F11
	KEY_F12 = 301,      // Key: F12
	KEY_LEFT_SHIFT = 340,      // Key: Shift left
	KEY_LEFT_CONTROL = 341,      // Key: Control left
	KEY_LEFT_ALT = 342,      // Key: Alt left
	KEY_LEFT_SUPER = 343,      // Key: Super left
	KEY_RIGHT_SHIFT = 344,      // Key: Shift right
	KEY_RIGHT_CONTROL = 345,      // Key: Control right
	KEY_RIGHT_ALT = 346,      // Key: Alt right
	KEY_RIGHT_SUPER = 347,      // Key: Super right
	KEY_KB_MENU = 348,      // Key: KB menu
	// Keypad keys
	KEY_KP_0 = 320,      // Key: Keypad 0
	KEY_KP_1 = 321,      // Key: Keypad 1
	KEY_KP_2 = 322,      // Key: Keypad 2
	KEY_KP_3 = 323,      // Key: Keypad 3
	KEY_KP_4 = 324,      // Key: Keypad 4
	KEY_KP_5 = 325,      // Key: Keypad 5
	KEY_KP_6 = 326,      // Key: Keypad 6
	KEY_KP_7 = 327,      // Key: Keypad 7
	KEY_KP_8 = 328,      // Key: Keypad 8
	KEY_KP_9 = 329,      // Key: Keypad 9
	KEY_KP_DECIMAL = 330,      // Key: Keypad .
	KEY_KP_DIVIDE = 331,      // Key: Keypad /
	KEY_KP_MULTIPLY = 332,      // Key: Keypad *
	KEY_KP_SUBTRACT = 333,      // Key: Keypad -
	KEY_KP_ADD = 334,      // Key: Keypad +
	KEY_KP_ENTER = 335,      // Key: Keypad Enter
	KEY_KP_EQUAL = 336,      // Key: Keypad =
	// Android key buttons
	KEY_BACK = 4,        // Key: Android back button
	KEY_MENU = 82,       // Key: Android menu button
	KEY_VOLUME_UP = 24,       // Key: Android volume up button
	KEY_VOLUME_DOWN = 25        // Key: Android volume down button
} KeyboardKey;

// Add backwards compatibility support for deprecated names
#define MOUSE_LEFT_BUTTON   MOUSE_BUTTON_LEFT
#define MOUSE_RIGHT_BUTTON  MOUSE_BUTTON_RIGHT
#define MOUSE_MIDDLE_BUTTON MOUSE_BUTTON_MIDDLE

// Mouse buttons
typedef enum
{
	MOUSE_BUTTON_LEFT = 0,       // Mouse button left
	MOUSE_BUTTON_RIGHT = 1,       // Mouse button right
	MOUSE_BUTTON_MIDDLE = 2,       // Mouse button middle (pressed wheel)
	MOUSE_BUTTON_SIDE = 3,       // Mouse button side (advanced mouse device)
	MOUSE_BUTTON_EXTRA = 4,       // Mouse button extra (advanced mouse device)
	MOUSE_BUTTON_FORWARD = 5,       // Mouse button fordward (advanced mouse device)
	MOUSE_BUTTON_BACK = 6,       // Mouse button back (advanced mouse device)
} MouseButton;

// Mouse cursor
typedef enum
{
	MOUSE_CURSOR_DEFAULT = 0,     // Default pointer shape
	MOUSE_CURSOR_ARROW = 1,     // Arrow shape
	MOUSE_CURSOR_IBEAM = 2,     // Text writing cursor shape
	MOUSE_CURSOR_CROSSHAIR = 3,     // Cross shape
	MOUSE_CURSOR_POINTING_HAND = 4,     // Pointing hand cursor
	MOUSE_CURSOR_RESIZE_EW = 5,     // Horizontal resize/move arrow shape
	MOUSE_CURSOR_RESIZE_NS = 6,     // Vertical resize/move arrow shape
	MOUSE_CURSOR_RESIZE_NWSE = 7,     // Top-left to bottom-right diagonal resize/move arrow shape
	MOUSE_CURSOR_RESIZE_NESW = 8,     // The top-right to bottom-left diagonal resize/move arrow shape
	MOUSE_CURSOR_RESIZE_ALL = 9,     // The omni-directional resize/move cursor shape
	MOUSE_CURSOR_NOT_ALLOWED = 10     // The operation-not-allowed shape
} MouseCursor;

// Gamepad buttons
typedef enum
{
	GAMEPAD_BUTTON_UNKNOWN = 0,         // Unknown button, just for error checking
	GAMEPAD_BUTTON_LEFT_FACE_UP,        // Gamepad left DPAD up button
	GAMEPAD_BUTTON_LEFT_FACE_RIGHT,     // Gamepad left DPAD right button
	GAMEPAD_BUTTON_LEFT_FACE_DOWN,      // Gamepad left DPAD down button
	GAMEPAD_BUTTON_LEFT_FACE_LEFT,      // Gamepad left DPAD left button
	GAMEPAD_BUTTON_RIGHT_FACE_UP,       // Gamepad right button up (i.e. PS3: Triangle, Xbox: Y)
	GAMEPAD_BUTTON_RIGHT_FACE_RIGHT,    // Gamepad right button right (i.e. PS3: Square, Xbox: X)
	GAMEPAD_BUTTON_RIGHT_FACE_DOWN,     // Gamepad right button down (i.e. PS3: Cross, Xbox: A)
	GAMEPAD_BUTTON_RIGHT_FACE_LEFT,     // Gamepad right button left (i.e. PS3: Circle, Xbox: B)
	GAMEPAD_BUTTON_LEFT_TRIGGER_1,      // Gamepad top/back trigger left (first), it could be a trailing button
	GAMEPAD_BUTTON_LEFT_TRIGGER_2,      // Gamepad top/back trigger left (second), it could be a trailing button
	GAMEPAD_BUTTON_RIGHT_TRIGGER_1,     // Gamepad top/back trigger right (one), it could be a trailing button
	GAMEPAD_BUTTON_RIGHT_TRIGGER_2,     // Gamepad top/back trigger right (second), it could be a trailing button
	GAMEPAD_BUTTON_MIDDLE_LEFT,         // Gamepad center buttons, left one (i.e. PS3: Select)
	GAMEPAD_BUTTON_MIDDLE,              // Gamepad center buttons, middle one (i.e. PS3: PS, Xbox: XBOX)
	GAMEPAD_BUTTON_MIDDLE_RIGHT,        // Gamepad center buttons, right one (i.e. PS3: Start)
	GAMEPAD_BUTTON_LEFT_THUMB,          // Gamepad joystick pressed button left
	GAMEPAD_BUTTON_RIGHT_THUMB          // Gamepad joystick pressed button right
} GamepadButton;

// Gamepad axis
typedef enum
{
	GAMEPAD_AXIS_LEFT_X = 0,     // Gamepad left stick X axis
	GAMEPAD_AXIS_LEFT_Y = 1,     // Gamepad left stick Y axis
	GAMEPAD_AXIS_RIGHT_X = 2,     // Gamepad right stick X axis
	GAMEPAD_AXIS_RIGHT_Y = 3,     // Gamepad right stick Y axis
	GAMEPAD_AXIS_LEFT_TRIGGER = 4,     // Gamepad back trigger left, pressure level: [1..-1]
	GAMEPAD_AXIS_RIGHT_TRIGGER = 5      // Gamepad back trigger right, pressure level: [1..-1]
} GamepadAxis;

// Gesture
// NOTE: It could be used as flags to enable only some gestures
typedef enum
{
	GESTURE_NONE = 0,        // No gesture
	GESTURE_TAP = 1,        // Tap gesture
	GESTURE_DOUBLETAP = 2,        // Double tap gesture
	GESTURE_HOLD = 4,        // Hold gesture
	GESTURE_DRAG = 8,        // Drag gesture
	GESTURE_SWIPE_RIGHT = 16,       // Swipe right gesture
	GESTURE_SWIPE_LEFT = 32,       // Swipe left gesture
	GESTURE_SWIPE_UP = 64,       // Swipe up gesture
	GESTURE_SWIPE_DOWN = 128,      // Swipe down gesture
	GESTURE_PINCH_IN = 256,      // Pinch in gesture
	GESTURE_PINCH_OUT = 512       // Pinch out gesture
} Gesture;

///////////////////////////////////////////////////////////////

#endif