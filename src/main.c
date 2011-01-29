#include <sys/time.h>
#include "engine.h"
#include "world/world.h"
#include "main.h"

const char AppName [] = "Reciv";
Application app = {
	ENGINE_DEFAULT_CTRL_RATE,
	WINDOW_DEFAULT_WIDTH,
	WINDOW_DEFAULT_HEIGHT,
	WINDOW_DEFAILT_LEFT_POSITION,
	WINDOW_DEFAILT_TOP_POSITION,
	WINDOW_DEFAULT_WIDTH,
	WINDOW_DEFAULT_HEIGHT,
	DEFAULT_FULLSCREEN_STATE,
	0.0,
	0.0
};

int mx = -1,	my = -1,
	mbx = -1, mby = -1,
	mapscroll = 0;
int mscroll = 0;

void update(float dt)
{
	if (skeys[SK_UP_ARROW])
		DragCamera(0, 1);
	else if (skeys[SK_DOWN_ARROW])
		DragCamera(0, -1);
	else if (skeys[SK_LEFT_ARROW])
		DragCamera(-1, 0);
	else if (skeys[SK_RIGHT_ARROW])
		DragCamera(1, 0);

	if (mapscroll)
		DragCamera(pow((mx - mbx)/200.0, 3), pow((mby - my)/200.0, 3));

	if (mscroll != 0)
	{
		ZoomCamera(mscroll);
		mscroll = 0;
	}

	UpdateWorld(dt);
	Redraw();
}

int draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glColor3f(1.0f, 1.0f, 1.0f);
	DrawWorld();

	return 0;
}

void resize()
{
	DragCamera(0, 0);
}

void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON)
	{
		if (state == GLUT_UP)
		{
			mapscroll = 0;
			mbx = mx = -1;
			mby = my = -1;
		}
		else
		{
			mapscroll = 1;
			mbx = mx = x;
			mby = my = y;
		}
	}
	else if (button == GLUT_SCROLL_UP && state == GLUT_DOWN)
		mscroll += 1;
	else if (button == GLUT_SCROLL_DOWN && state == GLUT_DOWN)
		mscroll -= 1;
}

void motion(int x, int y)
{
	if (mapscroll)
	{
		mx = x;
		my = y;
	}
}

void randomize()
{
	struct timeval time;
	gettimeofday(&time, NULL);
	srand((unsigned int)(time.tv_sec * 1000000 + time.tv_usec));
	printf("SRAND %u\n", (unsigned int)(time.tv_sec * 1000000 + time.tv_usec));
}

int main(int argc, char * argv[])
{
	InitEngine(&argc, argv);

	randomize();

	SetupWorld();
	eDestroyWorldFunc(DestroyWorld);
	eUpdateWorldFunc(update);
	eDrawWorldFunc(draw);
	eResizeWorldFunc(resize);
	eMouse(mouse);
	eMotion(motion);

	RunEngine();

	return 0;
}
