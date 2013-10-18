/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	sfx_msg.h

	ȿ������ ��������ݴϴ�.
=======================================================*/

#pragma once

#include "audiere.h"
#include "internal.h"

#define FULL_NAME_BUFFER_LENGTH	512
#define NAME_BUFFER_LENGTH		64

enum sfx_t
{
	SFX_NULL,
	SFX_LEAVE_CHAT,
	SFX_READY,
	SFX_WIN,
	SFX_ENTER,
	SFX_NUMBER
};

class opzSFXMgr
{
public:
			 opzSFXMgr(void);
			~opzSFXMgr(void);

	void	 Play(sfx_t sfx);

	void	 Update();		//����� �� �� outputstream�� �����ؾ� �Ѵ�.

private:
	LPCSTR	_fileNames[SFX_NUMBER];
	LPCSTR	_extNames[SFX_NUMBER];
	int		_fileNumbers[SFX_NUMBER];

	char	_fileFullName[512];
	LPSTR	_fileNamePtr;

	audiere::AudioDevicePtr _device;
	std::list<audiere::OutputStreamPtr> _streams;
};

