#include "director.h"
#include "asset.h"
#include "global.h"
#include "net_interface.h"

namespace Director
{
	bool			switch_flag = false;
	Scene *			next_scene;

	void SwitchScene(Scene * _next_scene)
	{
		switch_flag = true;
		next_scene = _next_scene;
	}

	void Begin(Scene * first_scene)
	{
		Scene *			scene = first_scene;
		client_msg_t	cl_msg;
		Event			event;
		Clock			clock;
		int				last_time = 0;

		while(G.window.isOpen())
		{
			G.present_time = clock.getElapsedTime().asMilliseconds();
			G.delta_time = G.present_time - last_time;
			last_time = G.present_time;

			G.sfx_mgr.Update();
			NetInterface::Update();

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
					delete scene;
					scene = next_scene;
				}
			}
		
			scene->Go();
			
			if (switch_flag)
			{
				switch_flag = false;
				delete scene;
				scene = next_scene;
			}
			else
			{
				G.window.display();
			}
		
			_ASSERTE( _CrtCheckMemory( ) );
		}
		delete scene;
	}
}