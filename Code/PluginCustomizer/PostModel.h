#ifndef POSTMODEL_H
#define POSTMODEL_H

#include "ModelBase.h"
#include "Common.h"

namespace FastCAEDesigner
{

	class PostModel : public ModelBase
	{
		Q_OBJECT
	public:
		PostModel(QString nameEng, QString nameChn, QString iconName, QObject *parent);
		~PostModel();
	};
}

#endif