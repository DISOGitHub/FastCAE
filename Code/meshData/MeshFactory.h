#ifndef _MESHFACTORY_H__
#define _MESHFACTORY_H__

#include "meshSet.h"
#include <QHash>

typedef MeshData::MeshSet* (*CREATEMESHSET)(int);

namespace MeshData
{
	class MESHDATAAPI MeshFactory
	{
	public:
		static MeshSet* CreateMeshSet(int type);
		static void registerFunction(int type, CREATEMESHSET fun);
		static void remove(int type);


	private:
		static QHash<int, CREATEMESHSET> _createSetFun;
	};

}



#endif