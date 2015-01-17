#include "gamer.h"
#include "global.h"
#include "protocol.h"

LeftGamer::LeftGamer()
	: _text(L"", G.default_font),
	_text_str(),
	_hand(),
	_hp_text(),
	_hp(MAX_HP),
	_bar_bg(*G.sprite_map[L"bar-bg"]),
	_bar(*G.sprite_map[L"hbar-green"])
{
	_text.setPosition(30.f, 400.f);
	_text.setColor(Color::White);

	_rects[SCISSORS] = G.sprite_map[L"left-scissors"]->getTextureRect();
	_rects[ROCK] = G.sprite_map[L"left-rock"]->getTextureRect();
	_rects[PAPER] = G.sprite_map[L"left-paper"]->getTextureRect();

	_hand.setTexture(*G.texture_map[L"play.png"]);
	_hand.setPosition(0.f, 300.f);
	SetHand(static_cast<hand_t>(rand() % 3));


	_hp_text.setFont(G.default_font);
	_hp_text.setString(to_wstring(_hp));
	_hp_text.setPosition(330.f, 350.f);
	_hp_text.setColor(Color::White);

	_bar_bg.setPosition(325.f, 345.f);
	_bar_bg.setOrigin(0,0);
	_bar.setPosition(330.f, 350.f);
	_bar.setOrigin(0,0);
}

LeftGamer::~LeftGamer()
{
}

void LeftGamer::Init()
{
	_hp = MAX_HP;
	_hp_text.setString(to_wstring(_hp));
	_bar.setScale(1.f,1.f);
}

const wchar_t * LeftGamer::GetText() const 
{
	return _text_str.c_str();
}

void LeftGamer::SetText(const wstring & text)
{
	_text_str = text;
	_text.setString(text);
}

void LeftGamer::SetHand(hand_t next_hand)
{
	_hand.setTextureRect(_rects[next_hand]);
	_hand.setPosition(0.f, 300.f);
	_hand.setRotation(0.f);
}

void LeftGamer::DecreaseHealth(int delta)
{
	if (delta < 0) return;
	if (_hp > static_cast<size_t>(delta))
		_hp -= delta;
	else
		_hp = 0;

	_hp_text.setString(to_wstring(_hp));
	_bar.setScale((float)_hp/MAX_HP,1.f);
	if (_hp > 2 * MAX_HP / 3)
		_bar.setTextureRect(G.sprite_map[L"hbar-green"]->getTextureRect());
	else if (_hp > MAX_HP / 3)
		_bar.setTextureRect(G.sprite_map[L"hbar-orange"]->getTextureRect());
	else
		_bar.setTextureRect(G.sprite_map[L"hbar-red"]->getTextureRect());

	_hand.setRotation(-10.f);
	_hand.move(-10.f,0);
}

bool LeftGamer::IsDead() const
{
	return _hp == 0U;
}

void LeftGamer::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(_bar_bg, states);
	target.draw(_bar, states);
	target.draw(_hand, states);
	target.draw(_text, states);
//	target.draw(_hp_text, states);
}

///////////


RightGamer::RightGamer()
	: _text(L"", G.default_font),
	_text_str(),
	_hand(),
	_hp_text(),
	_hp(MAX_HP),
	_bar_bg(*G.sprite_map[L"bar-bg"]),
	_bar(*G.sprite_map[L"hbar-green"])
{
	_text.setPosition(400.f, 50.f);
	_text.setColor(Color::White);

  	_rects[SCISSORS] = G.sprite_map[L"right-scissors"]->getTextureRect();
	_rects[ROCK] = G.sprite_map[L"right-rock"]->getTextureRect();
	_rects[PAPER] = G.sprite_map[L"right-paper"]->getTextureRect();

	_hand.setTexture(*G.texture_map[L"play.png"]);
	_hand.setPosition(350.f, 0.f);
	SetHand(static_cast<hand_t>(rand() % 3));

	_hp_text.setFont(G.default_font);
	_hp_text.setString(to_wstring(_hp));
	_hp_text.setPosition(200.f, 100.f);
	_hp_text.setColor(Color::White);

	_bar_bg.setPosition(195.f, 95.f);
	_bar_bg.setOrigin(0,0);
	_bar.setPosition(200.f, 100.f);
	_bar.setOrigin(0,0);
}

RightGamer::~RightGamer()
{
}

void RightGamer::Init()
{
	_hp = MAX_HP;
	_hp_text.setString(to_wstring(_hp));
	_bar.setScale(1.f,1.f);
}

const wchar_t * RightGamer::GetText() const 
{
	return _text_str.c_str();
}

void RightGamer::SetText(const wstring & text)
{
	_text_str = text;
	_text.setString(text);
	auto & bounds = _text.getLocalBounds();
	_text.setOrigin(bounds.width, bounds.height / 2.f);
}

void RightGamer::SetHand(hand_t next_hand)
{
	_hand.setTextureRect(_rects[next_hand]);
	_hand.setPosition(350.f, 0.f);
	_hand.setRotation(0.f);
}

void RightGamer::DecreaseHealth(int delta)
{
	if (delta < 0) return;
	if (_hp > static_cast<size_t>(delta))
		_hp -= delta;
	else
		_hp = 0;

	_hp_text.setString(to_wstring(_hp));
	_bar.setScale((float)_hp/MAX_HP,1.f);
	if (_hp > 2 * MAX_HP / 3)
		_bar.setTextureRect(G.sprite_map[L"hbar-green"]->getTextureRect());
	else if (_hp > MAX_HP / 3)
		_bar.setTextureRect(G.sprite_map[L"hbar-orange"]->getTextureRect());
	else
		_bar.setTextureRect(G.sprite_map[L"hbar-red"]->getTextureRect());

	_hand.setRotation(10.f);
	_hand.move(10.f, 0);
}

bool RightGamer::IsDead() const
{
	return _hp == 0U;
}

void RightGamer::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(_bar_bg, states);
	target.draw(_bar, states);
	target.draw(_hand, states);
	target.draw(_text, states);
//	target.draw(_hp_text, states);
}
