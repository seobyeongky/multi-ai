#include "sv_play_scene.h"
#include "../logic.h"
#include "sv_director.h"
#include "sv_global.h"
#include "sv_wait_scene.h"

#include <sstream>


extern "C" 
{ 
	#include <lauxlib.h>
	#include <lualib.h>
}

SvPlayScene * inst = nullptr;

SvPlayScene::SvPlayScene()
	: 
	_n(0),
	_left(),
	_right(),
	_left_hp(MAX_HP),
	_right_hp(MAX_HP),
	_left_random_count(0),
	_right_random_count(0),
	_left_data(),
	_right_data(),
	_continuous_draw_count(0),
	_packet(),
	_game_over(false),
	_nr_game_over_cl(0),
	_cur_working_side(0)
{
	inst = this;

	_packet.clear();
}

SvPlayScene::~SvPlayScene()
{
}

void SvPlayScene::Init()
{
}

void SvPlayScene::HandleNewCl(const client_t & new_cl)
{
	if (_game_over) ++_nr_game_over_cl;
}

void SvPlayScene::HandleGoneCl(const client_t & gone_cl)
{
}

void SvPlayScene::HandleRecvPacket(	const client_t & cl,
									cl_to_sv_t header,
									Packet & recv_packet)
{
	if (header == CL_TO_SV_GAME_OVER) ++_nr_game_over_cl;
	else SvScene::HandleRecvPacket(cl, header, recv_packet);
}

void SvPlayScene::Go()
{
	if (_game_over)
	{
		if (_nr_game_over_cl >= static_cast<int>(svG.client_map.size()))
		{
			SvDirector::SwitchScene(new SvWaitScene());
		}
		return;
	}

	int left_hand = GetNextHandFromLeft();
	int right_hand = GetNextHandFromRight();

	_left.push_back(left_hand);
	_right.push_back(right_hand);

	_packet.results.push_back(result_t(left_hand, right_hand));

	int left_dpoint = GetPoint(static_cast<hand_t>(left_hand), static_cast<hand_t>(right_hand));
	int right_dpoint = GetPoint(static_cast<hand_t>(right_hand), static_cast<hand_t>(left_hand));
	_left_hp += left_dpoint;
	_right_hp += right_dpoint;

	if (left_dpoint == 0 && right_dpoint == 0)
	{
		++_continuous_draw_count;
		if (CheckGameDraw()) return;
	}
	else
		_continuous_draw_count = 0;

	bool left_dead = _left_hp <= 0;
	bool right_dead = _right_hp <= 0;

	if (left_dead && right_dead)
	{
		// draw
		SendBasicPacket();
		_game_over = true;
		return;
	}
	else if (left_dead && !right_dead)
	{
		// right won
		SendBasicPacket();
		_game_over = true;
		return;
	}
	else if (!left_dead && right_dead)
	{
		// left won
		SendBasicPacket();
		_game_over = true;
		return;
	}
	else
	{
		if (_packet.results.size() >= RESULT_BUNDLE_SIZE)
			SendBasicPacket();
	}

	_n++;
}

int SvPlayScene::GetNextHandFromLeft()
{
	_cur_working_side = 0;

	return GetNextHand("left");
}

int SvPlayScene::GetNextHandFromRight()
{
	_cur_working_side = 1;
	return GetNextHand("right");
}

int SvPlayScene::GetNextHand(const char * tag)
{
	place_t & my_place = svG.place[_cur_working_side];

	lua_State* L = lua_open();
	luaopen_base(L);
	lua_register(L, "GetN", TL_SendN);
	lua_register(L, "GetRecordData", TL_SendCurrentRecord);
	lua_register(L, "GetFightData", TL_SendCurrentFightInfo);
	lua_register(L, "GetRandom", TL_GetRandom);
	lua_register(L, "SaveData", TL_SaveData);
	lua_register(L, "GetData", TL_GetSavedData);

	luaL_loadbuffer(L, my_place.script_buf.c_str(), my_place.script_buf.length(), tag);
	
	int res;
	if ((res = lua_pcall(L, 0, 1, 0)) != 0)
	{
		wchar_t buf[512];
		swprintf_s(buf, L"%hs", lua_tostring(L, -1));
		ErrorMsg(L, buf);
	}

	int result = GetResult(L);
	if (result == -1)
	{
		IncreaseRandomCount();
		result = rand() % 3;
	}

	lua_close(L);

	return result;
}

