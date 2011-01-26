#include <math.h>
#include "tank.h"
#include "world.h"

void UpdateTank(Tank * tank, float dt)
{
	tank->x += tank->vx*dt + tank->ax*pow(dt, 2)/2;
	tank->y += tank->vy*dt + tank->ay*pow(dt, 2)/2;
	tank->angle += tank->wa*dt + tank->omega*pow(dt, 2)/2;
	
	tank->vx += tank->ax*dt;
	tank->vy += tank->ay*dt;
	tank->wa += tank->omega*dt;
}

void DrawTank(Tank * tank)
{
	int sx, sy;

	sx = (int)tank->x - tank->world->camx + app.portw/2;
	sy = app.porth/2 - (int)tank->y + tank->world->camy;

	//glPushMatrix();
	//glRotatef(tank->angle, 1.0f, 0.0f, 0.0f);

	glBegin(GL_QUADS);
		glVertex2i(sx + tank->w/2, sy + tank->h/2);
		glVertex2i(sx - tank->w/2, sy + tank->h/2);
		glVertex2i(sx - tank->w/2, sy - tank->h/2);
		glVertex2i(sx + tank->w/2, sy - tank->h/2);
	glEnd();

	//glPopMatrix();
}
