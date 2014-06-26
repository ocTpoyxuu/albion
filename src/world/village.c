#include "village.h"

const int scienceLVL[] = {5, 7, 10, 15};
const int populationLVL[] = {1, 2, 3, 4};
const int atackLVL[] = {1, 2, 3, 4};
const int defenceLVL[] = {1, 2, 3, 4};
const int cost [] = {0, 50, 150, 300};

Village * getVillage(Unit * u)
{
	Village * v = (Village *)malloc(sizeof(Village));

	v->unit = u;
	v->age = 0;
	v->population = 10;
	v->knowns = 0;
	v->tech = 0;
	v->grScience = scienceLVL[0];
	v->grPopulation = populationLVL[0];
	v->propAtck = atackLVL[0];
	v->propDef = defenceLVL[0];

	return v;
}

void DrawVillage(Village * v)
{
	int x1, x2, y1, y2;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
		x1 = v->unit->map->tsx*v->unit->tx + v->unit->map->tsx*0.1;
		x2 = v->unit->map->tsx*(v->unit->tx + 1) - v->unit->map->tsx*0.1;
		y1 = v->unit->map->tsy*v->unit->ty + v->unit->map->tsy*0.1;
		y2 = v->unit->map->tsy*(v->unit->ty + 1) - v->unit->map->tsy*0.1;

		glColor3f(0.5 + 0.5/pow(v->age, 1/2.25), 0.0, 1.0);
		glVertex2i(x2, y2);
		glVertex2i(x1, y2);
		glVertex2i(x1, y1);
		glVertex2i(x2, y1);
	glEnd();
}
