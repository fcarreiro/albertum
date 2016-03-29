/********************************************************************
	EN ESTE ARCHIVO SE ESCRIBEN TODAS LAS RUTINAS DEL JUEGO MAS
	CERCANAS AL JUGADOR Y LA INTERFAZ GRAFICA (VENTANAS,
	MOVIMIENTOS, INICIO DEL JUEGO, FIN, ETC)
/*******************************************************************/

#include "stdafx.h"

/*************** VARIABLES EXTERNAS ********************************/

extern bool g_running;
extern IDirectDraw7* dd;
extern IDirectDrawSurface7* back;

/*************** DEFINICIONES PRIVADAS *****************************/

enum {
	IDC_USERNAME=1,
	IDC_PASSWORD,
	IDC_SERVER
};

/*************** VARIABLES PRIVADAS ********************************/

const char		szVersion[] = "Version 1.0 Alpha";
CWindow*		sVersion;

CWindowSystem	WindowSystem;			// Window System CLASS
CSoundSystem	SoundSystem;			// Sound System CLASS
CNetworkSystem	NetworkSystem;			// Networking System CLASS
CMap			GlobalMap;				// Map CLASS
CCharacter		MainCharacter;			// Your Character CLASS

MOUSESPECS		spMouse;				// Mouse State STRUCT
unsigned char	keystates[256];			// Keys State ARRAY

/*************** FUNCIONES PRIVADAS ********************************/

void game_login();

void cb_exit();
void cb_connect();

/*******************************************************************
	game_main: CORAZON DEL JUEGO, ACA PASA TODO
*******************************************************************/
void game_main()
{
	sVersion=WindowSystem.CreateNewWindow(C_STATIC); if(!sVersion) return;
	sVersion->SetCaption(szVersion);
	sVersion->SetPos(5,448);
	WindowSystem.AddWindow(sVersion);

	game_login();
}

/*******************************************************************
	game_login: MUESTRA LA VENTANA DE INGRESO Y BLABLABLA...
*******************************************************************/
void game_login()
{
	CWindow* wLogin;
	CWindow* wControl;

	////////// CREATE LOGIN DIALOG /////////////
	wLogin=WindowSystem.CreateNewWindow(C_GENERIC); if(!wLogin) return;
	wLogin->SetCaption("Ingresar");
	wLogin->SetPos(160,260);
	wLogin->SetSize(300,160);

	wControl=WindowSystem.CreateNewWindow(C_STATIC); if(!wControl) return;
	wControl->SetCaption("Nombre de usuario:");
	wControl->SetPos(10,10);
	wLogin->AddWindow(wControl);

	wControl=WindowSystem.CreateNewWindow(C_STATIC); if(!wControl) return;
	wControl->SetCaption("Contraseña (cualquiera):");
	wControl->SetPos(10,40);
	wLogin->AddWindow(wControl);

	wControl=WindowSystem.CreateNewWindow(C_STATIC); if(!wControl) return;
	wControl->SetCaption("Servidor:");
	wControl->SetPos(10,70);
	wLogin->AddWindow(wControl);

	wControl=WindowSystem.CreateNewWindow(C_BUTTON); if(!wControl) return;
	wControl->SetCaption("Salir");
	wControl->SetPos(5,110);
	wControl->SetOnClick(cb_exit);
	wLogin->AddWindow(wControl);

	wControl=WindowSystem.CreateNewWindow(C_BUTTON); if(!wControl) return;
	wControl->SetCaption("Continuar");
	wControl->SetPos(215,110);
	wControl->SetOnClick(cb_connect);
	wLogin->AddWindow(wControl);

	wControl=WindowSystem.CreateNewWindow(C_TEXTBOX); if(!wControl) return;
	wControl->SetMaxLength(20);
	wControl->SetPassword(true);
	wControl->SetProfile(WindowSystem.GetProfile());
	wControl->SetPos(10,52);
	wControl->SetSize(280,15);
	wControl->SetID(IDC_PASSWORD);
	wLogin->AddWindow(wControl);

	wControl=WindowSystem.CreateNewWindow(C_TEXTBOX); if(!wControl) return;
	wControl->SetMaxLength(25);
	wControl->SetPassword(false);
	wControl->SetProfile(WindowSystem.GetProfile());
	wControl->SetCaption("localhost");
	wControl->SetPos(10,82);
	wControl->SetSize(280,15);
	wControl->SetID(IDC_SERVER);
	wLogin->AddWindow(wControl);

	wControl=WindowSystem.CreateNewWindow(C_TEXTBOX); if(!wControl) return;
	wControl->SetMaxLength(20);
	wControl->SetPassword(false);
	wControl->SetProfile(WindowSystem.GetProfile());
	wControl->SetCaption("BetaTester");
	wControl->SetPos(10,22);
	wControl->SetSize(280,15);
	wControl->SetID(IDC_USERNAME);
	wLogin->AddWindow(wControl);

	////////// SHOW VERSION NUM ////////////////
	sVersion->Show();

	////////// SET BACKGROUND IMG //////////////
	WindowSystem.SetBackground("gfx\\inicio.bmp",dd);
	SoundSystem.PlayMusic("music\\inicio.mp3");
}

