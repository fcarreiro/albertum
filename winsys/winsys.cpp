#include "stdafx.h"

/*******************************************************************/

CWindowSystem::CWindowSystem()
{
	wDesktop.SetPos(0,0);
	wDesktop.SetSize(640,480);
	wDesktop.Hide();

	sBackground=NULL;
	pProfile=NULL;
	pMouse=NULL;
	bKeydown=0;
}

CWindowSystem::~CWindowSystem()
{
	Kill();
}

/*******************************************************************/

CWindow* CWindowSystem::CreateNewWindow(unsigned char a_class)
{
	CWindow* win;

	switch(a_class)
	{
	case C_GENERIC:
		win=new CWindow;
		if(win) wDesktop.AddWindow(win);
		break;

	case C_STATIC:
		win=new CStatic;
		break;

	case C_BUTTON:
		win=new CButton;
		break;

	case C_TEXTBOX:
		win=new CTextBox;
		break;

	default:
		win=NULL;
		break;
	}

	return win;
}

/*******************************************************************/

bool CWindowSystem::Init(void)
{
	return true;
}

/*******************************************************************/

void CWindowSystem::Kill(void)
{
	FreeBackground();
	FreeProfile();
	wDesktop.FreeSubwins();

	pMouse=NULL;
}

/*******************************************************************/

void CWindowSystem::Update(void)
{
	static bool dragging=false;
	CWindow* topmost=NULL;
	CWindow* topmostxy=NULL;

	topmost=wDesktop.GetTopmost();
	topmostxy=wDesktop.GetTopmostXY(pMouse->virtualx,pMouse->virtualy);

	// WINDOWS
	if(KeyDown(pMouse->state.rgbButtons[0]))
	{
		if(!dragging)
		{
		dragging=true;
		if(topmostxy)
			topmostxy->SendMessage(MWM_LBUTTONDOWN,pMouse->virtualx,pMouse->virtualy);
		}
	}
	else
	{
		if(dragging)
		{
		if(topmostxy)
			topmostxy->SendMessage(MWM_LBUTTONUP,pMouse->virtualx,pMouse->virtualy);
		}
		dragging=false;
	}

	if(dragging && (pMouse->state.lX || pMouse->state.lY))
	{
	if(topmost)
		topmost->SendMessage(MWM_DRAG,(DWORD)pMouse,NULL);
	}

	if(bKeydown)
	{
	if(topmost)	topmost->SendMessage(MWM_KEYDOWN,bKeydown,NULL);
	bKeydown=0;
	}
}

/*******************************************************************/

bool CWindowSystem::Render(IDirectDrawSurface7* surface)
{
	return RenderEx(surface, R_BACKGROUND | R_WINDOWS | R_MOUSE);
}

/*******************************************************************/

bool CWindowSystem::RenderEx(IDirectDrawSurface7* surface, unsigned char a_flags)
{
	RECT r;

	if(!surface || !pProfile) return false;

	if(sBackground && (a_flags & R_BACKGROUND))
	{
		r.top=0;
		r.left=0;
		r.bottom=480;
		r.right=640;

		surface->BltFast (0, 0, sBackground, &r, DDBLTFAST_WAIT);
	}

	if(a_flags & R_WINDOWS)
	{
		if(!pProfile) return false;

		wDesktop.Render(surface,pProfile);
	}

	if(a_flags & R_MOUSE)
	{
		if(!pMouse) return false;

		r.top=40;
		r.left=0;
		r.right=r.left+(pMouse->virtualx+32>=640 ? 640-pMouse->virtualx : 32);
		r.bottom=40+(pMouse->virtualy+32>=480 ? 480-pMouse->virtualy : 32);

		surface->BltFast(pMouse->virtualx, pMouse->virtualy, pProfile->sBitmap, &r, DDBLTFAST_SRCCOLORKEY | DDBLTFAST_WAIT);
	}

	return true;
}

/*******************************************************************/

bool CWindowSystem::SetBackground(const char* a_filename, IDirectDraw7* pdd)
{
	if(sBackground) FreeBackground();
	sBackground=DDLoadBitmap(pdd,a_filename,0,0);
	if(sBackground==NULL) return false;

	return true;
}

/*******************************************************************/

void CWindowSystem::FreeBackground(void)
{
	if(sBackground)
	{
		sBackground->Release();
		sBackground=NULL;
	}
}

/*******************************************************************/

bool CWindowSystem::LoadProfile(const char* a_profilename, IDirectDraw7* pdd)
{
	PROFILE profile;
	unsigned int r,g,b;
	std::string strTemp;
	char bitmap[25];
	
	strTemp="ws\\";
	strTemp+=a_profilename;
	strTemp+=".ini";

	memset(bitmap,0,sizeof(bitmap));
	GetPrivateProfileString("Style Options","Bitmap","",bitmap,sizeof(bitmap),strTemp.c_str());
	GetPrivateProfileString("Style Options","Font","",profile.fontname,sizeof(profile.fontname),strTemp.c_str());
	profile.fontheight=GetPrivateProfileInt("Style Options","Height",12,strTemp.c_str());
	r=GetPrivateProfileInt("Style Options","R",255,strTemp.c_str());
	g=GetPrivateProfileInt("Style Options","G",255,strTemp.c_str());
	b=GetPrivateProfileInt("Style Options","B",255,strTemp.c_str());
	profile.fontcolor=RGB(r,g,b);

	strTemp="ws\\";
	strTemp+=bitmap;

	profile.sBitmap=DDLoadBitmap(pdd,strTemp.c_str(),0,0);
	if(profile.sBitmap==NULL) return false;
	DDSetColorKey(profile.sBitmap,RGB(200,0,200));

	profile.fonthandle=CreateFont(
		profile.fontheight,	//	height
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
		DEFAULT_QUALITY,
		DEFAULT_PITCH,
		profile.fontname);

	if(!profile.fonthandle)
	{
		profile.sBitmap->Release();
		return false;
	}

	HDC fakedc;
	fakedc=CreateDC("DISPLAY",NULL,NULL,NULL);
	if(fakedc)
	{
		SelectObject(fakedc,profile.fonthandle);
		GetCharABCWidths(fakedc,
			0,
			255,
			(ABC*)(&profile.metrics));
		DeleteDC(fakedc);
	}
	else
	{
		profile.sBitmap->Release();
		DeleteObject(profile.fonthandle);
		return false;
	}

	pProfile=new PROFILE;
	if(!pProfile) return false;

	memcpy(pProfile,&profile,sizeof(PROFILE));

	return true;
}

/*******************************************************************/

void CWindowSystem::FreeProfile(void)
{
	if(pProfile)
	{
		pProfile->sBitmap->Release();
		DeleteObject(pProfile->fonthandle);

		delete pProfile;
		pProfile=NULL;
	}
}

/*******************************************************************/
