#include "map.h"

void drawMapQuad(Map *, int *);

Tile * getTile(Map * m, int x, int y)
{
	return m->tiles + m->width*((y + m->height) % m->height) + ((x + m->width) % m->width);
}

void MoveCameraAtTile(Map * m, int x, int y)
{
	int camx, camy;

	if (x < 0)
		camx = 0;
	else if (x >= m->width)
		camx = m->cam->width - m->tsx / 2;
	else
		camx = m->tsx*x + m->tsx / 2;

	if (y < 0)
		camy = 0;
	else if (y >= m->height)
		camy = m->cam->height - m->tsy / 2;
	else
		camy = m->tsy*y + m->tsy / 2;

	camMoveAbsolute(m->cam, camx, camy);
}

void DrawMap(Map * m)
{
	int wx1, wx2, wy1, wy2;
	int q[4];
	int portvw, portvh;
	int mapvx = m->width*m->tsx,
		mapvy = m->height*m->tsy;

	getViewport(m->cam, &portvw, &portvh);

	wx1 = m->cam->camx - portvw/2;
	wx2 = m->cam->camx + portvw/2;
	wy1 = m->cam->camy - portvh/2;
	wy2 = m->cam->camy + portvh/2;

	if (wx1 <= 0)
		q[0] = 0;
	else if (wx1 >= mapvx)
		q[0] = m->width;
	else
		q[0] = wx1 / m->tsx;

	if (wy1 <= 0)
		q[1] = 0;
	else if (wy1 >= mapvy)
		q[1] = m->height;
	else
		q[1] = wy1 / m->tsy;

	if (wx2 <= 0)
		q[2] = 0;
	else if (wx2 >= mapvx)
		q[2] = m->width;
	else
		q[2] = wx2 / m->tsx + 1;

	if (wy2 <= 0)
		q[3] = 0;
	else if (wy2 >= mapvx)
		q[3] = m->height;
	else
		q[3] = wy2 / m->tsy + 1;

	drawMapQuad(m, q);
}

void drawMapQuad(Map * m, int * quad)
{
	int i, j;
	int x1, y1, x2, y2;
	Tile * t;

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBegin(GL_QUADS);
	for (i = quad[0]; i < quad[2]; i++)
		for (j = quad[1]; j < quad[3]; j++)
		{
			x1 = m->tsx*i;
			x2 = m->tsx*(i + 1);
			y1 = m->tsy*j;
			y2 = m->tsy*(j + 1);
			t = getTile(m, i, j);

			glColor4fv(t->color);
			glVertex2i(x2, y2);
			glVertex2i(x1, y2);
			glVertex2i(x1, y1);
			glVertex2i(x2, y1);
		}
	glEnd();
}

Map * getMap(int width, int height, int tsx, int tsy)
{
	int i, j;
	Tile * t;
	Map * m;

	printf("Getting map with w %d h %d tsx %d tsy %d\n", width, height, tsx, tsy);

	m = (Map *)malloc(sizeof(Map));
	m->width = width;
	m->height = height;
	m->tiles = (Tile *)malloc(sizeof(Tile)*(m->width*m->height));
	m->tsx = tsx;
	m->tsy = tsy;
	m->cam = getCam(width*tsx, height*tsy);
	m->groundLevel = MAP_GROUND_LEVEL;
	m->waterLevel = MAP_WATER_LEVEL;
	m->mountainLevel = MAP_MOUNTAIN_LEVEL;
	m->mapStyle = MAP_DEFAULT_STYLE;

	for (i = 0; i < m->width; i++)
		for (j = 0; j < m->height; j++)
		{
			t = getTile(m, i, j);

			t->cx = i;
			t->cy = j;

			t->color[0] = 1.0f;
			t->color[1] = 1.0f;
			t->color[2] = 1.0f;
			t->color[3] = 1.0f;
			t->ground = 0.0f;
		}

	return m;
}

Map * generateMap(Map * m)
{
	int i, j;
	Tile * t;

	printf("Map generating\n");

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

Map * smoothMap(Map * m, float level)
{
	int i, j, k, l;
	Tile * t;
	float * grounds;

	printf("Map smoothing with level %f\n", level);

	grounds = (float *)malloc(sizeof(float)*m->width*m->height);

	printf ("Smoother map\n");

	for (i = 0; i < m->width; i++)
		for (j = 0; j < m->height; j++)
		{
			*(grounds + m->width*j + i) = 0.0f;

			for (k = i - 1; k <= i + 1; k++)
				for (l = j - 1; l <= j + 1; l++)
				{
					t = getTile(m, k, l);
					*(grounds + m->width*j + i) += (k == i && l == j ? level : 1.0)*t->ground;
				}
		}

	printf ("Flush smootheren map\n");

	for (i = 0; i < m->width; i++)
		for (j = 0; j < m->height; j++)
		{
			t = getTile(m, i, j);
			t->ground = *(grounds + m->width*j + i) / (8.0f + level);
		}

	free(grounds);

	return m;
}

Map * coverMap(Map * m)
{
	int i, j;
	Tile * t;

	printf("Map covering\n");

	switch (m->mapStyle)
	{
		case MAP_LEVEL:
			for (i = 0; i < m->width; i++)
				for (j = 0; j < m->height; j++)
				{
					t = getTile(m, i, j);

					if (t->ground <= m->groundLevel)
					{
						t->color[0] = 0.0f;
						t->color[1] = t->ground / m->groundLevel;
						t->color[2] = 1.0f - t->ground / m->groundLevel;
					}
					else
					{
						t->color[0] = t->ground / m->groundLevel - 1.0f;
						t->color[1] = 2.0f - t->ground / m->groundLevel;
						t->color[2] = 0.0f;
					}
				}
			break;

		case MAP_FLAT:
			for (i = 0; i < m->width; i++)
				for (j = 0; j < m->height; j++)
				{
					t = getTile(m, i, j);

					if (t->ground < m->waterLevel)
					{
						t->color[0] = 0.0f;
						t->color[1] = 0.0f;
						t->color[2] = 1.0f;
					}
					else if (t->ground >= m->waterLevel && t->ground < m->mountainLevel)
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
				}
			break;

		case MAP_HIGHMAP:
			for (i = 0; i < m->width; i++)
				for (j = 0; j < m->height; j++)
				{
					t = getTile(m, i, j);

					t->color[0] = t->ground;
					t->color[1] = t->ground;
					t->color[2] = t->ground;
				}
			break;
	}

	return m;
}

void minmaxMap(Map * m, float * min, float * max)
{
	int i, j;
	Tile * t;
	float imin, imax;
	int c;

	imin = 1.0f;
	imax = 0.0f;

	c = 0;
	for (i = 0; i < m->width; i++)
		for (j = 0; j < m->height; j++)
		{
			t = getTile(m, i, j);

			if (t->ground < imin)
				imin = t->ground;

			if (t->ground > imax)
				imax = t->ground;

			c++;
		}

	printf("Map ground min %f max %f on %d tiles\n", imin, imax, c);

	if (min != NULL)
		*min = imin;
	if (max != NULL)
		*max = imax;
}

Map * normalizeMap(Map * m)
{
	printf("Map normalizing\n");

	int i, j;
	Tile * t;
	float min, max;

	minmaxMap(m, &min, &max);

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

	printf("Map randomizing\n");

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
	int i, j;
	Tile * t;

	printf("Map randomizing\n");

	for (i = 0; i < m->width; i++)
		for (j = 0; j < m->height; j++)
		{
			t = getTile(m, i, j);
			t->ground = pow(t->ground*2.0, 2)/4.0;
		}

	return m;
}
