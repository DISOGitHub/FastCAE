#ifndef GMSH_PY_H__
#define GMSH_PY_H__

#include <QMultiHash>
#include "GmshModuleAPI.h"

namespace GUI
{
	class MainWindow;
}

namespace MainWidget
{
	class PreWindow;
}

class vtkDataSet;

namespace Gmsh
{
	class GmshModule;

	class GmshPy
	{
	public:
		GmshPy() = default;
		~GmshPy() = default;

		static void init(GmshModule* mod, GUI::MainWindow* m);
		static void setPreWindow(MainWidget::PreWindow* pre);
	 
		static void GenerateMesh3D(char* solids, char* type, int order, int method, double factor, double sizemin, 
			double sizemax, bool clean, bool isGridCoplanar, char* points, char* fields/*, char* physicals*/, bool selectall, bool selectvisible);
		static void GenerateMesh2D(char* solids, char* type, int order, int method, int smooth,	double factor, double sizemin, 
			double sizemax, bool clean, bool isGridCoplanar, char* points, char* fields/*, char* physicals*/, bool selectall, bool selectvisible);

	private:
		static vtkDataSet* meshFilter(vtkDataSet* dataset, int dim);
//		static int CellTypeToDim(int type);

	private:
		static GUI::MainWindow* _mainWindow;
		static MainWidget::PreWindow* _preWindow;
		static GmshModule* _gmshModule;
	};
}

extern "C"
{
	void GMSHAPI GenerateMesh3D(char* solids, char* type, int order, int method, double factor, double sizemin, double sizemax, bool clean, bool isGridCoplanar, char* points, char* fields/*, char* physicals*/,bool selectall, bool selectvisible);
	void GMSHAPI GenerateMesh2D(char* solids, char* type, int order, int method, int smooth, double factor, double sizemin, double sizemax, bool clean, bool isGridCoplanar, char* points, char* fields/*, char* physicals*/, bool selectall, bool selectvisible);
}

#endif // !GMSH_PY_H__
