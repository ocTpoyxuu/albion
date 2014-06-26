#ifndef _WARRIOR_H_
#define _WARRIOR_H_

#include "../main.h"
#include "village.h"
#include "unit.h"

#define WARRIOR_MAX_AGE 40

typedef struct tagWARRIOR {
	Unit * unit;
	Village * village;

	int age;
	int atack, defense;
} Warrior;

Warrior * getWarrior(Unit *, Village *);
void DrawWarrior(Warrior *);

#endif
