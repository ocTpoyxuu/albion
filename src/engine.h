#ifndef _ENGINE_HPP_
#define _ENGINE_HPP_

#include "main.h"

/* ascii codes for various special keys */
#define K_ESCAPE 27
#define SK_PAGE_UP 73
#define SK_PAGE_DOWN 81
#define SK_UP_ARROW 101
#define SK_DOWN_ARROW 103
#define SK_LEFT_ARROW 100
#define SK_RIGHT_ARROW 102
#define SK_HOME 106

#define GLUT_SCROLL_UP 3
#define GLUT_SCROLL_DOWN 4

extern char keys[256], skeys[256], btn[5];

void InitEngine(int *, char **);
void RunEngine();

void eUpdateWorldFunc(void (*)(float));
void eResizeWorldFunc(void (*)(void));
void eDrawWorldFunc(int (*)(void));
void eDestroyWorldFunc(void (*)(void));

void eKeyboardFunc(void (*)(unsigned char, int, int));
void eKeyboardUpFunc(void (*)(unsigned char, int, int));
void eSpecialKeyboardFunc(void (*)(int, int, int));
void eSpecialKeyboardUpFunc(void (*)(int, int, int));
void eMouse(void (*)(int, int, int, int));
void eMotion(void (*)(int, int));

void Redraw();

#endif
