#ifndef _MAIN_HPP_
#define _MAIN_HPP_

#include <GL/glut.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>
#include <stdio.h>
#include <math.h>

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
