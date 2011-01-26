#include "engine.h"
#include "world.h"
#include "main.h"

const char AppName [] = "Reciv";
Application app = {30.0, 800, 600, 200, 100, 800, 600, 1, 0.0, 0.0};

int mx = -1,	my = -1,
	mbx = -1, mby = -1,
	mapscroll = 0;
int mscroll = 0;

void update(float dt)
{
	if (skeys[SK_UP_ARROW])
		MoveCamera(0, -1);
	else if (skeys[SK_DOWN_ARROW])
		MoveCamera(0, 1);
	else if (skeys[SK_LEFT_ARROW])
		MoveCamera(-1, 0);
	else if (skeys[SK_RIGHT_ARROW])
		MoveCamera(1, 0);

	if (mapscroll)
		MoveCamera(pow((mx - mbx)/200.0, 3), pow((my - mby)/200.0, 3));

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
	MoveCamera(0, 0);
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

int main(int argc, char * argv[])
{
	InitEngine(&argc, argv);

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
