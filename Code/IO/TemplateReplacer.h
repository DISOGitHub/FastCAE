#ifndef _TEMPLATEREPLACER_H_
#define _TEMPLATEREPLACER_H_

#include "IOAPI.h"
#include <QString>
#include <QStringList>

namespace DataProperty
{
	class DataBase;
}

namespace IO
{
	class IOAPI TempalteReplacer
	{
	public:
		TempalteReplacer(DataProperty::DataBase* data);
		~TempalteReplacer() = default;

		void appendFile(QString f);
		void replace();
	
	private:
		void replaceFile(QString s);
		void write(QString f);

	private:
		QStringList _fileNames{};
		DataProperty::DataBase* _data{};
		QStringList _text{};
	};
}



#endif