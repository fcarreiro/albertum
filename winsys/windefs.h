#ifndef __WINDEFS_H__
#define __WINDEFS_H__

/*******************************************************************/

#define R_BACKGROUND	1
#define R_WINDOWS		2
#define R_MOUSE			4

#define MWM_DRAG		0
#define MWM_LBUTTONDOWN	1
#define MWM_LBUTTONUP	2
#define MWM_KILL		3
#define MWM_KEYDOWN		4

#define C_GENERIC		0
#define C_BUTTON		1
#define C_STATIC		2
#define C_TEXTBOX		3

/*******************************************************************/

typedef struct {
	long virtualx;
	long virtualy;
	DIMOUSESTATE state;
} MOUSESPECS;

typedef struct {
	IDirectDrawSurface7* sBitmap;
	ABC metrics[255];
	unsigned char fontheight;
	char fontname[25];
	HFONT fonthandle;
	DWORD fontcolor;
} PROFILE;

/*******************************************************************/

typedef void (*f_callback)();

/*******************************************************************/

#endif