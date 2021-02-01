#ifndef WRITERMATERIALCONFIG_H
#define WRITERMATERIALCONFIG_H

#include "WriterBase.h"
#include <QDomDocument>

namespace ConfigOption{
	class PostCurve;
}

namespace FastCAEDesigner{
	class ModelBase;

	class WriterMaterialConfig : public WriterBase
	{
	public:
		WriterMaterialConfig();
		~WriterMaterialConfig();

		bool Write(QList<ModelBase*> list);
		QMap<QString, QString> insertQString(QStringList var, QStringList value);

	private:
		bool WriteTreePara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> bcModelList);
		bool WriteTreeChildPara(QDomDocument &doc, QDomElement &root, ModelBase* model);

		//bool WriteTreeDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list, int number);
		//bool WriteTreeChildPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list, int number);
		//bool WriteMonitorDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list, int number);
		//bool WritePostDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list, int number);
		//bool WritePost2DDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list);
		//bool WritePost3DDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list);
		//bool WriteCurveDataPara(QDomDocument &doc, QDomElement &root, QList<ModelBase*> list);
	};
}


#endif