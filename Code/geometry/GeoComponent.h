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
		//设置几何组件类型
		void setType(GeoComponentType type);
		//获取几何组件类型
		GeoComponentType getGCType();
		//添加选取的项(前处理窗口传进来，键为几何，值为点、线、面、体的id)
		void appendSelectedItem(QMultiHash<GeometrySet*, int>& items);
		void appendSelectedItem(GeometrySet* geoSet, int itemIndex);
		void appendSelectedItem(GeometrySet* geoSet, QList<int>& itemIndexs);
		//设置选取项
		void setSelectedItems(QMultiHash<GeometrySet*, int>& items);
		//获取选取项
		QMultiHash<GeometrySet*, int>& getSelectedItems();
		//字符串转化为枚举
		static GeoComponentType stringTogcType(QString stype);
		//枚举转字符串
		static QString gcTypeToString(GeoComponentType type);
		//数据写入工程文件
		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* root);
		//从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* root);

	private:
		GeoComponentType _type;
		QMultiHash<GeometrySet*, int> _selectedItems{};
	};
}

#endif
