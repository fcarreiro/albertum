#ifndef __DDUTIL_H__
#define __DDUTIL_H__

/*******************************************************************/

#define RGB888_BGR888(p)  (RGB(GetBValue(p), GetGValue(p), GetRValue(p))
#define RGB888_2_565(p)   (WORD)(((0x00F80000  & p) >> 8)  | ((0x0000FC00 & p) >> 5)  | ((0x000000F8 & p) >> 3))
#define RGB888_2_555(p)   (WORD)(((0x00F80000  & p) >> 9)  | ((0x0000F800 & p) >> 6)  | ((0x000000F8 & p) >> 3))
#define RGB8888_2_4444(p) (WORD)((((0xF0000000 & p) >> 16) | ( 0x00F00000 & p) >> 12) | ((0x0000F000 & p) >> 8) 
#define RGB_16BIT555(r, g, b) ((r<<10) | (g<<5) | (b))
#define RGB_16BIT565(r, g, b) ((r<<11) | (g<<5) | (b))

/*******************************************************************/

IDirectDrawPalette  *DDLoadPalette(IDirectDraw7 *pdd, LPCSTR szBitmap);
IDirectDrawSurface7 *DDLoadBitmap(IDirectDraw7 *pdd, LPCSTR szBitmap, int dx, int dy);
HRESULT              DDReLoadBitmap(IDirectDrawSurface7 *pdds, LPCSTR szBitmap);
HRESULT              DDCopyBitmap(IDirectDrawSurface7 *pdds, HBITMAP hbm, int x, int y, int dx, int dy);
DWORD                DDColorMatch(IDirectDrawSurface7 *pdds, COLORREF rgb);
HRESULT              DDSetColorKey(IDirectDrawSurface7* pdds, DWORD color);

/*******************************************************************/

#endif