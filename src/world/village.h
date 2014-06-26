#ifndef _VILLAGE_H_
#define _VILLAGE_H_

#include "../main.h"
#include "unit.h"

#define WD_MASK   0xc0
#define WD_OFFSET 6
#define WA_MASK   0x30
#define WA_OFFSET 4
#define VP_MASK   0x0c
#define VP_OFFSET 2
#define VS_MASK   0x03
#define VS_OFFSET 0

#define VILLAGE_MAX_AGE 300

extern const int scienceLVL [];
extern const int populationLVL [];
extern const int atackLVL [];
extern const int defenceLVL [];
extern const int cost [];

typedef struct tagVILLAGE {
	Unit * unit;

	int age;
	int population;
	int knowns;

	char tech;

	int grScience;
	int grPopulation;
	int propAtck;
	int propDef;
} Village;

Village * getVillage(Unit *);
void DrawVillage(Village *);

#endif
