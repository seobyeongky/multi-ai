#include "util.h"

#include <io.h>
#include <locale.h>

void Msgbox(LPCWSTR format_string, va_list arg_list, LPCWSTR title)
{
	int length = _vscwprintf(format_string, arg_list) + 1;  
	WCHAR *buf = static_cast<WCHAR *>(
		_malloca(length * sizeof(WCHAR)));
	vswprintf_s(buf, length, format_string, arg_list);
	
//	LeaveLog(buf);
	MessageBox(NULL, buf, title, 0U);

	_freea(buf);
}

void ErrorMsg(LPCWSTR format_string, ...)
{
	va_list args;
	va_start(args, format_string);
	Msgbox(format_string, args, L"에러");
	va_end(args);
}

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

Color color_table[8] = {
	Color(168,8,8), Color(8, 52, 152), Color(32, 144, 112),
	Color(250, 250, 124), Color(0, 127, 127), Color(200, 162, 144),
	Color(64, 64, 64), Color(213, 197, 6)};

int color_stack_ptr = 0;

const Color & GetNextColor()
{
	++color_stack_ptr;
	return color_table[(color_stack_ptr - 1) % 8];
}

void GetScriptFileList(vector<wstring> * ptr)
{
	ptr->clear();

	_finddata_t fd;
    long handle;
    int result=1;
	int count = 0;
    handle=_findfirst("ai\\*.lua", &fd);
    if (handle == -1) return;

    while (result != -1)
	{
		WCHAR buf[512];
		size_t len = strlen(fd.name);
		swprintf_s(buf, L"%hs", fd.name, len);
		size_t wlen = wcslen(buf);
		wstring file_name(buf, buf + wlen);
		ptr->push_back(file_name);
		result=_findnext(handle, &fd);
     }
     _findclose(handle);
}

void GetScriptFromFile(const wstring & file_name, wstring * buf)
{
	buf->clear();

	wstring full_name = L"ai\\" + file_name;

	FILE * in = nullptr;
	_wfopen_s(&in, full_name.c_str(), L"r");
	if (in == nullptr) return;
	
	wchar_t BUF[1024];

	while (fgetws(BUF, sizeof(buf), in) != NULL)
	{
		*buf += BUF;
	}

	fclose(in);
}