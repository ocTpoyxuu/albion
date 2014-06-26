#include "warrior.h"

Warrior * getWarrior(Unit * u, Village * v)
{
	Warrior * w = (Warrior *)malloc(sizeof(Warrior));

	w->unit = u;
	w->village = v;
	w->age = 0;
	w->atack = 0;
	w->defense = 0;

	return w;
}

void DrawWarrior(Warrior * w)
{
	int x1, x2, x3, y1, y2;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_TRIANGLES);
		x1 = w->unit->map->tsx*w->unit->tx + w->unit->map->tsx*0.25;
		x2 = w->unit->map->tsx*w->unit->tx + w->unit->map->tsx*0.5;
		x3 = w->unit->map->tsx*w->unit->tx + w->unit->map->tsx*0.75;
		y1 = w->unit->map->tsy*w->unit->ty + w->unit->map->tsy*0.25;
		y2 = w->unit->map->tsy*w->unit->ty + w->unit->map->tsy*0.75;

		glColor3f(1.0, 0.0, 1.0);
		glVertex2i(x1, y1);
		glVertex2i(x3, y1);
		glVertex2i(x2, y2);
	glEnd();
}
