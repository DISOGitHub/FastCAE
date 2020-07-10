#ifndef PREWINDOWINTERACTORSTYLE_H
#define PREWINDOWINTERACTORSTYLE_H

//#include "mainWidgetsAPI.h"
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkDataSetMapper.h>
#include <QObject>
#include "moduleBase/ModuleType.h"
#include <vtkIdTypeArray.h>
#include <QMultiHash>
#include <QList>
#include "moduleBaseAPI.h"

//
#include <vtkSmartPointer.h>
#include <vtkPointData.h>
#include <vtkIdTypeArray.h>
#include <vtkDataSetSurfaceFilter.h>
#include <vtkRendererCollection.h>
#include <vtkProperty.h>
#include <vtkPlanes.h>
#include <vtkObjectFactory.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyData.h>
#include <vtkPointSource.h>
#include <vtkInteractorStyleRubberBandPick.h>
#include <vtkAreaPicker.h>
#include <vtkExtractGeometry.h>
#include <vtkDataSetMapper.h>
#include <vtkUnstructuredGrid.h>
#include <vtkVertexGlyphFilter.h>
#include <vtkIdFilter.h>

class vtkActor;
class vtkProperty;
class vtkDataSet;
class vtkIdTypeArray;
class vtkAreaPicker;
class vtkRenderWindow;
class vtkRenderer;
class QKeyEvent;
class QWidget;
class vtkCoordinate;

namespace GUI
{
	class MainWindow;
}
namespace DataProperty
{
	class DataBase;
	class PropertyList;
}

namespace MeshData
{
	class MeshData;
	class MeshKernal;
}

namespace ModuleBase
{
	class MODULEBASEAPI PropPickerInteractionStyle : public QObject, public vtkInteractorStyleRubberBandPick
	{
		Q_OBJECT
	public:
		static PropPickerInteractionStyle* New();
		vtkTypeMacro(PropPickerInteractionStyle, vtkInteractorStyleRubberBandPick);
		PropPickerInteractionStyle();
		~PropPickerInteractionStyle();
		//
		void SetPoints(vtkSmartPointer<vtkPolyData> points) { this->Points = points; }
		void connectToMainWindow(GUI::MainWindow* mw, QWidget* parent);
		void setRenderWindow(vtkRenderWindow* w);
		void setRender(vtkRenderer* r);
	private:
		vtkSmartPointer<vtkPolyData> Points;

		vtkSmartPointer<vtkActor> SelectedActor;

		vtkSmartPointer<vtkDataSetMapper> SelectedMapper;
	signals:
		void selectGeometry(vtkActor* actor, bool ctrlpress);
		void preSelectGeometry(vtkActor* actor, QVector<double*> points);
		void highLight(QMultiHash<vtkDataSet*, int>*);
		void dispalyInfo(DataProperty::PropertyList* prop);
		void higtLightActorDisplayPoint(bool on);
		void grabKeyBoard(bool g);
		void mouseReleasePoint(double*);
		void mouseMovePoint(double*);
		void mouseRightUp();
		void mouseMiddleUp();
		void mouseWhellMove();
		void rightDownMenu();
		//还原几何颜色
		void clearAllHighLight();

	private:
		virtual void OnLeftButtonDown() override;
		virtual void OnLeftButtonUp() override;
		virtual void OnMiddleButtonDown() override;
		virtual void OnMiddleButtonUp() override;
		virtual void OnRightButtonDown() override;
		virtual void OnRightButtonUp() override;
		virtual void OnMouseMove() override;
		virtual void OnMouseWheelForward() override;
		virtual void OnMouseWheelBackward() override;
		void OnKeyBoardUp();
		void OnKeyBoardDown();
		void displayNodeInfo(vtkDataSet* dataset, int id);

		bool isActorHightLighted(vtkActor* ac);

     public slots:
		void setSelectModel(int m);
		//type  0-press  1-release
		void keyEvent(int type, QKeyEvent* e);

	protected:
		void clickSelectGeometry();
		void clickSelectMeshNode();
		void clickSelectMeshCell();
		void boxSelectMeshNode();
		void boxSelectMeshCell();

		// xmin xmax ymin ymax
		void getBoxRange(int*);
		void selectMeshKernal(MeshData::MeshKernal* k, int *range);
		bool isPointInRange(int* p, int* range);

	private:
		vtkActor* _actor{};
		vtkProperty* _property{};
		GUI::MainWindow* _mainWindow{};
		QWidget* _parent{};
		vtkRenderWindow* _renderWindow{};
		vtkRenderer* _renderer{};
		SelectModel _selectModel{ SelectModel::None };
		//		vtkSmartPointer<vtkIdTypeArray> _selectItemIDs{};
		QMultiHash<vtkDataSet*, int> _selectItems{};
		bool _ctrlPressed{ false };
		bool _altPressed{ false };
		bool _mouseMoved{ false };
		bool _leftButtonDown{ false };
		DataProperty::DataBase* _dataProp{};
		MeshData::MeshData* _meshData{};

		int _startPos[2];
		int _endPos[2];
		bool _selected{ false };
		vtkActor* _preGeoSeltctActor{};
		vtkCoordinate* _coordinate{};

		QList<vtkActor*> _tempActorContainer{};
		QMultiHash<vtkActor*, double*> _actorPickedPoints{};

		int _currentTempIndex{ 0 };
	};
	//vtkStandardNewMacro(PropPickerInteractionStyle);
}

#endif
