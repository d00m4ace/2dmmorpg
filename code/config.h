#define USE_STDLIB 1

#define USE_FLOAT32 1

#define USE_FIXPT 1

//#define USE_FIXPT_TYPE int16
#define USE_FIXPT_TYPE int32
//#define USE_FIXPT_TYPE int64

//#define USE_FIXPT_FRACT_SHIFT 5U
//#define USE_FIXPT_FRACT_SHIFT 8U
#define USE_FIXPT_FRACT_SHIFT 10U
//#define USE_FIXPT_FRACT_SHIFT 12U
//#define USE_FIXPT_FRACT_SHIFT 16U

//#define USE_FIXPT_OVERFLOW_TYPE int16
//#define USE_FIXPT_OVERFLOW_TYPE int32
#define USE_FIXPT_OVERFLOW_TYPE int64

////////////////////////////////
// low end android 1280x720
//#define GAME_SCR_WIDTH_MIN 320
//#define GAME_SCR_HEIGHT_MIN 180
// mid end android 1600x720
//#define GAME_SCR_WIDTH_MIN 400
//#define GAME_SCR_HEIGHT_MIN 180
// hi end android 1080+
#define GAME_SCR_WIDTH_MIN 422
#define GAME_SCR_HEIGHT_MIN 216
////////////////////////////////
#define GAME_SCR_WIDTH_BORDER 6
#define GAME_SCR_HEIGHT_BORDER 6
////////////////////////////////
#define GAME_GUI_WIDTH 320
#define GAME_GUI_HEIGHT 180
////////////////////////////////

#if 0
#undef PRINT
#define PRINT(...) ((void)0)
#else
const char* txt_format(const char* text, ...);
char* txt_win1251_to_oem866(char* text);
#if defined(RAYLIB_VERSION)||1
#ifndef LOG_INFO
#define LOG_INFO 3
void TraceLog(int logLevel, const char* text, ...);
#endif // !LOG_INFO
#define PRINT(...) TraceLog(LOG_INFO, txt_win1251_to_oem866((char*)txt_format(__VA_ARGS__)))
//#define PRINT(...) TraceLog(LOG_INFO, __VA_ARGS__)
#else
#define PRINT(...) printf(txt_win1251_to_oem866((char*)txt_format("%s\n",txt_format(__VA_ARGS__))))
//#define PRINT(val, ...) printf(val "\n", ##__VA_ARGS__) 
#endif // RAYLIB_VERSION
#endif

////////////////////////////////
#define ASSERT(expression) assert(expression)
//#define ASSERT(expression) ((void)0)
////////////////////////////////
#define NETSESSION_STATE_BUF_SIZE (1024*9)
////////////////////////////////
#define NETSOCKET_STATE_BUF_SIZE (1024)
////////////////////////////////
#define CLIENT_VERSION 0x0100
#define CLIENT_PLATFORM_ID 0x01
////////////////////////////////
#define CLIENT_SEND_LIMIT NETSOCKET_STATE_BUF_SIZE
//#define CLIENT_SEND_LIMIT 250
//#define CLIENT_SEND_LIMIT 90
////////////////////////////////
#define CLIENT_TYPE_ID 111
////////////////////////////////
#define SERVER_VERSION 0x1000
#define SERVER_PLATFORM_ID 0x100
////////////////////////////////
#define SERVER_SEND_LIMIT NETSOCKET_STATE_BUF_SIZE
//#define SERVER_SEND_LIMIT 90
////////////////////////////////
//#define SERVER_MAX_NETSESSION 0
#define SERVER_MAX_NETSESSION 5000
#define SERVER_MAX_NETTHREADS (SERVER_MAX_NETSESSION * 5)
////////////////////////////////
#define DEFAULT_BACKLOG 10000
#define DEFAULT_LOOP_BACK_NOP_TIME_MS 30
////////////////////////////////

////////////////////////////////
#define DEFAULT_SOCKET_TIMEOUT_MS (5000)
#define DEFAULT_THREADS_TIMEOUT (6*CLOCKS_PER_SEC)
////////////////////////////////
//#define DEFAULT_SOCKET_TIMEOUT_MS (60000*5)
//#define DEFAULT_THREADS_TIMEOUT (5*60*CLOCKS_PER_SEC)
////////////////////////////////

////////////////////////////////
#define NETMONITOR_SHOW_NETPACKETS 0
#define NETMONITOR_SKIP_PACKET_COMMON_NOP_ID 1
////////////////////////////////
#define FILENAME_MAX_SIZE 80
////////////////////////////////
#define USER_NAME_MAX 20
#define USER_PASSWORD_MAX 50

#define PASSWORD_HASH_SALT "kSEo2InipM4sYIES"
////////////////////////////////
#define SERVER_IP_ADDRESS "192.168.2.6"
#define SERVER_PORT 7979
////////////////////////////////
#ifdef _GAMESERVER
//#define SOCKET_SEND_LAG rnd(100, 1000)
//#define SOCKET_RECV_LAG rnd(100, 1000)
#endif // _GAMESERVER
#ifdef _GAMECLIENT
#define SOCKET_SEND_LAG rnd(10, 100)
#define SOCKET_RECV_LAG rnd(10, 100)
//#define SOCKET_SEND_LAG 500
//#define SOCKET_RECV_LAG 500
#endif // _GAMECLIENT
////////////////////////////////

#ifdef PLATFORM_ANDROID
#define USE_LINUX
#define USE_PTHREAD
#endif // DEBUG

#ifdef __linux__
#define USE_LINUX
#define USE_PTHREAD
#endif // DEBUG

#ifdef _WIN32
//#define USE_CREATETHREAD
#define USE_PTHREAD
#endif // DEBUG

#ifndef bool
#include <stdbool.h>
#endif 

/*
#ifndef bool
#define bool _Bool
#endif
#ifndef true
#define true (1)
#endif
#ifndef false
#define false (0)
#endif
*/
