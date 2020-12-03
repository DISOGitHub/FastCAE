#include "TFRevMeshSet.h"
#include "meshData/meshSet.h"
#include "TFCouplingData.h"

namespace ThermalFluid
{

	TFRevMeshSet::TFRevMeshSet(QString name, MeshData::SetType type)
		:MeshData::MeshSet(name,type)
	{

	}


	TFRevMeshSet::TFRevMeshSet() : MeshData::MeshSet()
	{
		_type = MeshData::Element;
	}

	TFRevMeshSet::~TFRevMeshSet()
	{

	}

	void TFRevMeshSet::setEmissivity(double e)
	{
		_emissivity = e;
	}

	double TFRevMeshSet::getEmissivity()
	{
		return _emissivity;
	}

	void TFRevMeshSet::setReflectivity(double r)
	{
		_reflectivity = r;
	}

	double TFRevMeshSet::getReflectivity()
	{
		return _reflectivity;
	}

	void TFRevMeshSet::isRemov(bool re)
	{
		_remov = re;
	}

	bool TFRevMeshSet::isRemov()
	{
		return _remov;
	}

	void TFRevMeshSet::setCopy(QString c)
	{
		_copyGroupName = c;
	}

	QString TFRevMeshSet::getCopy()
	{
		return _copyGroupName;
	}

	vtkDataSet* TFRevMeshSet::getDisplayDataSet()
	{
		MeshData::MeshSet* s = TFCouplingModelData::getMeshSetByShortName(_copyGroupName);
		if (s == nullptr) return nullptr;
		return s->getDisplayDataSet();
	}

	void TFRevMeshSet::writeBinaryFile(QDataStream* dataStream)
	{
		*dataStream << (int)TFREVMESHSETTYPE<<_id<<_name;
		*dataStream << _remov << _copyGroupName << _emissivity << _reflectivity;
	}

	void TFRevMeshSet::readBinaryFile(QDataStream* dataStream)
	{
		int id{ 0 };
		QString name{};
		*dataStream >> id >> name;
		this->setID(id);
		this->setName(name);

		*dataStream >> _remov >> _copyGroupName >> _emissivity >> _reflectivity;

	}

}


