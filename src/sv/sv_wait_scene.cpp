#include "sv_wait_scene.h"
#include "sv_global.h"
#include "sv_util.h"
#include "sv_director.h"
#include "sv_play_scene.h"

#include "../internal.h"

SvWaitScene::SvWaitScene()
{
	for (int side = 0; side < 2; ++side)
	if (svG.place[side].cl != nullptr)
	{
		Packet send_packet;
		send_packet << TO_UINT16(SV_TO_CL_UNREGISTER)
					<< TO_UINT16(side);
		SendToAll(send_packet);
		svG.place[side].cl = nullptr;
	}
}

SvWaitScene::~SvWaitScene()
{
}

void SvWaitScene::Init()
{
}

void SvWaitScene::HandleNewCl(const client_t & new_cl)
{
}

void SvWaitScene::HandleGoneCl(const client_t & gone_cl)
{
}

void SvWaitScene::HandleRecvPacket(	const client_t & cl,
									cl_to_sv_t header,
									Packet & recv_packet)
{
	switch (header)
	{
	case CL_TO_SV_REQUEST_REGISTER:
		{
			sf::Uint16 __side;
			int side;
			if (!(recv_packet >> __side)) return;
			side = static_cast<int>(__side);
			if (!IsValidSide(side)) return;

			place_t & place = svG.place[side];
			if (place.cl == nullptr)
			{
				place.cl = &svG.client_map[cl.id];

				Packet send_packet;
				send_packet << TO_UINT16(SV_TO_CL_REGISTER_ACCEPTED);
				SafeSend(cl.id, send_packet);
			}
			else
			{
				Packet send_packet;
				send_packet << TO_UINT16(SV_TO_CL_REGISTER_DENIED);
				SafeSend(cl.id, send_packet);
			}
		}
		break;

	case CL_TO_SV_SCRIPT:
		{
			sf::Uint16 __side;
			int side = -1;
			if (!(recv_packet >> __side)) return;
			side = static_cast<int>(__side);
			if (!(IsValidSide(side))) return;
			place_t & place = svG.place[side];
			if (place.cl == nullptr || place.cl->basic_info.id != cl.id) return;
			wstring buf;
			if (!(recv_packet	>> place.script_name
								>> buf)) return;
			uni2multi(buf, &place.script_buf);
			Packet send_packet;
			send_packet << TO_UINT16(SV_TO_CL_REGISTER)
						<< TO_UINT16(side)
						<< cl.id
						<< place.script_name;
			SendToAll(send_packet);

			if (svG.place[1-side].cl != nullptr)
			{
				SvDirector::SwitchScene(new SvPlayScene());
			}
		}
		break;


	default:
		SvScene::HandleRecvPacket(cl, header, recv_packet);
	}
}

void SvWaitScene::Go()
{
}