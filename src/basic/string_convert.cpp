#include "string_convert.h"

const size_t MY_BUFSIZE = 100 * 1024;
wchar_t mywbuf[MY_BUFSIZE];
char mybuf[MY_BUFSIZE];

void multi2uni(const std::string & src, std::wstring * dest)
{
	const char * srcptr = src.c_str();
	dest->clear();
	size_t converted;
	errno_t err = mbstowcs_s(&converted, mywbuf, srcptr, MY_BUFSIZE - 1);
	dest->assign(mywbuf);
}

void uni2multi(const std::wstring & src, std::string * dest)
{
	const wchar_t * srcptr = src.c_str();
	dest->clear();
	size_t converted;
	errno_t err = wcstombs_s(&converted, mybuf, srcptr, MY_BUFSIZE - 1);
	dest->assign(mybuf);
}
