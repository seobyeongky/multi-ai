#include "intro_scene.h"
#include "util.h"
#include "asset.h"
#include "director.h"
#include "global.h"
#include "query_scene.h"
#include "play_scene.h"
#include "sv\sv_service.h"
#include "net_interface.h"

IntroScene::IntroScene()
	: _over(false),
	_ooparts_logo(*G.sprite_map[L"ooparts"]),
	_title_logo(*G.sprite_map[L"title-logo"]),
	_cur_logo(nullptr),
	_sfml_logo(*G.sprite_map[L"sfml-logo"]),
	_sfml_prefix_text(L"powered by", G.default_font, 30U),
	_state(OOPARTS_LOGO_STATE),
	_anim_count(0)
{
	SetMiddleOfLocal(&_ooparts_logo);
	SetMiddleOfScreen(&_ooparts_logo);

	SetMiddleOfLocal(&_title_logo);
	SetMiddleOfScreen(&_title_logo);

	_cur_logo = &_ooparts_logo;

	_sfml_logo.setScale(0.5f, 0.5f);
	_sfml_logo.setPosition(470.f, 490.f);
	_sfml_prefix_text.setPosition(300.f, 500.f);
}

IntroScene::~IntroScene()
{
}

bool IntroScene::HandleKeyPressed(Event::KeyEvent key)
{
	if(_state == OOPARTS_LOGO_STATE)
	{
		_state = TITLE_LOGO_STATE;
		_cur_logo = &_title_logo;
	}
	else if(_state == TITLE_LOGO_STATE)
	{
		Director::SwitchScene(
			new QueryTextScene(L"이름이 뭐요?", [](const wstring & name)
			{
				G.sfx_mgr.Play(SFX_ENTER);
				wstring arr[] = {L"방 만들기", L"방 들어가기", L"종료"};
				vector<wstring> vec(arr, arr + sizeof(arr) / sizeof(arr[0]));
				Director::SwitchScene(
					new QueryItemScene(name + L"님 반갑습니다.", vec, [name](size_t index)
					{
						G.sfx_mgr.Play(SFX_ENTER);
						switch (index)
						{
						case 0: // 방 만들기
							Director::SwitchScene(
								new QueryTextScene(L"방 이름은 뭐요?", [name](const wstring & room_name)
								{
									G.sfx_mgr.Play(SFX_ENTER);
									if (BeginSvService(room_name))
									{
										if (!NetInterface::Connect(L"localhost", name))
										{
											ErrorMsg(L"기달");
										}

										NetInterface::RegisterConnectedCallback([name, room_name](const server_info_t & sv_info, ID my_id)
										{
											Director::SwitchScene(new PlayScene(name, room_name, true, my_id));
										});
										NetInterface::RegisterConnectFailedCallback([]()
										{
											ErrorMsg(L"연결 실패");
										});
									}
									else
									{
										G.logger->Error(L"방 만들기 실패");
									}
								}));
							break;
						case 1: // 방 들어가기
							Director::SwitchScene(
								new QueryTextScene(L"방의 IP주소를 치십시오.", [name](const wstring & addr)
								{
									G.sfx_mgr.Play(SFX_ENTER);
									if (!NetInterface::Connect(addr, name))
									{
										ErrorMsg(L"기달");
									}
									NetInterface::RegisterConnectedCallback([name](const server_info_t & sv_info, ID my_id)
									{
										Director::SwitchScene(new PlayScene(name, sv_info.name, false, my_id));
									});
									NetInterface::RegisterConnectFailedCallback([]()
									{
										ErrorMsg(L"연결 실패");
									});
								}));
							break;
						case 2: // 종료
							G.window.close();
							break;
						}
					}));
			}));
	}
	else G.logger->Error(L"IntroScene : 알 수 없는 상태");
	
	return true;
}

void IntroScene::Go()
{
	if(_anim_count > 100) _anim_count = 0;

	if(_anim_count < 50)
		_cur_logo->setScale(1.f + 0.01f * _anim_count, 1.f + 0.01f * _anim_count);
	else
		_cur_logo->setScale(2.f - 0.01f * _anim_count, 2.f - 0.01f * _anim_count);

	++_anim_count;

	G.window.draw(_sfml_logo);
	G.window.draw(_sfml_prefix_text);
	G.window.draw(*_cur_logo);
}