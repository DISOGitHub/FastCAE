#ifndef _TFREVNESHSET_H__
#define _TFREVNESHSET_H__

#include "meshData/meshSet.h"
#include "ThermalFluidAPI.h"

#define TFREVMESHSETTYPE MeshData::SetType::UserDef+610

namespace ThermalFluid
{
	class TFRevMeshSet : public MeshData::MeshSet
	{
	public:
		TFRevMeshSet(QString name, MeshData::SetType type);
		TFRevMeshSet();
		~TFRevMeshSet();

		void setEmissivity(double e);
		double getEmissivity();

		void setReflectivity(double r);
		double getReflectivity();

		void isRemov(bool re);
		bool isRemov();

		void setCopy(QString c);
		QString getCopy();

	    vtkDataSet* getDisplayDataSet() override;
		//д���������ļ�
		virtual void writeBinaryFile(QDataStream* dataStream) override;
		//����������ļ�
		virtual void readBinaryFile(QDataStream* dataStream) override;

	private:
		bool _remov{ true }; //�ں�Ϊtrue
		QString _copyGroupName{};
		double _emissivity{ 0 };
		double  _reflectivity{ 0 };

	};

}


#endif