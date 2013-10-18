#pragma once

#include "internal.h"


namespace NetInterface
{
	void Begin();
	void End();

	bool Connect(const wstring & addr, const wstring & user_name);
	void GetFoundSv(vector<server_info_t> * ptr);

	void Update();

	void RegisterConnectFailedCallback(function<void()> callback);
	void RegisterConnectedCallback(function<void(const server_info_t & sv_info, ID my_id)> callback);
	void RegisterClientIntroCallback(function<void(const client_t & cl_info)> callback);
	void RegisterClientGoneCallback(function<void(const client_t & cl_info)> callback);
	void RegisterPacketCallback(sv_to_cl_t type, function<void(Packet & packet)> callback);
	void ClearCallbacks();
	void GetClients(smap<ID, client_t> * ptr);
}
