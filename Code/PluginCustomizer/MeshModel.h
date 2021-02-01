#ifndef MESHMODEL_H
#define MESHMODEL_H

#include "ModelBase.h"
#include "Common.h"

namespace FastCAEDesigner
{

	class MeshModel : public ModelBase
	{
		Q_OBJECT
	public:
		MeshModel(QString nameEng, QString nameChn, QString iconName, QObject *parent);
		~MeshModel();
	};
}

#endif