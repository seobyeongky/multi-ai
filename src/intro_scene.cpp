#include "intro_scene.h"
#include "util.h"
#include "asset.h"
#include "director.h"
#include "global.h"

IntroScene::IntroScene()
	: _over(false),
	_ooparts_logo(*G.sprite_map[L"ooparts"]),
	_title_logo(*G.sprite_map[L"title-logo"]),
	_cur_logo(nullptr),
	_sfml_logo(*G.sprite_map[L"sfml-logo"]),
	_sfml_prefix_text(L"powered by", G.default_font, 30U),
//	_bg(*G.sprite_map[L"pyramid"]),
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

//	FitScaleToScreen(&_bg);
}

IntroScene::~IntroScene()
{
}

void IntroScene::Init()
{	
	_state = OOPARTS_LOGO_STATE;
	_anim_count = 0;
}

bool IntroScene::HandleKeyPressed(Event::KeyEvent key)
{
	if(_state == OOPARTS_LOGO_STATE) { _state = TITLE_LOGO_STATE; _cur_logo = &_title_logo; }
	else if(_state == TITLE_LOGO_STATE) Director::SwitchScene(NAME_QUERY_SCENE);
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

//	G.window.draw(_bg);
	G.window.draw(_sfml_logo);
	G.window.draw(_sfml_prefix_text);
	G.window.draw(*_cur_logo);
}