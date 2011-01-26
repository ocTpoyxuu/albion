#include <sys/time.h>
#include "engine.h"

// Drawing stuff
int window;
int swap = 0;

// Timing stuff
struct timeval
	time,
	tmptime,
	drawframebase,
	controlframebase,
	secbit = {1, 0}; // 1 second time bit
int frameps = 0, drawps = 0;

// I/O stuff
char keys[256],
	skeys[256],
	btn[5];

// Handlers stuff
typedef struct tagHANDLERS {
	void (* updateWorld)(float);
	void (* resizeWorld)(void);
	int  (* drawWorld)(void);
	void (* destroyWorld)(void);
	void (* keyboard)(unsigned char, int, int);
	void (* keyboardup)(unsigned char, int, int);
	void (* special)(int, int, int);
	void (* specialup)(int, int, int);
	void (* mouse)(int, int, int, int);
	void (* motion)(int, int);
} Handlers;

Handlers handlers = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

void eUpdateWorldFunc(void (*f)(float))
{
	handlers.updateWorld = f;
}

void eResizeWorldFunc(void (*f)(void))
{
	handlers.resizeWorld = f;
}

void eDrawWorldFunc(int (*f)(void))
{
	handlers.drawWorld = f;
}

void eDestroyWorldFunc(void (*f)(void))
{
	handlers.destroyWorld = f;
}

void eKeyboardFunc(void (*f)(unsigned char key, int x, int y))
{
	handlers.keyboard = f;
}

void eKeyboardUpFunc(void (*f)(unsigned char key, int x, int y))
{
	handlers.keyboardup = f;
}

void eSpecialKeyboardFunc(void (*f)(int key, int x, int y))
{
	handlers.special = f;
}

void eSpecialKeyboardUpFunc(void (*f)(int key, int x, int y))
{
	handlers.specialup = f;
}

void eMouse(void (*f)(int button, int state, int x, int y))
{
	handlers.mouse = f;
}

void eMotion(void (*f)(int x, int y))
{
	handlers.motion = f;
}

void Redraw()
{
	swap = 1;
}

int DrawScene()
{
	int ret = 0;

	if (handlers.drawWorld != NULL)
		ret = (*(handlers.drawWorld))();

	return ret;
}

void callDestroyWorld()
{
	if (handlers.destroyWorld != NULL)
		(*(handlers.destroyWorld))();
}

void DrawGLScene()
{
	// Counters
	frameps++;
	if (swap)
		drawps++;

	gettimeofday(&time, NULL);
	timersub(&time, &drawframebase, &tmptime);
	if (timercmp(&tmptime, &secbit, >=))
	{
		app.fps = frameps*(float)(secbit.tv_sec * 1000000 + secbit.tv_usec) / (tmptime.tv_sec * 1000000 + tmptime.tv_usec);
		app.dps = drawps*(float)(secbit.tv_sec * 1000000 + secbit.tv_usec) / (tmptime.tv_sec * 1000000 + tmptime.tv_usec);
		frameps = 0;
		drawps = 0;
		drawframebase = time;

		printf("FPS %f DPS %f\n", app.fps, app.dps);
	}


	if (swap)
	{
		swap = DrawScene();

		glutSwapBuffers();
	}
}

void UpdateScene(int value)
{
	float dt;
	gettimeofday(&time, NULL);
	timersub(&time, &controlframebase, &tmptime);
	dt = (float)(tmptime.tv_sec*1000000 + tmptime.tv_usec)/(1000000);

	// Checking keys
	if (keys[K_ESCAPE])
	{
		glutDestroyWindow(window);
		exit(0);
	}

	// Updating world
	if (handlers.updateWorld != NULL)
		(*(handlers.updateWorld))(dt);

	controlframebase = time;
	value++;
	if (value == app.ctlfrate)
		value = 0;
	glutTimerFunc(1000/app.ctlfrate, UpdateScene, value);
}

void ReSizeGLScene(int Width, int Height)
{
	if (Height == 0)
		Height = 1;

	app.portw = Width;
	app.porth = Height;

	glViewport(0, 0, app.portw, app.porth);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, app.portw, 0, app.porth);

	glMatrixMode(GL_MODELVIEW);

	if (handlers.resizeWorld)
		(*(handlers.resizeWorld))();

	swap = 1;
}

void keyboardDown(unsigned char key, int x, int y)
{
	keys[key] = 1;

	if (handlers.keyboard)
		(*(handlers.keyboard))(key, x, y);
}

void keyboardUp(unsigned char key, int x, int y)
{
	keys[key] = 0;

	if (handlers.keyboardup)
		(*(handlers.keyboardup))(key, x, y);
}

void specialKeyDown(int key, int x, int y)
{
	skeys[key] = 1;

	if (handlers.special)
		(*(handlers.special))(key, x, y);
}

void specialKeyUp(int key, int x, int y)
{
	skeys[key] = 0;

	if (handlers.specialup)
		(*(handlers.specialup))(key, x, y);
}

void GLMouse(int button, int state, int x, int y)
{
	/*printf("Mouse event at x %d y %d with state %x button %x", x, y, state, button);*/
	btn[button] = (state == GLUT_DOWN ? 1 : 0);
	/*printf(" btn{%d, %d, %d, %d, %d}\n", btn[0], btn[1], btn[2], btn[3], btn[4]);*/

	if (handlers.mouse)
		(*(handlers.mouse))(button, state, x, y);
}

void GLMotion(int x, int y)
{
	if (handlers.motion)
		(*(handlers.motion))(x, y);
}

void InitEngine(int * argc, char ** argv)
{
	glutInit(argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(app.winw, app.winh);
	glutInitWindowPosition(app.winl, app.wint);

	window = glutCreateWindow(AppName);

	glutDisplayFunc(DrawGLScene);
	glutTimerFunc(1000/app.ctlfrate, UpdateScene, 0);
	glutIdleFunc(DrawGLScene);
	glutReshapeFunc(ReSizeGLScene);
	glutKeyboardUpFunc(keyboardUp);
	glutKeyboardFunc(keyboardDown);
	glutSpecialFunc(specialKeyDown);
	glutSpecialUpFunc(specialKeyUp);
	glutMouseFunc(GLMouse);
	glutMotionFunc(GLMotion);

	if (app.fullscreen)
		glutFullScreen();
	ReSizeGLScene(app.winw, app.winh);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void RunEngine()
{
	frameps = 0;
	drawps = 0;
	gettimeofday(&controlframebase, NULL);
	gettimeofday(&drawframebase, NULL);

	glutMainLoop();
}
