#ifndef _MAP_H_
#define _MAP_H_

#include "../main.h"
#include "cam.h"

#define TILE_WATER 0
#define TILE_PLANE 1
#define TILE_MOUNTAIN 2

typedef struct tagTILE {
	int cx, cy;
	int uid;

	int road;

	GLfloat ground;
	GLfloat color[4];
} Tile;

typedef struct tagMAP {
	int tsx, tsy;

	int width, height;
	Tile * tiles;
	Cam * cam;

	float waterLevel;
	float mountainLevel;
	float groundLevel;

	short mapStyle;
} Map;

/* Выводим карту. На входе:
 * int * -- массив четырёх вершин, задающих прямоугольник тайлов, которые будут выводиться
 */
void DrawMap(Map *);
void drawMapQuad(Map *, int *);

void MoveCameraAtTile(Map *, int, int);

void getViewQuad(Map *, int *);
Tile * getTile(Map *, int, int);
int getTileType(Map *, Tile *);

Map * getMap(int, int, int, int);
Map * generateMap(Map *);
Map * smoothMap(Map *, float);
Map * coverMap(Map *);
Map * normalizeMap(Map *);
Map * randomizeMap(Map *);
Map * quadMap(Map *);
void minmaxMap(Map *, float * min, float * max);

#endif
