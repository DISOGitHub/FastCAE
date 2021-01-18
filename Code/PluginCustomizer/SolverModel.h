#ifndef SOLVERMODEL_H
#define SOLVERMODEL_H

#include "ModelBase.h"
#include "CustomParameterModel.h"
#include "Common.h"

namespace FastCAEDesigner
{
	class SolverModel : public CustomParameterModel
	{
		Q_OBJECT
	public:
		SolverModel(QString nameEng, QString nameChn, QString iconName, QObject *parent);
		~SolverModel();
	};
}

#endif