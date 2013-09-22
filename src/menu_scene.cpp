#include "menu_scene.h"
#include "util.h"
#include "asset.h"
#include "global.h"

MenuScene::MenuScene() :
	_bg(*G.sprite_map[L"menu-bg"]),
	_host_text(L"방 만들기", G.default_font, 40U),
	_join_text(L"방 들어가기", G.default_font, 40U),
	_select_rect(*G.sprite_map[L"selected"]),
	_selected_mode(HOST)
{
	FitScaleToScreen(&_bg);

	_host_text.setColor(Color::Black);
	SetMiddleOfScreen(&_host_text);
	SetMiddleOfLocal(&_host_text);
	_host_text.move(0.f, -50.f);

	_join_text.setColor(Color::Black);
	SetMiddleOfScreen(&_join_text);
	SetMiddleOfLocal(&_join_text);
	_join_text.move(0.f, 50.f);

	SelectText(&_host_text);
}

MenuScene::~MenuScene()
{
}

void MenuScene::Init()
{
}

bool MenuScene::HandleKeyPressed(Event::KeyEvent key)
{
	if (key.code == Keyboard::Up)
	{
		_selected_mode = HOST;
		SelectText(&_host_text);
		return true;
	}
	else if (key.code == Keyboard::Down)
	{
		_selected_mode = JOIN;
		SelectText(&_join_text);
		return true;
	}
	else return false;
}

void MenuScene::Go()
{
	G.window.draw(_bg);
	G.window.draw(_host_text);
	G.window.draw(_join_text);
	G.window.draw(_select_rect);
}

void MenuScene::SelectText(Text * item)
{
	_select_rect.setPosition(item->getPosition());
	SetScaleToSize(&_select_rect,
		item->getLocalBounds().width + 10.f,
		item->getLocalBounds().height + 10.f);
	auto bounds = item->getLocalBounds();
	_select_rect.move(bounds.left, bounds.top);
	SetMiddleOfLocal(&_select_rect);
}