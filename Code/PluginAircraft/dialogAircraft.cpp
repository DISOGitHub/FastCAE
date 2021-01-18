#include "ui_dialogAircraft.h"
#include "dialogAircraft.h"
#include "AircraftManager.h"
#include "DataManager.h"
#include <QTableWidgetItem>
#include <QDebug>
namespace Aircraft
{

	CreateAircraftDialog::CreateAircraftDialog()
	{

		_ui = new Ui::DialogAircraft;
		_ui->setupUi(this);
		Qt::WindowFlags flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;
		setWindowFlags(flags);
		this->init();
		
	}


	void CreateAircraftDialog::init()
	{

	}

	void CreateAircraftDialog::closeEvent(QCloseEvent *)
	{

	}

	void CreateAircraftDialog::reject() 
	{
		this->close();
	}

	void CreateAircraftDialog::accept()
	{
		/*_material->clearMaterial();
		this->updateOneMaterial();
		QDialog::accept();
		this->close();*/
	}

	
	

}