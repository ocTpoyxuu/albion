#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "../main.h"
#include "map.h"
#include "unit.h"

#define WORLD_WARRIOR_COST 50
#define WORLD_WORKER_COST 60
#define WORLD_SETTLER_COST 85

typedef struct tagWORLD {
	Map * map;

	UnitList * villages;
	UnitList * workers;
	UnitList * warriors;
	UnitList * settlers;

	float time;
	int turn;
} World;

void SetupWorld();
void FillWorld();
void DestroyWorld();
void UpdateWorld(float);
void DrawWorld();

void ZoomCamera(int);
void DragCamera(int, int);

#endif
