#pragma once

#define WIN32_LEAN_AND_MEAN
#define DIRECTINPUT_VERSION 0x0800
//#define DEBUG_MODE
//#define DISABLE_SOUND

#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <ddraw.h>
#include <dinput.h>
#include <winsock.h>
#include <algorithm>
#include <vector>
#include <string>
#include <sstream>

#include "game\game.h"
#include "ao.h"
#include "bass.h"
#include "ijl.h"

#include "misc\misc.h"

#include "directx\ddutil.h"
#include "directx\directx.h"

#include "sound\sound.h"

#include "character\character.h"

#include "mapengine\mapengine.h"

#include "network\network.h"

#include "winsys\windefs.h"
#include "winsys\windows.h"
#include "winsys\winsys.h"
#include "winsys\static.h"
#include "winsys\button.h"
#include "winsys\textbox.h"