#ifndef PREWINDOW_H
#define PREWINDOW_H

#include "mainWidgetsAPI.h"
#include "moduleBase/graph3DWindow.h"
#include "moduleBase/ModuleType.h"

namespace MeshData
{
	class MeshData;
}
namespace Geometry
{
	class GeometryData;
	class GeometrySet;
	class GeometryDatum;
	class GeoComponent;
}

namespace ModelData
{
	class ModelDataSingleton;
}

namespace GUI
{
	class MainWindow;
}

class vtkActor;
class TopoDS_Shape;

namespace MainWidget
{
	class GeometryViewProvider;
	class SketchViewProvider;
	class MeshViewProvider;

	class MAINWIDGETSAPI PreWindow :public ModuleBase::Graph3DWindow
	{
		Q_OBJECT
	public:
		PreWindow(GUI::MainWindow* mw, int id = -1, ModuleBase::GraphWindowType type = ModuleBase::GraphWindowType::PreWindows);
		~PreWindow();
		
		Geometry::GeometrySet* getSelectedGeoSet();
		//��ȡѡ��ļ���
		QMultiHash<Geometry::GeometrySet*, int>* getGeoSelectItems();
		//�����Ѿ�ѡ��ļ���
		ModuleBase::SelectModel getSelectModel();
		QMultiHash<vtkDataSet*, int>* getSelectItems();

		MeshViewProvider* getMeshViewProvider();
	signals:
		//�ر�
		void closed();
		//��ʾ������״
		void showGeoSet(Geometry::GeometrySet* set, bool render = true);
		//�Ƴ�������״����ʾ
		void removeGemoActors(Geometry::GeometrySet* set);
		//��ʾ��׼
		void showDatum(Geometry::GeometryDatum*);
		//�Ƴ����λ�׼��ʾ
		void removeGeoDatumActors(Geometry::GeometryDatum*);
		//����ѡ��ģʽ
		void setGeoSelectMode(int);
		//ѡ��ļ���Ԫ��
		void selectGeoActorShape(vtkActor* ac, int shape, Geometry::GeometrySet* set);
		//�����ӿ�
		void geoShapeSelected(Geometry::GeometrySet*shape, int index);//��ѡ�нӿ�
		void highLightGeometrySet(Geometry::GeometrySet* s, bool on);//������ʾ����
		void highLightGeometryPoint(Geometry::GeometrySet* s, int id, bool on);//������ʾ��
		void highLightGeometryEdge(Geometry::GeometrySet* s, int id, bool on);//������ʾ��
		void highLightGeometryFace(Geometry::GeometrySet* s, int id, bool on);//������ʾ��
		void highLightGeometrySolid(Geometry::GeometrySet* s, int id, bool on);//������ʾʵ��
		void clearGeometryHighLight();//������и�������
		void removeSetDataSig(const int index);
		//����������ȾԪ��
		void updateMeshActorSig();
		void highLighKernel(MeshData::MeshKernal* k);
		void highLighMeshSet(MeshData::MeshSet* set);
		void setMeshSelectMode(int model);
		void highLighDataSet(vtkDataSet* dataset);
		void clearMeshSetHighLight();		
		
		void highLightGeoComponentSig(Geometry::GeoComponent* aGC);
		
	public slots:
	    //����ѡ��ģʽ
	    void setSelectModel(int mode) override;
		void setGeoSelectItems(QMultiHash<Geometry::GeometrySet*, int> items);
		//���ò�ͼ����
		void setSketchType(ModuleBase::SketchType t);
		//ѡ����������
		void setDisplay(QString m) override;
		//����������Ⱦ
		void updateMeshActor();
		//
		void highLighSet(QMultiHash<vtkDataSet*, int>* items);
		//���¼�����Ⱦ
		void updateGeometryActor();
		//����һ���������;
		void highLightGeoComponentSlot(Geometry::GeoComponent*);
		
	private slots:
		void updateGraphOption() override;
		//����
		void updateGeoDispaly(int index, bool display);
		void removeGemoActor(const int index);
		//����
		void updateMeshDispaly(int index, bool display);
		void removeMeshActor(const int index);
		//��ͼ
		void startSketch(bool start, double* loc, double* dir);

	private:
		//QList<vtkActor*> _meshActors{};
		//QList<vtkActor*> _geometryActors{};
		ModuleBase::SelectModel _selectModel;
		QMultiHash<vtkDataSet*, int>* _selectItems{};
		//
		MeshData::MeshData* _meshData{};
		Geometry::GeometryData* _geometryData{};

		ModelData::ModelDataSingleton* _modelData{};
		int _selectedGeoIndex{ -1 };

		GeometryViewProvider* _geoProvider{};
		MeshViewProvider * _meshProvider{};
		SketchViewProvider* _sketchProvider{};

	};
}

#endif
