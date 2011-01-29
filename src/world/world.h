#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "../main.h"
#include "map.h"

typedef struct tagWORLD {
	Map * map;
} World;

void SetupWorld();
void DestroyWorld();
void UpdateWorld(float);
void DrawWorld();

void ZoomCamera(int);
void DragCamera(int, int);

#endif
