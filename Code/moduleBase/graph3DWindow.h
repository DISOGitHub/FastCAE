#ifndef _GRAPH3DWINDOW_H_
#define _GRAPH3DWINDOW_H_

#include <QWidget>
#include <vtkSmartPointer.h>
#include <vtkAutoInit.h>
#include "moduleBaseAPI.h"
#include "graphWindowBase.h"
#include "ModuleType.h"
#include <QString>
#include <QStringList>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
//VTK_MODULE_INIT(vtkRenderingContextOpenGL2);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
//VTK_MODULE_INIT(vtkRenderingGL2PSOpenGL2);
//VTK_MODULE_INIT(vtkIOExportOpenGL2);

class vtkActor;
class vtkDataSetMapper;
class vtkRenderer;
class vtkRenderWindow;
class vtkRenderWindowInteractor;
class vtkOrientationMarkerWidget;
class vtkTextWidget;
class vtkProp;
class vtkScalarBarWidget;
class vtkLookupTable;
class vtkDataSet;
class vtkPolyData;
class vtkIdTypeArray;
class vtkCaptionWidget;


namespace GUI
{
	class MainWindow;
}

namespace Ui
{
	class Graph3DWindow;
}

namespace MeshData
{
	class MeshSet;
	class MeshKernal;
}

namespace ModuleBase
{
	enum ActorType
	{
		D2,
		D3,
	};

	class PropPickerInteractionStyle;

	class MODULEBASEAPI Graph3DWindow : /*public QWidget,*/ public GraphWindowBase
	{
		Q_OBJECT
	public:
		Graph3DWindow(GUI::MainWindow* mainwindow, int id, GraphWindowType type,bool connectToMainwindow = false);
		virtual ~Graph3DWindow() =0 ;
		//�����Ⱦ����
		void AppendActor(vtkProp* actor, ActorType type = ActorType::D3, bool reRender = true, bool reset = true);
		//�Ƴ���Ⱦ����
		void RemoveActor(vtkProp* actor);
		//����/������Ⱦ��һ��
		void enableActor(vtkActor* actor, bool show = true);
		void saveImage(QString fileName, int w, int h, bool showdlg) override;
		void setViewValue(int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3) override;
		void updateScalarBar(vtkLookupTable* lookuptable, QString title = QString(""));
		void updateScalarBarLecel(const int n);
		int getScalarBarLevel();
		//�����ӽ�
		void resetCamera();
		//���������ػ�
		void updateGraphOption() override;
		void reTranslate() override;
		void addCaption(double* pos, QString cap);
		//��ȡ������
		PropPickerInteractionStyle* getInteractionStyle();
		//��ȡ��Ⱦ��
		vtkRenderer* getRenderer();
		//��ȡѡ��ģʽ
		SelectModel getSelectModel();
		//��ȡ������������ϵ�¸߶�
		double getWorldHight();
		//��ȡ������������ϵ�¿��
		double getWorldWidth();

	public slots:
	     //�ػ�
		void reRender();

	protected slots :
		 void setSelectType(int model);
	    void enableKeyBoard(bool on);
		void mouseWheelMove();
	

	private:
		void init();
		void initAxes();
		void initText();
		void initScalarBar();
		
	private:
		void fitView() override;
		void setViewXPlus() override;
		void setViewXMiuns() override;
		void setViewYPlus() override;
		void setViewYMiuns() override;
		void setViewZPlus() override;
		void setViewZMiuns() override;
		//void setViewValue(int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3) override;

		void keyPressEvent(QKeyEvent *e) override;
		void keyReleaseEvent(QKeyEvent *e) override;
		void resizeEvent(QResizeEvent * e) override;
		void leaveEvent(QEvent *event) override;
		void enterEvent(QEvent *event) override;
// 
	signals:
		//type  0-press  1-release
		void keyEvent(int type, QKeyEvent* e);
		void selectGeometry(vtkActor* actor, bool ctrlpress);
		void preSelectGeometry(vtkActor* actor, QVector<double*> points);
		void reRenderSig();
		void clearAllHighLight();
		//��ԭ������ɫ
		void rightDownMenuSig();
		//����
		void highLighSet(QMultiHash<vtkDataSet*, int>* items);
		void highLightActorDispalyPoint(bool on);

	protected:
		Ui::Graph3DWindow* _ui{};
//		QStringList m_ltFilePath = QStringList();
	
		vtkSmartPointer<vtkRenderer> _render{};
		vtkSmartPointer<vtkRenderWindow> _renderWindow{};
		vtkSmartPointer< vtkRenderWindowInteractor > _interactor{};

		vtkSmartPointer<vtkOrientationMarkerWidget> _axesWidget{};
		vtkSmartPointer<vtkTextWidget> _textWidget{};
		vtkSmartPointer<vtkScalarBarWidget> _scalarBarWidget{};
		vtkLookupTable* _lookupTable{};
		QList<vtkSmartPointer<vtkCaptionWidget>> _captionList{};

		//ʰȡ���
		PropPickerInteractionStyle* _interactionStyle{};
		SelectModel _selectModel{ None };
 
	};
}


#endif
