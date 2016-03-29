#ifndef __DIRECTX_H__
#define __DIRECTX_H__

/*******************************************************************/

#define KeyDown(data) ((data & 0x80) ? true : false)
#define KeyUp(data) ((data & 0x80) ? false : true)

/*******************************************************************/

extern IDirectDraw7* dd;
extern IDirectDrawSurface7* back;

/*******************************************************************/

// DX
bool DXStartup();
void DXFinish();

// GFX
void DXFlip();
void DXClearBuffer();
void DXDrawTile(int number, IDirectDrawSurface7* ds, int x, int y, unsigned long width, unsigned long height, bool colorkey);
void DXDrawText(int x, int y, const char* text, int height, const char* fontname, DWORD color, HFONT a_font);

// CONTROL
void DXUpdateDevices(DIMOUSESTATE* ms, LPVOID ks);

/*******************************************************************/

#endif