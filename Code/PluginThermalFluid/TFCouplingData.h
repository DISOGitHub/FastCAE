#ifndef __TFPLUGINMODELDATA_H_
#define __TFPLUGINMODELDATA_H_

#include "ThermalFluidAPI.h"
#include "ModelData/modelDataBaseExtend.h"

#define TFCouplingType PluginDefType+410

class QDomDocument;

namespace MeshData
{
	class MeshSet;
}

namespace ThermalFluid
{
	class ThermalData;

	class THERMALFLUIDAPI TFCouplingModelData : public ModelData::ModelDataBaseExtend
	{

	public:
		TFCouplingModelData(ProjectTreeType t);
		~TFCouplingModelData();

		//����INPF�еĶ�����ȡmeshset
		static MeshData::MeshSet* getMeshSetByShortName(const QString& name);

		void setThermalData(ThermalData* td);
		ThermalData* getThermalData();
		QDomDocument* getDomDocument();
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* e);
		void readDataFromProjectFile(QDomElement* e);

	private:
		ThermalData* _thermalData;
	};
}


#endif