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
		//��ԭ������ɫ��
		void RestoreGeoColor();
// 		//�����ѡȡģʽ
// 		void activeSelectGeo(bool on);
// 		//�رռ���ѡȡģʽ
// 		void closeSelectGeo(int geomodel);
	signals:
		void geoShapeSelected(Geometry::GeometrySet*shape, int index);
	private slots:
	    //������ʾ����
	    void highLightGeometrySet(Geometry::GeometrySet* s, bool on);//������ʾ����
		void highLightGeometrySolid(Geometry::GeometrySet* s, int id, bool on);//������ʾʵ��
		vtkPolyData* getSolidPolyData(Geometry::GeometrySet* s, int id);
		void highLightGeometryPoint(Geometry::GeometrySet* s, int id, bool on);//������ʾ��
		vtkPolyData* getPointPolyData(Geometry::GeometrySet* s, int id);
		void highLightGeometryEdge(Geometry::GeometrySet* s, int id, bool on);//������ʾ��
		vtkPolyData* getEdgePolyData(Geometry::GeometrySet* s, int id);
		void highLightGeometryFace(Geometry::GeometrySet* s, int id, bool on);//������ʾ��
		vtkPolyData* getFacePolyData(Geometry::GeometrySet* s, int id);
		void clearGeometryHighLight();//��ո�������
		void clearAllHighLight();//������и�������
		void clearSelectActors(Geometry::GeometrySet* s);//����������ѡ���actor
		void showActor(vtkPolyData* pd, bool state);//��ӻ�ɾ��actor
		bool judgePreShowActor(vtkPolyData* pd);//�ж�Ԥ��ʾactor
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
		//����ctrlpress״̬���actor
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
		//��ʵ���
		QMultiHash<Geometry::GeometrySet*, vtkPolyData*>_solidPolyDatas{};
		//
		QMultiHash<Geometry::GeometrySet*, vtkActor*> _setActors{};
		QHash<vtkActor*, int> _actorShapeHash{};
		//��������
		QMultiHash<vtkActor*, vtkPolyData*> _actorPolydataHash{};//���ڴ洢���е㣬�ߣ���actor��Ӧ�����еĵ��㣬�ߣ��漯��
		QMultiHash<vtkPolyData*, Handle(TopoDS_TShape)> _polydataTShapeSetHash{};//���ڴ洢ʵ���Ӧ�����handle�����ݼ���
		QHash<vtkPolyData*, int> _polydataShapeIdHash{};//��������ʵ���Ӧ��id��
		QHash<vtkPolyData*, Handle(TopoDS_TShape)> _polydataTShapeHash{};//����polydata��Ӧ��handle����
		QHash<vtkPolyData*, vtkActor*> _selectPolydataActorHash{};//ѡ�е�actord�ļ���
		//Ԥѡ��
		QPair<vtkPolyData*, vtkActor*>  _pre{};
		ModuleBase::SelectModel _selectType{ ModuleBase::None };
		QMultiHash<Geometry::GeometrySet*, int> _selectItems{};

	};

}

#endif
