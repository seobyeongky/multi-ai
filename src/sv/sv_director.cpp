#pragma once

#include "sv_scene.h"
#include "sv_director.h"
#include "sv_global.h"
#include "../util.h"

sv_global_t svG;

namespace SvDirector
{
	bool			switch_flag = false;
	SvScene *		next_scene;
	bool			working = false;
	bool			end_flag = false;

	void SwitchScene(SvScene * _next_scene)
	{
		switch_flag = true;
		next_scene = _next_scene;
	}

	void Begin(SvScene * first_scene)
	{
		end_flag = false;

		SvScene *		scene = first_scene;
		server_msg_t	msg;
		Clock			clock;
		int				last_time = 0;

		working = true;
		while (!end_flag)
		{
			svG.present_time = clock.getElapsedTime().asMilliseconds();
			svG.delta_time = svG.present_time - last_time;
			last_time = svG.present_time;

			while (GetServerMsg(&msg))
			{
				switch (msg.type)
				{
				case SVMSG_NEW_CLIENT:
					svG.client_map[msg.client_info.id].basic_info = msg.client_info;
					scene->HandleNewCl(msg.client_info);
					break;
				case SVMSG_CLIENT_GONE:
					svG.client_map.erase(msg.client_info.id);
					scene->HandleGoneCl(msg.client_info);
					break;
				case SVMSG_PACKET_RECV:
					{
						unsigned short header;
						if (!(msg.packet >> header)) break;
						scene->HandleRecvPacket(msg.client_info, static_cast<cl_to_sv_t>(header), msg.packet);
					}
					break;
				case SVMSG_CLIENT_DELAY_TIME_UPDATED:
					{
						client_map_t::Iter iter;
						if (svG.client_map.find(msg.client_info.id, &iter))
						{
							(*iter).element().delay_time = msg.delay_time;
						}
					}
					break;
				case SVMSG_BIND_FAILED:
					ErrorMsg(L"바인드 실패!");
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
		
			Sleep(10); 
		}
		working = false;

		delete scene;
	}

	void End()
	{
		end_flag = true;
	}

	bool IsWorking()
	{
		return working;
	}
}