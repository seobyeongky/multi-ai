/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	name_query_scene.h

	QueryScene 클래스의 헤더 파일입니다.
=======================================================*/

#pragma once

#include "scene.h"

class QueryTextScene : public Scene {
public:
	typedef function<void(const wstring & text)> callback_t;

public:
				QueryTextScene(const wstring & message, callback_t next);
				~QueryTextScene();
		
	bool		HandleIMEUpdated(wchar_t code, bool done);

	bool		HandleTextEntered(wchar_t code);

	bool		HandleKeyPressed(Event::KeyEvent key);

	void		Go();

private:
	Sprite		_bg;
	Text		_query_label;
	EditText	_input_text;

	callback_t	_next;

	void		UpdateInputTextOrigin();
};

class QueryItemScene : public Scene {
public:
	typedef function<void(size_t index)> callback_t;

public:
					QueryItemScene(	const wstring & message,
									const vector<wstring> & items,
									callback_t next);
					~QueryItemScene();
	
//	bool			HandleIMEUpdated(wchar_t code, bool done);

//	bool			HandleTextEntered(wchar_t code);

	bool			HandleKeyPressed(Event::KeyEvent key);

	void			Go();

private:
	Sprite			_bg;
	Text			_query_label;
	Menu			_menu;

	callback_t		_next;	
};