#include "worker.h"

GLUquadric * quadric = NULL;

Worker * getWorker(Unit * u, Village * v)
{
	Worker * w = (Worker *)malloc(sizeof(Worker));

	w->unit = u;
	w->village = v;
	w->age = 0;

	return w;
}

void DrawWorker(Worker * w)
{
	int r;

	if (quadric == NULL)
		quadric = gluNewQuadric();

	r = (w->unit->map->tsy > w->unit->map->tsx ? w->unit->map->tsy : w->unit->map->tsx)*0.45;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPushMatrix();
	glTranslated(w->unit->map->tsx*w->unit->tx + w->unit->map->tsx/2,
		w->unit->map->tsy*w->unit->ty + w->unit->map->tsy/2, 0);
	gluDisk(quadric, 0.0, r, 60, 10);
	glPopMatrix();
}
