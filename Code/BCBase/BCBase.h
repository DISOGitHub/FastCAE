#ifndef _BCBASE_H_
#define _BCBASE_H_

#include "BCBaseAPI.h"
#include "DataProperty/DataBase.h"
#include "BCType.h"

namespace MeshData
{
	class MeshData;
	class MeshSet;
}

namespace BCBase
{


	class BCBASEAPI BCBase : public DataProperty::DataBase
	{
	public:
		BCBase();
		~BCBase() = default;
		//拷贝数据
		virtual void copy(DataBase* data) override;
		//设置网格组件ID
		void setMeshSetID(int id);
		//获取网格组件ID
		int getMeshSetID();
		//获取网格组件名称
		QString getMeshSetName();
		//获取网格组件
		MeshData::MeshSet* getMeshSet();
		//设置边界条件类型
		void setType(BCType t);
		//获取边界条件
		BCType getType();

		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		virtual void readDataFromProjectFile(QDomElement* ele) override;

	protected:
		BCType _type{ None };
		int _meshSetID{ -1 };
		 
		MeshData::MeshSet* _set{};
		MeshData::MeshData* _mesh{};

	};
}

#endif 