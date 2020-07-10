#ifndef _GMSHDIALOG_BASE__H_
#define _GMSHDIALOG_BASE__H_

#include "GeometryWidgets/geoDialogBase.h"
#include <QList>

namespace Py
{
	class PythonAagent;
}


namespace Gmsh
{
	class LocalField;
//	class PhysicalsGroups;

	class LocalPoint
	{
	public:
		LocalPoint() = default;
		~LocalPoint() = default;

		double x{ 0 };
		double y{ 0 };
		double z{ 0 };
		double value{ 0 };

	};

	class GmshDialogBase :public GeometryWidget::GeoDialogBase
	{
		Q_OBJECT
	public:
		GmshDialogBase(GUI::MainWindow* m, MainWidget::PreWindow* pre);
		~GmshDialogBase();

		QList<LocalPoint*>* getLocalPoints();
		QList<LocalField*>* getLocalFields();
//		QList<PhysicalsGroups*>* getPhysicalsGroups();

	signals:
		void showDialog(QDialog*);

	protected:
		void appendPointSizeFiled();
		void appendSizeFields();
		//void appendPhysicals();

	protected:
		Py::PythonAagent* _pyAgent{};
		QList <LocalPoint*> _localPoints{};
		QList<LocalField*> _localFields{};
//		QList<PhysicalsGroups*> _physicalsGroups{};
	};

}

#endif