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
	    void on_startCustom_slot();  //槽函数--开始定制
		void on_finishCustom_slot(); //槽函数--完成定制
		void on_cancelCustom_slot(); //槽函数--取消定制
		void on_share_slot();        //槽函数--发布

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

		void UpdateCreateSketchToolBar(int status);		//更新创建草图工具栏
		void UpdateFeatureModingToolBar(int status);	//更新特征建模工具栏
		void UpdateFeatureOperationsToolBar(int status);//更新特征操作工具栏

		void UpdateSurfaceAction(int status);
		void UpdateSolidAction(int status);
		void UpdateCheckMeshAction(int status);
		void UpdateCreateSetAction(int status);
		void updateImportGeometryAction(QString suffix);
		void updateImportMeshAction(QString suffix);

	private:
		
		QMenu* CreateTopMenu(QString text,bool enable = true);  //建立顶级菜单
		QToolBar* CreateToolBar(QString text);	//建立工具栏

		QAction* CreateActionSeparator(QMenu *menu);
		QAction* CreateAction(QString text, QObject* parent, bool enable = false);//建立菜单项里的子项
		QAction* CreateAction(QString text, QMenu* parentMenu, QObject* parent, bool enable = false);//建立子菜单，并将子项插入值顶层菜单中
		QAction* CreateAction(QString text, QString icon, QString ShortcutKey, QMenu* parentMenu, QObject* parent, bool enable = false);//建立子菜单，并将子项插入值顶层菜单中
		QAction* CreateAction(QString text, QString icon, QToolBar* parentTool, QObject* parent);//建立子项，并将其插入工具栏，带图标

		void FillCustomMenu();   //建立定制菜单
		void FillHelpMenu();     //建立帮助菜单
		void FillFileMenu();     //建立文件菜单
		void FillGeometryMenu(); //建立几何菜单
		void FillMeshMenu();     //建立网格菜单
		void FillSolveMenu();	 //建立求解菜单	

		//void GratherMenuToList();        //收集需要在定制切换时控制使能的菜单列表
		//void SetMenuListEnable(bool b);  //设定菜单列表中菜单项的使能
		//void RemoveActionList();         //卸载插件时，删除子菜单
		//void RemoveMenuList();           //卸载插件时，删除顶层菜单  

		void FillFileToolBar();					//建立文件工具栏
		void FillSplitMeshToolBar();			//建立网格剖分工具栏
		void FillSelectToolBar();				//建立选择工具栏
		void FillViewToolBar();					//建立查看网格几何工具栏
		void FillSolveToolBar();				//建立求解工具栏
		void FillViewerToolBar();				//建立视图工具栏
		void FillCheckCreatToolBar();			//建立网格检测创建组件工具栏
		void FillURDoToolBar();					//建立撤销恢复工具栏
		void FillCreateSketchToolBar();			//建立草绘工具栏
		void FillFeatureModingToolBar();		//建立几何建模工具栏
		void FillFeatureOperations1ToolBar();	//建立几何处理工具栏1
		void FillFeatureOperations2ToolBar();	//建立几何处理工具栏2
		void FillFeatureOperations3ToolBar();	//建立几何处理工具栏3
		
		template <typename T>
		void RemoveList(QList<T> list); //卸载插件时，删除子菜单和顶层菜单

		//void UpdateActionStatus();
		//void UpdateToolBarStatusIntime(int status);//及时更新工具栏状态
		void initMenuAndToolBar();
		void initActionstatus(FastCAEDesigner::QFWidgetAction* action,bool on);	//初始化菜单工具栏

		void quitCustomizer();

		void copySolverDependencyFiles(QList<QStringList> dependencyfiles,QString path);

		void updateMeshSelectAndView();
		void updateGeometrySelectAndView();
		void updateSelectOff();

	private:
		GUI::MainWindow* _mainWindow{};
		CustomizerPlugin* _plugin{};

		QMenu* _menu{};           //定制菜单 
		QAction* _cusAction{};
		QAction* _startAction{};  //定制菜单--开始定制
		QAction* _finishAction{}; //定制菜单--完成定制
		QAction* _cancelAction{}; //定制菜单--取消定制
		QAction* _shareAction{};  //定制菜单--发布

		//帮助菜单
		QMenu* _menuHelp{};       //帮助菜单
		QAction* _userInfo{};     //帮助菜单--用户信息
		QAction* _userDoc{};      //帮助菜单--用户手册

		//文件菜单
		QMenu* _menuFile{};         //文件菜单
		QAction* _importGeometry{}; //文件菜单--导入几何
		QAction* _exportGeometry{}; //文件菜单--导出几何

		//几何菜单
		QMenu* _menuGeometry;            //几何菜单
		QAction* _geoFeatureModeling{};  //几何菜单--特征建模
		QAction* _geoFeatureOperation{}; //几何菜单--特征操作
		QAction* _geoDraft{};            //几何菜单--草图

		FastCAEDesigner::QFWidgetAction *_geoFeatureModelingCheckbox{};  //几何菜单--特征定制
		FastCAEDesigner::QFWidgetAction *_geoFeatureOperationCheckbox{}; //几何菜单--特征操作
		FastCAEDesigner::QFWidgetAction *_geoDraftCheckbox{};            //几何菜单--草图

		//网格菜单
		QMenu* _menuMesh;            //网格菜单
		//FastCAEDesigner::QFWidgetAction *_meshSurfaceCheckbox{}; //网格菜单--面网格剖分
		//FastCAEDesigner::QFWidgetAction *_meshSolidCheckbox{};   //网格菜单--体网格剖分
		
		//QAction* _meshGen{};                                     //网格菜单--生成网格
		QAction* _meshCheck{};                                   //网格菜单--网格检查
		QAction* _meshCreateSet{};                              //网格菜单--创建组件
		QAction* _surfaceMesh{};                                    //网格菜单--surface
		QAction* _solidMesh{};                                  //网格菜单--solid

		QAction* _importMesh{};	//导入网格
		QAction* _exportMesh{};	//导出网格

		FastCAEDesigner::QFWidgetAction *_meshSurfaceCheckbox;
		FastCAEDesigner::QFWidgetAction *_meshSolidCheckbox;
		FastCAEDesigner::QFWidgetAction *_meshCheckingCheckbox;
		FastCAEDesigner::QFWidgetAction *_meshCreateCheckbox;

		QToolBar* _fileToolBar;					//文件工具栏
		QToolBar* _splitMeshToolBar;			//网格剖分工具栏
		QToolBar* _selectToolBar;				//选择工具栏
		QToolBar* _viewToolBar;					//查看网格几何工具栏
		QToolBar* _solveToolBar;				//求解工具栏
		QToolBar* _viewerToolBar;				//视图工具栏
		QToolBar* _checkCreatToolBar;			//网格检测创建组件工具栏
		QToolBar* _URDoToolBar;					//撤销恢复工具栏
		QToolBar* _createSketchToolBar;			//草绘工具栏
		QToolBar* _featureModingToolBar;		//几何建模工具栏
		QToolBar* _featureOperations1ToolBar;	//几何处理工具栏1
		QToolBar* _featureOperations2ToolBar;	//几何处理工具栏2
		QToolBar* _featureOperations3ToolBar;	//几何处理工具栏3

		QMenu* _menuSolve;
		QAction* _solverManager{};

		QAction* _importMeshAction{};
		QAction* _importGeoAction{};


		//需要控制的菜单列表
		QList<QWidget*> _menuAndtoolbarList;         //顶层菜单列表
		QList<QAction*> _actionList;     //子菜单列表
		
		FastCAEDesigner::FunctionTreeSetup *_functionTreeSetup{ nullptr };

		FastCAEDesigner::EditorSolverManager* _editorSolver{ nullptr };

		//几何网格选择操作
		QAction* _selectOff{};
		QAction* _meshSelect1{};
		QAction* _meshSelect2{};
		QAction* _meshSelect3{};
		QAction* _meshSelect4{};
		QAction* _geoSelect1{};
		QAction* _geoSelect2{};
		QAction* _geoSelect3{};
		QAction* _geoSelect4{};
		//几何网格视图操作
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