#ifndef _SETTLER_H_
#define _SETTLER_H_

#include "../main.h"
#include "village.h"
#include "unit.h"

#define SETTLER_MAX_AGE 50

typedef struct tagSETTLER {
	Unit * unit;
	Village * village;

	int age;
} Settler;

Settler * getSettler(Unit *, Village *);
void DrawSettler(Settler *);

#endif
