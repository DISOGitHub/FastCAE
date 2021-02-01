#include "MeshFactory.h"
#include "meshSet.h"
#include "CgnsBCZone.h"
#include "CgnsFamily.h"

namespace MeshData
{
	QHash<int, CREATEMESHSET> MeshFactory::_createSetFun = QHash<int, CREATEMESHSET>();

	MeshData::MeshSet* MeshFactory::CreateMeshSet(int type)
	{
		SetType stype = (SetType)type;
		MeshSet* set = nullptr;
		switch (stype)
		{
		case MeshData::Node:
			set = new MeshSet("", Node); break;
		case MeshData::Element:
			set = new MeshSet("", Element); break; 
		case MeshData::Family:
			set = new CgnsFamily;  break;
		case MeshData::BCZone:
			set = new CgnsBCZone; break;
		default:
			CREATEMESHSET fun = _createSetFun.value(type);
			if (fun!= nullptr)
				set = fun(type);
			break;
		}
		return set;

	}

	void MeshFactory::registerFunction(int type, CREATEMESHSET fun)
	{
		_createSetFun.insert(type, fun);
	}

	void MeshFactory::remove(int type)
	{
		_createSetFun.remove(type);
	}

}