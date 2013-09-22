#include "logger.h"

#include <chrono>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>

Logger::Logger(const wstring & file_name, WindowHandle h_wnd)
	: _f_stream(), _clock(), _h_wnd(h_wnd)
{
	_f_stream.imbue(locale("korean"));
	_f_stream.open(file_name, fstream::app);

	typedef chrono::system_clock clock_t;
	auto		now = clock_t::now();
	time_t		now_c = clock_t::to_time_t(now);
	struct tm	parts;
	localtime_s(&parts, &now_c);

	_clock.restart();

	_f_stream	<< L"=======================================================" << endl
				<< L"\t" << (1900 + parts.tm_year) << L"��"
				<< (1 + parts.tm_mon) << L"�� "
				<< parts.tm_mday << L"��\t"
				<< parts.tm_hour << L"�� "
				<< parts.tm_min << L"�� "
				<< parts.tm_sec << L"�ʺ���" << endl
				<< L"=======================================================" << endl;
}

Logger::~Logger()
{
	_f_stream.close();
}

void Logger::Info(const wstring & msg)
{
	float elapsed_time = _clock.getElapsedTime().asSeconds();

	_f_stream << elapsed_time << L"�� : " << msg << endl;
}

void Logger::Warning(const wstring & msg)
{
	float elapsed_time = _clock.getElapsedTime().asSeconds();

	_f_stream << elapsed_time << L"�� : [���]" << msg << endl;
}

void Logger::Error(const wstring & msg)
{
	float elapsed_time = _clock.getElapsedTime().asSeconds();

	_f_stream << elapsed_time << L"�� : [����]" << msg << endl;

	MessageBox(_h_wnd, msg.c_str(), L"����", MB_OK | MB_ICONERROR);
}