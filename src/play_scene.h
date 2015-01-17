#pragma once

#include "scene.h"
#include "player.h"
#include "chat_box.h"
#include "gamer.h"
#include "pop_msg.h"

#include <queue>

class PlayScene : public Scene
{
public:
				 PlayScene(	const wstring & name,
							const wstring & room_name,
							bool is_host,
							ID my_id);
				~PlayScene();

	virtual bool HandleIMEUpdated(wchar_t code, bool done);

	virtual bool HandleTextEntered(wchar_t code);

	virtual bool HandleKeyPressed(Event::KeyEvent key);


	void		 Go();

private:
	typedef smap<ID, player_t> player_map_t;

	enum state_t
	{
		WAIT,
		PLAY
	};

	enum input_state_t
	{
		MAIN_MENU,
		FILE_MENU
	};

	typedef queue<pair<result_t,result_t>> result_queue_t;

private:
	bool			_is_host;
	ID				_my_id;
	player_t *		_me;
	player_map_t	_player_map;
	state_t			_state;
	Text			_room_name_text;
	ChatBox			_chat_box;
	Menu			_menu;
	Menu			_file_menu;
	input_state_t	_input_state;
	bool			_left_exists;
	LeftGamer		_left_gamer;
	bool			_right_exists;
	RightGamer		_right_gamer;
	size_t			_continuous_draw_count;
	size_t			_round;
	result_queue_t	_result_queue;
	PopMsg			_pop;
	int				_accum_time;
	Text			_player_text;
	wofstream		_gamelog;
	Sprite			_bg;

	void	HandleGameResult();

	void	HandleQueue();
	void	HandleNetwork();

	void	AddPlayer(const client_t & basic_info);
	void	SyncPlayers();
	void	SendGameOverAckPacket();
	void	UpdatePlayerText();
};