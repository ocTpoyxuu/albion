#include <stdlib.h>
#include "world.h"
#include "warrior.h"
#include "settler.h"
#include "worker.h"
#include "village.h"

#define DIR_S  0x1
#define DIR_ES 0x2
#define DIR_E  0x4
#define DIR_NE 0x8
#define DIR_N  0x10
#define DIR_WN 0x20
#define DIR_W  0x40
#define DIR_WS 0x80

World * world;

void UpdateVillage(Village *);
void UpdateWorker(Worker *);
void UpdateWarrior(Warrior *);
void UpdateSettler(Settler *);
void unitMoveRandom(Unit *);
float distToVillage(Unit *, Village *);
int getSRate(Unit *);

void villageExpliciting(Village *);

void ZoomCamera(int s)
{
	camZoom(world->map->cam, s);
	camTest(world->map->cam);
}

void DragCamera(int dx, int dy)
{
	camDrag(world->map->cam, dx, dy);
}

void SetupWorld()
{
	world = (World *)malloc(sizeof(World));

	world->map = getMap(MAP_X, MAP_Y, TILE_SIZE_X, TILE_SIZE_Y);
	world->villages = getUnitList();
	world->warriors = getUnitList();
	world->workers  = getUnitList();
	world->settlers = getUnitList();

	world->turn = 0;
	world->time = WORLD_SEC_PER_TURN;

	/*randomizeMap(world->map);*/
	generateMap(world->map);
	randomizeMap(world->map);
	quadMap(world->map);
	smoothMap(world->map, 0.025);
	smoothMap(world->map, 0.01);
	smoothMap(world->map, 0.025);

	/*normalizeMap(world->map);*/
	/*smoothMap(world->map, 0.01);*/
	/*quadMap(world->map);*/
	/*normalizeMap(world->map);*/
	/*smoothMap(world->map, 0.025);*/
	coverMap(world->map);

	MoveCameraAtTile(world->map, world->map->width / 2, world->map->height / 2);
	camZoom(world->map->cam, MAP_DEFAULT_ZOOM);

	FillWorld();
}

void FillWorld()
{
	int i, j, k, uid;
	Unit * u;
	Village * v;
	Tile * t;
	/*Warrior * war;*/
	/*Settler * s;*/
	/*Worker * work;*/

	// Генерируем деревни
	for (k = 0; k < world->map->width*world->map->height/(64*64) + 1; k++)
	{
		u = getUnit(UNIT_TYPE_VILLAGE, 0, 0, world->map, 0);
		v = getVillage(u);
		setUnit(u, v);
		uid = addUnit(u, world->villages);

		do
		{
			i = rand() % world->map->width;
			j = rand() % world->map->height;
			t = getTile(world->map, i, j);
		}
		while (getTileType(world->map, t) == TILE_WATER || t->uid != 0);

		unitMove(u, i, j);
		t->uid = uid;
		t->road = 1;
	}
	printf("Generate villages:\n");
	printList(world->villages);

	/*// Генерируем воинов*/
	/*v = NULL;*/
	/*for (k = 0; k < world->map->width*world->map->height/(64*64) + 1; k++)*/
	/*{*/
		/*do*/
		/*{*/
			/*i = rand() % world->map->width;*/
			/*j = rand() % world->map->height;*/
		/*}*/
		/*while (getTileType(world->map, getTile(world->map, i, j)) == TILE_WATER);*/
		
		/*u = getUnit(UNIT_TYPE_WARRIOR, i, j, world->map, 0);*/
		/*war = getWarrior(u, v);*/
		/*setUnit(u, war);*/
		/*getTile(world->map, i, j)->uid = addUnit(u, world->warriors);*/
	/*}*/
	/*printf("Generate warriors:\n");*/
	/*printList(world->warriors);*/

	/*// Генерируем рабочих*/
	/*for (k = 0; k < world->map->width*world->map->height/(64*64) + 1; k++)*/
	/*{*/
		/*do*/
		/*{*/
			/*i = rand() % world->map->width;*/
			/*j = rand() % world->map->height;*/
		/*}*/
		/*while (getTileType(world->map, getTile(world->map, i, j)) == TILE_WATER);*/
		
		/*u = getUnit(UNIT_TYPE_WORKER, i, j, world->map, 0);*/
		/*work = getWorker(u, v);*/
		/*setUnit(u, work);*/
		/*addUnit(u, world->workers);*/
	/*}*/
	/*printf("Generate workers:\n");*/
	/*printList(world->workers);*/

	/*// Генерируем поселенцев*/
	/*for (k = 0; k < world->map->width*world->map->height/(64*64) + 1; k++)*/
	/*{*/
		/*do*/
		/*{*/
			/*i = rand() % world->map->width;*/
			/*j = rand() % world->map->height;*/
		/*}*/
		/*while (getTileType(world->map, getTile(world->map, i, j)) == TILE_WATER);*/
		
		/*u = getUnit(UNIT_TYPE_SETTLER, i, j, world->map, 0);*/
		/*s = getSettler(u, v);*/
		/*setUnit(u, s);*/
		/*addUnit(u, world->settlers);*/
	/*}*/
	/*printf("Generate settlers:\n");*/
	/*printList(world->settlers);*/
}

