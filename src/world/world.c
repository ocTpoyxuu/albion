#include <stdlib.h>
#include "world.h"
#include "tank.h"

World * world;

Tank * t1, * t2;

void ZoomCamera(int s)
{
	camZoom(world->map->cam, s);
	camTest(world->map->cam);
}

void DragCamera(int dx, int dy)
{
	camDrag(world->map->cam, dx, dy);
}

void SetupWorld()
{
	world = (World *)malloc(sizeof(World));

	world->map = getMap(MAP_X, MAP_Y, TILE_SIZE_X, TILE_SIZE_Y);

	/*randomizeMap(world->map);*/
	generateMap(world->map);
	randomizeMap(world->map);
	minmaxMap(world->map, NULL, NULL);
	quadMap(world->map);
	minmaxMap(world->map, NULL, NULL);
	smoothMap(world->map, 0.025);
	minmaxMap(world->map, NULL, NULL);
	smoothMap(world->map, 0.01);
	minmaxMap(world->map, NULL, NULL);
	smoothMap(world->map, 0.025);
	minmaxMap(world->map, NULL, NULL);

	/*normalizeMap(world->map);*/
	/*smoothMap(world->map, 0.01);*/
	/*quadMap(world->map);*/
	/*normalizeMap(world->map);*/
	/*smoothMap(world->map, 0.025);*/
	coverMap(world->map);

	/*MoveCameraAtTile(world->map, world->map->width / 2, world->map->height / 2);*/
	MoveCameraAtTile(world->map, 5, 5);
	camZoom(world->map->cam, MAP_DEFALUT_ZOOM);

	t1 = (Tank *)malloc(sizeof(Tank));
	t1->x = 250;
	t1->y = 0;
	t1->h = 250;
	t1->w = 250;
	t1->vx = 50;
	t1->vy = 50;
	t1->ax = 5;
	t1->ay = 15;
	t1->angle = 45;
	t1->wa = 45;
	t1->omega = 10;
	t1->world = world;

	t2 = (Tank *)malloc(sizeof(Tank));
	t2->x = 350;
	t2->y = 0;
	t2->h = 250;
	t2->w = 250;
	t2->vx = 10;
	t2->vy = 0;
	t2->ax = 0;
	t2->ay = 10;
	t2->angle = 180;
	t2->wa = 20;
	t2->omega = 0;
	t2->world = world;
}

void DestroyWorld()
{
	free(world->map->cam);
	free(world->map->tiles);
	free(world->map);
	free(world);
}


void DrawWorld()
{
	int portvw, portvh;

	getViewport(world->map->cam, &portvw, &portvh);

	glPushMatrix();
	glLoadIdentity();
	glScalef(world->map->cam->coef, world->map->cam->coef, 1.0);
	glTranslated(-world->map->cam->camx + portvw/2, -world->map->cam->camy + portvh/2, 0);

	DrawMap(world->map);

	/*DrawTank(t1);*/
	/*DrawTank(t2);*/

	glPopMatrix();
}

void UpdateWorld(float dt)
{
	/*UpdateTank(t1, dt);*/
	/*UpdateTank(t2, dt);*/
}