bool SvPlayScene::CheckGameDraw()
{
	if (_continuous_draw_count > 100)
	{
		SendGameDrawPacket();
		SvDirector::SwitchScene(new SvWaitScene());
		return true;
	}
	return false;
}
void SvPlayScene::DumpState(lua_State * L, Packet & packet)
{
	wstringstream mystream;
	lua_Debug ar;
	if(lua_getstack(L, 0, &ar) == 0)
	{
		mystream << L"상세정보 얻기 실패" << endl;
	}
	else
	{
		lua_getinfo(L, "nSlufL", &ar);
		if (ar.name != NULL)
			mystream << L"이름" << ar.name << endl;
		mystream << L"소스:" << ar.source << endl;
		if (ar.currentline != -1)
			mystream << L"실행라인:" << ar.currentline << endl;
		mystream << L"소스코드:" << ar.short_src << endl;
		mystream << L"함수 정의 코드 라인:" << ar.linedefined << endl;
	}
	packet << mystream.str();
}

void SvPlayScene::WarnMsg(lua_State * L, const wstring & msg)
{
	wstring & scriptname = svG.place[_cur_working_side].script_name;
	Packet send_packet;
	send_packet << TO_UINT16(SV_TO_CL_SCRIPT_ERROR)
				<< scriptname
				<< msg;
	DumpState(L, send_packet);
	SendToAll(send_packet);
}

void SvPlayScene::ErrorMsg(lua_State * L, const wstring & msg)
{
	wstring & scriptname = svG.place[_cur_working_side].script_name;
	Packet send_packet;
	send_packet << TO_UINT16(SV_TO_CL_SCRIPT_ERROR)
				<< scriptname
				<< msg;
	DumpState(L, send_packet);
	SendToAll(send_packet);
}

void SvPlayScene::IncreaseRandomCount()
{
	if (_cur_working_side == 0)
	{
		_left_random_count++;
		if (_left_random_count > static_cast<int>(RANDOM_PENALTY_NUMBER))
		{
			_left_random_count = 0;
			_left_hp--;
			_packet.left_penalty++;
		}
	}
	else
	{
		_right_random_count++;
		if (_right_random_count > static_cast<int>(RANDOM_PENALTY_NUMBER))
		{
			_right_random_count = 0;
			_right_hp--;
			_packet.right_penalty++;
		}
	}
}

int SvPlayScene::TL_SendN(lua_State *L)
{
	lua_pushnumber(L, (double)inst->_n);
	return 1;
}

int SvPlayScene::TL_SendCurrentRecord(lua_State *L)
{
	int n = (int)lua_gettop(L);
	if (n == 0)
	{
		inst->WarnMsg(L, L"GetRecordData함수를 매개 변수없이 호출했습니다.");
		return 0;
	}
	if (!lua_isnumber(L, n))
	{
		string tn = lua_typename(L, lua_type(L, n));
		wstring wtn(tn.begin(), tn.end());
		inst->WarnMsg(L, L"GetRecrodData함수의 매개 변수가 숫자가 아닙니다.(" + wtn + L")");
		return 0;
	}
	int i = (int)lua_tonumber(L, n);

	if (i < 0 || inst->_n <= i)
	{
		wstring wmsg = L"GetRecordData함수가 잘못된 index(" + to_wstring(i) +  L")을 참조합니다.";
		wmsg += L"현재 n:" + to_wstring(inst->_n);
		inst->WarnMsg(L, wmsg);
		return 0;
	}

	if (inst->_cur_working_side == 0)
	{
		lua_pushnumber(L, (double)inst->_left[i]);
		lua_pushnumber(L, (double)inst->_right[i]);
	}
	else
	{
		lua_pushnumber(L, (double)inst->_right[i]);
		lua_pushnumber(L, (double)inst->_left[i]);
	}
	
	return 2;
}

int SvPlayScene::TL_SendCurrentFightInfo(lua_State *L)
{
	int n = (int)lua_gettop(L);
	if (n == 0)
	{
		inst->WarnMsg(L, L"GetFightData함수를 매개 변수없이 호출했습니다.");
		return 0;
	}
	if (!lua_isnumber(L, n))
	{
		string tn = lua_typename(L, lua_type(L, n));
		wstring wtn(tn.begin(), tn.end());
		inst->WarnMsg(L, L"GetFightData함수의 매개 변수가 숫자가 아닙니다.(" + wtn + L")");
		return 0;
	}
	int i = (int)lua_tonumber(L, n);

	if (i < 0 || inst->_n <= i)
	{
		wstring wmsg = L"GetFightData함수가 잘못된 index(" + to_wstring(i) +  L")을 참조합니다.";
		wmsg += L"현재 n:" + to_wstring(inst->_n);
		inst->WarnMsg(L, wmsg);
		return 0;
	}
	// point[i][j] : i와 j가 싸웠을 때 i쪽이 받는 점수 
	// GetPoint와는 다른 거임.
	const int point[3][3] = {{0, -1, 1}, {1, 0, -1}, {-1, 1, 0}};

	if (inst->_cur_working_side == 0)
	{
		lua_pushnumber(L, (double)point[inst->_left[i]][inst->_right[i]]);
	}
	else
	{
		lua_pushnumber(L, (double)point[inst->_right[i]][inst->_left[i]]);
	}
	return 1;
}

