#ifndef _WORKER_H_
#define _WORKER_H_

#include "../main.h"
#include "village.h"
#include "unit.h"

#define WORKER_MAX_AGE 50

typedef struct tagWORKER {
	Unit * unit;
	Village * village;

	int age;
} Worker;

Worker * getWorker(Unit *, Village *);
void DrawWorker(Worker *);

#endif
