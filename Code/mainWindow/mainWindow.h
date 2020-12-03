/****************************
声明主窗口基本元素，以及实现布局
libaojun
2017.08.28
*************************** */
#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include "mainWindowAPI.h"
#include <QMainWindow>
#include <QHash>
#include <QPair>
#include <QString>
#include "moduleBase/messageWindowBase.h"


class XToolManger;
class QTreeWidgetItem;
class QSignalMapper;
class vtkDataSet;
class QMenu;
class QKeyEvent;
class QLabel;
class vtkActor;
class QAction;
class QToolBar;

namespace Ui
{
	class MainWindow;
}

namespace DataProperty
{
	class DataBase;
}

namespace MeshData
{
	class MeshSet;
	class MeshKernal;
}

namespace Geometry
{
	class GeometrySet;
	class GeoComponent;
}

namespace MainWidget
{
	class ControlPanel;
	class MessageWindow;
	class ProcessWindow;
	class PreWindow;
}

namespace Post
{
	class PostWindowBase;
	class RealTimeWindowBase;
}

namespace XReport
{
	class ReportWindow;
}

namespace GUI
{
	class SubWindowManager;
	class SignalHandler;
	class Translator;
	class CustomizerHelper;

	class MAINWINDOWAPI MainWindow : public QMainWindow
	{
		Q_OBJECT

			friend CustomizerHelper;

	public:
		MainWindow();
		~MainWindow();
		//获取ui
		Ui::MainWindow* getUi();
		//获取翻译
		Translator* GetTranslator();
		QString getMD5();
		void setMD5(const QString& md5);
		//设置当前文件
		void setCurrentFile(const QString& file);
		//清空关闭所有窗口
		void clearWidgets();
		SubWindowManager* getSubWindowManager();
		CustomizerHelper*  getCustomizerHelper();
		SignalHandler* getSignalHandler();
		//获取控制面板
		MainWidget::ControlPanel* getControlPanel();
		//获取控制台
		MainWidget::MessageWindow* getMessageWindow();
		//更新recentMenu
		void updateRecentMenu();
		//action状态控制
		void setActionVisible(QString objname, bool enable);
		//设置图标
		void setIcon(QString iconPath);
		//根据object name 获取action
		QAction* getAction(QString& objName);
		//根据object name 获取toolbar
		QToolBar* getToolBar(QString& objName);
		//根据object name 获取menu
		QMenu* getMenu(QString& objName);

	signals:
		/*更新注册接口*/
		void updateInterfaces();
		/*发送信息给状态栏 */
		void sendInfoToStatesBar(QString);
		/*显示装状态栏绘图范围*/
		void showGraphRangeSig(double, double);
		/*模型树点击事件  鼠标信号，eventType 0- 左键单机，1-右键单击，2- 左键双击 */
		void treeMouseEvent(int eventType, QTreeWidgetItem* item, int projectID);
		/*更新模型树信号 */
		void updatePhysicsTreeSignal();
		//更新材料信号
		void updateMaterialTreeSig();
		/*打印输出信息 */
		void printMessageToMessageWindow(QString message);
		///<MG output message
		void printMessageToMessageWindow(ModuleBase::Message message);
		//脚本输出信息
		void printMessageSig(int type, QString m);
		/*开始求解 */
		void startSolve();
		/*暂停求解 */
		void pauseSolve();
		/*终止求解 */
		void stopSolve(QWidget* w);
		/*添加进度条 */
		void addProcessBarSig(QWidget* w);
		//求解进程结束
		void processFinished(int pid);

