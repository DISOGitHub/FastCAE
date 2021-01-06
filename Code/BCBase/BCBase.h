#ifndef _BCBASE_H_
#define _BCBASE_H_

#include "BCBaseAPI.h"
#include "DataProperty/DataBase.h"
#include "DataProperty/ComponentBase.h"
#include "BCType.h"

namespace MeshData
{
	class MeshData;
	class MeshSet;
}

namespace Geometry
{
	class GeoComponent;
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
		//绑定组件ID
		void bingdingComponentID(int id);
		//获取绑定组件ID
		int getComponentID();
		//获取绑定组件名称
		QString getComponentName();
		//获取绑定组件(可能是网格组件，也可能是几何组件，可以函数来获取类型)
		DataProperty::ComponentBase* getComponent();
		//设置绑定组件
		void setComponent(DataProperty::ComponentBase*);
		//设置边界条件类型
		void setBCType(BCType t);
		//获取边界条件
		BCType getBCType();

		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		virtual void readDataFromProjectFile(QDomElement* ele) override;

	protected:
		BCType _BCtype{ None };
		int _ComponentID{ -1 };

		DataProperty::ComponentBase* _component{};
		MeshData::MeshData* _mesh{};
	};
}

#endif 