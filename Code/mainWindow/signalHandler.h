#ifndef _SIGNALHANDLER_H_
#define _SIGNALHANDLER_H_

#include <QObject>
#include <QStringList>
#include "mainWindowAPI.h"

class QDialog;
class QTreeWidgetItem;
class vtkDataSet;

namespace Post
{
	class PostWindowBase;
}

namespace GUI
{
	class MainWindow;
	class SolveProcessManager;

	class MAINWINDOWAPI SignalHandler : public QObject
	{
		Q_OBJECT
	public:
		SignalHandler(MainWindow* mainwindow);
		~SignalHandler();

	signals:
		void importMeshPySig(QString, QString, int);
		void exportMeshPySig(QString, QString, int);
		void open3DGraphWindowPySig();
		void open2DPlotWindowPySig();
		bool openProjectFileSig(QString fileName);
		void saveToProjectFileSig(QString fileName);
		void solveProjectSig(int projectIndex, int solverIndex);
		void projectFileProcessedSig(QString file, bool success, bool isread);
		

	public:
		//不要通过返回值判断
		bool importGeometry(const QStringList &filenames);
		bool exportGeometry(QString f);
		QString getMD5();
		/*创建工程 */
		void on_actionNew();
		///获取求解管理器
		SolveProcessManager* getSolveProcessManager();
		
	
	public slots:
		//导入网格
		bool importMeshSlot(const QString &fileName, const QString& suffix, int modelId);
		//导出网格
		bool exportMeshSlot(const QString &fileName, const QString& suffix, int modelId);
		///清除数据
		void clearData(bool unlock = true);
		/*求解 */
		void on_actionSolve();
		/*切换为英语 */
		void on_actionEnglish();
		/*切换为中文 */
		void on_actionChinese();
		/*处理模型树事件 */
		void handleTreeMouseEvent(int eventtype, QTreeWidgetItem*item, int proID);
		///求解
		void solveProjectPy(int projectIndex, int solverIndex);
		void solveProject(int projectIndex, int solverIndex);
		///生成面网格
		void generateSurfaceMesh();
		///生成体网格
		void generateSolidMesh();
		//生成流体域网格
		void generateFluidMesh();
		///生成网格
		void genMesh();
		//添加求解器生成网格
		void appendGeneratedMesh(QString name, vtkDataSet* dataset);
		///刷新Action状态
		void updateActionsStates();
		//独立打开2D后处理窗口
		void open2DPlotWindow();
		void open2DPlotWindowPy();//提交py代码
		//独立打开3D后处理窗口
		void open3DGraphWindow();
		void open3DGraphWindowPy();
		//关闭后处理窗口
		void closePostWindow(Post::PostWindowBase* p);
		//保存图片
		void saveImange();
		bool openProjectFile(QString fileName);
		void projectFileProcessed(QString filename, bool success, bool read);
		void saveToProjectFile(QString fileName);
		//检查网格质量
		void meshChecking();
		//显示用户引导
		void showUserGuidence(bool start = false);
		//创建几何
		void undo();
		void redo();
		void createBox();
		void createCylinder();
		void CreateSphere();
		void CreateCone();
		//void CreateCylindricalComplex();
		//void CreateBoxComplex();
		void CreatePoint();
		void CreateEdge();
		void CreateFace();
		void CreateFilet();
		void CreateVariableFillet();
		void CreateChamfer();
		void CreateBoolCut();
		void CreateBoolFause();
		void CreateBoolCommon();
		void MirrorFeature();
		void CreateExtrusion();
		void MoveFeature();
		void RotateFeature();
		void CreateRevol();
		void CreateLoft();
		void CreateSweep();
		void CreateDatumplane();
		void DrawGraphicsLine();
		void DrawGraphicsRectangle();
		void DrawGraphicsCircle();
		void DrawGraphicsArc();
		void DrawGraphicsPolyline();
		void DrawGraphSpline();
		void showDialog(QDialog* d);
		void MakeMatrix();
		void MeasureDistance();
		void GeoSplitter();
		void MakeFillHole();
		void MakeRemoveSurface();
		void MakeFillGap();
	private:
		void handleSingleClickEvent(QTreeWidgetItem*item, int proID);
		void openPreWinPy();

	private:
		MainWindow* _mainWindow{};
		int _proID{ -1 };
		SolveProcessManager* _solveProcessManager{};
		bool _launched{ false };
//		QString _currentFilePath{};

	};
}

#endif
