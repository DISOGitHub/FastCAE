#ifndef _CUSTOMIZERACTIONMANAGER_H__
#define _CUSTOMIZERACTIONMANAGER_H__

#include <QObject>
#include <QDialog>
#include <QToolBar>

class QMenu;
class QAction;

namespace GUI
{
	class MainWindow;
	//xuxinwei 20200305
	class SubWindowManager;
}

namespace FastCAEDesigner
{
	class QFWidgetAction;
	class FunctionTreeSetup;
	class EditorSolverManager;
}

namespace Plugins
{
	class CustomizerPlugin;

	class CustomActionManager : public QObject
	{
		Q_OBJECT
	public:
		CustomActionManager(CustomizerPlugin* plugin, GUI::MainWindow* m);
		~CustomActionManager();

		void init();
		void release();
		void reTranslate();

	private slots:
	    void on_startCustom_slot();  //�ۺ���--��ʼ����
		void on_finishCustom_slot(); //�ۺ���--��ɶ���
		void on_cancelCustom_slot(); //�ۺ���--ȡ������
		void on_share_slot();        //�ۺ���--����

		//Added xvdongming
		void OnParaBaseSetup();
		void OnParaUserManualSetup();
		void OnParaFunctionParaSetup();


		void OnParaImportGeometrySetup();
		void OnParaExportGeometrySetup();
		//Added xvdongming

		void OnParaImportMeshSetup();
		void OnParaExportMeshSetup();

		void OnParaSolverManager();

		void UpdateCreateSketchToolBar(int status);		//���´�����ͼ������
		void UpdateFeatureModingToolBar(int status);	//����������ģ������
		void UpdateFeatureOperationsToolBar(int status);//������������������

		void UpdateSurfaceAction(int status);
		void UpdateSolidAction(int status);
		void UpdateCheckMeshAction(int status);
		void UpdateCreateSetAction(int status);
		void updateImportGeometryAction(QString suffix);
		void updateImportMeshAction(QString suffix);

	private:
		
		QMenu* CreateTopMenu(QString text,bool enable = true);  //���������˵�
		QToolBar* CreateToolBar(QString text);	//����������

		QAction* CreateActionSeparator(QMenu *menu);
		QAction* CreateAction(QString text, QObject* parent, bool enable = false);//�����˵����������
		QAction* CreateAction(QString text, QMenu* parentMenu, QObject* parent, bool enable = false);//�����Ӳ˵��������������ֵ����˵���
		QAction* CreateAction(QString text, QString icon, QString ShortcutKey, QMenu* parentMenu, QObject* parent, bool enable = false);//�����Ӳ˵��������������ֵ����˵���
		QAction* CreateAction(QString text, QString icon, QToolBar* parentTool, QObject* parent);//���������������빤��������ͼ��

		void FillCustomMenu();   //�������Ʋ˵�
		void FillHelpMenu();     //���������˵�
		void FillFileMenu();     //�����ļ��˵�
		void FillGeometryMenu(); //�������β˵�
		void FillMeshMenu();     //��������˵�
		void FillSolveMenu();	 //�������˵�	

		//void GratherMenuToList();        //�ռ���Ҫ�ڶ����л�ʱ����ʹ�ܵĲ˵��б�
		//void SetMenuListEnable(bool b);  //�趨�˵��б��в˵����ʹ��
		//void RemoveActionList();         //ж�ز��ʱ��ɾ���Ӳ˵�
		//void RemoveMenuList();           //ж�ز��ʱ��ɾ������˵�  

		void FillFileToolBar();					//�����ļ�������
		void FillSplitMeshToolBar();			//���������ʷֹ�����
		void FillSelectToolBar();				//����ѡ�񹤾���
		void FillViewToolBar();					//�����鿴���񼸺ι�����
		void FillSolveToolBar();				//������⹤����
		void FillViewerToolBar();				//������ͼ������
		void FillCheckCreatToolBar();			//���������ⴴ�����������
		void FillURDoToolBar();					//���������ָ�������
		void FillCreateSketchToolBar();			//�����ݻ湤����
		void FillFeatureModingToolBar();		//�������ν�ģ������
		void FillFeatureOperations1ToolBar();	//�������δ�������1
		void FillFeatureOperations2ToolBar();	//�������δ�������2
		void FillFeatureOperations3ToolBar();	//�������δ�������3
		
		template <typename T>
		void RemoveList(QList<T> list); //ж�ز��ʱ��ɾ���Ӳ˵��Ͷ���˵�

		//void UpdateActionStatus();
		//void UpdateToolBarStatusIntime(int status);//��ʱ���¹�����״̬
		void initMenuAndToolBar();
		void initActionstatus(FastCAEDesigner::QFWidgetAction* action,bool on);	//��ʼ���˵�������

		void quitCustomizer();

		void copySolverDependencyFiles(QList<QStringList> dependencyfiles,QString path);

		void updateMeshSelectAndView();
		void updateGeometrySelectAndView();
		void updateSelectOff();

	private:
		GUI::MainWindow* _mainWindow{};
		CustomizerPlugin* _plugin{};

