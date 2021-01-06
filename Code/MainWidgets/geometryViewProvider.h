#ifndef GEOMETRYVIEWPROVIDER_H_
#define GEOMETRYVIEWPROVIDER_H_

#include <QObject>
#include <QMultiHash>
#include <QHash>
#include <QList>
#include "moduleBase/ModuleType.h"
#include <vtkPolyData.h>
#include <TopoDS.hxx>

class TopoDS_Shape;
class TopoDS_Edge;
class vtkActor;

namespace GUI
{
	class MainWindow;
}

namespace Geometry
{
	class GeometryData;
	class GeometrySet;
	class GeometryDatum;
}


namespace MainWidget
{
	class PreWindow;

	class GeometryViewProvider: public QObject
	{
		Q_OBJECT
	public:
		GeometryViewProvider(GUI::MainWindow* mainwindow, PreWindow* preWin);
		~GeometryViewProvider();

		void showShape(TopoDS_Shape& shape,Geometry::GeometrySet* set,bool render = true);
		void updateGeoActors();
		void updateGraphOption();
		void updateDiaplayStates(Geometry::GeometrySet* s, bool visibility);
		QMultiHash<Geometry::GeometrySet*, int>* getGeoSelectItems();
		void setGeoSelectItems(QMultiHash<Geometry::GeometrySet*, int> items);
		//
		bool  findSelectPolydata(vtkActor * ac, QVector<double*> points, vtkPolyData* &findPolydata);
		//
		bool findCurrentLinePolydata(vtkActor * ac, QVector<double*> points, double &minDis, vtkPolyData* &findPolyData);
		bool findCurrentPointPolydata(vtkActor * ac, QVector<double*> points, vtkPolyData*&findPolydata);
		bool findcurrentFacePolydata(vtkActor * ac, QVector<double*> points, vtkPolyData* &findPolydata);
		bool findcurrentSolidPolydata(vtkPolyData* facePolydata, vtkPolyData* &findPolydata);
		bool findcurrentBody(vtkActor *ac, vtkPolyData* &findPolydata);
		void clearPre_();
	public slots:
		void showGeoSet(Geometry::GeometrySet* set, bool render = true);
		void showDatum(Geometry::GeometryDatum* datm);
		void removeActors(Geometry::GeometrySet* set);
		void removeDatumActors(Geometry::GeometryDatum* plane);
		void setGeoSelectMode(int);
		void selectGeometry(vtkActor* ac, bool ctrlpress);
		void preSelectGeometry(vtkActor* ac, QVector<double*> points);
		void setGeometryDisplay( bool v, bool c, bool f);
		//void rightDownCreateMenu();
		void slotShowGeometryAll();
		//
		//void slotHideGeometry();
		//还原几何颜色。
		void RestoreGeoColor();
// 		//激活几何选取模式
// 		void activeSelectGeo(bool on);
// 		//关闭几何选取模式
// 		void closeSelectGeo(int geomodel);
	signals:
		void geoShapeSelected(Geometry::GeometrySet*shape, int index);
	private slots:
	    //高亮显示函数
	    void highLightGeometrySet(Geometry::GeometrySet* s, bool on);//高亮显示主体
		void highLightGeometrySolid(Geometry::GeometrySet* s, int id, bool on);//高亮显示实体
		vtkPolyData* getSolidPolyData(Geometry::GeometrySet* s, int id);
		void highLightGeometryPoint(Geometry::GeometrySet* s, int id, bool on);//高亮显示点
		vtkPolyData* getPointPolyData(Geometry::GeometrySet* s, int id);
		void highLightGeometryEdge(Geometry::GeometrySet* s, int id, bool on);//高亮显示边
		vtkPolyData* getEdgePolyData(Geometry::GeometrySet* s, int id);
		void highLightGeometryFace(Geometry::GeometrySet* s, int id, bool on);//高亮显示面
		vtkPolyData* getFacePolyData(Geometry::GeometrySet* s, int id);
		void clearGeometryHighLight();//清空高亮对象
		void clearAllHighLight();//清空所有高亮对象
		void clearSelectActors(Geometry::GeometrySet* s);//清空主体高亮选择的actor
		void showActor(vtkPolyData* pd, bool state);//添加或删除actor
		bool judgePreShowActor(vtkPolyData* pd);//判断预显示actor
		void hideGeometry(Geometry::GeometrySet* set, QList<vtkActor*> actors, int index);
	private:
		void init();
		void showVertex(Geometry::GeometrySet* set); 
		void showEdge(Geometry::GeometrySet* set);
		void showFace(Geometry::GeometrySet* set);
		void fillSolid(Geometry::GeometrySet* set);
		void showDatumPlane(Geometry::GeometryDatum* datum);
		void clearAllSet();
		void setPickable(QList<vtkActor*> acs, bool visibility);
		void removeAllSetActors(Geometry::GeometrySet* set, vtkActor* ac);

	private:
		PreWindow* _preWindow{};
		GUI::MainWindow* _mainWindow{};
		Geometry::GeometryData* _geoData{};
		//根据ctrlpress状态存放actor
	//	QList<vtkActor*> _addActors{};
		//
		QList<vtkActor*> _vertexActors{};
		QList<vtkActor*> _edgeActors{};
		QList<vtkActor*> _faceActors{};
		bool _showvertex{ false };
		bool _showedge{ false };
		bool _showface{ true };
//		bool _activeSeletGeo{ false };
		bool _hasShowed{ false };
		//放实体的
		QMultiHash<Geometry::GeometrySet*, vtkPolyData*>_solidPolyDatas{};
		//
		QMultiHash<Geometry::GeometrySet*, vtkActor*> _setActors{};
		QHash<vtkActor*, int> _actorShapeHash{};
		//新增集合
		QMultiHash<vtkActor*, vtkPolyData*> _actorPolydataHash{};//用于存储所有点，线，面actor对应的所有的单点，线，面集合
		QMultiHash<vtkPolyData*, Handle(TopoDS_TShape)> _polydataTShapeSetHash{};//用于存储实体对应的面的handle的数据集合
		QHash<vtkPolyData*, int> _polydataShapeIdHash{};//单点线面实体对应的id号
		QHash<vtkPolyData*, Handle(TopoDS_TShape)> _polydataTShapeHash{};//单面polydata对应的handle数据
		QHash<vtkPolyData*, vtkActor*> _selectPolydataActorHash{};//选中的actord的集合
		//预选择
		QPair<vtkPolyData*, vtkActor*>  _pre{};
		ModuleBase::SelectModel _selectType{ ModuleBase::None };
		QMultiHash<Geometry::GeometrySet*, int> _selectItems{};

	};

}

#endif
