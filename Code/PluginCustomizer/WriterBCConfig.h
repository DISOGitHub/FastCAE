#ifndef WRITEBCCONFIG_H
#define WRITEBCCONFIG_H

#include "WriterBase.h"
#include <QDomDocument>

namespace FastCAEDesigner{
	class ModelBase;

	class WriteBCConfig : public WriterBase
	{
	public:
		WriteBCConfig();
		~WriteBCConfig();

		bool Write(QList<ModelBase*> bcModelList);
		QMap<QString, QString> insertQString(QStringList var, QStringList value);

	private:
		bool WriteTreePara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> bcModelList);
		bool WriteTreeChildPara(QDomDocument &doc, QDomElement &root, ModelBase* model);
	};
}


#endif