void DestroyWorld()
{
	delUnitList(world->villages);
	delUnitList(world->warriors);
	delUnitList(world->workers);
	delUnitList(world->settlers);

	free(world->map->cam);
	free(world->map->tiles);
	free(world->map);
	free(world);
}

void DrawUnits(int * q)
{
	UnitElem * e;

	// Рисуем деревни
	e = world->villages->first;
	while (e != NULL)
	{
		if (e->unit->tx >= q[0] && e->unit->tx <= q[2] &&
			e->unit->ty >= q[1] && e->unit->ty <= q[3])
			DrawVillage(e->unit->ucon);

		e = e->next;
	}

	// Рисуем воинов
	e = world->warriors->first;
	while (e != NULL)
	{
		if (e->unit->tx >= q[0] && e->unit->tx <= q[2] &&
			e->unit->ty >= q[1] && e->unit->ty <= q[3])
			DrawWarrior(e->unit->ucon);

		e = e->next;
	}

	// Рисуем рабочих
	e = world->workers->first;
	while (e != NULL)
	{
		if (e->unit->tx >= q[0] && e->unit->tx <= q[2] &&
			e->unit->ty >= q[1] && e->unit->ty <= q[3])
			DrawWorker(e->unit->ucon);

		e = e->next;
	}

	// Рисуем поселенцев
	e = world->settlers->first;
	while (e != NULL)
	{
		if (e->unit->tx >= q[0] && e->unit->tx <= q[2] &&
			e->unit->ty >= q[1] && e->unit->ty <= q[3])
			DrawSettler(e->unit->ucon);

		e = e->next;
	}
}

void DrawWorld()
{
	int portvw, portvh;
	int q[4];

	getViewQuad(world->map, q);

	getViewport(world->map->cam, &portvw, &portvh);

	glLoadIdentity();
	glScalef(world->map->cam->coef, world->map->cam->coef, 1.0);
	glTranslated(-world->map->cam->camx + portvw/2, -world->map->cam->camy + portvh/2, 0);

	drawMapQuad(world->map, q);

	DrawUnits(q);
}

void UpdateWorld(float dt)
{
	UnitElem * e;

	world->time += dt;

	if (world->time >= WORLD_SEC_PER_TURN)
	{
		world->time = 0;
		world->turn++;
	}
	else
		return;

	printf("\nNew turn #%d has started!\n", world->turn);

	// Обновляем деревни
	e = world->villages->first;
	while (e != NULL)
	{
		UpdateVillage(e->unit->ucon);

		e = e->next;
	}

	// Обновляем воинов
	e = world->warriors->first;
	while (e != NULL)
	{
		UpdateWarrior(e->unit->ucon);

		e = e->next;
	}

	// Обновляем рабочих
	e = world->workers->first;
	while (e != NULL)
	{
		UpdateWorker(e->unit->ucon);

		e = e->next;
	}

	// Обновляем поселенцев
	e = world->settlers->first;
	while (e != NULL)
	{
		UpdateSettler(e->unit->ucon);

		e = e->next;
	}
}

