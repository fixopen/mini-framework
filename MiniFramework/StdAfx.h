
#pragma once

#define WIN32_LEAN_AND_MEAN	
#define _CRT_SECURE_NO_DEPRECATE

#include <windows.h>
#include <objbase.h>
#include <zmouse.h>

#include "..\DuiLib\UIlib.h"

using namespace DuiLib;

#ifdef _DEBUG
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Debug\\DuiLib.lib")
//#		pragma comment(lib, "..\\Debug\\TroyControls.lib")
#       pragma comment(lib, "..\\Debug\\uWebSockets.lib")
#   else
#       pragma comment(lib, "..\\Debug\\DuiLibA.lib")
#		pragma comment(lib, "..\\Debug\\TroyControlsA.lib")
#   endif
#else
#   ifdef _UNICODE
#       pragma comment(lib, "..\\Release\\DuiLib.lib")
//#       pragma comment(lib, "..\\Release\\TroyControls.lib")
//#       pragma comment(lib, "..\\Release\\uWebSockets.lib")
#   else
#       pragma comment(lib, "..\\Release\\DuiLibA.lib")
//#       pragma comment(lib, "..\\Release\\TroyControlsA.lib")
#   endif
#endif


