#ifndef WRITETREECONFIG_H
#define WRITETREECONFIG_H

#include "WriterBase.h"

namespace FastCAEDesigner{
	class ModelBase;

	class WriteTreeConfig : public WriterBase
	{
	public:
		WriteTreeConfig();
		~WriteTreeConfig();

		bool Write(QList<ModelBase*> list);
		QMap<QString, QString> insertQString(QStringList var, QStringList value);

	private:	
		bool WriteTreePara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list);
		bool WriteTreeChildPara(QDomDocument &doc, QDomElement &root, ModelBase* model);
		bool wirteSimulationAndSolverGrandSon(QDomDocument &doc, QDomElement &root, ModelBase* model);

	private:
		int _index{ 1 };
	};
}

#endif

