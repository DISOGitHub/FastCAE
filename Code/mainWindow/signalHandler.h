#ifndef _SIGNALHANDLER_H_
#define _SIGNALHANDLER_H_

#include <QObject>
#include <QStringList>

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

	class SignalHandler : public QObject
	{
		Q_OBJECT
	public:
		SignalHandler(MainWindow* mainwindow);
		~SignalHandler();

	signals:
		void importMeshPySig(QStringList);
		void exportMeshPySig(QString);
		void open3DGraphWindowPySig();
		void open2DPlotWindowPySig();
		bool openProjectFileSig(QString fileName);
		void saveToProjectFileSig(QString filename);
		void solveProjectSig(int projectIndex, int solverIndex);
		

	public:
		bool importMesh(const QStringList &filenames);
		bool importGeometry(const QStringList &filenames);
		bool exportGeometry(QString f);
		QString getMD5();
		/*创建工程 */
		void on_actionNew();
		///清除数据
		void clearData();

	public slots:
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
		///生成网格
		void genMesh();
		//添加求解器生成网格
		void appendGeneratedMesh(QString name, vtkDataSet* dataset);
		//导出网格
		void exportMeshByID(QString filename, int kenerlID = -1);
		void exportMeshPy(QString filename);
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
		void saveToProjectFile(QString filename);
		//检查网格质量
		void meshChecking();
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
		//void showDemo();

	private:
		void handleSingleClickEvent(QTreeWidgetItem*item, int proID);
		void openPreWinPy();

	private:
		MainWindow* _mainWindow{};
		int _proID{ -1 };
//		QString _currentFilePath{};

	};
}


#endif
