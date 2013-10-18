#pragma once

#include "sv_scene.h"

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

private:
	struct result_t
	{
		int left;
		int right;

		result_t(int _left, int _right):left(_left),right(_right){}
	};

	struct basic_packet_t
	{
		vector<result_t> results;
		int left_penalty;
		int right_penalty;

		void clear(){results.clear(); left_penalty = right_penalty = 0;}
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

	int				_continuous_draw_count;

	basic_packet_t	_packet;

	bool			_game_over;
	int				_nr_game_over_cl;

	int				_cur_working_side;

	int			GetNextHandFromLeft();
	int			GetNextHandFromRight();
	int			GetNextHand(const char * tag);

	bool		CheckGameDraw();

	void		DumpState(lua_State * L, Packet & packet);
	void		WarnMsg(lua_State * L, const wstring & msg);
	void		ErrorMsg(lua_State * L, const wstring & msg);
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