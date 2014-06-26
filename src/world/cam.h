#ifndef _CAM_H_
#define _CAM_H_

typedef struct tagCAM {
	int width, height;

	int camx, camy;
	int vcamx, vcamy;

	int camstep;

	int zoom;
	float coef;
} Cam;

Cam * getCam(int width, int height);

void getViewport(Cam *, int *, int *);

void camTest(Cam *);
void camMoveAbsolute(Cam *, int, int);
void camDrag(Cam *, float, float);
void camZoom(Cam *, int);

#endif
