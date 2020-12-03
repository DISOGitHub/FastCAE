#ifndef _GEOCOMPONENT_H_
#define _GEOCOMPONENT_H_

#include "geometryAPI.h"
#include "DataProperty/ComponentBase.h"
#include <qhash.h>

class vtkPolyData;

namespace Geometry
{
	enum GeoComponentType
	{
		Node,
		Line,
		Surface,
		Body
	};

	class GeometrySet;

	class GEOMETRYAPI GeoComponent : public DataProperty::ComponentBase
	{
	public:
		GeoComponent(QString name, GeoComponentType type);
		~GeoComponent();
		//���ü����������
		void setType(GeoComponentType type);
		//��ȡ�����������
		GeoComponentType getGCType();
		//���ѡȡ����(ǰ�����ڴ���������Ϊ���Σ�ֵΪ�㡢�ߡ��桢���id)
		void appendSelectedItem(QMultiHash<GeometrySet*, int>& items);
		void appendSelectedItem(GeometrySet* geoSet, int itemIndex);
		void appendSelectedItem(GeometrySet* geoSet, QList<int>& itemIndexs);
		//����ѡȡ��
		void setSelectedItems(QMultiHash<GeometrySet*, int>& items);
		//��ȡѡȡ��
		QMultiHash<GeometrySet*, int>& getSelectedItems();
		//�ַ���ת��Ϊö��
		static GeoComponentType stringTogcType(QString stype);
		//ö��ת�ַ���
		static QString gcTypeToString(GeoComponentType type);
		//����д�빤���ļ�
		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* root);
		//�ӹ����ļ���������
		virtual void readDataFromProjectFile(QDomElement* root);

	private:
		GeoComponentType _type;
		QMultiHash<GeometrySet*, int> _selectedItems{};
	};
}

#endif
