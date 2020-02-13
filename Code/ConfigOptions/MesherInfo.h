#ifndef _MESHERINFO_H_
#define _MESHERINFO_H_

#include "ConfigOptionsAPI.h"
#include "DataProperty/DataBase.h"
#include <QString>

namespace ConfigOption
{

	class CONFIGOPTIONSAPI MesherInfo: public DataProperty::DataBase
	{
	public:
		MesherInfo();
		~MesherInfo() = default;

		QString getMesherName();
		QString getTemplate();
		QString getOutput();
		void readParameters(QDomElement* ele) override;
		void writeParameters(QDomDocument* doc, QDomElement* parent) override;


	private:

	private:
		QString _mesherName{};
		QString _template{};
		QString _output{};

	};

}



#endif