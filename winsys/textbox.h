#ifndef __TEXTBOX_H__
#define __TEXTBOX_H__

/****************************************************/

class CTextBox : public CWindow
{
public:
	CTextBox();
	virtual ~CTextBox();

	// FUNCTIONS
	virtual void SetMaxLength(unsigned long a_len) {maxlen=a_len;}
	virtual void SetProfile(PROFILE* p) {prof=p;}
	virtual void SetPassword(bool p) {ispassword=p;}
	virtual bool Render(IDirectDrawSurface7* surface, PROFILE* prof);

	virtual bool wm_lbuttondown(int a_x, int a_y);
	virtual bool wm_lbuttonup(int a_x, int a_y) {return true;}
	virtual bool wm_ldrag(MOUSESPECS* sp) {return true;}
	virtual bool wm_keydown(char key);

private:
	PROFILE* prof;
	bool ispassword;
	unsigned long maxlen;
};

/****************************************************/

#endif