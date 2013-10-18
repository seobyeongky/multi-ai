#pragma once

#include "internal.h"

class IGamer : public Drawable
{
public:
	virtual void Init() = 0;
	virtual const wchar_t * GetText() const = 0;
	virtual void SetText(const wstring & text) = 0;
	virtual void SetHand(hand_t next_hand) = 0;
	virtual void DecreaseHealth() = 0;
	virtual bool IsDead() const = 0;
};

class LeftGamer : public IGamer
{	
public:
	LeftGamer();
	virtual ~LeftGamer();

	virtual void Init();
	virtual const wchar_t * GetText() const;
	virtual void SetText(const wstring & text);
	virtual void SetHand(hand_t next_hand);
	virtual void DecreaseHealth();
	virtual bool IsDead() const;

private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	Text	_text;
	wstring	_text_str;
	IntRect	_rects[3];
	Sprite	_hand;
	Text	_hp_text;
	size_t	_hp;
};

class RightGamer: public IGamer
{	
public:
	RightGamer();
	virtual ~RightGamer();

	virtual void Init();
	virtual const wchar_t * GetText() const;
	virtual void SetText(const wstring & text);
	virtual void SetHand(hand_t next_hand);
	virtual void DecreaseHealth();
	virtual bool IsDead() const;

private:
	virtual void draw(RenderTarget& target, RenderStates states) const;

private:
	Text	_text;
	wstring	_text_str;
	IntRect	_rects[3];
	Sprite	_hand;
	Text	_hp_text;
	size_t	_hp;
};