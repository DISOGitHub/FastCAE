#ifndef WRITEDATACONFIG_H
#define WRITEDATACONFIG_H

#include "WriterBase.h"
#include <QDomDocument>

namespace ConfigOption{
	class PostCurve;
}

namespace FastCAEDesigner{
	class ModelBase;

	class WriteDataConfig : public WriterBase
	{
	public:
		WriteDataConfig();
		~WriteDataConfig();

		bool Write(QList<ModelBase*> list);
		QMap<QString, QString> insertQString(QStringList var, QStringList value);

	private:
		bool WriteTreeDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list, int number);
		bool WriteTreeChildPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list, int number);
		bool WriteMonitorDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list, int number);
		bool WritePostDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list, int number);
		bool WritePost2DDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list);
		bool WritePost3DDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list);
		bool WriteCurveDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list);
		bool writeTreeGrandSonPara(QDomDocument &doc, QDomElement &root, ModelBase* model, int number);

	private:
		int _index{ 1 };
	};
}


#endif