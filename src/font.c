#include <stdarg.h>
#include "font.h"

void glPrintString(float x, float y, float size, short bold, short mono, const char* str, ...)
{
	char* p;
	char text[MAX_LENGTH];
	va_list ap;

	va_start(ap, str);
	vsprintf(text, str, ap);
	va_end(ap);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0.0f);
	glScalef(0.01f * size, 0.01f * size, 1.0f);
	if(bold)
		glLineWidth(2 * size);
	for (p = text; *p; p++)
	{
		if(mono)
			glutStrokeCharacter(GLUT_STROKE_MONO_ROMAN, *p);
		else
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
	}
	glPopMatrix();
}
