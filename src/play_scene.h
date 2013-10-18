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

	struct result_t
	{
		hand_t left;
		hand_t right;
	};

	typedef queue<result_t> result_queue_t;

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
	result_queue_t	_result_queue;
	PopMsg			_pop;
	int				_accum_time;

	void	HandleQueue();
	void	HandleNetwork();

	void	AddPlayer(const client_t & basic_info);
	void	SyncPlayers();
	void	SendGameOverAckPacket();
};