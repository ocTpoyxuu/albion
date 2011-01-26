#include <stdlib.h>
#include "world.h"
#include "tank.h"

const unsigned int mapx = 1024;
const unsigned int mapy = 1024;

const int tilesizex = 50;
const int tilesizey = 50;
const float zoommax = 16.0;
const float zoommin = -16.0;
const float zoomcin = 0.0625;
const float zoomcout = 0.05;

const int camstep = 10;

Tile * getTile(int, int);

World * world;
Tank * t1, * t2;

void SetupWorld()
{
	int i, j;
	Tile * t;

	world = (World *)malloc(sizeof(World));

	world->camx = 0;
	world->camy = 0;
	world->camstep = camstep;
	world->tsx = tilesizex;
	world->tsy = tilesizey;
	world->zoom = 0.0;
	world->coef = 1.0;

	world->map = (Map *)malloc(sizeof(Map));
	world->map->width = mapx;
	world->map->height = mapy;
	world->map->tiles = (Tile *)malloc(sizeof(Tile)*(world->map->width*world->map->height));

	for (i = 0; i < world->map->width; i++)
		for (j = 0; j < world->map->height; j++)
		{
			t = getTile(i, j);
			float c = (rand() % 16777216) / 16777216.0f;

			t->color[0] = c;
			t->color[1] = c;
			t->color[2] = c;
			t->color[3] = 1.0f;
			t->cx = i;
			t->cy = j;
		}

/*MoveCameraAtTile(world->map->width / 2, world->map->height / 2);*/

	MoveCameraAtTile(5,5);

	t1 = (Tank *)malloc(sizeof(Tank));
	t1->x = 250;
	t1->y = 0;
	printf("Tank1 x %f y %f\n", t1->x, t1->y);
	t1->h = 50;
	t1->w = 50;
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
	t2->h = 50;
	t2->w = 50;
	t2->vx = -10;
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
	free(world->map->tiles);
	free(world->map);
	free(world);
}

void MoveCamera(float dx, float dy)
{
	int mapvx = world->map->width*world->tsx,
		mapvy = world->map->height*world->tsy;
	int portvw = app.portw/world->coef,
		portvh = app.porth/world->coef;

	world->camx += dx*(world->camstep);
	world->camy += dy*(world->camstep);

	if (world->camx - portvw/2 < 0)
		world->camx = portvw/2;
	if (world->camx + portvw/2 > mapvx)
		world->camx = mapvx - portvw/2;
	if (world->camy - portvh/2 < 0)
		world->camy = portvh/2;
	if (world->camy + portvh/2 > mapvy)
		world->camy = mapvy - portvh/2;
}

void MoveCameraAtTile(int x, int y)
{
	int mapvx = world->map->width*world->tsx,
		mapvy = world->map->height*world->tsy;
	int portvw = app.portw/world->coef,
		portvh = app.porth/world->coef;

	if (x < 0)
		world->camx = 0;
	else if (x >= world->map->width)
		world->camx = mapvx - world->tsx / 2;
	else
		world->camx = world->tsx*x + world->tsx / 2;

	if (y < 0)
		world->camy = 0;
	else if (y >= world->map->height)
		world->camy = mapvy - world->tsy / 2;
	else
		world->camy = world->tsy*y + world->tsy / 2;

	if (world->camx - portvw/2 < 0)
		world->camx = portvw/2;
	if (world->camx + portvw/2 > mapvx)
		world->camx = mapvx - portvw/2;
	if (world->camy - portvh/2 < 0)
		world->camy = portvh/2;
	if (world->camy + portvh/2 > mapvy)
		world->camy = mapvy - portvh/2;
}

void ZoomCamera(int s)
{
	int z = world->zoom + s;

	if (z > zoommax)
		z = zoommax;
	else if (z < zoommin)
		z = zoommin;
	world->zoom = z;

	if (z >= 0)
		world->coef = 1 + world->zoom*zoomcin;
	else if (z < 0)
		world->coef = 1 + world->zoom*zoomcout;
}

void DrawWorld()
{
	int i, j;
	int wx1, wx2, wy1, wy2;
	int cx1, cx2, cy1, cy2;
	int mapvx = world->map->width*world->tsx,
		mapvy = world->map->height*world->tsy;
	int portvw = app.portw/world->coef,
		portvh = app.porth/world->coef;
	Tile * t;

	wx1 = world->camx - portvw/2;
	wx2 = world->camx + portvw/2;
	wy1 = world->camy - portvh/2;
	wy2 = world->camy + portvh/2;

	if (wx1 <= 0)
		cx1 = 0;
	else if (wx1 >= mapvx)
		cx1 = world->map->width;
	else
		cx1 = wx1 / world->tsx;

	if (wy1 <= 0)
		cy1 = 0;
	else if (wy1 >= mapvy)
		cy1 = world->map->height;
	else
		cy1 = wy1 / world->tsy;

	if (wx2 <= 0)
		cx2 = 0;
	else if (wx2 >= mapvx)
		cx2 = world->map->width;
	else
		cx2 = wx2 / world->tsx + 1;

	if (wy2 <= 0)
		cy2 = 0;
	else if (wy2 >= mapvx)
		cy2 = world->map->height;
	else
		cy2 = wy2 / world->tsy + 1;

	glScalef(world->coef, world->coef, 1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_QUADS);
	for (i = cx1; i < cx2; i++)
		for (j = cy1; j < cy2; j++)
		{
			t = getTile(i, j);

			int x1 = world->tsx*(t->cx) - world->camx + portvw/2;
			int x2 = world->tsx*(t->cx + 1) - world->camx + portvw/2;
			int y1 = portvh/2 - world->tsy*(t->cy) + world->camy;
			int y2 = portvh/2 - world->tsy*(t->cy + 1) + world->camy;

			glColor4f(t->color[0], t->color[1], t->color[2], t->color[3]);
			glVertex2i(x1, y2);
			glVertex2i(x1, y1);
			glVertex2i(x2, y1);
			glVertex2i(x2, y2);
		}
	glEnd();
	

	DrawTank(t1);
	DrawTank(t2);
	
	
	/*glColor3f(1.0, 0.0, 0.0);*/
	/*glBegin(GL_LINES);*/
		/*glVertex2i(portvw/4, portvh/4);*/
		/*glVertex2i(3*portvw/4, portvh/4);*/

		/*glVertex2i(3*portvw/4, portvh/4);*/
		/*glVertex2i(3*portvw/4, 3*portvh/4);*/

		/*glVertex2i(3*portvw/4, 3*portvh/4);*/
		/*glVertex2i(portvw/4, 3*portvh/4);*/

		/*glVertex2i(portvw/4, 3*portvh/4);*/
		/*glVertex2i(portvw/4, portvh/4);*/
	/*glEnd();*/
}

void UpdateWorld(float dt)
{
	UpdateTank(t1, dt);
	UpdateTank(t2, dt);
}

Tile * getTile(int x, int y)
{
	return world->map->tiles + world->map->width*y + x;
}
