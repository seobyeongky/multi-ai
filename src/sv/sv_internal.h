#pragma once

// OPZNET
#include <opznet\server.h>

// CRT
#include <cassert>

// STL
#include <vector>
#include <thread>
#include <mutex>

// WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

// LUA 5.1
extern "C" 
{ 
	#include <lua.h>
}

// BASIC
#include "..\basic\s_map.h"
#include "..\basic\\logger.h"

#include "..\protocol.h"

using namespace std;
using namespace opznet;
using namespace sf;