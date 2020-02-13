#ifndef CONFIGGATAREADER_H_
#define CONFIGGATAREADER_H_

#include "ConfigOptionsAPI.h"
#include <QString>

namespace ConfigOption
{
	class ConfigDataBase;

	class CONFIGOPTIONSAPI ConfigDataReader 
	{
	public:
		ConfigDataReader(QString path, ConfigDataBase* data);
		~ConfigDataReader() = default;

		QString read();

	private:
		QString _path{};
		ConfigDataBase* _data{};
	};


}


#endif // !CONFIGGATAREADER_H_
