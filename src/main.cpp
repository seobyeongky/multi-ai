#include "global.h"
#include "asset.h"
#include "director.h"
#include "intro_scene.h"
#include "net_interface.h"
#include "sfx_mgr.h"

#include <locale.h>

global_t G;

class LoggerLoader
{
public:
	LoggerLoader(void){G.logger = new Logger(L"log.txt", G.window.getSystemHandle());}
	~LoggerLoader(){delete G.logger;}
};

class AssetLoader
{
public:
	AssetLoader(void){LoadAssets();}
	~AssetLoader(){ReleaseAssets();}
};

class NetInterfaceLoader
{
public:
	NetInterfaceLoader(void){NetInterface::Begin();}
	~NetInterfaceLoader(){NetInterface::End();}
};

///////////////// MAIN FUNC ///////////////////
int main(int argc, char * argv[])
{
	_wsetlocale(LC_ALL, L"korean");

	Sleep(1000);

	G.window.create(VideoMode(640, 480), L"기막힌 가위바위보", Style::Titlebar || Style::Close);
	G.window.setFramerateLimit(60U);

	LoggerLoader l;
	AssetLoader a;
	NetInterfaceLoader n;

	if(G.bg_music) G.bg_music->play();

	Director::Begin(new IntroScene());
	
	return 0;
}

