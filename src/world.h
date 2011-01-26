#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "main.h"

extern const unsigned int mapx;
extern const unsigned int mapy;
extern const int tilesizex;
extern const int tilesizey;
extern const int camstep;

typedef struct tagTILE {
	int cx, cy;

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
