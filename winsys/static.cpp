#include "stdafx.h"

/***********************************************************/

CStatic::CStatic()
{
}

CStatic::~CStatic()
{
}

/***********************************************************/

bool CStatic::Render(IDirectDrawSurface7* surface, PROFILE* prof)
{
	if(!IsVisible()) return true;

	DXDrawText(
		CWindow::parent->GetX()+CWindow::x,
		CWindow::parent->GetY()+CWindow::y+15,
		strCaption.c_str(),
		prof->fontheight,
		NULL,
		0,
		prof->fonthandle);

	return true;
}

/***********************************************************/