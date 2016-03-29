#ifndef __WINDOWS_H__
#define __WINDOWS_H__

/****************************************************/

class CWindow {
public:
	CWindow();
	virtual ~CWindow();

	// FUNCTIONS
	bool AddWindow(CWindow* w);
	bool RemoveWindow(CWindow* w, bool del);
	void FreeSubwins(void);

	virtual bool Render(IDirectDrawSurface7* surface, PROFILE* prof);

	void Show(void) {isshown=true;}
	void Hide(void) {isshown=false;}
	void BringToTop(void);
	bool IsActive(void);
	bool IsVisible(void) {return isshown;}
	
	CWindow* GetParent(void) {return parent;}
	CWindow* GetTopmost(void);
	CWindow* GetTopmostXY(int a_x, int a_y);
	CWindow* FindWindow(const char* a_name);
	CWindow* FindWindowByID(unsigned long a_id);

	void SetID(unsigned long a_id) {ID=a_id;}
	void SetParent(CWindow* w) {parent=w;}
	void SetCaption(const char* a_caption) {strCaption=a_caption;}
	void SetPos(int a_x, int a_y) {x=a_x;y=a_y;}
	void SetSize(unsigned long a_width, unsigned long a_height) {width=a_width;height=a_height;}

	const char* GetText(void) {return strCaption.c_str();}
	unsigned long GetID(void) {return ID;}
	bool GetItemText(unsigned long a_id, char* a_buffer, unsigned long a_maxsize);
	int GetX(void) {return x;}
	int GetY(void) {return y;}
	unsigned long GetWidth(void) {return width;}
	unsigned long GetHeight(void) {return height;}

	bool SendMessage(unsigned char message, unsigned long lparam, unsigned long wparam);
	virtual bool wm_kill(void);
	virtual bool wm_lbuttondown(int a_x, int a_y);
	virtual bool wm_lbuttonup(int a_x, int a_y);
	virtual bool wm_ldrag(MOUSESPECS* sp);
	virtual bool wm_keydown(char key) {return true;}

	// BUTTON's
	virtual void SetOnClick(f_callback c) {}

	// TEXTBOXE's
	virtual void SetMaxLength(unsigned long a_len) {}
	virtual void SetProfile(PROFILE* p) {}
	virtual void SetPassword(bool p) {}

	// VARIABLES
private:
	std::vector<CWindow*> subwins;
	bool ismoving;
	unsigned long ID;

protected:
	CWindow* parent;
	std::string strCaption;
	bool isshown;
	int x;
	int y;
	unsigned long width;
	unsigned long height;
};

/****************************************************/

#endif