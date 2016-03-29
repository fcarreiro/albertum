#include "stdafx.h"

/**********************************************************************/

CWindow::CWindow()
{
	parent=NULL;
	isshown=true;
	ismoving=false;
	x=0; y=0;
	width=0; height=0;
}

CWindow::~CWindow()
{
	OutputDebugString("CWindow::~CWindow() <-> [");

	if(strCaption!="")
		OutputDebugString(strCaption.c_str());
	else
		OutputDebugString("unknown");

	OutputDebugString("] has been deleted!\n");

	FreeSubwins();
}

/*******************************************************************/

bool CWindow::GetItemText(unsigned long a_id, char* a_buffer, unsigned long a_maxsize)
{
	CWindow* win;
	unsigned long len;

	win=FindWindowByID(a_id); if(!win) return false;
	len=strlen(win->GetText());
	if(len>a_maxsize) return false;
	memset(a_buffer,0,a_maxsize);
	memcpy(a_buffer,win->GetText(),len);

	return true;
}

/*******************************************************************/

CWindow* CWindow::FindWindow(const char* a_name)
{
	for(unsigned long i=0;i<subwins.size();++i)
	{
		if(subwins[i]->strCaption==a_name) return subwins[i];
	}

	return NULL;
}

/*******************************************************************/

CWindow* CWindow::FindWindowByID(unsigned long a_id)
{
	for(unsigned long i=0;i<subwins.size();++i)
	{
		if(subwins[i]->GetID()==a_id) return subwins[i];
	}

	return NULL;
}

/*******************************************************************/

void CWindow::FreeSubwins(void)
{
	for(unsigned long i=0;i<subwins.size();++i) {
		delete subwins[i];
	}

	subwins.clear();
}

/*******************************************************************/

bool CWindow::AddWindow(CWindow* w)
{
	if (!w) return false;
	
	std::vector<CWindow*>::iterator it;
	it=std::find(subwins.begin(),subwins.end(),w);
	if (it==subwins.end()) subwins.push_back(w);
	w->SetParent(this);

	return true;
}

bool CWindow::RemoveWindow(CWindow* w, bool del)
{
	if (!w) return false;
	
	std::vector<CWindow*>::iterator it;
	it=std::find(subwins.begin(),subwins.end(),w);
	if (it!=subwins.end()) subwins.erase(it);
	else return false;
	
	if(del) delete w;

	return true;
}

/*******************************************************************/

void CWindow::BringToTop(void)
{
  if(IsActive()) return;

  if (parent) {
    CWindow* p = parent;
    p->RemoveWindow(this,false);
    p->AddWindow(this);
  }
}

/*******************************************************************/

bool CWindow::IsActive(void)
{
	if(!parent) return false;

	return (this == parent->subwins.back());
}

/*******************************************************************/

CWindow* CWindow::GetTopmost(void)
{
	if(!subwins.size()) return NULL;
	return subwins[subwins.size()-1];
}

/*******************************************************************/

CWindow* CWindow::GetTopmostXY(int a_x, int a_y)
{
	CWindow* r=NULL;

	if(!subwins.size()) return NULL;

	for(int i=0;i<subwins.size();++i)
	{
		if( a_x < subwins.at(i)->GetX()+subwins.at(i)->GetWidth() &&
			a_x > subwins.at(i)->GetX() &&
			a_y < subwins.at(i)->GetY()+subwins.at(i)->GetHeight() &&
			a_y > subwins.at(i)->GetY())
		{
			r=subwins.at(i);
		}
	}

	return r;
}

/*******************************************************************/

bool CWindow::SendMessage(unsigned char message, unsigned long lparam, unsigned long wparam)
{
	// EXECUTE
	switch(message) {
		case MWM_KEYDOWN:
			if(!wm_keydown(lparam)) return false;
			break;
		case MWM_KILL:
			if(!wm_kill()) return false;
			break;
		case MWM_LBUTTONDOWN:
			if(!wm_lbuttondown(lparam,wparam)) return false;
			break;
		case MWM_LBUTTONUP:
			if(!wm_lbuttonup(lparam,wparam)) return false;
			break;
		case MWM_DRAG:
			if(!wm_ldrag((MOUSESPECS*)lparam)) return false;
			break;
		default:
			// UNKNOWN MESSAGE
			return false;
	}

	// DISPATCH
	for(unsigned long i=0;i<subwins.size();++i)	{
		if(!subwins[i]->SendMessage(message,lparam,wparam)) return false;
	}

	return true;
}

/*******************************************************************/

bool CWindow::wm_kill(void)
{
	if(IsActive())
	{
		parent->RemoveWindow(this,true);
		return false;
	}
	else
	{
		return true;
	}
}

bool CWindow::wm_ldrag(MOUSESPECS* sp)
{
	if(!IsActive() || !ismoving) return true;

	if( sp->virtualx-sp->state.lX < x+width &&
		sp->virtualx-sp->state.lX > x &&
		sp->virtualy-sp->state.lY < y+15 &&
		sp->virtualy-sp->state.lY > y)
	{
		x+=sp->state.lX;
		y+=sp->state.lY;
		if(x<0) x=0;
		if(y<0) y=0;
		if(x+width>640) x=640-width;
		if(y+height>480) y=480-height;
	}

	return true;
}

bool CWindow::wm_lbuttondown(int a_x, int a_y)
{
	if(!IsVisible()) return true;

	if( a_x < x+width &&
		a_x > x &&
		a_y < y+height &&
		a_y > y)
	{
		if(a_y<y+15) ismoving=true;

		BringToTop();
	}

	return true;
}

bool CWindow::wm_lbuttonup(int a_x, int a_y)
{
	ismoving=false;

	return true;
}

/*******************************************************************/

bool CWindow::Render(IDirectDrawSurface7* surface, PROFILE* prof)
{
	if(isshown)
	{

	RECT r,s;

	/**** fill background ****/
	s.top=0;
	s.left=81;
	s.right=120;
	s.bottom=15;
	r.top=y;
	r.left=x;
	r.right=x+width;
	r.bottom=y+height;
	surface->Blt(&r, prof->sBitmap, &s, DDBLT_WAIT, NULL);

	s.left+=40;
	s.right+=40;
	++r.top;
	++r.left;
	--r.right;
	--r.bottom;
	surface->Blt(&r, prof->sBitmap, &s, DDBLT_WAIT, NULL);

	/**** draw title ****/
	s.top=0;
	s.left=0;
	s.right=80;
	s.bottom=15;
	r.top=y+1;
	r.left=x+1;
	r.right=x+width-1;
	r.bottom=y+16;
	surface->Blt(&r, prof->sBitmap, &s, DDBLT_WAIT, NULL);

	/**** draw text ****/
	DXDrawText(x+5,y+2,strCaption.c_str(),prof->fontheight,NULL,prof->fontcolor,prof->fonthandle);

	}

	/**** draw childs ****/
	for(unsigned long i=0;i<subwins.size();++i) {
		subwins[i]->Render(surface,prof);
	}

	return true;
}

/*******************************************************************/