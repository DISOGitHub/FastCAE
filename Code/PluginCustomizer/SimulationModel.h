#ifndef SIMULATIONMODEL_H
#define SIMULATIONMODEL_H

#include "ModelBase.h"
#include "CustomParameterModel.h"
#include "Common.h"

namespace FastCAEDesigner
{
	class SimulationModel : public CustomParameterModel
	{
		Q_OBJECT
	public:
		SimulationModel(QString nameEng, QString nameChn, QString iconName, QObject *parent);
		~SimulationModel();
	};
}

#endif