void UpdateVillage(Village * v)
{
	char techlvl;
	static int randm[2] = {23, 26};
	/*static int randm[2] = {5, 26};*/
	int r, s, foundFlag;
	int i, j, uid;
	Unit * u;
	Tile * t;
	Warrior * war;
	Settler * setl;
	Worker * work;

	v->age++;

	/* Двигаем науку */
	techlvl = (v->tech & VS_MASK) >> VS_OFFSET;
	if (techlvl < 3 && cost[techlvl + 1] <= v->knowns)
	{
		v->knowns -= cost[techlvl + 1];
		/*printf("Tech: %x -> (%x) -> (%x) -> ", v->tech, (v->tech & ~VS_MASK), ((techlvl + 1) << VS_OFFSET));*/
		v->tech = (v->tech & ~VS_MASK) | ((techlvl + 1) << VS_OFFSET);
		/*printf("%x\n", v->tech);*/
		v->grScience = scienceLVL[(int)techlvl];
		printf("Village at %d:%d learned new tech 'science lvl %d'\n",
			v->unit->tx, v->unit->ty,
			techlvl + 1);
	}

	techlvl = (v->tech & VP_MASK) >> VP_OFFSET;
	if (techlvl < 3 && cost[techlvl + 1] <= v->knowns)
	{
		v->knowns -= cost[techlvl + 1];
		/*printf("Tech: %x -> (%x) -> (%x) -> ", v->tech, (v->tech & ~VP_MASK), ((techlvl + 1) << VP_OFFSET));*/
		v->tech = (v->tech & ~VP_MASK) | ((techlvl + 1) << VP_OFFSET);
		/*printf("%x\n", v->tech);*/
		v->grPopulation = populationLVL[(v->tech & VP_MASK) >> VP_OFFSET];
		printf("Village at %d:%d learned new tech 'population lvl %d'\n",
			v->unit->tx, v->unit->ty,
			techlvl + 1);
	}

	techlvl = (v->tech & WD_MASK) >> WD_OFFSET;
	if (techlvl < 3 && cost[techlvl + 1] <= v->knowns)
	{
		v->knowns -= cost[techlvl + 1];
		/*printf("Tech: %x -> (%x) -> (%x) -> ", v->tech, (v->tech & ~WD_MASK), ((techlvl + 1) << WD_OFFSET));*/
		v->tech = (v->tech & ~WD_MASK) | ((techlvl + 1) << WD_OFFSET);
		/*printf("%x\n", v->tech);*/
		v->propDef = defenceLVL[(v->tech & WD_MASK) >> WD_OFFSET];
		printf("Village at %d:%d learned new tech 'defence lvl %d'\n",
			v->unit->tx, v->unit->ty,
			techlvl + 1);
	}

	techlvl = (v->tech & WA_MASK) >> WA_OFFSET;
	if (techlvl < 3 && cost[techlvl + 1] <= v->knowns)
	{
		v->knowns -= cost[techlvl + 1];
		/*printf("Tech: %x -> (%x) -> (%x) -> ", v->tech, (v->tech & ~WA_MASK), ((techlvl + 1) << WA_OFFSET));*/
		v->tech = (v->tech & ~WA_MASK) | ((techlvl + 1) << WA_OFFSET);
		/*printf("%x\n", v->tech);*/
		v->propAtck = atackLVL[(v->tech & WA_MASK) >> WA_OFFSET];
		printf("Village at %d:%d learned new tech 'atack lvl %d'\n",
			v->unit->tx, v->unit->ty,
			techlvl + 1);
	}

	/* Генерим юнитов рандомно */
	r = 1 + (int)(27.0*rand()/(RAND_MAX+1.0));
	for (s = 0; s < 2 && randm[s] < r; s++)
		;
	switch (s)
	{
		/* Проверяем на возможность сгенерить поселенца */
		case 0:
			if (v->population >= 10 + WORLD_SETTLER_COST)
			{

				foundFlag = 0;

				for (i = v->unit->tx - 1; i <= v->unit->tx + 1 && !foundFlag; i++)
					for (j = v->unit->ty - 1; j <= v->unit->ty + 1 && !foundFlag; j++)
					{
						t = getTile(world->map, i, j);
						if ((getTileType(world->map, t) != TILE_WATER) && (t->uid == 0))
							foundFlag = 1;
					}

				if (foundFlag == 1)
				{
					printf("Village at %d:%d has trained settler(%d:%d)(%d %d)\n", v->unit->tx, v->unit->ty, t->cx, t->cy, getTileType(world->map, t), t->uid);
					v->population -= WORLD_SETTLER_COST;

					u = getUnit(UNIT_TYPE_SETTLER, t->cx, t->cy, world->map, 0);
					setl = getSettler(u, v);
					setUnit(u, setl);
					uid = addUnit(u, world->settlers);
					t->uid = uid;
				}
			}
			break;

		/* Проверяем на возможность сгенерить воина */
		case 1:
			if (v->population >= 10 + WORLD_WARRIOR_COST)
			{
				foundFlag = 0;

				for (i = v->unit->tx - 1; i <= v->unit->tx + 1 && !foundFlag; i++)
					for (j = v->unit->ty - 1; j <= v->unit->ty + 1 && !foundFlag; j++)
					{
						t = getTile(world->map, i, j);
						if ((getTileType(world->map, t) != TILE_WATER) && (t->uid == 0))
							foundFlag = 1;
					}

				if (foundFlag == 1)
				{
					printf("Village at %d:%d has trained warrior(%d:%d)(%d %d)\n", v->unit->tx, v->unit->ty, t->cx, t->cy, getTileType(world->map, t), t->uid);
					v->population -= WORLD_WARRIOR_COST;

					u = getUnit(UNIT_TYPE_WARRIOR, t->cx, t->cy, world->map, 0);
					war = getWarrior(u, v);
					war->atack = v->propAtck;
					war->defense = v->propDef;
					setUnit(u, war);
					t->uid = addUnit(u, world->warriors);
				}
			}
			break;

		/* Проверяем на возможность сгенерить рабочего */
		case 2:
			if (v->population >= 10 + WORLD_WORKER_COST)
			{
				foundFlag = 0;

				for (i = v->unit->tx - 1; i <= v->unit->tx + 1 && !foundFlag; i++)
					for (j = v->unit->ty - 1; j <= v->unit->ty + 1 && !foundFlag; j++)
					{
						t = getTile(world->map, i, j);
						if ((getTileType(world->map, t) != TILE_WATER) && (t->uid == 0))
							foundFlag = 1;
					}

				if (foundFlag == 1)
				{
					printf("Village at %d:%d has trained worker(%d:%d)(%d %d)\n", v->unit->tx, v->unit->ty, t->cx, t->cy, getTileType(world->map, t), t->uid);
					v->population -= WORLD_WORKER_COST;

					u = getUnit(UNIT_TYPE_WORKER, t->cx, t->cy, world->map, 0);
					work = getWorker(u, v);
					setUnit(u, work);
					uid = addUnit(u, world->workers);
					t->uid = uid;
				}
			}
			break;
	}

	v->population += v->grPopulation;
	v->knowns += v->grScience;

	if (v->age >= VILLAGE_MAX_AGE)
	{
		printf("Village at %d:%d became extinct\n", v->unit->tx, v->unit->ty);

		villageExpliciting(v);
		t = getTile(world->map, v->unit->tx, v->unit->ty);
		t->uid = 0;
		remUnit(v->unit, world->villages);
	}
}

