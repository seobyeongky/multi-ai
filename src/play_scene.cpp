#include "play_scene.h"
#include "global.h"
#include "net_interface.h"
#include "sv\sv_service.h"
#include "util.h"
#include "logic.h"

#include <iostream>
#include <sstream>

PlayScene::PlayScene(const wstring & name, const wstring & room_name,
					 bool is_host, ID my_id)
	: _is_host(is_host), _my_id(my_id),
	_state(WAIT),
	_room_name_text(room_name, G.default_font, 25U),
	_chat_box(),
	_menu(Menu::delta_t(Vector2f(0.f, 50.f), -15.f, 1.f), Keyboard::Space),
	_file_menu(Menu::delta_t(Vector2f(0.f, 40.f), -8.f, 1.f), Keyboard::Space),
	_input_state(MAIN_MENU),
	_left_exists(false),
	_left_gamer(),
	_right_exists(false),
	_right_gamer(),
	_continuous_draw_count(0),
	_round(0),
	_result_queue(),
	_pop(),
	_accum_time(0),
	_player_text(L"", G.default_font, 20U),
	_gamelog(L"gamelog.txt", ios::out),
	_bg(*G.sprite_map[L"bg"])
{
	G.window.setTitle(L"기막힌 가위바위보 - " + room_name + L"방");
	_room_name_text.setPosition(50.f, 50.f);
	
	_player_map[_my_id] = player_t(name, GetNextColor());
	_me = &_player_map[_my_id];

	_gamelog.imbue(locale("korean"));

	SyncPlayers();

	NetInterface::RegisterClientIntroCallback([this](const client_t & cl_info)
	{
		AddPlayer(cl_info);
		UpdatePlayerText();
	});

	NetInterface::RegisterClientGoneCallback([this](const client_t & cl_info)
	{
		_player_map.erase(cl_info.id);
		UpdatePlayerText();
	});
	
	NetInterface::RegisterPacketCallback(SV_TO_CL_CHAT, [this](Packet & packet)
	{
		ID cl_id;
		wstring msg;
		if (!(packet >> cl_id >> msg)) return;
		auto & player = _player_map[cl_id];
		_chat_box.AddChatMsg(player.color, player.name,
			Color(235, 235, 255), msg);
		G.sfx_mgr.Play(SFX_LEAVE_CHAT);
	});

	NetInterface::RegisterPacketCallback(SV_TO_CL_REGISTER_DENIED, [this](Packet & packet)
	{
		_chat_box.AddInfoMsg(L"서버에서 등록을 거절했습니다.");
	});

	NetInterface::RegisterPacketCallback(SV_TO_CL_REGISTER, [this](Packet & packet)
	{
		sf::Uint16 __side;
		int side;
		ID cl_id;
		wstring script_name;
		if (!(packet >> __side >> cl_id >> script_name)) return;
		player_t & player = _player_map[cl_id];
		wstring text = player.name + L" : " + script_name;
		side = static_cast<int>(__side);
		if (side == 0)
		{
			_left_gamer.Init();
			_left_gamer.SetText(text);
			_left_exists = true;
		}
		else
		{
			_right_gamer.Init();
			_right_gamer.SetText(text);
			_right_exists = true;
		}

		if (_left_exists == false || _right_exists == false)
		{
			G.attract_music->reset();
			G.attract_music->play();
		}
		else
		{
			// start
			_continuous_draw_count = 0;
			_round = 0;
			_gamelog
				<< L"---- Round begin : "
				<< _left_gamer.GetText()
				<< L" vs "
				<< _right_gamer.GetText()
				<< endl;
			while(!_result_queue.empty()) _result_queue.pop();
			G.attract_music->stop();
			G.fight_music->reset();
			G.fight_music->play();
		}
	});

	NetInterface::RegisterPacketCallback(SV_TO_CL_BASIC_INFO, [this](Packet & packet)
	{
		if (!_left_exists || !_right_exists) return;

		basic_packet_t basic;
		packet >> basic;

		for (auto & pair : basic.results)
		{
			_result_queue.push(pair);
		}
	});

	NetInterface::RegisterPacketCallback(SV_TO_CL_UNREGISTER, [this](Packet & packet)
	{
		sf::Uint16 __side;
		if (!(packet >> __side)) return;
		int side = static_cast<int>(__side);
		if (side == 0) _left_exists = false;
		else _right_exists = false;

		G.fight_music->stop();
		while(!_result_queue.empty()) _result_queue.pop();
	});

	_chat_box.Reset(*_me);

	class MyItem : public IItem
	{
	public:
		MyItem(const wstring & string, function<void()> next)
				: _text(string, G.default_font, 20),
				_rect(*G.sprite_map[L"selected"]),
				_next(next)
			{
				_text.setColor(Color(200, 200, 200));
				auto bounds = _text.getLocalBounds();
				_text.setOrigin(bounds.width, bounds.height);

				SetScaleToSize(&_rect, bounds.width + 10.f, bounds.height + 10.f);
				_rect.move(bounds.left + 5.f, bounds.top + 5.f);
				auto rect_bounds = _rect.getLocalBounds();
				_rect.setOrigin(rect_bounds.width, rect_bounds.height);
		}

			virtual void selected()
			{
				_text.setColor(Color::Green);
				_draw_rect = true;
			}

			virtual void unselected()
			{
				_text.setColor(Color(200, 200, 200));
				_draw_rect = false;
			}

			virtual void activated()
			{
				_next();
			}

		private:
			virtual void draw(RenderTarget& target, RenderStates states) const
			{
				states.transform *= getTransform();
				target.draw(_text, states);
				if (_draw_rect) target.draw(_rect, states);
			}

		private:
			Text				_text;
			Sprite				_rect;
			bool				_draw_rect;
			function<void()>	_next;
	};

	_menu.setRotation(30.f);
	_menu.setPosition(600.f, 400.f);
	for (int side = 1; side >= 0; --side)
	{
		wstring side_name;
		if (side == 0)
			side_name = L"왼쪽";
		else
			side_name = L"오른쪽";

		_menu.pushItem(new MyItem(side_name + L"에 등록하기", [this, side]()
		{
			vector<wstring> file_list;
			GetScriptFileList(&file_list);
			if (file_list.empty()) 
			{
				_chat_box.AddAlarmMsg(L"ai폴더에 lua파일이 없습니다.");
				return;
			}
			_file_menu.clear();
			for (auto it = file_list.begin(); it != file_list.end(); ++it)
			{
				wstring & file_name = *it;
				_file_menu.pushItem(new MyItem(file_name, [this, file_name, side]()
				{
					NetInterface::RegisterPacketCallback(SV_TO_CL_REGISTER_ACCEPTED, [this, file_name, side](Packet & packet)
					{
						wstring buf;
						GetScriptFromFile(file_name, &buf);
						Packet send_packet;
						send_packet	<< TO_UINT16(CL_TO_SV_SCRIPT)
									<< TO_UINT16(side)
									<< file_name
									<< buf;
						SafeSend(send_packet);
						_chat_box.AddInfoMsg(L"서버에게 스크립트" + file_name + L" 파일 보내는 중");
					});

					Packet send_packet;
					send_packet <<	TO_UINT16(CL_TO_SV_REQUEST_REGISTER)
								<<	TO_UINT16(side);
					SafeSend(send_packet);

					_input_state = MAIN_MENU;
				}));
			}
			_input_state = FILE_MENU;
		}));
	}

	_file_menu.setRotation(35.f);
	_file_menu.setPosition(430.f, 270.f);
	
	_player_text.setPosition(550.f, 130.f);
	_player_text.setColor(Color::White);
	UpdatePlayerText();

	FitScaleToScreen(&_bg);
}

