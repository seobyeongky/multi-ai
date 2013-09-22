/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS 
=========================================================
	internal.h

	여러 기본적인 header file들을 포함시켰습니다.
=======================================================*/

#pragma once

// OPZNET
#include <opznet\client.h>
#include <opznet\server.h>

// SFML
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

// audiere
#include <audiere.h>

// WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

// CRT
#include <cstring>
#include <cassert>

// STL
#include <vector>
#include <map>
#include <list>
#include <deque>
#include <string>
#include <thread>
#include <queue>

// BASIC
#include "basic\s_map.h"
#include "basic\d_input.h"
#include "basic\\logger.h"
#include "basic\edit_text.h"
#ifdef _DEBUG
	#include "basic\mem_leak.h"
#endif

// PROTOCOL
//#include "protocol.h"



using namespace std;
using namespace opznet;
using namespace sf;


#define SafeDelete(p) if(p != nullptr){delete p; p = nullptr; }
#define SafeDeleteArray(p) if(p != nullptr){delete[] p; p = nullptr; }
#define SafeRelease(p) if(p != nullptr){p->Release(); p = nullptr; }


#define UNICODE_ESC L'\x1b'

#define SV_MAX_NR_CL		16
#define SV_MAX_NR_PLAYER	8