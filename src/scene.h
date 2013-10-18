/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	scene.h

	기본 scene class의 header file입니다.
=======================================================*/

#pragma once

#include "internal.h"

class Scene {
/*	client program에서 중요한 역할을 하는 scene의 기본 abstract class입니다.
	client program은 FSM model을 기반으로 scene을 관리합니다.
	scene이 하나의 state라고 생각할 수 있습니다. */

public:
						 Scene() {}

	// IME 관련...
	virtual bool		 HandleIMEUpdated(wchar_t code, bool done) {return false; }

	// 사용자가 Text를 입력했을 때 main logic에서 호출합니다.
	virtual bool		 HandleTextEntered(wchar_t code) {return false; }

	// 사용자가 Key를 눌렀을 때 main logic에서 호출됩니다.
	virtual bool		 HandleKeyPressed(Event::KeyEvent key) {return false; }

	// OPZNET client service에서 메시지가 왔을 때 호출됩니다.
	virtual bool		 HandleClientMsg(client_msg_t & msg) {return false;}

	// 매 프레임마다 호출되는 함수입니다.
	virtual void		 Go() = 0;

private:
						 Scene(const Scene &other);					// 복사 방지용
						 Scene & operator = (const Scene &other);	// 복사 방지용
};