#ifndef _MAINWIDGETSPY_H_
#define _MAINWIDGETSPY_H_

#include "mainWidgetsAPI.h"

namespace GUI
{
	class MainWindow;
}

namespace Py
{
	class PythonAagent;
}

namespace MainWidget
{
	class ControlPanel;
	class GeometryTreeWidget;
	class MeshWidget;
	class PhysicsWidget;

	class MainWidgetPy
	{
	public:
		static void init(GUI::MainWindow* mw, ControlPanel* p, GeometryTreeWidget*g, MeshWidget* m, PhysicsWidget* ph);

		static void createCase(char*name, char* type);
		static void deleteCase(int id);
		static void startMesher(char* mesher);
		static void importProject(int id);
		static void caseRename(int pid, char* newname);
		static void updateGeometrySubTree(int id);
		static void updateComponentSubTree(int id);
		static void updateBCSubTree(int id);
		static void updatePostTree(int id);
		static void viewCounter(int id, char* variable);
		static void viewVector(int id, char* variable);
		static void loadFromMaterialLib(char* namelist);
		static void CreateMaterial(char* name, char*type);
		static void RemoveFromMaterialLib(char* namelist);
		static void DeleteMaterial(int id, char* name);
		static void AddMaterialToLib(int id, char* name);

	private:
		static GUI::MainWindow* _mainWindow;
		static ControlPanel* _controlPanel;
		static GeometryTreeWidget* _geoWidget;
		static MeshWidget* _meshWidget;
		static PhysicsWidget* _physicsWidget;
		static Py::PythonAagent* _pyAgent;

	};
}

extern "C"
{
	void MAINWIDGETSAPI createCase(char* name, char* type);
	void MAINWIDGETSAPI deleteCase(int id);
	void MAINWIDGETSAPI startMesher(char* mesher);
	void MAINWIDGETSAPI importProject(int id);
	void MAINWIDGETSAPI caseRename(int pid, char* newname);
	void MAINWIDGETSAPI updateGeometrySubTree(int id);
	void MAINWIDGETSAPI updateComponentSubTree(int id);
	void MAINWIDGETSAPI updateBCSubTree(int id);
	void MAINWIDGETSAPI updatePostTree(int id);
	void MAINWIDGETSAPI viewCounter(int id, char* variable);
	void MAINWIDGETSAPI viewVector(int id, char* variable);
	void MAINWIDGETSAPI loadFromMaterialLib(char* namelist);
	void MAINWIDGETSAPI CreateMaterial(char* name, char* type);
	void MAINWIDGETSAPI RemoveFromMaterialLib(char* namelist);
	void MAINWIDGETSAPI DeleteMaterial(int id, char* name);
	void MAINWIDGETSAPI AddMaterialToLib(int id, char* name);
}

#endif