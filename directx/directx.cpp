#include "stdafx.h"

/***************************************************************************************/

extern HWND g_hwnd;

IDirectInput8* di;
IDirectInputDevice8* keyboard;
IDirectInputDevice8* mouse;

#ifdef DEBUG_MODE
LPDIRECTDRAWCLIPPER clipper;
#endif

IDirectDraw7* dd;
IDirectDrawSurface7* primary;
IDirectDrawSurface7* back;

DWORD r_fps=0;
DWORD c_fps=0;
DWORD lasttick=0;

//JPEG_CORE_PROPERTIES jcprops;

/***************************************************************************************/

bool DXStartup()
{
	/*** DIRECTINPUT ***/
	DirectInput8Create(GetModuleHandle(0),DIRECTINPUT_VERSION,IID_IDirectInput8,(void **)&di,0);
	di->CreateDevice(GUID_SysKeyboard, &keyboard, 0);
	keyboard->SetCooperativeLevel(g_hwnd,DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	keyboard->SetDataFormat(&c_dfDIKeyboard);
	keyboard->Acquire();

	di->CreateDevice(GUID_SysMouse, &mouse, NULL);
	mouse->SetCooperativeLevel(g_hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	mouse->SetDataFormat(&c_dfDIMouse);
	mouse->Acquire();

	/*** DIRECTDRAW ***/
	ShowCursor(0);
    DirectDrawCreateEx(0, (void**)&dd, IID_IDirectDraw7, 0);
#ifndef DEBUG_MODE
	dd->SetCooperativeLevel(g_hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT);
	dd->SetDisplayMode(640, 480, 16, 0, 0);

	DDSURFACEDESC2 ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT; 
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX; 
	ddsd.dwBackBufferCount = 1;
	
	dd->CreateSurface(&ddsd, &primary, 0);
	ddsd.ddsCaps.dwCaps = DDSCAPS_BACKBUFFER; 
	primary->GetAttachedSurface(&ddsd.ddsCaps, &back);
#else
	dd->SetCooperativeLevel(g_hwnd, DDSCL_NORMAL);

	DDSURFACEDESC2 ddsd;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
	
	dd->CreateSurface(&ddsd, &primary, NULL);
	
	ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_CAPS;
	ddsd.dwWidth = 640;
	ddsd.dwHeight = 480;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;

	dd->CreateSurface(&ddsd, &back, NULL);

	dd->CreateClipper(0, &clipper, NULL);
	clipper->SetHWnd(0, g_hwnd);
	primary->SetClipper(clipper);
#endif
	/*** JPEG ***/
/*	ijlInit(&jcprops);
	jcprops.JPGFile = "splash.jpg";
	ijlRead(&jcprops, IJL_JFILE_READPARAMS);
	
	BYTE *buffer24;
	long szbuff24 = (jcprops.JPGWidth * 24 + 7) / 8	* jcprops.JPGHeight;
	buffer24 = new BYTE [szbuff24];
	
	jcprops.DIBWidth    = jcprops.JPGWidth;
	jcprops.DIBHeight   = jcprops.JPGHeight;
	jcprops.DIBChannels = 3;
	jcprops.DIBColor    = IJL_BGR;
	jcprops.DIBPadBytes = IJL_DIB_PAD_BYTES(jcprops.JPGWidth, 3);
	jcprops.DIBBytes    = reinterpret_cast <BYTE*> (buffer24);
	//jcprops.DIBHeight = - jcprops.JPGHeight;

	switch(jcprops.JPGChannels)
	{
	case 1: jcprops.JPGColor = IJL_G;
		break;
    case 3: jcprops.JPGColor = IJL_YCBCR;
		break;  
	default:
		jcprops.DIBColor = (IJL_COLOR)IJL_OTHER;
		jcprops.JPGColor = (IJL_COLOR)IJL_OTHER;
		break;
	}

	ijlRead(&jcprops, IJL_JFILE_READWHOLEIMAGE);

	HBITMAP hbm;
	hbm = CreateBitmap (jcprops.JPGWidth, jcprops.JPGHeight, 1, 24, buffer24);

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize   = sizeof(ddsd);
	ddsd.dwFlags  = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth  = jcprops.JPGWidth;
	ddsd.dwHeight = jcprops.JPGHeight;

	dd->CreateSurface(&ddsd, &splash, NULL);
	DDCopyBitmap(splash, hbm, 0, 0, 0, 0);

	delete buffer24;
	ijlFree(&jcprops);
*/

	return true;
}

/***************************************************************************************/

void DXFlip()
{
	char fps[20];
	DWORD dif;

	dif=GetTickCount()-lasttick;

	if(dif>=1000)
	{
		lasttick=GetTickCount();
		r_fps=c_fps/(dif/1000);
		c_fps=0;
	}
	else
	{
		++c_fps;
	}

	sprintf(fps,"FPS: %d",r_fps);
	DXDrawText(590,5,fps,12,"Arial",RGB(255,0,0),NULL);
#ifndef DEBUG_MODE
	primary->Flip(0, DDFLIP_WAIT);
#else
	RECT    rcSrc;
	RECT    rcDest;
	POINT   p;

	p.x = 0; p.y = 0;
	ClientToScreen(g_hwnd, &p);
	GetClientRect(g_hwnd, &rcDest);
	OffsetRect(&rcDest, p.x, p.y);
	SetRect(&rcSrc, 0, 0, 640, 480);
	primary->Blt(&rcDest, back, &rcSrc, DDBLT_WAIT, NULL);
#endif
}

/***************************************************************************************/

void DXClearBuffer()
{
    DDBLTFX clsBltFX;
    ZeroMemory(&clsBltFX, sizeof(clsBltFX));
    clsBltFX.dwSize = sizeof(clsBltFX);
    back->Blt(0, 0, 0, DDBLT_COLORFILL | DDBLT_WAIT, &clsBltFX);
}

/***************************************************************************************/

void DXFinish()
{
	ShowCursor(1);

	if (keyboard)
	{
		keyboard->Unacquire();
		keyboard->Release();
	}

	if (mouse)
	{
		mouse->Unacquire();
		mouse->Release();
	}
  
	if (di)
    di->Release();

	if(dd)
	{
		dd->Release();
		dd = 0;
		if(primary)
		{
			primary->Release();
			primary = 0;
		}
	}
}

/***************************************************************************************/

void DXDrawTile(int number, IDirectDrawSurface7* ds, int x, int y, unsigned long width, unsigned long height, bool colorkey)
{
	RECT Image;

	Image.left   = 0;
	Image.top    = number*height;
	Image.right  = width;
	Image.bottom = Image.top+height;

	if(x+32 >= 640)
	{
		Image.right=640-x;
	}
	if(x < 0)
	{
		Image.left=abs(x);
		x=0;
	}
	if(y+32 >= 480)
	{
		Image.bottom=Image.top+(480-y);
	}
	if(y < 0)
	{
		Image.top+=abs(y);
		y=0;
	}

	if(colorkey)
		back->BltFast (x, y, ds, &Image, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
	else
		back->BltFast (x, y, ds, &Image, DDBLTFAST_WAIT);
}

/***************************************************************************************/

void DXDrawText(int x, int y, const char* text, int height, const char* fontname, DWORD color, HFONT a_font)
{
	HDC surfacedc;
	HFONT font;
	HGDIOBJ lastobj;
	std::string ss;

	/**** create font ****/
	if(a_font==NULL)
	{
	font=CreateFont(
		height,	//	height
		0,	//	width
		0,
		0,
		0,
		false,
		false,
		false,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,//ANTIALIASED_QUALITY
		DEFAULT_PITCH,
		fontname);
	}
	else
	{
		font=a_font;
	}

	/**** conversion ****/
	ss=text;

	/**** texto ****/
	back->GetDC(&surfacedc);
	SetBkMode(surfacedc, TRANSPARENT);
	SetTextColor(surfacedc, color);
	lastobj=SelectObject(surfacedc,font);

	while(true)
	{
		if(ss.find("\n",0) != std::string::npos)
		{
			TextOut(surfacedc, x, y, ss.c_str(), ss.find("\n",0));
		}
		else
		{
			TextOut(surfacedc, x, y, ss.c_str(), ss.length());
		}

		if(std::find(ss.begin(),ss.end(),'\n')!=ss.end())
		{
			ss=ss.substr(ss.find_first_of('\n')+1);
			y+=height;
		}
		else
		{
			break;
		}
	}

	back->ReleaseDC(surfacedc);

	/**** clean ****/
	if(a_font==NULL) DeleteObject(font);
	SelectObject(surfacedc,lastobj);
}

/***************************************************************************************/

void DXUpdateDevices(DIMOUSESTATE* ms, LPVOID ks)
{
	keyboard->GetDeviceState(256, ks);
	mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)ms);
}

/***************************************************************************************/