int SvPlayScene::TL_GetRandom(lua_State *L)
{
	lua_pushnumber(L, (double)rand());

	inst->IncreaseRandomCount();

	return 1;
}

int SvPlayScene::TL_SaveData(lua_State *L)
{
	int n = (int)lua_gettop(L);
	if (n <= 1)
	{
		inst->WarnMsg(L, L"SaveData함수의 매개 변수는 2개여야 합니다. (현재" + to_wstring(n) + L"개)");
		return 0;
	}
	if (!lua_isnumber(L, n))
	{
		string tn = lua_typename(L, lua_type(L, n));
		wstring wtn(tn.begin(), tn.end());
		inst->WarnMsg(L, L"SaveData함수의 첫 번째 매개 변수가 숫자가 아닙니다.(" + wtn + L")");
		return 0;
	}
	if (!lua_isnumber(L, n-1))
	{
		string tn = lua_typename(L, lua_type(L, n-1));
		wstring wtn(tn.begin(), tn.end());
		inst->WarnMsg(L, L"SaveData함수의 두 번째 매개 변수가 숫자가 아닙니다.(" + wtn + L")");
		return 0;
	}
	int i = (int)lua_tonumber(L, n - 1);
	int j = (int)lua_tonumber(L, n);

	if (i < 0)
	{
		wstring wmsg = L"SaveData함수가 0이하의 index(" + to_wstring(i) + L")에 접근합니다.";
		inst->WarnMsg(L, wmsg);
		return 0;
	}

	if (inst->_cur_working_side == 0)
	{
		if (static_cast<int>(inst->_left_data.size()) <= i)
			inst->_left_data.resize(i + 1, 0);
		inst->_left_data[i] = j;
	}
	else
	{
		if (static_cast<int>(inst->_right_data.size()) <= i)
			inst->_right_data.resize(i + 1, 0);
		inst->_right_data[i] = j;
	}
	return 0;
}

int SvPlayScene::TL_GetSavedData(lua_State *L)
{
	int n = (int)lua_gettop(L);
	if (n == 0)
	{
		inst->WarnMsg(L, L"GetData함수가 매개 변수 없이 호출됐습니다.");
		return 0;
	}
	if (!lua_isnumber(L, n))
	{
		string tn = lua_typename(L, lua_type(L, n));
		wstring wtn(tn.begin(), tn.end());
		inst->WarnMsg(L, L"GetData함수의 매개 변수가 숫자가 아닙니다.(" + wtn + L")");
		return 0;
	}
	int i = (int)lua_tonumber(L, n);

	vector<int> * vec_ptr = nullptr;
	if (inst->_cur_working_side == 0)
		vec_ptr = &inst->_left_data;
	else
		vec_ptr = &inst->_right_data;

	if (i < 0 || static_cast<int>(vec_ptr->size()) <= i)
	{
		lua_pushnumber(L, 0);
		wstring warn_msg = L"GetData함수가 잘못된 index(" + to_wstring(i) + L")를 참조합니다.";
		warn_msg += L"\n현재 저장된 배열의 크기는 " + to_wstring(vec_ptr->size());
		inst->WarnMsg(L, warn_msg);
		return 0;
	}
	else
	{
		lua_pushnumber(L, (*vec_ptr)[i]);
	}

	return 1;
}

int SvPlayScene::GetResult(lua_State * L)
{
	lua_getglobal(L, "result");
	int n = lua_gettop(L);
	if (n == 0)
	{
		ErrorMsg(L, L"result값을 찾을 수가 없습니다!");
		return -1;
	}
	if (!lua_isnumber(L, n))
	{
		ErrorMsg(L, L"result가 숫자가 아닙니다.");
		return -1;
	}
	int result = (int)lua_tonumber(L, n);
	
	if(result < 0 || result > 2)result = 0;
	return result;
}

void SvPlayScene::SendGameDrawPacket()
{
	Packet send_packet;
	send_packet << TO_UINT16(SV_TO_CL_GAME_DRAW);
	SendToAll(send_packet);
}

void SvPlayScene::SendBasicPacket()
{
	if (_packet.results.size() == 0) return;

	Packet send_packet;
	send_packet	<< TO_UINT16(SV_TO_CL_BASIC_INFO)
				<< _packet.results.size();

	for (auto it = _packet.results.begin(); it != _packet.results.end(); ++it)
	{
		send_packet << TO_UINT16((*it).left) << TO_UINT16((*it).right);
	}

	send_packet	<< TO_UINT16(_packet.left_penalty)
				<< TO_UINT16(_packet.right_penalty);
	SendToAll(send_packet);

	_packet.clear();
}