		/****几何相关信号**** */
		void startSketchSig(bool start, double* loc, double* dir);
		//清除所有高亮
		void clearAllHighLight();
		/*刷新几何树 */
		void importGeometrySig(QStringList f);
		void exportGeometrySig(QString);
		void updateGeometryTreeSig();
		void updateGeoDispalyStateSig(int index, bool display);
		void removeGeometryActorSig(int index);
		void highLightGeometrySetSig(Geometry::GeometrySet* s, bool on);
		/****网格相关信号***** */
		void importMeshByNamesSig(QString name);
		void importMeshDataSetSig(vtkDataSet* dataset);
//		void exportMeshByIDSig(QString fileName, QString suffix, int kID);
		void editMeshSig(int dim, int kindex);
		void updateMeshTreeSig();
		void updateSetTreeSig();
		void updateMeshDispalyStateSig(int index, bool display);
		void updateMeshSetVisibleSig(MeshData::MeshSet*);
		void removeMeshActorSig(int index);
		void removeSetDataSig(int index);
		void highLightSetSig(MeshData::MeshSet* set);
		void highLightGeoComponentSig(Geometry::GeoComponent*);
		void highLightKernelSig(MeshData::MeshKernal* k);
		void highLightDataSetSig(vtkDataSet* dataset);
		//清空数据
		void clearDataSig();
		///更新前处理窗口所有几何网格Actor
		//		void updatePreActors();
		/*创建物理模型 */
		void createPhysiceModelSig();
		/*更新属性框 */
		void updateProperty(DataProperty::DataBase* data);
		//更新参数窗口
		void updateParaWidget(QWidget* w);
		///关闭前处理窗口
		void closePreWindowSig();
		///打开后处理窗口
		void openPostWindowSig(Post::PostWindowBase* pw);
		///打开实时曲线窗口,
		void openRealTimeWindowSig(Post::RealTimeWindowBase* w, int proID);
		///展示后处理窗口
		void showPostWindowInfoSig(int id, int type);
		///关闭后处理窗口
		void closePostWindowSig(Post::PostWindowBase* w);
		///关闭实时曲线窗口
		void closeRealTimeWindowSig(Post::RealTimeWindowBase* w);
		///更新实时曲线窗口
		void updateRealTimePlotSig(QString fileName);
		///切换选择模式
		void selectModelChangedSig(int i);
		///切换显示模型
		void displayModeChangedSig(QString m);
		///切换几何显示模型
		//void displayGeometryChangedSig(QString mode);
		///求解项目
		void solveProjectSig(int proIndex, int solverIndex);
		//添加新生成网格
		void appendGeneratedMesh(QString name, vtkDataSet* dataset);
		//显示报告窗口
		void openReportWindowSig(XReport::ReportWindow* w);
		//关闭报告窗口
		void closeReportWindowSig(XReport::ReportWindow* w);
		///设置键盘事件
		void enableGraphWindowKeyBoard(bool on);
		///更新Action状态
		void updateActionStatesSig();
		void updatePreMeshActorSig();
		void updatePreGeometryActorSig();//徐文强2020/6/4添加
		///根据绘图设置更新绘图
		void updateGraphOptionsSig();
		//保存图片 winType 0- 前处理窗口 1-后处理   Wintype为前处理时winhandle可任意
		void saveImageSig(QString fileName, int winType, Post::PostWindowBase*winhandle, int w, int h);
		// 		//面网格划分
		// 		void surfaceMeshSig(Geometry::GeometrySet*);
		// 		//体网格划分
		// 		void solidMeshSig(Geometry::GeometrySet*);
		//保存图片
		void saveImage(int w, int h, QString file);
		//清除高亮
		void clearHighLightSig();
		//前处理窗口打开
		void preWindowOpenedSig(MainWidget::PreWindow* p);
		//固定位置显示对话框
		void showDialogSig(QDialog*);
		//记录几何显示控件状态
		void selectGeometryDisplay(bool, bool, bool);
		//在几何上选取点、线、面。
		void selectGeometryModelChangedSig(int);
		//更新工具栏状态
		void updateActionsStatesSig();
		//关闭主窗口
		void closeMainWindow();

		public slots:
		/*状态栏显示信息 */
		void setStatusBarInfo(QString);
		///切换语言
		void ChangeLanguage(QString lang);
		///切换选择模式，改变工具栏选中状态
		void selectModelChanged(int model);
		//切换显示模式
		void setDisplay(QString m);
		//切换几何显示模式
		void selectGeometryModelChanged(int m);
		void setGeometryDisplay();
		// 		///<MG 显示/隐藏 工具栏/菜单栏 中的某一菜单/某一项
		// 		void showToolMenu(QString name, bool show);

