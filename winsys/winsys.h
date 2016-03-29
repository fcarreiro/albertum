#ifndef __WINSYS_H__
#define __WINSYS_H__

/*******************************************************************/

class CWindowSystem {
public:
	CWindowSystem();
	~CWindowSystem();

	bool Init(void);
	void Kill(void);
	void Update(void);
	bool Render(IDirectDrawSurface7* surface);
	bool RenderEx(IDirectDrawSurface7* surface, unsigned char a_flags);
	bool SetBackground(const char* a_filename, IDirectDraw7* pdd);
	void FreeBackground(void);
	bool LoadProfile(const char* a_profilename, IDirectDraw7* pdd);
	void FreeProfile(void);
	void SetMousePointer(MOUSESPECS* a_mouse) {pMouse=a_mouse;}
	void SetMousePosition(long a_x, long a_y) {if(!pMouse) return;pMouse->virtualx=a_x;pMouse->virtualy=a_y;}
	void SetKeydown(unsigned char a_key) {bKeydown=a_key;}

	CWindow* CreateNewWindow(unsigned char a_class);
	CWindow* FindWindow(const char* a_name) {return wDesktop.FindWindow(a_name);}
	CWindow* FindWindowByID(unsigned long a_id) {return wDesktop.FindWindowByID(a_id);}
	PROFILE* GetProfile(void) {return pProfile;}
	bool RemoveWindow(CWindow* a_win) {return wDesktop.RemoveWindow(a_win,true);}
	bool AddWindow(CWindow* a_win) {return wDesktop.AddWindow(a_win);}

private:
	CWindow wDesktop;

	IDirectDrawSurface7* sBackground;
	PROFILE* pProfile;
	MOUSESPECS* pMouse;
	unsigned char bKeydown;
};

/*******************************************************************/

#endif