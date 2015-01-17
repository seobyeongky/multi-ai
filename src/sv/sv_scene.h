#pragma once

#include "sv_internal.h"

class SvScene {
public:
	// member �������� �ʱ�ȭ��Ű�� �Լ��Դϴ�.
	// derived scene�� instance�� ����Ǳ� ������ �������Լ��� �ƴ� Init�Լ��� �ʱ�ȭ��Ű�ʽÿ�.
	virtual void		Init() = 0;

	// ���ο� client�� ������ ���Ŀ� ȣ��˴ϴ�.
	// server context���� �� client�� ���� ���� �����Դϴ�.
	virtual void		HandleNewCl(const client_t & new_cl) = 0;

	// � client�� �������� ���� �ڿ� ȣ��˴ϴ�.
	// server context���� ���� client�� ���� �ִ� �����Դϴ�.
	virtual void		HandleGoneCl(const client_t & gone_cl) = 0;


	virtual void		HandleRecvPacket(	const client_t & cl,
											cl_to_sv_t header,
											Packet & recv_packet);

	virtual void		Go() = 0;
};