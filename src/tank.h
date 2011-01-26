#ifndef _TANK_HPP_
#define _TANK_HPP_

#include "main.h"
#include "world.h"

typedef struct tagTANK {
	int w, h;
	float x, y;
	float vx, vy;
	float ax, ay;
	float angle;
	float wa;
	float omega;

	World * world;
} Tank;

void UpdateTank(Tank *, float);

void DrawTank(Tank *);


#endif
