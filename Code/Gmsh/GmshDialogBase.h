#ifndef _GMSHDIALOG_BASE__H_
#define _GMSHDIALOG_BASE__H_

#include "GeometryWidgets/geoDialogBase.h"
#include "GmshModuleAPI.h"
#include <QList>

namespace Py
{
	class PythonAagent;
}

namespace MeshData{
	class MeshKernal;
}


namespace Gmsh
{
//	class PhysicalsGroups;
	class LocalDensity;
	

	class /*GMSHAPI*/ GmshDialogBase :public GeometryWidget::GeoDialogBase
	{
		Q_OBJECT
	public:
		GmshDialogBase(GUI::MainWindow* m, MainWidget::PreWindow* pre);
		~GmshDialogBase();


//		QList<PhysicalsGroups*>* getPhysicalsGroups();
		QList<LocalDensity*>* getLocalDesities();

	signals:
		void showDialog(QDialog*);
		void highLightMeshKernal(MeshData::MeshKernal*);

	protected:

		//void appendPhysicals();
		void appendLocalDesities();

	protected:
		Py::PythonAagent* _pyAgent{};
//		QList<PhysicalsGroups*> _physicalsGroups{};
		QList<LocalDensity*> _localDensities;

	};

}

#endif