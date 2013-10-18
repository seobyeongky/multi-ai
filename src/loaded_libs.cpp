#ifdef _DEBUG
	#pragma comment(lib, "sfml-graphics-d.lib")
	#pragma comment(lib, "sfml-main-d.lib")
	#pragma comment(lib, "sfml-window-d.lib")
	#pragma comment(lib, "sfml-system-d.lib")
	#pragma comment(lib, "opznet-client-d.lib")
	#pragma comment(lib, "opznet-server-d.lib")
#else
	#pragma comment(lib, "sfml-graphics.lib")
	#pragma comment(lib, "sfml-main.lib")
	#pragma comment(lib, "sfml-window.lib")
	#pragma comment(lib, "sfml-system.lib")
	#pragma comment(lib, "opznet-client.lib")
	#pragma comment(lib, "opznet-server.lib")
#endif

#pragma comment(lib, "audiere.lib")

#pragma comment(lib, "lua51.lib")
#pragma comment(lib, "lua5.1.lib")

#pragma comment(lib, "xmllite.lib")
#pragma comment(lib, "shlwapi.lib")
#pragma comment(lib, "winmm.lib")

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")