#ifndef SETMEMBER_H_
#define SETMEMBER_H_

#include "meshDataAPI.h"
#include <vtkSmartPointer.h>
#include <vtkIdTypeArray.h>

class vtkDataSet;

namespace MeshData
{
	class MESHDATAAPI SetMember
	{
	public:
		SetMember() = default;
		~SetMember() = default;

		void setDataSet(int id);
		void setDataSet(vtkDataSet* data);
		void setIDList(vtkIdTypeArray *idarray);
		int getCount();
		int getMeshKernalID();
		vtkDataSet* getDataSet();
		int getDataSetID();
		vtkIdTypeArray* getIDList();
		QString getMeshKernalName();
	
		

	private:
		int _dateSetID{ -1 };
		vtkDataSet* _dateSet{};
		vtkSmartPointer<vtkIdTypeArray> _idList{};

		

	};
}


#endif