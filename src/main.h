#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <stdio.h>
#include <math.h>

#define WINDOW_DEFAULT_WIDTH 800
#define WINDOW_DEFAULT_HEIGHT 600
#define WINDOW_DEFAILT_TOP_POSITION 100
#define WINDOW_DEFAILT_LEFT_POSITION 200
#define ENGINE_DEFAULT_CTRL_RATE 30.0f
#define DEFAULT_FULLSCREEN_STATE 0

#define MAP_X 512
#define MAP_Y 512
#define TILE_SIZE_X 50
#define TILE_SIZE_Y 50

#define CAM_STEP 10
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

#endif
