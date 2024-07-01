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
///////////////////////////////////////////////////////////////
#if defined(PLATFORM_ANDROID)
#include "../platform/android/tools.h"
#endif
///////////////////////////////////////////////////////////////
#include "../hardware/hal.h"
///////////////////////////////////////////////////////////////
#if defined(_WIN32)
#include "../platform/win32/win32.h"
#endif
///////////////////////////////////////////////////////////////
#if defined(USE_LINUX)
#include "../platform/linux/linux.h"
#endif
///////////////////////////////////////////////////////////////
#include "../game/game01/img/img.h"
#include "../game/game01/txt/txt_enums.h"
#include "../game/game01/img/img.c"
#include "../game/game01/txt/txt_en.c"
#include "../game/game01/txt/txt_ru.c"
///////////////////////////////////////////////////////////////
//#include "game-empty.c"
//#include "game-ui.c"
#include "chrs2mobs.c"
//#include "pixeleditor/main.c"
//#include "tools/imgimport.c"
//#include "tools/imgsplit.c"
//#include "tools/codegen.c"
///////////////////////////////////////////////////////////////
//#define _NETWORK
//#define _GAMESERVER
//#define _GAMECLIENT
#ifdef _NETWORK
#ifdef _GAMESERVER
//#include "network/myserver.c"
#include "2dmmorpg/server/gameserver.c"
#endif // _GAMESERVER
#ifdef _GAMECLIENT
//#include "network/myclient.c"
#include "2dmmorpg/client/gameclient.c"
#endif // _GAMECLIENT
#endif // _NETWORK
///////////////////////////////////////////////////////////////

/*
typedef enum
{
    EVENT_1,
    EVENT_2,
    EVENT_3,
    EVENT_INVALID
} event_t;

typedef enum
{
    STATE_INITIAL,
    STATE_A,
    STATE_B,
    STATE_C,
    STATE_FINAL
} state_t;

// State transition table
state_t state_table[][EVENT_INVALID] = {
    {STATE_A, STATE_B, STATE_INITIAL, },
    {STATE_FINAL, STATE_B, STATE_C, },
    {STATE_A, STATE_FINAL, STATE_INITIAL, },
    {STATE_A, STATE_B, STATE_FINAL, },
    {STATE_FINAL, STATE_FINAL, STATE_FINAL, }
};

state_t current_state = STATE_INITIAL;

int main(void)
{
    event_t input;

    while(current_state != STATE_FINAL)
    {
        printf("Current state: %d\n", current_state);
        printf("Enter event (1, 2, 3): ");
        scanf("%d", &input);
        if(input >= EVENT_INVALID || input <= 0)
        {
            printf("Invalid event\n\n");
        }
        else
        {
            current_state = state_table[current_state][input - 1];
            printf("\n");
        }
    }

    printf("Current state: %d\n", current_state);

    return 0;
}
*/

/*
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int sq_gen(int a, int b);

int sq_gen(int a, int b)
{
    static int i, resume = 0;
    if(resume) goto resume; else resume = 1;
    printf("Initializing, start = %d, end = %d\n", a, b);
    for(i = a; i <= b; i += 1)
    {
        printf("Looping, counter = %d\n", i);
        return i * i;
    resume:;
    }
    resume = 0;
    return 0;
}

int main(int argc, char** argv)
{
    int square;

    //assert(argc == 3);
    while((square = sq_gen((int)1, (int)10)))
    {
        printf("main() square = %d\n", square);
    }

    return 0;
}
*/