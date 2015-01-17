#pragma once

#include "sv_scene.h"

#include <sstream>

class SvPlayScene : public SvScene {
public:
	SvPlayScene();
	~SvPlayScene();

	virtual void Init();
	virtual void HandleNewCl(const client_t & new_cl);
	virtual void HandleGoneCl(const client_t & gone_cl);
	virtual void HandleRecvPacket(	const client_t & cl,
									cl_to_sv_t header,
									Packet & recv_packet);
	virtual void Go();
	
	static int	l_my_print(lua_State* L);

private:
	struct result_buf_t
	{
		int hand;
		int penalty;
		wstringstream log;

		result_buf_t(){}
		result_buf_t(const result_buf_t & other){}
		void clear() {hand = 0; penalty = 0; log.str(wstring()); log.clear();}
	};

private:
	int				_n;
	vector<int>		_left;
	vector<int>		_right;
	int				_left_hp;
	int				_right_hp;
	int				_left_random_count;
	int				_right_random_count;
	vector<int>		_left_data;
	vector<int>		_right_data;

	size_t			_continuous_draw_count;

	result_buf_t	_left_result;
	result_buf_t	_right_result;
	result_buf_t *	_cur_result;
	basic_packet_t	_packet;

	bool			_game_over;
	int				_nr_game_over_cl;

	int				_cur_working_side;

	int			GetNextHandFromLeft();
	int			GetNextHandFromRight();
	int			GetNextHand(const char * tag);

	void		DumpState(lua_State * L, Packet & packet);
	void		IncreaseRandomCount();

	// bound To Lua functions....
	static int	TL_SendN(lua_State *L);
	static int	TL_SendCurrentRecord(lua_State *L);
	static int	TL_SendCurrentFightInfo(lua_State *L);
	static int	TL_GetRandom(lua_State * L);
	static int	TL_SaveData(lua_State *L);
	static int	TL_GetSavedData(lua_State *L);

	int			GetResult(lua_State * L);

	void		SendGameDrawPacket();
	void		SendBasicPacket();
};