#include "stdafx.h"

/**********************************************************************/

HINSTANCE   g_hinstance;
HWND        g_hwnd;
bool        g_running = true;
const char* g_appName = "Albertum-OnLine";

/****************** FUNCIONES EXTERNAS ********************************/

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l);

/**********************************************************************/

bool createWindow();

/**********************************************************************/

int APIENTRY WinMain(HINSTANCE hinstance, HINSTANCE,
                     LPSTR cmdLine, int)
{
    g_hinstance = hinstance;

    if(!createWindow())
        return -1;

    SetFocus(g_hwnd);
    ShowWindow(g_hwnd, SW_SHOWNORMAL);
    UpdateWindow(g_hwnd);

	// STARTUP
	if(!game_startup())
	{
		game_shutdown();
		return -1;
	}

	/************** IMPORTANTE ****************/
	game_main();
	/************** IMPORTANTE ****************/
	
    while(g_running)
    {
        MSG msg;
        if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
                return msg.wParam;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
		game_update();
    }

	// SHUTDOWN
	game_shutdown();

    return 0;
}

/*********************************************************************/

bool createWindow()
{
    WNDCLASSEX wc;
    ZeroMemory(&wc, sizeof(wc));

    wc.hbrBackground    = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hCursor          = LoadCursor(0, IDC_ARROW);
    wc.hIcon            = LoadIcon(0, IDI_APPLICATION);
    wc.hIconSm          = LoadIcon(0, IDI_WINLOGO);
    wc.hInstance        = g_hinstance;
    wc.lpfnWndProc      = wndProc;
    wc.lpszClassName    = g_appName;
    wc.cbSize           = sizeof(wc);
    wc.style            = CS_VREDRAW | CS_HREDRAW;

    if(RegisterClassEx(&wc))
    {
#ifndef DEBUG_MODE
	g_hwnd = CreateWindowEx(
		WS_EX_TOPMOST,
		g_appName,
		g_appName,
		WS_POPUP | WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		0, 0, g_hinstance, 0);
#else
	g_hwnd = CreateWindow(g_appName,
		g_appName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		g_hinstance,
		NULL);
#endif
	return g_hwnd != 0;
    }

    return false;
}

/**********************************************************************/