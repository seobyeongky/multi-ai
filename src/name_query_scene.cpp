#include "name_query_scene.h"
#include "asset.h"
#include "director.h"
#include "global.h"

NameQueryScene::NameQueryScene() :
	_bg(*G.sprite_map[L"menu-bg"]),
	_query_label(L"이름을 입력해주세요", G.default_font, 45),
	_name(G.default_font, 40)
{
	{
		auto bounds = _bg.getLocalBounds();
		_bg.setScale(G.window.getSize().x/bounds.width,
			G.window.getSize().y/bounds.height);
	}

	{
		_query_label.setColor(Color(50, 50, 50));
		_query_label.setPosition(static_cast<float>(G.window.getSize().x/2),
			static_cast<float>(G.window.getSize().y/2) - 70.f);
		auto bounds = _query_label.getLocalBounds();
		_query_label.setOrigin(bounds.width/2, bounds.height/2);
	}

	{
		_name.setTextColor(Color::Black);
		_name.setCaretColor(Color::Black);
		_name.setPosition(static_cast<float>(G.window.getSize().x/2),
			static_cast<float>(G.window.getSize().y/2));
		UpdateNameOrigin();
	}
}

NameQueryScene::~NameQueryScene()
{
}

void NameQueryScene::Init()
{
}

bool NameQueryScene::HandleIMEUpdated(wchar_t code, bool done)
{
	if (_name.handleIMEUpdated(code, done))
		UpdateNameOrigin();
	return true;
}
	
bool NameQueryScene::HandleTextEntered(wchar_t code)
{
	if (_name.handleEnteredKey(code))
		UpdateNameOrigin();
	return true;
}

bool NameQueryScene::HandleKeyPressed(Event::KeyEvent key)
{
	if (_name.handleKeyPress(key))
		UpdateNameOrigin();
	else
	{
		if (key.code == Keyboard::Return)
		{
			G.my_name = _name.getString();
			Director::SwitchScene(MENU_SCENE);
		}
	}
	return true;
}

void NameQueryScene::Go()
{
	G.window.draw(_bg);
	G.window.draw(_query_label);
	G.window.draw(_name);
}

void NameQueryScene::UpdateNameOrigin()
{
	auto bounds = _name.getLocalBounds();
	_name.setOrigin(bounds.width/2, 0);
}