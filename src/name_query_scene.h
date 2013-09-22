/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	name_query_scene.h

	NameQueryScene 클래스의 헤더 파일입니다.
=======================================================*/

#pragma once

#include "scene.h"

class NameQueryScene : public Scene {
//	test용 scene입니다.

public:
				 NameQueryScene();
				~NameQueryScene();

	void		 Init();
	
	bool		 HandleIMEUpdated(wchar_t code, bool done);

	bool		 HandleTextEntered(wchar_t code);

	bool		 HandleKeyPressed(Event::KeyEvent key);

	void		 Go();

private:
	Sprite		_bg;
	Text		_query_label;
	EditText	_name;

	void		 UpdateNameOrigin();
};