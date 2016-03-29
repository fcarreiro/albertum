#ifndef __STATIC_H__
#define __STATIC_H__

/****************************************************/

class CStatic : public CWindow
{
public:
	CStatic();
	virtual ~CStatic();

	// FUNCTIONS
	virtual void SetCallback(f_callback c) {}
	virtual bool Render(IDirectDrawSurface7* surface, PROFILE* prof);

	virtual bool wm_lbuttondown(int a_x, int a_y) {return true;}
	virtual bool wm_lbuttonup(int a_x, int a_y) {return true;}
	virtual bool wm_ldrag(MOUSESPECS* sp) {return true;}
	virtual bool wm_keydown(char key) {return true;}
};

/****************************************************/

#endif