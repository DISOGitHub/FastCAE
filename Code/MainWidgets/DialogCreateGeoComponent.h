#ifndef _DIALOGCREATEGEOCOMPONENT_H_
#define _DIALOGCREATEGEOCOMPONENT_H_

#include "mainWidgetsAPI.h"
#include "SelfDefObject/QFDialog.h"
#include "geometry/GeoComponent.h"

namespace Ui
{
	class CreateGeoComponentDialog;
}

namespace MainWidget
{
	class PreWindow;

	class MAINWIDGETSAPI CreateGeoComponentDialog : public QFDialog
	{
		Q_OBJECT

	public:
		CreateGeoComponentDialog(GUI::MainWindow* mainwindow, PreWindow* prewindow);
		~CreateGeoComponentDialog();

	private:
		void InitDialog();

	private slots:
		void on_buttonOk();

	signals:
		void updataGeoComponentTree();

	private:
		PreWindow* _preWindow;
		Ui::CreateGeoComponentDialog* _ui;
		Geometry::GeoComponentType _type;
	};
}

#endif