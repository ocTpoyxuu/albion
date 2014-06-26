#ifndef _UNIT_H_
#define _UNIT_H_

#include "map.h"

#define UNIT_TYPE_VILLAGE 0
#define UNIT_TYPE_WORKER 1
#define UNIT_TYPE_WARRIOR 2
#define UNIT_TYPE_SETTLER 3

typedef struct tagUNIT {
	short type;
	void * ucon;
	Map * map;

	int tx, ty;
	int srate;

	short isMovable;
} Unit;

typedef struct tagUNITELEM {
	int uid;
	Unit * unit;

	struct tagUNITELEM * next;
	struct tagUNITELEM * prev;
} UnitElem;

typedef struct tagUNITLIST {
	int num;
	int luid;

	UnitElem * first;
	UnitElem * last;
} UnitList;

int addUnit(Unit *, UnitList *);
void remUnit(Unit *, UnitList *);
void remUnitByUID(int, UnitList *);

Unit * findUnitByUID(int, UnitList *);

UnitList * getUnitList();
void delUnitList(UnitList *);
Unit * getUnit(short, int, int, Map *, short);
void unitMove(Unit *, int, int);
void unitMoveRel(Unit *, int, int);
void setUnit(Unit *, void *);
void printUnitElem(UnitElem *);
void printList(UnitList *);

#endif