void UpdateWorker(Worker * w)
{
	Tile * t, * tn;
	int i;
	int k, l;
	int roadFlag;

	w->age++;
	
	t = getTile(world->map, w->unit->tx, w->unit->ty);

	for (i = getSRate(w->unit); i > 0; i--)
	{
		roadFlag = 0;
		if (!t->road)
			for (k = t->cx - 1; k <= t->cx + 1 && !roadFlag; k++)
				for (l = t->cy - 1; l <= t->cy + 1 && !roadFlag; l++)
				{
					tn = getTile(world->map, k, l);
					if (tn->road)
						roadFlag = 1;
				}

		if (roadFlag)
			t->road = 1;
		else
			unitMoveRandom(w->unit);
	}

	if (w->age >= WORKER_MAX_AGE)
	{
		printf("Worker at %d:%d has died\n",
			w->unit->tx, w->unit->ty);

		t = getTile(world->map, w->unit->tx, w->unit->ty);
		t->uid = 0;
		remUnit(w->unit, world->workers);
	}
}

void UpdateWarrior(Warrior * w)
{
	Tile * t, * tn;
	int i, j, k, fight;
	Unit * u;
	Warrior * wf;
	int dMe, dWF;

	w->age++;

	t = getTile(world->map, w->unit->tx, w->unit->ty);

	for (i = getSRate(w->unit); i > 0; i--)
	{
		fight = 0;
		for (k = -1; k <= 1 && !fight; k++)
			for (j = -1; j <= 1 && !fight; j++)
			{
				if (k == 0 && j == 0)
					continue;

				tn = getTile(world->map, w->unit->tx + k, w->unit->ty + j);
				/*printf("Test rel(%d:%d):(%d:%d)->(%d:%d)\n", k, j, w->unit->tx, w->unit->ty, tn->cx, tn->cy);*/
				if (tn->uid != 0 && (u = findUnitByUID(tn->uid, world->warriors)) != NULL)
				{
					dMe = 0;
					dWF = 0;
					wf = (Warrior *)u->ucon;

					if (wf->village != NULL && wf->village == w->village)
						continue;

					printf("Fighting between %d(%d:%d) and %d(%d:%d).",
						t->uid, t->cx, t->cy,
						tn->uid, tn->cx, tn->cy);

					if (wf->atack >= w->defense)
					{
						dMe = 1;
						printf(" 1 died.");
					}
					if (w->atack >= wf->defense)
					{
						dWF = 1;
						printf(" 2 died");
					}
					printf("\n");


					if (dWF)
						remUnit(wf->unit, world->warriors);

					if (dMe)
					{
						remUnit(w->unit, world->warriors);
						return;
					}

					fight = 1;
				}
			}

		if (!fight)
			unitMoveRandom(w->unit);
	}

	if (w->age >= WARRIOR_MAX_AGE)
	{
		printf("Warrior at %d:%d has died\n",
			w->unit->tx, w->unit->ty);

		t = getTile(world->map, w->unit->tx, w->unit->ty);
		t->uid = 0;
		remUnit(w->unit, world->warriors);
	}
}

