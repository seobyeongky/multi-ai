/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	logger.h

	Logger class의 header file
=======================================================*/

// STL
#include <string>
#include <fstream>

// SFML
#include <SFML\System.hpp>
#include <SFML\Window.hpp>

using namespace std;
using namespace sf;

class Logger {
/*	file에 기록을 남깁니다. */

public:
			 Logger(const wstring & file_name, WindowHandle h_wnd);
			~Logger();

	// 정보 알림용으로, file에 기록을 남깁니다.
	void	 Info(const wstring & msg);

	// 경고용으로 file에 기록을 남깁니다.
	void	 Warning(const wstring & msg);

	// 오류가 났을 때 입니다. file에 기록을 남기고, MessageBox로 보여줍니다.
	void	 Error(const wstring & msg);

private:
	wofstream		_f_stream;
	Clock			_clock;

	WindowHandle	_h_wnd;
};