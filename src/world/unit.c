#include "unit.h"
#include "../main.h"

void remUnitElem(UnitElem *, UnitList *);

int addUnit(Unit * u, UnitList * ul)
{
	UnitElem * ue = (UnitElem *)malloc(sizeof(UnitElem));

	ue->uid = ul->luid++;
	ue->unit = u;

	ue->next = NULL;
	ue->prev = ul->last;
	if (ul->last != NULL)
		ul->last->next = ue;
	ul->last = ue;
	if (ul->first == NULL)
		ul->first = ue;

	ul->num++;
	return ue->uid;
}

void remUnit(Unit * u, UnitList * ul)
{
	UnitElem * ue;

	ue = ul->first;
	while (ue != NULL)
	{
		if (ue->unit == u)
			break;
		else
			ue = ue->next;
	}

	if (ue == NULL)
		return;

	remUnitElem(ue, ul);
}

void remUnitByUID(int uid, UnitList * ul)
{
	Unit * u = findUnitByUID(uid, ul);
	remUnit(u, ul);
}

void remUnitElem(UnitElem * ue, UnitList * ul)
{
	if (ue->prev != NULL)
		ue->prev->next = ue->next;
	else
		ul->first = ue->next;
	if (ue->next != NULL)
		ue->next->prev = ue->prev;
	else
		ul->last = ue->prev;

	if (ue->unit->ucon != NULL)
		free(ue->unit->ucon);
	free(ue->unit);
	free(ue);

	ul->num--;
}

Unit * findUnitByUID(int uid, UnitList * ul)
{
	UnitElem * ue;

	ue = ul->first;
	while (ue != NULL)
	{
		if (ue->uid == uid)
			return ue->unit;
		else
			ue = ue->next;
	}

	return NULL;
}

UnitList * getUnitList()
{
	UnitList * ul = (UnitList *)malloc(sizeof(UnitList));

	ul->last = ul->first = NULL;
	ul->num = 0;
	ul->luid = 1;

	return ul;
}

void delUnitList(UnitList * ul)
{
	UnitElem * ue, * uet;

	uet = ul->first;

	while (uet != NULL)
	{
		ue = uet->next;

		remUnitElem(uet, ul);

		uet = ue;
	}
}

void setUnit(Unit * u, void * mu)
{
	u->ucon = mu;
}

Unit * getUnit(short type, int tx, int ty, Map * m, short isMovable)
{
	Unit * u;

	if((u = (Unit *)malloc(sizeof(Unit))) == NULL)
		return NULL;

	u->type = type;
	u->ucon = NULL;
	u->map = m;
	u->tx = tx;
	u->ty = ty;
	u->srate = 1;
	u->isMovable = isMovable;

	return u;
}

void unitMove(Unit * u, int tx, int ty)
{
	u->tx = tx;
	u->ty = ty;
}

void unitMoveRel(Unit * u, int dx, int dy)
{
	u->tx += dx;
	u->ty += dy;
}

void printUnitElem(UnitElem * u)
{
	printf("%d(%d) -- %s at %d:%d%s\n", u->uid,
		getTile(u->unit->map, u->unit->tx, u->unit->ty)->uid,
		(u->unit->type == UNIT_TYPE_VILLAGE ? "Village" :
		(u->unit->type == UNIT_TYPE_WARRIOR ? "Warrior" : 
		(u->unit->type == UNIT_TYPE_WORKER ? "Worker" : 
		(u->unit->type == UNIT_TYPE_SETTLER ? "Settler" : "Unknown")))),
		u->unit->tx, u->unit->ty,
		(u->unit->isMovable ? " (movable)" : ""));
}

void printList(UnitList * list)
{
	UnitElem * elem;

	elem = list->first;
	while (elem != NULL)
	{
		printUnitElem(elem);
		elem = elem->next;
	}
}
