#include "stdafx.h"

/***********************************************************/

CButton::CButton()
{
	pushed=false;
	OnClick=NULL;
	CWindow::width=80;
	CWindow::height=25;
}

CButton::~CButton()
{
}

/***********************************************************/

bool CButton::Render(IDirectDrawSurface7* surface, PROFILE* prof)
{
	RECT r;

	if(!IsVisible()) return true;

	/**** draw button ****/
	if(!pushed)
	{
	r.left=0;
	r.right=80;
	r.top=15;
	r.bottom=40;
	}
	else
	{
	r.left=80;
	r.right=160;
	r.top=15;
	r.bottom=40;
	}

	back->BltFast(CWindow::parent->GetX()+CWindow::x,
		CWindow::parent->GetY()+CWindow::y+15,
		prof->sBitmap,
		&r,
		DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY);

	/**** draw text ****/
	DXDrawText(CWindow::parent->GetX()+CWindow::x+10,
		CWindow::parent->GetY()+CWindow::y+22,
		strCaption.c_str(),
		12,
		NULL,
		prof->fontcolor,
		prof->fonthandle);

	return true;
}

/***********************************************************/

bool CButton::wm_lbuttondown(int a_x, int a_y)
{
	if(!OnClick) return true;

	if( a_x < CWindow::x+CWindow::width+CWindow::parent->GetX() &&
		a_x > CWindow::x+CWindow::parent->GetX() &&
		a_y < CWindow::y+CWindow::height+CWindow::parent->GetY()+15 &&
		a_y > CWindow::y+CWindow::parent->GetY()+15)
	{
		pushed=true;
	}

	return true;
}

bool CButton::wm_lbuttonup(int a_x, int a_y)
{
	if(!OnClick) return true;

	pushed=false;

	if( a_x < CWindow::x+CWindow::width+CWindow::parent->GetX() &&
		a_x > CWindow::x+CWindow::parent->GetX() &&
		a_y < CWindow::y+CWindow::height+CWindow::parent->GetY()+15 &&
		a_y > CWindow::y+CWindow::parent->GetY()+15)
	{
		OnClick();

		return false;
	}

	return true;
}

/***********************************************************/