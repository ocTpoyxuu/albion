#include <stdlib.h>
#include "world.h"
#include "tank.h"

const unsigned int mapx = 512;
const unsigned int mapy = 512;

const int tilesizex = 50;
const int tilesizey = 50;
const float zoommax = 16.0;
const float zoommin = -16.0;
const float zoomcin = 0.0625;
const float zoomcout = 0.05;

const int camstep = 10;

World * world;

void DrawMapVertex(World *);
Tile * getTile(Map *, int, int);
void DrawMap(World *);
Map * getMap(World *, int, int);
Map * generateMap(Map *);
Map * smoothMap(Map *);
Map * coverMap(Map *);
Map * normalizeMap(Map *);
Map * randomizeMap(Map *);
Map * quadMap(Map *);

Tank * t1, * t2;

void SetupWorld()
{
	int i;

	world = (World *)malloc(sizeof(World));

	world->camx = 0;
	world->camy = 0;
	world->camstep = camstep;
	world->tsx = tilesizex;
	world->tsy = tilesizey;
	world->zoom = 0.0;
	world->coef = 1.0;
	world->map = getMap(world, mapx, mapy);

	/*randomizeMap(world->map);*/
	generateMap(world->map);
	smoothMap(world->map);
	quadMap(world->map);
	normalizeMap(world->map);
	coverMap(world->map);

	/*MoveCameraAtTile(world->map->width / 2, world->map->height / 2);*/
	MoveCameraAtTile(5, 5);
	ZoomCamera(-16);

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
	glScalef(world->coef, world->coef, 1.0);

	DrawMap(world);
	/*DrawMapVertex(world);*/

	DrawTank(t1);
	DrawTank(t2);
}

void UpdateWorld(float dt)
{
	UpdateTank(t1, dt);
	UpdateTank(t2, dt);
}

Tile * getTile(Map * m, int x, int y)
{
	return m->tiles + m->width*(y % m->height) + (x % m->width);
}

