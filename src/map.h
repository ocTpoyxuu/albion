#ifndef _MAP_H_
#define _MAP_H_

#include "main.h"

typedef struct tagTILE {
	int cx, cy;

	float color[4];
} Tile;

typedef struct tagMAP {
	int width, height;
	Tile * tiles;
} Map;

/* Выводим карту. На входе:
 * int * -- массив четырёх вершин, задающих прямоугольник тайлов, которые будут выводиться
 */
void DrawMap(int **);

#endif
