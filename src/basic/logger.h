/*OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS OOPARTS
=========================================================
	logger.h

	Logger class�� header file
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
/*	file�� ����� ����ϴ�. */

public:
			 Logger(const wstring & file_name, WindowHandle h_wnd);
			~Logger();

	// ���� �˸�������, file�� ����� ����ϴ�.
	void	 Info(const wstring & msg);

	// �������� file�� ����� ����ϴ�.
	void	 Warning(const wstring & msg);

	// ������ ���� �� �Դϴ�. file�� ����� �����, MessageBox�� �����ݴϴ�.
	void	 Error(const wstring & msg);

private:
	wofstream		_f_stream;
	Clock			_clock;

	WindowHandle	_h_wnd;
};