#include "settler.h"

Settler * getSettler(Unit * u, Village * v)
{
	Settler * s = (Settler *)malloc(sizeof(Settler));

	s->unit = u;
	s->village = v;
	s->age = 0;

	return s;
}

void DrawSettler(Settler * s)
{
	int x1, x2, x3, y1, y2, y3;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
		x1 = s->unit->map->tsx*s->unit->tx + s->unit->map->tsx*0.125;
		x2 = s->unit->map->tsx*s->unit->tx + s->unit->map->tsx*0.5;
		x3 = s->unit->map->tsx*s->unit->tx + s->unit->map->tsx*0.875;
		y1 = s->unit->map->tsy*s->unit->ty + s->unit->map->tsy*0.125;
		y2 = s->unit->map->tsy*s->unit->ty + s->unit->map->tsy*0.5;
		y3 = s->unit->map->tsy*s->unit->ty + s->unit->map->tsy*0.875;

		glColor3f(1.0, 0.0, 1.0);
		glVertex2i(x2, y1);
		glVertex2i(x3, y2);
		glVertex2i(x2, y3);
		glVertex2i(x1, y2);
	glEnd();
}
