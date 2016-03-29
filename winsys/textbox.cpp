#include "stdafx.h"

/***********************************************************/

CTextBox::CTextBox()
{
	ispassword=false;
	maxlen=0;
	prof=NULL;
}

CTextBox::~CTextBox()
{
}

/***********************************************************/

bool CTextBox::wm_lbuttondown(int a_x, int a_y)
{
	if(!CWindow::IsVisible()) return true;

	if( a_x < CWindow::x+CWindow::width+CWindow::parent->GetX() &&
		a_x > CWindow::x+CWindow::parent->GetX() &&
		a_y < CWindow::y+CWindow::height+CWindow::parent->GetY()+15 &&
		a_y > CWindow::y+CWindow::parent->GetY()+15)
	{
		BringToTop();
	}

	return true;
}

bool CTextBox::wm_keydown(char key)
{
	if(!CWindow::IsActive() || !prof) return true;

	if(key>31)
	{
		if(maxlen)
		{
			if(CWindow::strCaption.length() >= maxlen) return true;
		}

		if(ispassword)
		{
			if(CWindow::strCaption.length()*(prof->metrics['*'].abcA+prof->metrics['*'].abcB+prof->metrics['*'].abcC)<width-10)
				CWindow::strCaption+=key;
		}
		else
		{
			if(_stringwidth(CWindow::strCaption.c_str(),(ABC*)&prof->metrics)<width-10)
				CWindow::strCaption+=key;
		}
	}
	else
	{
		if(key==8 && CWindow::strCaption.length()>0) CWindow::strCaption.erase(CWindow::strCaption.end()-1);
	}

	return true;
}

/***********************************************************/

bool CTextBox::Render(IDirectDrawSurface7* surface, PROFILE* prof)
{
	RECT r,s;

	if(!IsVisible()) return true;

	s.top=0;
	s.left=81;
	s.right=120;
	s.bottom=15;
	r.top=CWindow::y+CWindow::parent->GetY()+15;
	r.left=CWindow::x+CWindow::parent->GetX();
	r.right=CWindow::x+CWindow::parent->GetX()+CWindow::width;
	r.bottom=CWindow::y+CWindow::parent->GetY()+CWindow::height+15;
	back->Blt(&r, prof->sBitmap, &s, DDBLT_WAIT, NULL);

	if(!CWindow::IsActive())
	{
	s.left+=40;
	s.right+=40;
	++r.top;
	++r.left;
	--r.right;
	--r.bottom;
	back->Blt(&r, prof->sBitmap, &s, DDBLT_WAIT, NULL);
	}
	else
	{
		// agregar cursor al final
	}

	if(CWindow::strCaption.length()==0) return true;

	if(!ispassword)
	{
	DXDrawText(CWindow::x+CWindow::parent->GetX()+2,
		CWindow::y+CWindow::parent->GetY()+17,
		CWindow::strCaption.c_str(),
		12,
		NULL,
		CWindow::IsActive() ? 0xFFFFFF : 0x0,
		prof->fonthandle);
	}
	else
	{
		std::string asterisks;
		_fillstring(asterisks, strCaption, '*');

		DXDrawText(CWindow::x+CWindow::parent->GetX()+2,
		CWindow::y+CWindow::parent->GetY()+17,
		asterisks.c_str(),
		12,
		NULL,
		CWindow::IsActive() ? 0xFFFFFF : 0x0,
		prof->fonthandle);
	}

	return true;
}

/***********************************************************/