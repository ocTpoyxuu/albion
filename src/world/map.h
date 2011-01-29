#ifndef _MAP_H_
#define _MAP_H_

#include "../main.h"
#include "cam.h"

typedef struct tagTILE {
	GLfloat ground;
	GLfloat color[4];
} Tile;

typedef struct tagMAP {
	int tsx, tsy;

	int width, height;
	Tile * tiles;
	Cam * cam;
} Map;

/* Выводим карту. На входе:
 * int * -- массив четырёх вершин, задающих прямоугольник тайлов, которые будут выводиться
 */
void DrawMap(Map *);

void MoveCameraAtTile(Map *, int, int);

Tile * getTile(Map *, int, int);

Map * getMap(int, int, int, int);
Map * generateMap(Map *);
Map * smoothMap(Map *);
Map * coverMap(Map *);
Map * normalizeMap(Map *);
Map * randomizeMap(Map *);
Map * quadMap(Map *);

#endif
