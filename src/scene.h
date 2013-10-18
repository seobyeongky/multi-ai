/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	scene.h

	�⺻ scene class�� header file�Դϴ�.
=======================================================*/

#pragma once

#include "internal.h"

class Scene {
/*	client program���� �߿��� ������ �ϴ� scene�� �⺻ abstract class�Դϴ�.
	client program�� FSM model�� ������� scene�� �����մϴ�.
	scene�� �ϳ��� state��� ������ �� �ֽ��ϴ�. */

public:
						 Scene() {}

	// IME ����...
	virtual bool		 HandleIMEUpdated(wchar_t code, bool done) {return false; }

	// ����ڰ� Text�� �Է����� �� main logic���� ȣ���մϴ�.
	virtual bool		 HandleTextEntered(wchar_t code) {return false; }

	// ����ڰ� Key�� ������ �� main logic���� ȣ��˴ϴ�.
	virtual bool		 HandleKeyPressed(Event::KeyEvent key) {return false; }

	// OPZNET client service���� �޽����� ���� �� ȣ��˴ϴ�.
	virtual bool		 HandleClientMsg(client_msg_t & msg) {return false;}

	// �� �����Ӹ��� ȣ��Ǵ� �Լ��Դϴ�.
	virtual void		 Go() = 0;

private:
						 Scene(const Scene &other);					// ���� ������
						 Scene & operator = (const Scene &other);	// ���� ������
};