#ifndef GMSHSCRIPTWRITER_H
#define GMSHSCRIPTWRITER_H

#include <QString>
#include <QTextStream>
#include <QMultiHash>

class TopoDS_Compound;

namespace Gmsh{
	class GmshScriptWriter
	{
	public:
		GmshScriptWriter();
		~GmshScriptWriter();

		//写gmsh脚本
		void writeGmshScript(QString path);
		//流体域网格剖分
		void writeFluidMeshScript(QString dir, QMultiHash<int,int> solid,QList<int> curve, QList<int> surface);
		
		//设置组合体
		void setCompound(TopoDS_Compound* compound);
		//设置网格剖分类型
		void setElementType(QString type);
		//设置网格剖分阶次
		void setElementOrder(int order);
		//设置网格剖分方法
		void setMethod(int method);
		//设置网格剖分尺寸因子
		void setFactor(int factor);
		//设置最小尺寸
		void setMinSize(double min);
		//设置最大尺寸
		void setMaxSize(double max);
		//设置光滑系数
		void setSmooth(int smooth);
		//几何清理
		void setGeoClean(bool clean);
		//网格连贯性
		void setGridCoplanar(bool cop);
		//设置局部密度-点
		void setSizePoints(QString points);
		//设置局部密度-区域
		void setSizeFields(QString fields);
		//设置流体域范围
		//void setFluidField(QList<double*> field);

	private:
		//gmsh基本设置
		void writeGeneralSetting(QTextStream* out);
		//gmsh网格连贯性
		void writeGridCoplanar(QTextStream* out);
		//局部密度---点
		void writeSizeAtPoints(QTextStream* out);
		//局部密度---区域
		void writeSizeFields(QTextStream* out);
		//局部密度---立方体
		void writeBoxFieldScript(QTextStream* out, QStringList list, int& index);
		//局部密度---球
		void writeBallFieldScript(QTextStream* out, QStringList list, int& index);
		//局部密度---圆柱
		void writeCylinderFieldScript(QTextStream* out, QStringList list, int& index);
		//局部密度---实体
		void writeSolidsFieldScript(QTextStream* out, QStringList list, int& index);
		void writeSolidFieldScript(QTextStream* out, int& index, QMultiHash<int, int> solidHash, double* val, bool back);
		//去除点线面体
		void writeSpecifiedMeshScript(QTextStream* out);
		
		//流体域网格剖分
		void writeFluidFieldScript(QTextStream* out, QList<double> banbox, QList<int> curve, QList<int> surface);


		//	void physicalsGroup(QTextStream* out);
		//	void physicalsScript(QTextStream* out,QString type,QMultiHash<QString,int> pHash);

		//获取在新组合体的索引,返回值大于0， 若返回0则为错误标记
		//type 1-点 2-线 3-面 4-实体
		int getShapeIndexInCompound(int setid, int index, int itype);
		QList<int> getShapeIndexListInSolid(QMultiHash<int, int> setHash, int itype);
		//获取实体包围盒
		//返回值 Xmin Xmax Ymin Ymax Zmin Zmax
		//错误值 链表为空
		QList<double> getSolidBndBox(QMultiHash<int, int> setHash);

		double getFluidMeshSize(double xl, double yl, double zl);
		double getShrinkFactor(double xl, double yl, double zl);

	private:
		int _dim{ -1 };
	
		TopoDS_Compound* _compound{};
		int _elementType{ 0 };
		int _elementOrder{ -1 };
		int _method{ -1 };
		double _sizeFactor{ 0.0 };
		double _minSize{ 0.0 };
		double _maxSize{ 0.0 };
		bool _geoclean{ false };
		int _smoothIteration{ 0 };
		bool _isGridCoplanar{ false };
		QString _sizeAtPoints{};
		QString _sizeFields{};
		//QString _physicals{};

		//QList<double*> _fluidField{};
	};
}
#endif // GMSHSCRIPTWRITER_H
