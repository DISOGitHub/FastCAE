#ifndef DIALOGCREATESET_H
#define DIALOGCREATESET_H

#include "mainWidgetsAPI.h"
#include "SelfDefObject/QFDialog.h"
#include "moduleBase/ModuleType.h"
#include <QString>

class vtkDataSet;

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

	private slots:
		void stateChangedSlot(int);
		void currentIndexChangedSlot(int);

	private:
		void init();
		QString getNameFromUi();
		void accept() override;
		bool selectFromGeo();
		QString generateGeoIDs(QString type);
		void hideFindCondition();
		void showFindCondition();
		void createSetAccept();
		void findConplanarAccept();

	private:
		Ui::CreateSetDialog* _ui{};
		GUI::MainWindow* _mainWindow{};
		PreWindow* _preWindow{};
		MeshData::MeshData* _meshData{};

		ModuleBase::SelectModel _model;
		bool _isGeo{ false };
		vtkDataSet* _data;
		int _seedId{ -1 };
		QString _geoIds;
	};
}


#endif