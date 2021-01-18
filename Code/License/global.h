#pragma once

#include <QObject>

namespace XMG{

	class Global : public QObject
	{
	private:
		Global() = default;
		~Global() = default;

	public:

		typedef enum tag_OS_Type
		{
			OS_WIN = 0,
			OS_ANDROID,
			OS_IOS,
			OS_LINUX,
			OS_UNIX,
		}OS_Type;

	public:
		///<MG the instance of global
		static Global * instance();

		///<MG get the operating system type
		OS_Type operatingSystem() const;

		///<MG run cmd and get the result
		QString runCommand(const QString & cmd) const;
	private:
		static Global * _instance;
	};

}