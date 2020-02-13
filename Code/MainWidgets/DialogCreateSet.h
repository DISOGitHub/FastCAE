#ifndef DIALOGCREATESET_H
#define DIALOGCREATESET_H

#include "mainWidgetsAPI.h"
#include "SelfDefObject/QFDialog.h"
#include <QString>

namespace GUI
{
	class MainWindow;
}

namespace Ui
{
	class CreateSetDialog;
}

namespace MeshData
{
	class MeshData;
	class MeshKernal;
}

namespace MainWidget
{
	class PreWindow;

	class MAINWIDGETSAPI CreateSetDialog : public QFDialog
	{
		Q_OBJECT

	public:
		CreateSetDialog(GUI::MainWindow* mainwindow, PreWindow* prewindow);
		~CreateSetDialog();

	signals:
		void updateSetTree();

	private:
		void init();
		QString getNameFromUi();
		void accept() override;
		bool selectFromGeo();
		QString generateGeoIDs(QString type);

	private:
		Ui::CreateSetDialog* _ui{};
		GUI::MainWindow* _mainWindow{};
		PreWindow* _preWindow{};
		MeshData::MeshData* _meshData{};
	};
}


#endif