		void updatePreGeometryActor();
		void updatePreMeshActor();
		///导入几何
		void on_importGeometry();
		///导出几何
		void on_exportGeometry();
		//生成体网格
		void on_solidMesh();
		//生成面网格
		void on_surfaceMesh();
		//生成网格
		void on_genMesh();
		//打印信息
		void printMessage(int type, QString m);
		//导入网格
//		void importMesh(QString fileName, QString s, int modelId);
		//导入几何
		void importGeometry(QStringList f);
		//导出几何
		void exportGeometry(QString f);
		//更新工具栏信息
		void updateActionsStates();

		private slots:
		/*关闭主窗口 */
		void closeWindow();
		///新建项目
		void on_actionNew();
		///打开工程文件
		void on_actionOpen();
		///保存工程文件
		void on_actionSave();
		///工程文件另存为
		void on_actionSaveAs();
		///导入网格
		void on_importMesh();

		//导入网格
		void importMeshDataset(vtkDataSet* dataset);
		//导出网格
		void on_exportMesh();
		///设置工作目录
		void setWorkingDir();
		///启动求解器
		void on_solve();
		//单独工程求解
		void solveProject(int id);
		///管理求解器
		void on_solverManager();
		///设置求解选项
		void on_solveOption();
		//设置绘图选项
		void on_graphOption();
		//About对话框
		void on_about();
		//打开用户手册 
		void on_userManual();
		//打开最近文件
		void openRencentFile(QString file);
		//创建组件（Set）
		void on_CreateSet();
		void on_CreateGeoComponent();
		//保存脚本
		void on_SaveScript();
		//执行脚本
		void on_ExecuateScript();
		//草绘点击
		void on_sketchClicked();
		//显示窗口范围
		void showGraphRange(double, double);
		//开始草绘
		void startSketch(bool s);
		//网格过滤
		void on_FilterMesh();
		//创建VTK空间变换窗口
		void on_VTKTranslation();

	private:
		/*初始化Menu*/
		//		void initMenu();

		/*信号槽关联 */
		void connectSignals();
		/*注册模块 */
		void registerMoudel();
		///初始化工具栏
		void initToolBar();
		/*重写QWidget虚函数  关闭主窗口事件 */
		void closeEvent(QCloseEvent *event) override;

		//键盘按下事件
		void keyPressEvent(QKeyEvent *e) override;
		void keyReleaseEvent(QKeyEvent *e) override;
		void showEvent(QShowEvent *e) override;

		bool isLoadRecordScripFile();
	private:
		Ui::MainWindow* _ui{};
		Translator* _translator{};
		SignalHandler* _signalHandler{};
		SubWindowManager* _subWindowManager{};
		CustomizerHelper* _customizerHelper{};
		//		bool _designModel{ false };


		//		QHash<int, Post3D::Post3DWindow*> _post3DWindow{};
		QString _currentFile{};
		QString _MD5{};

		MainWidget::ControlPanel* _controlPanel{};
		MainWidget::ProcessWindow* _processWindow{};
		MainWidget::MessageWindow* _messageWindow{};

		QSignalMapper* _viewSignalMapper{};
		QSignalMapper* _selectSignalMapper{};
		QSignalMapper* _displayModeSignalMapper{};
		QSignalMapper* _selectGeometryModeMapper{};

		QMenu* _recentMenu{};
		QSignalMapper* _recentFileMapper{};

		QLabel* _graphRange{};
		///<MG tool manger
		// 		XToolManger * _toolManger;
		// 
		// 		///<MG external menu 
		// 		QMap<QString, QMenu *> _externalMenus{};
		// 
		// 		///<MG external action
		// 		QMap<QString, QAction*> _externalActions{};

	};

}

#endif



