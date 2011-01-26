#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "main.h"

typedef struct tagTILE {
	int cx, cy;

	float ground;

	float color[4];
} Tile;

typedef struct tagMAP {
	int width, height;
	Tile * tiles;
} Map;

typedef struct tagWORLD {
	Map * map;

	int camx, camy;
	int vcamx, vcamy;
	int camstep;
	int tsx, tsy;
	int zoom;
	float coef;
} World;

void SetupWorld();
void DestroyWorld();

void UpdateWorld(float);
void MoveCamera(float, float);
void MoveCameraAtTile(int, int);
void ZoomCamera(int);

void DrawWorld();

#endif