void DrawMap(World * w)
{
	int i, j;
	int wx1, wx2, wy1, wy2;
	int cx1, cx2, cy1, cy2;
	int mapvx = w->map->width*w->tsx,
		mapvy = w->map->height*w->tsy;
	int portvw = app.portw/w->coef,
		portvh = app.porth/w->coef;
	Tile * t;

	wx1 = w->camx - portvw/2;
	wx2 = w->camx + portvw/2;
	wy1 = w->camy - portvh/2;
	wy2 = w->camy + portvh/2;

	if (wx1 <= 0)
		cx1 = 0;
	else if (wx1 >= mapvx)
		cx1 = w->map->width;
	else
		cx1 = wx1 / w->tsx;

	if (wy1 <= 0)
		cy1 = 0;
	else if (wy1 >= mapvy)
		cy1 = w->map->height;
	else
		cy1 = wy1 / w->tsy;

	if (wx2 <= 0)
		cx2 = 0;
	else if (wx2 >= mapvx)
		cx2 = w->map->width;
	else
		cx2 = wx2 / w->tsx + 1;

	if (wy2 <= 0)
		cy2 = 0;
	else if (wy2 >= mapvx)
		cy2 = w->map->height;
	else
		cy2 = wy2 / w->tsy + 1;


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glBegin(GL_QUADS);
	for (i = cx1; i < cx2; i++)
		for (j = cy1; j < cy2; j++)
		{
			t = getTile(w->map, i, j);

			int x1 = w->tsx*(t->cx) - w->camx + portvw/2;
			int x2 = w->tsx*(t->cx + 1) - w->camx + portvw/2;
			int y1 = portvh/2 - w->tsy*(t->cy) + w->camy;
			int y2 = portvh/2 - w->tsy*(t->cy + 1) + w->camy;

			glColor4f(t->color[0], t->color[1], t->color[2], t->color[3]);
			glVertex2i(x1, y2);
			glVertex2i(x1, y1);
			glVertex2i(x2, y1);
			glVertex2i(x2, y2);
		}
	glEnd();
	
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

void DrawMapVertex(World * w)
{
	int i, j;
	Tile * t, * tb, * tr, * tbr;

	glScalef(w->coef, w->coef, 1.0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glPushMatrix();
	/*glTranslated(-w->camx, -w->camy, 0);*/

	glBegin(GL_QUADS);
	for (i = 0; i < w->map->width; i++)
		for (j = 0; j < w->map->height; j++)
		{
			t = getTile(w->map, i, j);
			tb = getTile(w->map, i, j + 1);
			tr = getTile(w->map, i + 1, j);
			tbr = getTile(w->map, i + 1, j + 1);

			int x1 = w->tsx*t->cx;
			int x2 = w->tsx*(t->cx + 1);
			int y1 = w->tsy*t->cy;
			int y2 = w->tsy*(t->cy + 1);

			glColor4f(tb->color[0], tb->color[1], tb->color[2], tb->color[3]);
			glVertex2i(x1, y2);
			glColor4f(t->color[0], t->color[1], t->color[2], t->color[3]);
			glVertex2i(x1, y1);
			glColor4f(tr->color[0], tr->color[1], tr->color[2], tr->color[3]);
			glVertex2i(x2, y1);
			glColor4f(tbr->color[0], tbr->color[1], tbr->color[2], tbr->color[3]);
			glVertex2i(x2, y2);
		}
	/*glColor3f(1.0, 0.0, 0.0);*/
	/*glVertex2i(0, 1000);*/
	/*glColor3f(1.0, 1.0, 0.0);*/
	/*glVertex2i(0, 0);*/
	/*glColor3f(0.0, 1.0, 0.0);*/
	/*glVertex2i(1000, 0);*/
	/*glColor3f(0.0, 1.0, 1.0);*/
	/*glVertex2i(1000, 1000);*/
	glEnd();

	glPopMatrix();
	

	/*DrawTank(t1);*/
	/*DrawTank(t2);*/
	
	
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

Map * getMap(World * w, int width, int height)
{
	int i, j;
	Tile * t;
	Map * m;

	m = (Map *)malloc(sizeof(Map));
	m->width = width;
	m->height = height;
	m->tiles = (Tile *)malloc(sizeof(Tile)*(m->width*m->height));

	for (i = 0; i < m->width; i++)
		for (j = 0; j < m->height; j++)
		{
			t = getTile(m, i, j);

			t->color[0] = 1.0f;
			t->color[1] = 1.0f;
			t->color[2] = 1.0f;
			t->color[3] = 1.0f;
			t->cx = i;
			t->cy = j;
			t->ground = 0.0f;
		}

	return m;
}

Map * generateMap(Map * m)
{
	int i, j;
	Tile * t;

	for (j = 0; j < m->height; j++)
	{
		t = getTile(m, 0, j);
		t->ground = 0.0f;
		t = getTile(m, m->width - 1, j);
		t->ground = 0.0f;
	}

	for (i = 0; i < m->width; i++)
	{
		t = getTile(m, i, 0);
		t->ground = 0.0f;
		t = getTile(m, i, m->height - 1);
		t->ground = 0.0f;
	}

	for (i = 1; i < m->width - 1; i++)
		for (j = 1; j < m->height - 1; j++)
		{
			t = getTile(m, i, j);

			t->ground = (rand() % 256) / 255.0f;
		}

	return m;
}

Map * smoothMap(Map * m)
{
	int i, j, k, l;
	Tile * t;
	float * grounds;

	grounds = (float *)malloc(sizeof(float)*m->width*m->height);

	for (i = 1; i < m->width - 1; i++)
		for (j = 1; j < m->height - 1; j++)
		{
			*(grounds + m->width*j + i) = 0.0f;

			for (k = i - 1; k <= i + 1; k++)
				for (l = j - 1; l <= j + 1; l++)
				{
					t = getTile(m, k, l);
					*(grounds + m->width*j + i) += t->ground;
				}

			t = getTile(m, i, j);
			t->ground = *(grounds + m->width*j + i) / 9.0f;
		}

	free(grounds);

	return m;
}

Map * coverMap(Map * m)
{
	const float waterLevel = 0.3;
	const float mountainLevel = 0.7;
	const float groundLevel = 0.5;
	int i, j;
	Tile * t;

	for (i = 0; i < m->width; i++)
		for (j = 0; j < m->height; j++)
		{
			t = getTile(m, i, j);

			/*if (t->ground <= groundLevel)*/
			/*{*/
				/*t->color[0] = 0.0f;*/
				/*t->color[1] = t->ground / groundLevel;*/
				/*t->color[2] = 1.0f - t->ground / groundLevel;*/
			/*}*/
			/*else*/
			/*{*/
				/*t->color[0] = t->ground / groundLevel - 1.0f;*/
				/*t->color[1] = 2.0f - t->ground / groundLevel;*/
				/*t->color[2] = 0.0f;*/
			/*}*/

			if (t->ground < waterLevel)
			{
				t->color[0] = 0.0f;
				t->color[1] = 0.0f;
				t->color[2] = 1.0f;
			}
			else if (t->ground >= waterLevel && t->ground < mountainLevel)
			{
				t->color[0] = 0.0f;
				t->color[1] = 1.0f;
				t->color[2] = 0.0f;
			}
			else
			{
				t->color[0] = 1.0f;
				t->color[1] = 0.0f;
				t->color[2] = 0.0f;
			}

			/*t->color[0] = t->ground;*/
			/*t->color[1] = t->ground;*/
			/*t->color[2] = t->ground;*/
		}

	return m;
}

Map * normalizeMap(Map * m)
{
	int i, j;
	Tile * t;
	float min = 1.0f, max = 0.0f;

	for (i = 0; i < m->width; i++)
		for (j = 0; j < m->height; j++)
		{
			t = getTile(m, i, j);

			if (t->ground < min)
				min = t->ground;

			if (t->ground > max)
				max = t->ground;
		}

	printf("Min %f max %f\n", min, max);

	for (i = 0; i < m->width; i++)
		for (j = 0; j < m->height; j++)
		{
			t = getTile(m, i, j);

			t->ground = (t->ground - min) / (max - min);
		}

	return m;
}

Map * randomizeMap(Map * m)
{
	int i, j, k;
	float c;
	Tile * t;

	for (k = 0; k < m->width*m->height/10; k++)
	{
		i = rand()%(m->width);
		j = rand()%(m->height);
		c = (rand()%601 - 300)/300.0f;
		t = getTile(m, i, j);
		t->ground += c*(c < 0 ? t->ground : 1.0 - t->ground);
	}

	return m;
}

Map * quadMap(Map * m)
{
	int i, j, k;
	float c;
	Tile * t;

	for (i = 0; i < m->width; i++)
		for (j = 0; j < m->height; j++)
		{
			t = getTile(m, i, j);
			t->ground = pow(t->ground*2.0, 2)/4.0;
		}

	return m;
}