PlayScene::~PlayScene()
{
	_gamelog.close();
	if (_is_host) EndSvService();
}

bool PlayScene::HandleIMEUpdated(wchar_t code, bool done)
{
	return _chat_box.HandleIMECompMsg(code, done);
}

bool PlayScene::HandleTextEntered(wchar_t code)
{
	if(!_chat_box.CheckEnterDone())
	{
		if(code == UNICODE_ESC)
		{
			_chat_box.AlertEnterDone();
			return true;
		}
		else return _chat_box.HandleTextEntered(code);
	}
	else
	{
		if(code == UNICODE_ESC)
		{
			DisconnectToServer();
			G.window.close();
			return true;
		}
		else return false;
	}

	return _chat_box.HandleTextEntered(code);
}

bool PlayScene::HandleKeyPressed(Event::KeyEvent key)
{
	if(!_chat_box.CheckEnterDone())
	{
		bool result = _chat_box.HandleKeyPressed(key);
//		if(_chat_box.CheckEnterDone()) ResetVKeyState();
		return result;
	}
	else
	{
		if(key.code == Keyboard::Return) {_chat_box.BeginToEnter(); return true; }
		if (!_right_exists || !_left_exists)
		{
			if (_input_state == MAIN_MENU)
			{
				return _menu.handleKeyPressed(key);
			}
			else if (_input_state == FILE_MENU)
			{
				return _file_menu.handleKeyPressed(key);
			}
		}
		return false;	
	}
}

