#ifndef CREATECHILDMODELFACTORY_H
#define CREATECHILDMODELFACTORY_H

#include <QString>
#include <QObject>


namespace FastCAEDesigner
{
	class ModelBase;
}

namespace FastCAEDesigner
{
	class CreateChildModelFactory
	{
	public:
		static ModelBase* Create(int parentModelType, QString nameEng, QString nameChn, QString icon, QObject *parent);
		static ModelBase* Create(int parentModelType, QString fileName, QObject *parent);
	};
}

#endif

