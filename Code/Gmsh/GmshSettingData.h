#ifndef GMSHSETTINGDATA_H
#define GMSHSETTINGDATA_H

#include <QList>
#include <QMultiHash>
#include <QString>
#include "GmshModuleAPI.h"
#include "DataProperty/DataBase.h"

namespace Gmsh{
	class LocalField;
	class LocalPoint;
	class LocalDensity;

	class GMSHAPI GmshSettingData:public DataProperty::DataBase
	{
	public:
		GmshSettingData();
		~GmshSettingData();
		
		void copy(DataBase* data) override;

		//设置实体Hash
		void setSolidHash(QMultiHash<int, int> sh);
		//获取实体Hash
		QMultiHash<int, int> getSolidHash();
		//设置面剖分Hash
		void setSurfaceHash(QMultiHash<int, int> sh);
		//获取面剖分Hash
		QMultiHash<int, int> getSurfaceHash();
		//设置剖分类型
		void setElementType(QString type);
		//获取剖分类型
		QString getElementType();
		//设置剖分阶次
		void setElementOrder(int order);
		//获取剖分阶次
		int getElementOrder();
		//设置剖分方法
		void setMethod(int m);
		//获取剖分方法
		int getMethod();
		//设置尺寸因子
		void setSizeFactor(double sf);
		//获取尺寸因子
		double getSizeFactor();
		//设置最小尺寸
		void setMinSize(double min);
		//获取最小尺寸
		double getMinSize();
		//设置最大尺寸
		void setMaxSize(double max);
		//获取最大尺寸
		double getMaxSize();
		//设置网格清理
		void setGeoClean(bool c);
		//获取网格清理
		bool getGeoClean();
		//设置光滑度
		void setSmoothIteration(int smooth);
		//获取光滑度
		int getSmoothIteration();
		//设置网格连贯性
		void setGridCoplanar(bool gc);
		//获取网格连贯性 
		bool getGridCoplanar();
		//设置局部密度
		void setSizeAtPoints(QString ps);
		void setSizeFields(QString fs);
		void setLocalDesities(QList<LocalDensity*> list);
		//获取局部密度
		QList<LocalDensity*> getLocalDesities();
		//设置全选状态
		void setSelectAll(bool b);
		//获取全选状态
		bool getSelectAll();
		//设置选择可见项状态
		void setSelectVisiable(bool b);
		//获取选择可见项状态
		bool getSelectVisiable();
		//设置网格编号
		void setMeshID(int id);
		//获取网格编号
		int getMeshID();
		//设置指定单元类型
		void setCells(QList<int> cells);
		//获取指定单元类型
		QList<int> getCells();

	private:
		void appendBoxField(QStringList list);
		void appendBallField(QStringList list);
		void appendCylinderField(QStringList list);
		void appendSolidsField(QStringList list);
		void appendFrustumField(QStringList list);

	private:
		int _meshID{ -1 };
		QMultiHash<int, int> _solidHash{};
		QMultiHash<int, int> _surfaceHash{};
		QString _elementType{};
		int _elementOrder{ -1 };
		int _method{ -1 };
		double _sizeFactor{ 0.0 };
		double _minSize{ 0.0 };
		double _maxSize{ 0.0 };
		bool _geoclean{ false };
		int _smoothIteration{ 0 };
		bool _gridCoplanar{ false };
		bool _selectall{ false };
		bool _selectvisible{ false };

		QList<LocalDensity*> _locals{};
		QList<int> _cells{};
	};
}

#endif

