#include "gamer.h"
#include "global.h"
#include "protocol.h"

LeftGamer::LeftGamer()
	: _text(L"", G.default_font),
	_text_str(),
	_hand(),
	_hp_text(),
	_hp(MAX_HP)
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
}

LeftGamer::~LeftGamer()
{
}

void LeftGamer::Init()
{
	_hp = MAX_HP;
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
}

void LeftGamer::DecreaseHealth()
{
	if (_hp > 0U)
	{
		_hp--;
		_hp_text.setString(to_wstring(_hp));
	}
}

bool LeftGamer::IsDead() const
{
	return _hp == 0U;
}

void LeftGamer::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(_hand, states);
	target.draw(_text, states);
	target.draw(_hp_text, states);
}

///////////


RightGamer::RightGamer()
	: _text(L"", G.default_font),
	_text_str(),
	_hand(),
	_hp_text(),
	_hp(MAX_HP)
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
}

RightGamer::~RightGamer()
{
}

void RightGamer::Init()
{
	_hp = MAX_HP;
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
}

void RightGamer::DecreaseHealth()
{
	if (_hp > 0U)
	{
		_hp--;
		_hp_text.setString(to_wstring(_hp));
	}
}

bool RightGamer::IsDead() const
{
	return _hp == 0U;
}

void RightGamer::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(_hand, states);
	target.draw(_text, states);
	target.draw(_hp_text, states);
}