void UpdateSettler(Settler * s)
{
	Tile * t;
	Village * v;
	Unit * u;
	int i;

	s->age++;

	for (i = getSRate(s->unit); i > 0; i--)
	{
		if ((s->village != NULL && distToVillage(s->unit, s->village) >= 10.0) || s->village == NULL)
		{
			t = getTile(world->map, s->unit->tx, s->unit->ty);
			u = getUnit(UNIT_TYPE_VILLAGE, t->cx, t->cy, world->map, 0);
			v = getVillage(u);
			setUnit(u, v);
			t->uid = addUnit(u, world->villages);
			t->road = 1;

			remUnit(s->unit, world->settlers);
			return;
		}
		else
			unitMoveRandom(s->unit);
	}

	if (s->age >= SETTLER_MAX_AGE)
	{
		printf("Settler at %d:%d has died\n",
			s->unit->tx, s->unit->ty);

		t = getTile(world->map, s->unit->tx, s->unit->ty);
		t->uid = 0;
		remUnit(s->unit, world->settlers);
	}
}

float distToVillage(Unit * u, Village * v)
{
	return sqrt(pow(u->tx - v->unit->tx, 2) + pow(u->ty - v->unit->ty, 2));
}

int dirByOffset(int i, int j)
{
	switch (3*j + i + 4)
	{
		case 0:
			return DIR_WS;
		case 1:
			return DIR_S;
		case 2:
			return DIR_ES;
		case 3:
			return DIR_W;
		default:
		case 4:
			return -1;
		case 5:
			return DIR_E;
		case 6:
			return DIR_WN;
		case 7:
			return DIR_N;
		case 8:
			return DIR_NE;
	}

	return -1;
}

