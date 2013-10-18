/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	intro_scene.h

	IntroScene Ŭ������ ��� �����Դϴ�.
=======================================================*/

#pragma once

#include "scene.h"

class IntroScene : public Scene {
//	�� ó�� �ΰ� �����ִ� scene�Դϴ�.

public:
	IntroScene();
	~IntroScene();

	bool		HandleKeyPressed(Event::KeyEvent key);

	void		Go();

private:
	bool		_over;
	Sprite		_ooparts_logo;
	Sprite		_title_logo;
	Sprite *	_cur_logo;
	Sprite		_sfml_logo;
	Text		_sfml_prefix_text;
//	Sprite		_bg;

	enum state_t
	{
		OOPARTS_LOGO_STATE,
		TITLE_LOGO_STATE
	}			_state;

	int			_anim_count;
};