///////////////////////////////////////////////////////////////
#include <stdlib.h>
#include <stdio.h>

#include <errno.h>
#include <string.h>
///////////////////////////////////////////////////////////////
#if defined(_WIN32)

#ifndef __STDC_VERSION__
//ISO released a corrigendum yesterday 2012-07-15 Cor 1:2012 for C11: value __STDC_VERSION__ is now fixed to 201112L
#define __STDC_VERSION__ 201112L
#endif

#define PTW32_STATIC_LIB
#define HAVE_STRUCT_TIMESPEC

#endif
///////////////////////////////////////////////////////////////
#include <stdatomic.h>

#include <pthread.h>
#include <sched.h>
///////////////////////////////////////////////////////////////
#include "raylib.h"

#define LOGI(...) TraceLog(LOG_INFO, __VA_ARGS__)
//#define LOGI(...) (void)0
///////////////////////////////////////////////////////////////
#include "..\platform\android\tools.h"
///////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////
struct android_app_test
{
	pthread_mutex_t mutex;
	pthread_cond_t cond;

	pthread_t thread;

	int running;
};

static void* android_app_entry(void* param)
{
	struct android_app_test* android_app_test = (struct android_app_test*)param;

	pthread_mutex_lock(&android_app_test->mutex);
	android_app_test->running = 1;
	pthread_cond_broadcast(&android_app_test->cond);
	pthread_mutex_unlock(&android_app_test->mutex);

	int i = 0;
	while(1)
	{
		i++;

		int pollingDelay = 10;
		//sleep:
#ifdef _WIN32
//		Sleep(pollingDelay);
#else
		usleep(pollingDelay * 1000);  // sleep for 100 milliSeconds  - usleep - suspend execution for microsecond intervals
#endif
	}

	return NULL;
}

///////////////////////////////////////////////////////////////

atomic_int acnt;
int cnt;
void* adding(void* input)
{
	for(int i = 0; i < 10000; i++)
	{
		//acnt++;
		atomic_fetch_add(&acnt, 1);
		cnt++;
	}
	pthread_exit(NULL);
}

///////////////////////////////////////////////////////////////

int main(void)
{
#if defined(PLATFORM_ANDROID)
	AutoHideNavBarOnGainedFocus();
#endif

	//--------------------------------------------------------------------------------------

	pthread_t tid[10];
	for(int i = 0; i < 5; i++)
		pthread_create(&tid[i], NULL, adding, NULL);
	for(int i = 0; i < 5; i++)
		pthread_join(tid[i], NULL);

	//printf("the value of acnt is %d\n", acnt);
	//printf("the value of cnt is %d\n", cnt);

	LOGI("=>the value of acnt is %d", acnt);
	LOGI("=>the value of cnt is %d", cnt);

	//--------------------------------------------------------------------------------------

	struct android_app_test* android_app_test = calloc(1, sizeof(struct android_app_test));

	pthread_mutex_init(&android_app_test->mutex, NULL);
	pthread_cond_init(&android_app_test->cond, NULL);

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	pthread_create(&android_app_test->thread, &attr, android_app_entry, android_app_test);

	// Wait for thread to start.
	pthread_mutex_lock(&android_app_test->mutex);
	while(!android_app_test->running)
	{
		pthread_cond_wait(&android_app_test->cond, &android_app_test->mutex);
	}
	pthread_mutex_unlock(&android_app_test->mutex);

	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 0;
	int screenHeight = 0;

	//SetConfigFlags(FLAG_FULLSCREEN_MODE);

	InitWindow(screenWidth, screenHeight, "raylib [audio] example - Multichannel sound playing");
	//InitWindow(0, 0, "raylib [audio] example - Multichannel sound playing");

	screenWidth = GetScreenWidth();
	screenHeight = GetScreenHeight();

	screenWidth = GetRenderWidth();
	screenHeight = GetRenderHeight();

	InitAudioDevice();      // Initialize audio device

	Sound fxWav = LoadSound("resources/sound.wav");         // Load WAV audio file
	Sound fxOgg = LoadSound("resources/target.ogg");        // Load OGG audio file

	SetSoundVolume(fxWav, 0.20f);
	SetSoundVolume(fxOgg, 0.20f);

	SetTargetFPS(60);       // Set our game to run at 60 frames-per-second
	//--------------------------------------------------------------------------------------

	// Main game loop
	while(!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		if(IsKeyPressed(KEY_ENTER) || IsGestureDetected(GESTURE_TAP)) PlaySoundMulti(fxWav);     // Play a new wav sound instance
		if(IsKeyPressed(KEY_SPACE) || IsGestureDetected(GESTURE_DOUBLETAP)) PlaySoundMulti(fxOgg);     // Play a new ogg sound instance
		//----------------------------------------------------------------------------------

		// Draw
		//----------------------------------------------------------------------------------
		BeginDrawing();

		ClearBackground(RAYWHITE);

		DrawText("MULTICHANNEL SOUND PLAYING", 20, 20, 20, GRAY);
		DrawText("Press SPACE to play new ogg instance!", 200, 120, 20, LIGHTGRAY);
		DrawText("Press ENTER to play new wav instance!", 200, 180, 20, LIGHTGRAY);

		DrawText(TextFormat("CONCURRENT SOUNDS PLAYING: %02i", GetSoundsPlaying()), 220, 280, 20, RED);

		EndDrawing();
		//----------------------------------------------------------------------------------
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------
	StopSoundMulti();       // We must stop the buffer pool before unloading

	UnloadSound(fxWav);     // Unload sound data
	UnloadSound(fxOgg);     // Unload sound data

	CloseAudioDevice();     // Close audio device

	CloseWindow();          // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	return 0;
}
