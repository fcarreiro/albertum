#ifndef __BUTTON_H__
#define __BUTTON_H__

/****************************************************/

class CButton : public CWindow
{
public:
	CButton();
	virtual ~CButton();

	// FUNCTIONS
	virtual void SetOnClick(f_callback c) {OnClick=c;}
	virtual bool Render(IDirectDrawSurface7* surface, PROFILE* prof);

	virtual bool wm_lbuttondown(int a_x, int a_y);
	virtual bool wm_lbuttonup(int a_x, int a_y);
	virtual bool wm_ldrag(MOUSESPECS* sp) {return true;}
	virtual bool wm_keydown(char key) {return true;}

private:
	f_callback OnClick;
	bool pushed;
};

/****************************************************/

#endif