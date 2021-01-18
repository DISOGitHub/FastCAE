#include "setMember.h"
#include "meshSingleton.h"
#include "meshKernal.h"

namespace MeshData
{
	void SetMember::setDataSet(vtkDataSet* dataset)
	{
		_dateSet = dataset;
		int id = MeshData::getInstance()->getIDByDataSet(dataset);
		_dateSetID = id;
	}
	void SetMember::setIDList(vtkIdTypeArray *idarray)
	{
		_idList = idarray;
		
	}
	void SetMember::setDataSet(int id)
	{
		if (id < 0) return;
		MeshKernal* k = MeshData::getInstance()->getKernalByID(id);
		if (k == nullptr) return;
		vtkDataSet* data = k->getMeshData();
		if (data == nullptr) return;
		_dateSet = data;
		_dateSetID = id;
	}
	int SetMember::getMeshKernalID()
	{
		return _dateSetID;
	}
	QString SetMember::getMeshKernalName()
	{
		QString name;
		MeshKernal* k = MeshData::getInstance()->getKernalByID(_dateSetID);
		if (k != nullptr)
			name = k->getName();
		return name;
	}
	int SetMember::getCount()
	{
		if (_idList == nullptr) return -1;
		int n = _idList->GetNumberOfValues();
		return n;
	}
	vtkDataSet* SetMember::getDataSet()
	{
		return _dateSet;
	}
	int SetMember::getDataSetID()
	{
		return _dateSetID;
	}
	vtkIdTypeArray* SetMember::getIDList()
	{
		return _idList;
	}

}