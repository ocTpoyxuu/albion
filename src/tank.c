#include <math.h>
#include "tank.h"

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
	int portvw = app.portw/tank->world->coef,
		portvh = app.porth/tank->world->coef;

	sx = (int)tank->x - tank->world->camx + portvw/2;
	sy = portvh/2 - (int)tank->y + tank->world->camy;

	glPushMatrix();
	glTranslated(sx, sy, 0);
	glRotatef(tank->angle, 0.0f, 0.0f, 1.0f);

	glColor3f(1,0,0);
	glBegin(GL_QUADS);
		glVertex2i( tank->w/2,  tank->h/2);
		glVertex2i(-tank->w/2,  tank->h/2);
		glVertex2i(-tank->w/2, -tank->h/2);
		glVertex2i( tank->w/2, -tank->h/2);
	glEnd();

	glPopMatrix();
}