		QMenu* _menu{};           //���Ʋ˵� 
		QAction* _cusAction{};
		QAction* _startAction{};  //���Ʋ˵�--��ʼ����
		QAction* _finishAction{}; //���Ʋ˵�--��ɶ���
		QAction* _cancelAction{}; //���Ʋ˵�--ȡ������
		QAction* _shareAction{};  //���Ʋ˵�--����

		//�����˵�
		QMenu* _menuHelp{};       //�����˵�
		QAction* _userInfo{};     //�����˵�--�û���Ϣ
		QAction* _userDoc{};      //�����˵�--�û��ֲ�

		//�ļ��˵�
		QMenu* _menuFile{};         //�ļ��˵�
		QAction* _importGeometry{}; //�ļ��˵�--���뼸��
		QAction* _exportGeometry{}; //�ļ��˵�--��������

		//���β˵�
		QMenu* _menuGeometry;            //���β˵�
		QAction* _geoFeatureModeling{};  //���β˵�--������ģ
		QAction* _geoFeatureOperation{}; //���β˵�--��������
		QAction* _geoDraft{};            //���β˵�--��ͼ

		FastCAEDesigner::QFWidgetAction *_geoFeatureModelingCheckbox{};  //���β˵�--��������
		FastCAEDesigner::QFWidgetAction *_geoFeatureOperationCheckbox{}; //���β˵�--��������
		FastCAEDesigner::QFWidgetAction *_geoDraftCheckbox{};            //���β˵�--��ͼ

		//����˵�
		QMenu* _menuMesh;            //����˵�
		//FastCAEDesigner::QFWidgetAction *_meshSurfaceCheckbox{}; //����˵�--�������ʷ�
		//FastCAEDesigner::QFWidgetAction *_meshSolidCheckbox{};   //����˵�--�������ʷ�
		
		//QAction* _meshGen{};                                     //����˵�--��������
		QAction* _meshCheck{};                                   //����˵�--������
		QAction* _meshCreateSet{};                              //����˵�--�������
		QAction* _surfaceMesh{};                                    //����˵�--surface
		QAction* _solidMesh{};                                  //����˵�--solid

		QAction* _importMesh{};	//��������
		QAction* _exportMesh{};	//��������

		FastCAEDesigner::QFWidgetAction *_meshSurfaceCheckbox;
		FastCAEDesigner::QFWidgetAction *_meshSolidCheckbox;
		FastCAEDesigner::QFWidgetAction *_meshCheckingCheckbox;
		FastCAEDesigner::QFWidgetAction *_meshCreateCheckbox;

		QToolBar* _fileToolBar;					//�ļ�������
		QToolBar* _splitMeshToolBar;			//�����ʷֹ�����
		QToolBar* _selectToolBar;				//ѡ�񹤾���
		QToolBar* _viewToolBar;					//�鿴���񼸺ι�����
		QToolBar* _solveToolBar;				//��⹤����
		QToolBar* _viewerToolBar;				//��ͼ������
		QToolBar* _checkCreatToolBar;			//�����ⴴ�����������
		QToolBar* _URDoToolBar;					//�����ָ�������
		QToolBar* _createSketchToolBar;			//�ݻ湤����
		QToolBar* _featureModingToolBar;		//���ν�ģ������
		QToolBar* _featureOperations1ToolBar;	//���δ�������1
		QToolBar* _featureOperations2ToolBar;	//���δ�������2
		QToolBar* _featureOperations3ToolBar;	//���δ�������3

		QMenu* _menuSolve;
		QAction* _solverManager{};

		QAction* _importMeshAction{};
		QAction* _importGeoAction{};


		//��Ҫ���ƵĲ˵��б�
		QList<QWidget*> _menuAndtoolbarList;         //����˵��б�
		QList<QAction*> _actionList;     //�Ӳ˵��б�
		
		FastCAEDesigner::FunctionTreeSetup *_functionTreeSetup{ nullptr };

		FastCAEDesigner::EditorSolverManager* _editorSolver{ nullptr };

		//��������ѡ�����
		QAction* _selectOff{};
		QAction* _meshSelect1{};
		QAction* _meshSelect2{};
		QAction* _meshSelect3{};
		QAction* _meshSelect4{};
		QAction* _geoSelect1{};
		QAction* _geoSelect2{};
		QAction* _geoSelect3{};
		QAction* _geoSelect4{};
		//����������ͼ����
		QAction* _meshDisplay1{};
		QAction* _meshDisplay2{};
		QAction* _meshDisplay3{};
		QAction* _meshDisplay4{};
		QAction* _geoDisplay1{};
		QAction* _geoDisplay2{};
		QAction* _geoDisplay3{};

		QAction* _geoUndo{};
		QAction* _geoRedo{};

		bool _meshSuffixNull{ false };
		bool _meshOperateNull{ false };
		bool _geoSuffixNull{ false };
		bool _geoOperateNull{ false };

		//xuxinwei 20200305
		GUI::SubWindowManager* _subWindowManager{};
    };

}



#endif