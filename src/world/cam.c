#include "../main.h"
#include "cam.h"

Cam * getCam(int width, int height)
{
	Cam * c;

	if ((c = (Cam *)malloc(sizeof(Cam))) == NULL)
		return NULL;

	c->width = width;
	c->height = height;
	c->camx = 0;
	c->camy = 0;
	c->camstep = CAM_STEP;
	c->zoom = 0.0;
	c->coef = 1.0;

	camTest(c);

	return c;
}

void camTest(Cam * c)
{
	int portvw = app.portw/c->coef,
		portvh = app.porth/c->coef;

	if (c->camx - portvw/2 < 0)
		c->camx = portvw/2;
	if (c->camx + portvw/2 > c->width)
		c->camx = c->width - portvw/2;
	if (c->camy - portvh/2 < 0)
		c->camy = portvh/2;
	if (c->camy + portvh/2 > c->height)
		c->camy = c->height - portvh/2;
}

void camDrag(Cam * c, float dx, float dy)
{
	c->camx += dx*(c->camstep);
	c->camy += dy*(c->camstep);

	camTest(c);
}

void camMoveAbsolute(Cam * c, int x, int y)
{
	c->camx = x;
	c->camy = y;

	camTest(c);
}


void camZoom(Cam * c, int s)
{
	int z = c->zoom + s;

	if (z > CAM_ZOOM_MAX)
		z = CAM_ZOOM_MAX;
	else if (z < CAM_ZOOM_MIN)
		z = CAM_ZOOM_MIN;
	c->zoom = z;

	if (z >= 0)
		c->coef = 1 + z*CAM_ZOOM_COEF_IN;
	else if (z < 0)
		c->coef = 1 + z*CAM_ZOOM_COEF_OUT;
}

void getViewport(Cam * c, int * pw, int * ph)
{
	*pw = app.portw/c->coef;
	*ph = app.porth/c->coef;
}
