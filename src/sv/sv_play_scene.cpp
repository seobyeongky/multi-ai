#include "sv_play_scene.h"
#include "../logic.h"
#include "sv_director.h"
#include "sv_global.h"
#include "sv_wait_scene.h"
#include "../basic/string_convert.h"

#include <sstream>


extern "C" 
{ 
	#include <lauxlib.h>
	#include <lualib.h>
}

static void DumpStack(lua_State *L, wstringstream & ss) {
    int i=lua_gettop(L);
    while(  i   ) {
		int t = lua_type(L, i);
		switch (t) {
			case LUA_TSTRING:
				ss << i << L": " << lua_tostring(L, i) << endl;
				break;
			case LUA_TBOOLEAN:
				ss << i << L": " << (lua_toboolean(L, i) ? L"true" : L"false") << endl;
				break;
			case LUA_TNUMBER:
				ss << i << L": " << lua_tonumber(L, i) << endl;
				break;
			default: 
				ss << i << L": " << lua_typename(L, t) << endl;
				break;
		}
		i--;
    }
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
	_left_result(),
	_right_result(),
	_cur_result(nullptr),
	_packet(),
	_game_over(false),
	_nr_game_over_cl(0),
	_cur_working_side(0)
{
	inst = this;

	_left_result.log.imbue(locale("korean"));
	_right_result.log.imbue(locale("korean"));

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

	//
	// Clear (left/right) result
	//
	_left_result.clear();
	_right_result.clear();

	//
	// Run Lua script and get hand
	//
	_cur_result = &_left_result;
	int left_hand = GetNextHandFromLeft();
	_cur_result = &_right_result;
	int right_hand = GetNextHandFromRight();

	_left.push_back(left_hand);
	_right.push_back(right_hand);

	//
	// packet works
	//
	_left_result.hand = left_hand;
	_right_result.hand = right_hand;

	result_t left_result;
	left_result.hand = _left_result.hand;
	left_result.penalty = _left_result.penalty;
	left_result.log = _left_result.log.str();
	result_t right_result;
	right_result.hand = _right_result.hand;
	right_result.penalty = _right_result.penalty;
	right_result.log = _right_result.log.str();
	_packet.results.push_back(pair<result_t,result_t>(left_result, right_result));


	//
	// server logic
	//
	int left_dpoint = GetPoint(static_cast<hand_t>(left_hand), static_cast<hand_t>(right_hand));
	int right_dpoint = GetPoint(static_cast<hand_t>(right_hand), static_cast<hand_t>(left_hand));
	_left_hp += left_dpoint;
	_right_hp += right_dpoint;

	if (left_dpoint == 0 && right_dpoint == 0)
	{
		++_continuous_draw_count;
		
		if (_continuous_draw_count > MAX_CONTINUOUS_DRAW)
		{
			SendBasicPacket();
			_game_over = true;
			return;
		}
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

/*
static int  Traceback(lua_State *L) {
    lua_getfield(L, LUA_GLOBALSINDEX, "debug");
    lua_getfield(L, -1, "traceback");
    lua_pushvalue(L, 1);
    lua_pushinteger(L, 2);
    lua_call(L, 2, 1);
	string sk = lua_tostring(L, -1);
//	ss << lua_tostring(L, -1) << endl;
	return 1;
}
*/

int SvPlayScene::l_my_print(lua_State* L) {
	wstringstream & ss = inst->_cur_result->log;
	int i=lua_gettop(L);
    while(  i   ) {
		int t = lua_type(L, i);
		switch (t) {
			case LUA_TSTRING:
				ss << lua_tostring(L, i);
				break;
			case LUA_TBOOLEAN:
				ss << (lua_toboolean(L, i) ? L"true" : L"false");
				break;
			case LUA_TNUMBER:
				ss << lua_tonumber(L, i);
				break;
			default: 
				ss << lua_typename(L, t);
				break;
		}
		if (i > 1)
		{
			ss << L'\t';
		}
		i--;
    }
	ss << endl;

    return 0;
}

static const struct luaL_reg printlib [] = {
  {"print", SvPlayScene::l_my_print},
  {NULL, NULL} /* end of array */
};

int SvPlayScene::GetNextHand(const char * tag)
{
	place_t & my_place = svG.place[_cur_working_side];

	lua_State* L = lua_open();
	luaopen_base(L);
	luaopen_debug(L);
	lua_getglobal(L, "_G");
	luaL_register(L, NULL, printlib);
	lua_pop(L, 1);
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
		wstringstream ss;
		ss << L"Lua runtime exception 발생! : " << lua_tostring(L, -1) << endl;
//		Traceback(L);
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

void SvPlayScene::DumpState(lua_State * L, Packet & packet)
{
	wstringstream mystream;
	lua_Debug ar;
	if(lua_getstack(L, 0, &ar) != 0)
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

void SvPlayScene::IncreaseRandomCount()
{
	if (_cur_working_side == 0)
	{
		_left_random_count++;
		if (_left_random_count > static_cast<int>(RANDOM_PENALTY_NUMBER))
		{
			_left_random_count = 0;
			_left_hp--;
			_left_result.penalty++;
			_left_result.log << L"[INFO] Random penalty!" << endl;
		}
	}
	else
	{
		_right_random_count++;
		if (_right_random_count > static_cast<int>(RANDOM_PENALTY_NUMBER))
		{
			_right_random_count = 0;
			_right_hp--;
			_right_result.penalty++;
			_right_result.log << L"[INFO] Random penalty!" << endl;
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
		inst->_cur_result->log
			<< L"[WARNING] GetRecordData함수를 매개 변수없이 호출했습니다."
			<< endl;
		return 0;
	}
	if (!lua_isnumber(L, n))
	{
		inst->_cur_result->log
			<< L"[WARNING] GetRecrodData함수의 매개 변수가 숫자가 아닙니다.("
			<< lua_typename(L, lua_type(L, n))
			<< L")"
			<< endl;
		return 0;
	}
	int i = (int)lua_tonumber(L, n);

	if (i < 0 || inst->_n <= i)
	{
		inst->_cur_result->log
			<< L"[WARNING] GetRecordData함수가 잘못된 index("
			<< i
			<< L")을 참조합니다."
			<< endl;
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
		inst->_cur_result->log
			<< L"[WARNING] GetFightData함수를 매개 변수없이 호출했습니다."
			<< endl;
		return 0;
	}
	if (!lua_isnumber(L, n))
	{
		inst->_cur_result->log
			<< L"[WARNING] GetFightData함수의 매개 변수가 숫자가 아닙니다.("
			<< lua_typename(L, lua_type(L, n))
			<< L")"
			<< endl;
		return 0;
	}
	int i = (int)lua_tonumber(L, n);

	if (i < 0 || inst->_n <= i)
	{
		inst->_cur_result->log
			<<  L"[WARNING] GetFightData함수가 잘못된 index("
			<< i
			<< L")을 참조합니다.";
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
		inst->_cur_result->log
			<< L"[WARNING] SaveData함수의 매개 변수는 2개여야 합니다. ("
			<< n
			<< L"개로 호출함)"
			<< endl;
		return 0;
	}
	if (!lua_isnumber(L, n))
	{
		inst->_cur_result->log
			<< L"[WARNING] SaveData함수의 첫 번째 매개 변수가 숫자가 아닙니다.("
			<< lua_typename(L, lua_type(L, n))
			<< L")"
			<< endl;
		return 0;
	}
	if (!lua_isnumber(L, n-1))
	{
		inst->_cur_result->log
			<< L"[WARNING] SaveData함수의 두 번째 매개 변수가 숫자가 아닙니다.("
			<< lua_typename(L, lua_type(L, n-1))
			<< L")"
			<< endl;
		return 0;
	}
	int i = (int)lua_tonumber(L, n - 1);
	int j = (int)lua_tonumber(L, n);

	if (i < 0)
	{
		inst->_cur_result->log
			<< L"[WARNING] SaveData함수가 0이하의 index("
			<< i
			<< L")에 접근합니다."
			<< endl;
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
		inst->_cur_result->log
			<< L"[WARNING] GetData함수가 매개 변수 없이 호출됐습니다."
			<< endl;
		return 0;
	}
	if (!lua_isnumber(L, n))
	{
		inst->_cur_result->log
			<< L"[WARNING] GetData함수의 매개 변수가 숫자가 아닙니다.("
			<< lua_typename(L, lua_type(L, n))
			<< L")"
			<< endl;
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

		inst->_cur_result->log
			<< L"[WARNING] GetData함수가 잘못된 index("
			<< i
			<< L")를 참조합니다."
			<< endl;
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
		inst->_cur_result->log
			<< L"[ERROR] result값을 찾을 수가 없습니다!"
			<< endl;
		return -1;
	}
	if (!lua_isnumber(L, -1))
	{
		inst->_cur_result->log
			<< L"[ERROR] result가 숫자가 아닙니다."
			<< L"(다른 에러가 Lua-C 스택을 오염시켜서 생긴 부작용일 가능성도 있음)"
			<< endl
			<< L"- Stack dump"
			<< endl;
		DumpStack(L, inst->_cur_result->log);
		return -1;
	}
	int result = (int)lua_tonumber(L, -1);
	
	if(result < 0 || result > 2)result = 0;
	return result;
}

void SvPlayScene::SendBasicPacket()
{
	if (_packet.results.size() == 0) return;

	Packet send_packet;
	send_packet	<< TO_UINT16(SV_TO_CL_BASIC_INFO)
				<< _packet;

	SendToAll(send_packet);

	_packet.clear();
}