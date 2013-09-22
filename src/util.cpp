#include "util.h"

DirChanger::DirChanger()
	: _pre_dir()
{
	Init();
}

DirChanger::DirChanger(const wchar_t * new_dir)
	: _pre_dir()
{
	Init();
	Change(new_dir);
}

DirChanger::~DirChanger()
{
	Change(_pre_dir.c_str());
}

void DirChanger::Change(const wchar_t * new_dir)
{
	_wchdir(new_dir);
}

void DirChanger::Init()
{
	size_t			buf_size = 128;
	vector<wchar_t>	buf(buf_size);
	
	for(;;)
	{
		_wgetcwd(&buf[0], buf_size);
		if(buf[0] != L'\0')
			break;
		buf_size *= 2;
		if(buf_size > 1024)
			G.logger->Error(L"DirChanger : working directory의 경로 이름 길이가 너무 깁니다.");
		buf.resize(buf_size);
	}
	_pre_dir.assign(&buf[0], buf.size());
}