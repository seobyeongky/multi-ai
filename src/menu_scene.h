/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	menu_scene.h

	MenuScene 클래스의 헤더 파일입니다.
=======================================================*/

#pragma once

#include "scene.h"

class MenuScene : public Scene {
//	메뉴 scene입니다.

public:
	MenuScene();
	~MenuScene();

	void		Init();

	bool		HandleKeyPressed(Event::KeyEvent key);

	void		Go();

private:
	Sprite			_bg;
	Text			_host_text;
	Text			_join_text;
	Sprite			_select_rect;

	enum
	{
		HOST,
		JOIN
	}				_selected_mode;

	void SelectText(Text * item);
};