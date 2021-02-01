#include "global.h"
#ifdef Q_OS_WIN
#include <windows.h>
#endif // Q_OS_WIN

namespace XMG{

	Global * Global::_instance = NULL;

	Global * Global::instance()
	{
		if (!_instance)
		{
			_instance = new Global();
		}
		return _instance;
	}

	Global::OS_Type Global::operatingSystem() const
	{
#ifdef Q_OS_WIN
		return OS_WIN;
#elif defined Q_OS_LINUX
		return OS_LINUX;
#elif defined Q_OS_IOS
		return OS_IOS;
#elif defined Q_OS_UNIX
		return OS_UNIX;
#elif defined Q_OS_ANDROID
		return OS_ANDROID;
#endif
	}

	QString Global::runCommand(const QString & cmd) const
	{
		char buf[2048] = { 0 };    //缓冲区
		DWORD len;
		HANDLE hRead, hWrite;    // 管道读写句柄
		STARTUPINFO si;
		PROCESS_INFORMATION pi;
		SECURITY_ATTRIBUTES sa;

		//ZeroMemory( buf, 2047 );
		sa.nLength = sizeof(sa);
		sa.bInheritHandle = TRUE;    // 管道句柄是可被继承的
		sa.lpSecurityDescriptor = NULL;

		// 创建匿名管道，管道句柄是可被继承的
		if (!CreatePipe(&hRead, &hWrite, &sa, 2048))
		{
			return QString("");
		}

		ZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		si.hStdError = hWrite;
		si.hStdOutput = hWrite;

		// 创建子进程,运行命令,子进程是可继承的
		if (!CreateProcess(NULL, (LPWSTR)cmd.toStdWString().c_str(), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi))
		{
			printf("创建进程失败!(%#x)\n", (unsigned int)GetLastError());
			CloseHandle(hRead);
			CloseHandle(hWrite);
			return QString("");
		}
		// 写端句柄已被继承,本地需要关闭,不然读管道时将被阻塞
		CloseHandle(hWrite);
		// 读管道内容,并显示
		QString result;
		while (ReadFile(hRead, buf, 2047, &len, NULL))
		{
			result.append(buf);
			ZeroMemory(buf, 2047);
		}
		CloseHandle(hRead);
		return result;
	}

}