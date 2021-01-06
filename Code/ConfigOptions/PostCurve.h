#ifndef _POSTCURVE_H_
#define _POSTCURVE_H_

#include "DataProperty/DataBase.h"
#include "ConfigOptions/ConfigOptionsAPI.h"
#include <QString>

namespace ConfigOption
{

	class CONFIGOPTIONSAPI PostCurve : public DataProperty::DataBase
	{
	public:
		PostCurve() = default;
		~PostCurve() = default;
		virtual void copy(DataBase* data) override;

		void setDescribe(QString d);
		QString getDescribe();
		void setXVariable(QString x);
		QString getXVariable();
		void setYVariable(QString y);
		QString getYVariable();
		void setFile(QString f);
		QString getFile();

	private:
		QString _describe{};
		QString _xVariable{};
		QString _yVariable{};
		QString _file{};

	};


}



#endif