/*******************************************************************
	game_startup: INICIA DX, SONIDO, ETC;
*******************************************************************/
bool game_startup()
{
	/////////// STARTS DIRECTX /////////////
	if(!DXStartup()) return false;

	/////////// ASK 4 PATIENCE /////////////
	DXClearBuffer();
	DXDrawText(5,5,"Cargando, por favor espere...",12,"Arial",0xFFFFFF,NULL);
	DXFlip();

	/////////// STARTS WINSYS //////////////
	if(!WindowSystem.Init()) return false;
	if(!WindowSystem.LoadProfile("default",dd)) return false;
	WindowSystem.SetMousePointer(&spMouse);
	WindowSystem.SetMousePosition(320,240);

	/////////// STARTS NETSYS //////////////
	if(!NetworkSystem.Init()) return false;

	/////////// STARTS SOUND ///////////////
	if(!SoundSystem.Init()) return false;

	return true;
}

/*******************************************************************
	game_shutdown: LIBERA RECURSOS Y CIERRA EL JUEGO
*******************************************************************/
void game_shutdown()
{
//	WindowSystem.Kill();	NOT NEEDED
//	SoundSystem.Kill();		NOT NEEDED
//	NetworkSystem.Kill();	NOT NEEDED

	DXFinish();
}

/*******************************************************************
	gfx_update: BORRA EL BUFFER, Y DIBUJA LO QUE SEA
				NECESARIO
*******************************************************************/
void gfx_update()
{
	////////// CLEARS SCENE ///////////
	DXClearBuffer();

	////////// RENDER SCENE ///////////
	WindowSystem.Render(back);

	////////// FLIPS SCENE ////////////
	DXFlip();
}

/*******************************************************************
	control_update: CAPTURA EL ESTADO DE LOS CONTROLES
					Y EFECTUA LOS CAMBIOS NECESARIOS
*******************************************************************/
void control_update()
{
	//////////// UPDATES DEVICES ////////////////
	DXUpdateDevices(&spMouse.state, &keystates);
	spMouse.virtualx+=spMouse.state.lX;
	spMouse.virtualy+=spMouse.state.lY;
	if(spMouse.virtualx<0) spMouse.virtualx=0;
	if(spMouse.virtualx>640) spMouse.virtualx=640;
	if(spMouse.virtualy<0) spMouse.virtualy=0;
	if(spMouse.virtualy>480) spMouse.virtualy=480;

	////////// UPDATES WINDOW SYSTEM ///////////
	WindowSystem.Update();

	//////// CHECKS FOR SPECIAL KEYS ///////////
	if(KeyDown(keystates[DIK_ESCAPE])) g_running=false;
}

/*******************************************************************
	cb_exit: FUNCION LLAMADA DESDE UN BOTON:
			 SALE DEL JUEGO
*******************************************************************/
void cb_exit()
{
	g_running=false;
}

/*******************************************************************
	cb_connect: TRATA DE CONECTARSE CON EL SERVIDOR, ETC
*******************************************************************/
void cb_connect()
{
	CWindow *wLogin;
	char username[20];
	char password[20];
	char server[50];
	
	wLogin=WindowSystem.FindWindow("Ingresar"); if(!wLogin) return;

	wLogin->GetItemText(IDC_USERNAME,username,sizeof(username));
	wLogin->GetItemText(IDC_PASSWORD,password,sizeof(password));
	wLogin->GetItemText(IDC_SERVER,server,sizeof(server));

	OutputDebugString("====> [");
	OutputDebugString(username);
	OutputDebugString("] - [");
	OutputDebugString(password);
	OutputDebugString("] - [");
	OutputDebugString(server);
	OutputDebugString("]\n");

	WindowSystem.RemoveWindow(wLogin);

	sVersion->Hide();
}

/*******************************************************************/

LRESULT CALLBACK wndProc(HWND hwnd, UINT msg, WPARAM w, LPARAM l)
{
	switch(msg)
	{
	case WM_CHAR:
		WindowSystem.SetKeydown(w);
		break;
	case WM_KEYDOWN:
		if(w==8) WindowSystem.SetKeydown(8);
		break;
    }

    return DefWindowProc(hwnd, msg, w, l);
}

/*******************************************************************/