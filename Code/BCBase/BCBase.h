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
		//��������
		virtual void copy(DataBase* data) override;
		//�����ID
		void bingdingComponentID(int id);
		//��ȡ�����ID
		int getComponentID();
		//��ȡ���������
		QString getComponentName();
		//��ȡ�����(���������������Ҳ�����Ǽ�����������Ժ�������ȡ����)
		DataProperty::ComponentBase* getComponent();
		//���ð����
		void setComponent(DataProperty::ComponentBase*);
		//���ñ߽���������
		void setBCType(BCType t);
		//��ȡ�߽�����
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