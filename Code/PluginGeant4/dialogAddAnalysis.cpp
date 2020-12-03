#include "dialogAddAnalysis.h"
#include "ui_dialogAddAnalysis.h"
#include "SolidTube.h"

namespace Geant4 {
	DialogAddAnalysis::DialogAddAnalysis(QWidget * parent /*= nullptr*/)
	{
		m_ui = new Ui::DialogAddAnalysis();
		m_ui->setupUi(this);
		this->init();
	}

	DialogAddAnalysis::~DialogAddAnalysis()
	{
		delete m_ui;
	}

	void DialogAddAnalysis::renameWindow(QString name)
	{
		this->setWindowTitle(name);
	}

	QString DialogAddAnalysis::getAnalysisName() const
	{
		return m_ui->Name->text();
	}

	int DialogAddAnalysis::getAnalysisType() const
	{
		return m_ui->comboBox->currentIndex();
	}

	void DialogAddAnalysis::init()
	{
	
	}

	void DialogAddAnalysis::closeEvent(QCloseEvent *)
	{

	}

	void DialogAddAnalysis::reject()
	{
		QDialog::reject();
	}

	void DialogAddAnalysis::accept()
	{
		QDialog::accept();
	}

}