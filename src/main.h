#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#if defined(__linux) || defined(_WIN32)
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define DEBUG 0

#define WINDOW_DEFAULT_WIDTH 1000
#define WINDOW_DEFAULT_HEIGHT 768
#define WINDOW_DEFAILT_TOP_POSITION 0
#define WINDOW_DEFAILT_LEFT_POSITION 200
#define ENGINE_DEFAULT_CTRL_RATE 30.0f
#define DEFAULT_FULLSCREEN_STATE 0

#define MAP_FLAT 0
#define MAP_LEVEL 1
#define MAP_HIGHMAP 2

#define MAP_X 256
#define MAP_Y 256
#define TILE_SIZE_X 50
#define TILE_SIZE_Y 50
#define MAP_WATER_LEVEL 0.3
#define MAP_MOUNTAIN_LEVEL 0.7
#define MAP_GROUND_LEVEL 0.5
#define MAP_DEFAULT_STYLE MAP_FLAT
#define MAP_DEFAULT_ZOOM -8
/*#define MAP_DEFAULT_ZOOM 16*/
/*#define WORLD_SEC_PER_TURN 3.0*/
#define WORLD_SEC_PER_TURN 0.25

#define CAM_STEP 5
#define CAM_ZOOM_MAX 16.0f
#define CAM_ZOOM_MIN -16.0f
#define CAM_ZOOM_COEF_IN 0.0625f
#define CAM_ZOOM_COEF_OUT 0.05f

typedef struct tagAPPLICATION {
	float ctlfrate;
	int winw, winh;
	int winl, wint;
	int portw, porth;
	int fullscreen;

	float fps, dps;
} Application;

extern const char AppName [];
extern Application app;

void randomize();

#endif