void PlayScene::Go()
{
	_pop.Update();
	HandleQueue();
	HandleNetwork();
	_chat_box.Update();

	G.window.draw(_bg);
	if (_left_exists) G.window.draw(_left_gamer);
	if (_right_exists) G.window.draw(_right_gamer);
	if (!_right_exists || !_left_exists)
	{
		G.window.draw(_menu);

		if (_input_state == FILE_MENU)
			G.window.draw(_file_menu);
	}
	G.window.draw(_chat_box);
	G.window.draw(_player_text);
	G.window.draw(_pop);
}

void PlayScene::HandleGameResult()
{
	if ((!_left_gamer.IsDead() && _right_gamer.IsDead()) ||
		(!_left_gamer.IsDead() && !_right_gamer.IsDead() && _left_gamer.GetHealth() > _right_gamer.GetHealth()))
	{
		wstring msg = _left_gamer.GetText();
		msg += L" 승리!";
		_gamelog << msg << endl;
		_pop.Show(msg.c_str());
		G.sfx_mgr.Play(SFX_WIN);
		SendGameOverAckPacket();
		assert(_result_queue.empty());
	}
	else if ((_left_gamer.IsDead() && !_right_gamer.IsDead()) ||
		(!_left_gamer.IsDead() && !_right_gamer.IsDead() && _left_gamer.GetHealth() < _right_gamer.GetHealth()))
	{
		wstring msg = _right_gamer.GetText();
		msg += L" 승리!";
		_gamelog << msg << endl;
		_pop.Show(msg.c_str());
		G.sfx_mgr.Play(SFX_WIN);
		SendGameOverAckPacket();
		assert(_result_queue.empty());
	}
	else
	{
		_pop.Show(L"비겼습니다.");
		_gamelog << L"비겼습니다." << endl;
		G.sfx_mgr.Play(SFX_WIN);
		SendGameOverAckPacket();
		assert(_result_queue.empty());
	}
}

void PlayScene::HandleQueue()
{
	_accum_time += G.delta_time;

	int gotime = 30;
	if (_left_gamer.GetHealth() < MAX_HP / 3 || _right_gamer.GetHealth() < MAX_HP / 3)
		gotime = 50;
	if (_left_gamer.GetHealth() < 20 || _right_gamer.GetHealth() < 20)
		gotime = 200;

	if (_accum_time > gotime)
	{
		_accum_time -= gotime;
		if (!_result_queue.empty())
		{
			auto & pair = _result_queue.front();

			hand_t left = (hand_t)pair.first.hand;
			hand_t right = (hand_t)pair.second.hand;
			_left_gamer.SetHand(left);
			_right_gamer.SetHand(right);
			if (GetPoint(left, right) < 0)
			{
				_left_gamer.DecreaseHealth(1);
				_continuous_draw_count = 0;
			}
			else if (GetPoint(right, left) < 0)
			{
				_right_gamer.DecreaseHealth(1);
				_continuous_draw_count = 0;
			}
			else
				_continuous_draw_count++;

			if (pair.first.penalty) _left_gamer.DecreaseHealth(pair.first.penalty);
			if (pair.second.penalty) _right_gamer.DecreaseHealth(pair.second.penalty);
			
			if (pair.first.log.length() || pair.second.log.length())
			{
				_gamelog << L"--- Round" << _round << endl;
				if (pair.first.log.length())
				{
					_gamelog << L"-- "  << _left_gamer.GetText() << L" :" << endl;
					_gamelog << pair.first.log;
				}
				if (pair.second.log.length())
				{
					_gamelog << L"-- "  << _right_gamer.GetText() << L" :" << endl;
					_gamelog << pair.second.log;
				}
				_gamelog.flush();
			}
			_result_queue.pop();

			if (_left_gamer.IsDead() || _right_gamer.IsDead() ||
				_continuous_draw_count > MAX_CONTINUOUS_DRAW)
			{
				HandleGameResult();
			}
			else
				_round++;
		}
	}
}

void PlayScene::HandleNetwork()
{
}

void PlayScene::AddPlayer(const client_t & basic_info)
{
	ID id = basic_info.id;
	const Color & color = GetNextColor();
	_player_map.insert(id, player_t(basic_info.name, color));
}

void PlayScene::SyncPlayers()
{
	smap<ID, client_t> client_map;
	NetInterface::GetClients(&client_map);
	for (auto it = client_map.begin(); it != client_map.end(); ++it)
		AddPlayer((*it).element());
}

void PlayScene::SendGameOverAckPacket()
{
	Packet send_packet;
	send_packet << TO_UINT16(CL_TO_SV_GAME_OVER);
	SafeSend(send_packet);
}

void PlayScene::UpdatePlayerText()
{
	wstringstream ws;
	for (auto player : _player_map)
	{
		ws << player.element().name << endl;
	}
	_player_text.setString(ws.str());
}