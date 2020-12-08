#ifndef GEOMETRYSET_H
#define GEOMETRYSET_H

#include "geometryAPI.h"
#include "DataProperty/DataBase.h"
#include <QString>
//#include <TopoDS_Shape.hxx>
//#include <vtkSmartPointer.h>

class TopoDS_Shape;
class vtkDataSet;

namespace Geometry
{
	enum GeometryType
	{
		NONE = 0,
		IGES,
		STEP,
		STL,
		DATUM, 
	};

	class GeometryModelParaBase;

	class GEOMETRYAPI GeometrySet : public DataProperty::DataBase
	{
	public:
		//构造函数
		GeometrySet(GeometryType type = NONE, bool needID = true);
		~GeometrySet();
		//删除全部子形状并释放子形状内存
		void releaseSubSet();
		void setID(int id) override;
		//重置最大ID，不要轻易调用
		static void resetMaxID();
		//根据ID获取子形状
		GeometrySet* getSetByID(int id);
// 		int getID();
// 		void setName(const QString& name);
// 		QString getName();
// 		void setFilePath(const QString& filepath);
// 		QString getFilePath();

		//获取点线面数量 1-点 2-线 3-面 4-实体，错误返回值：-1
		int getGeoMemberCount(int type);
		//获取第index个形状 type 1-点 2-线 3-面 4-实体，
		TopoDS_Shape* getShape(int type, int index);
		////获取第index个形状 type 1-点 2-线 3-面 4-实体，
		const TopoDS_Shape& getRealShape(int type, int index);
		//设置可见性
		void setVisible(bool v);
		//形状是否可见
		bool isVisible();
		//设置类型
		void setType(GeometryType type);
		//获取类型
		GeometryType getType();
		//设置形状拓扑
		void setShape(TopoDS_Shape* shape);
		//获取形状拓扑
		TopoDS_Shape* getShape();
// 		void setStlDataSet(vtkSmartPointer<vtkDataSet> polyData);
// 		vtkDataSet* getStlDataSet();
		//获取最大ID
		static int getMaxID();
		//设置操作参数
		void setParameter(GeometryModelParaBase* p);
		//获取操作参数
		GeometryModelParaBase* getParameter();
		bool isEditable();
		//移除子形状
		void removeSubSet(GeometrySet* set);
		//添加字形状
		void appendSubSet(GeometrySet* set);
		//获取子形状数目
		int getSubSetCount();
		//获取第index个子形状
		GeometrySet* getSubSetAt(int index);

		void dataToStream(QDataStream* s) override;
		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* ele, bool isDisp = false);
		virtual void readDataFromProjectFile(QDomElement* e, bool isDiso = false);
		//写出brep文件，路径不能出现中文
		bool writeBrep(QString name);
		//读入brep文件，路径不能出现中文
		bool readBrep(QString name);

	protected:
		void writeSubSet(QDomDocument* doc, QDomElement* parent, bool isDiso = false);
		void readSubSet(QDomElement* e, bool isDiso = false);

	protected:
//		int _id{ -1 };
		GeometryType _type{ NONE };
//		QString _name{};
//		QString _filePath{};
		bool _visible{ true };
		TopoDS_Shape* _shape{};
//		vtkSmartPointer<vtkDataSet> _polyData{};
		QList<GeometrySet*> _subSetList{};
		GeometryModelParaBase* _parameter{};

	private:
		static int idOffset;
		static TopoDS_Shape* tempShape;
	};
}

#endif
