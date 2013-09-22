#include "director.h"
#include "asset.h"
#include "global.h"

global_t G;

namespace Director
{
	// 전방선언
	void InternalLoop(scene_id_t first_scene);
	

	bool			switch_flag = false;
	scene_id_t		next_scene;

	////////////// public ///////////////
	void SwitchScene(scene_id_t sid)
	{
		switch_flag = true;
		next_scene = sid;
	}

	void Begin(wstring title, size_t width, size_t height, scene_id_t first_scene)
	{
		G.window.create(VideoMode(width, height), title, Style::Titlebar);
		G.window.setFramerateLimit(60U);

		G.logger = new Logger(L"log.txt", G.window.getSystemHandle());

		LoadAssets();

		if(G.bg_music) G.bg_music->play();

		CreateScenes();

		InternalLoop(first_scene);

		ReleaseAssets();

		delete G.logger;
	}

	///////////// private ///////////
	void InternalLoop(scene_id_t first_scene)
	{
		Scene *			scene;
		client_msg_t	cl_msg;
		Event			event;
		Clock			clock;
		int				last_time = 0;

		scene = SCENE_TABLE[first_scene];
		scene->Init();

		while(G.window.isOpen())
		{
			G.present_time = clock.getElapsedTime().asMilliseconds();
			G.delta_time = G.present_time - last_time;
			last_time = G.present_time;

			G.window.clear(sf::Color::Black);

			while(G.window.pollEvent(event))
			{
				switch(event.type)
				{
				case Event::IMEUpdated:
					if (!(scene->HandleIMEUpdated(event.ime.unicode, event.ime.done)))
					{
					}
					break;

				case Event::TextEntered:
					if(!(scene->HandleTextEntered(event.text.unicode)))
					{
						if(event.text.unicode == UNICODE_ESC) G.window.close();
					}
					break;

				case Event::KeyPressed:
					if(!(scene->HandleKeyPressed(event.key)))
					{
					}
					break;

				case Event::Closed:
					G.window.close();
					break;
				}

				if (switch_flag)
				{
					switch_flag = false;
					scene = SCENE_TABLE[next_scene];
					scene->Init();
				}
			}
		
			scene->Go();
			
			if (switch_flag)
			{
				switch_flag = false;
				scene = SCENE_TABLE[next_scene];
				scene->Init();
			}
			else
			{
				G.window.display();
			}
		
			_ASSERTE( _CrtCheckMemory( ) );
		}
	}
}