int getSRate(Unit * u)
{
	return u->srate*(getTile(world->map, u->tx, u->ty)->road ? 2.0 : 1.0);
}


void unitMoveRandom(Unit * u)
{
	Tile * t;
	char directions = 0;
	char dnum = 0;
	int i, j, r;
	int uid;

	/* Расчитываем, какие направления нам доступны */
	for (i = -1; i <= 1; i++)
		for (j = -1; j <= 1; j++)
		{
			r = dirByOffset(i, j);
			if (r == -1)
				continue;

			t = getTile(world->map, u->tx + i, u->ty + j);
			if (getTileType(world->map, t) != TILE_WATER && t->uid == 0)
			{
				directions |= r;
				dnum++;
			}
		}
	if (dnum > 0)
	{
		/*printf("Dir %x(%d) oldc %d:%d", directions, dnum, u->tx, u->ty);*/
		r = 1 + (int)((float)dnum*rand()/(RAND_MAX+1.0));
		for (i = 0; i < 8; i++)
		{
			if (directions & (1 << i))
				r--;
			if (r == 0)
				break;
		}

		switch (directions & (1 << i))
		{
			case DIR_WS:
				i = -1;
				j = -1;
				break;
			case DIR_S:
				i = 0;
				j = -1;
				break;
			case DIR_ES:
				i = 1;
				j = -1;
				break;
			case DIR_W:
				i = -1;
				j = 0;
				break;
			case DIR_E:
				i = 1;
				j = 0;
				break;
			case DIR_WN:
				i = -1;
				j = 1;
				break;
			case DIR_N:
				i = 0;
				j = 1;
				break;
			case DIR_NE:
				i = 1;
				j = 1;
				break;
		}

		/*printf(" offset %d:%d", i, j);*/

		t = getTile(world->map, u->tx, u->ty);
		/*printf(" oldu %d", t->uid);*/
		uid = t->uid;
		t->uid = 0;
		unitMoveRel(u, i, j);
		t = getTile(world->map, u->tx, u->ty);
		t->uid = uid;

		/*printf(" newc %d:%d", u->tx, u->ty);*/
		/*printf(" newu %d\n", t->uid);*/
	}
}

void villageExpliciting(Village * v)
{
	UnitElem * e;
	Warrior * war;
	Settler * setl;
	Worker * work;

	e = world->warriors->first;
	while (e != NULL)
	{
		war = e->unit->ucon;
		if (war->village == v)
			war->village = NULL;
		e = e->next;
	}

	e = world->workers->first;
	while (e != NULL)
	{
		work = e->unit->ucon;
		if (work->village == v)
			work->village = NULL;
		e = e->next;
	}

	e = world->settlers->first;
	while (e != NULL)
	{
		setl = e->unit->ucon;
		if (setl->village == v)
			setl->village = NULL;
		e = e->next;
	}
}
