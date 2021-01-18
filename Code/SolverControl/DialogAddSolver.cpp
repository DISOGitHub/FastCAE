#include "DialogAddSolver.h"
#include "ui_DialogAddSolver.h"
#include "DialogSolverManager.h"
#include "ConfigOptions/SolverConfig.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/SolverInfo.h"
#include "IO/IOConfig.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

namespace SolverControl
{

	AddSolverDialog::AddSolverDialog(GUI::MainWindow* mainwin, SolverManagerDialog* p, ConfigOption::SolverInfo* solver)
		:_parent(p), _modifySolver(solver), QFDialog(mainwin)
	{
		_ui = new Ui::AddSolverDialog;
		_ui->setupUi(this);
		if (_modifySolver != nullptr)  _isEdit = true;
		_solverOption = ConfigOption::ConfigOption::getInstance()->getSolverOption();
		this->init();
	}

	AddSolverDialog::~AddSolverDialog()
	{
		delete _ui;
	}
	
	void AddSolverDialog::init()
	{
		this->initCombox();
		if (!_isEdit) return;

		this->setWindowTitle(QString(tr("Edit Solver")));

		ConfigOption::SolverType type = _modifySolver->getType();
		if (type == ConfigOption::ThirdParty) _ui->tpradioButton->setChecked(true);
		
		const QString name = _modifySolver->getName();
		_ui->nameLineEdit->setText(name);

		const QString path = _modifySolver->getExePath();
		_ui->pathlineEdit->setText(path);

		const QString arg = _modifySolver->getParameter();
		_ui->arguementsLineEdit->setText(arg);
		
		const QString proce = _modifySolver->getProcessKeyWord();
		_ui->processkeyLineEdit->setText(proce);

		const QString templte = _modifySolver->getTemplate();
		const QString forma = _modifySolver->getInputFormat();
		if (templte.isEmpty()) _ui->fileRadioButton->setChecked(true);
		_ui->templateLineEdit->setText(templte);
		_ui->FileComboBox->setCurrentText(_inpTextData.key(forma));

		const QString trans = _modifySolver->getTransfer();
		_ui->transformerComboBox->setCurrentText(_transferTextData.key(trans));
	}

	void AddSolverDialog::on_out_OkButton_clicked()
	{
		const QString name = _ui->nameLineEdit->text();
		const QString path = _ui->pathlineEdit->text();

		if (name.isEmpty() || path.isEmpty())
		{
			QMessageBox::warning(this, QString(tr("Warning")), QString(tr("Name and path must be given !")));
			return;
		}
		if (!_isEdit)
		{
			_modifySolver = new ConfigOption::SolverInfo;
			_solverOption->appendSolver(_modifySolver);
		}

		ConfigOption::SolverType type = ConfigOption::SelfDevelop;
		if (_ui->tpradioButton->isChecked()) type = ConfigOption::ThirdParty;

		_modifySolver->setType(type);
		_modifySolver->setEXEPath(path);
		_modifySolver->setName(name);

		QString arg = _ui->arguementsLineEdit->text();
		_modifySolver->setParameter(arg);
		QString pro = _ui->processkeyLineEdit->text();
		_modifySolver->setProcessKeyWord(pro);

		QString tem, format;
		if (!_ui->templateRadioButton->isChecked())
		{
			tem.clear();
			format = _ui->FileComboBox->currentData(Qt::UserRole).toString();
		}
		else
		{
			tem = _ui->templateLineEdit->text();
			format.clear();
		}
		_modifySolver->setInputFormat(format);
		_modifySolver->setTemplate(tem);
		
		qDebug() << format;
		qDebug() << tem;

		const QString trans = _ui->transformerComboBox->currentData(Qt::UserRole).toString();
		_modifySolver->setTransfer(trans);

		qDebug() << trans;

		_parent->update();
		this->close();
	}

	void AddSolverDialog::on_out_CancelButton_clicked()
	{
		this->close();
	}

	void AddSolverDialog::on_fileSelect_clicked()
	{
		QString fileName = QFileDialog::getOpenFileName(this, QString(tr("Solver")), "", QString(tr("All Files (*.*)")));
		if (fileName.isEmpty()) return;
		_ui->pathlineEdit->setText(fileName);
	}

	void AddSolverDialog::on_fileSelect_1_clicked()
	{
		QString fileName = QFileDialog::getOpenFileName(this, QString(tr("Solver")), "", QString(tr("All Files (*.*)")));
		if (fileName.isEmpty()) return;
		_ui->templateLineEdit->setText(fileName);
	}

	void AddSolverDialog::initCombox()
	{
		QString text(tr("Default (xml)"));
		_ui->FileComboBox->addItem(text, QString("xml"));
		_inpTextData[text] = QString("xml");

		text = QString(tr("Default (None)"));
		_ui->transformerComboBox->addItem(text, QString("none"));
		_transferTextData[text] = QString("none");

		QStringList formatList = IO::IOConfigure::getInputFileFormat();
		QStringList transfers = IO::IOConfigure::getOutputFileTransfers();

		for (QString f : formatList)
		{
			_ui->FileComboBox->addItem(f,f);
			_inpTextData[f] = f;
		}
		for (QString t : transfers)
		{
			_ui->transformerComboBox->addItem(t, t);
			_transferTextData[t] = t;
		}
	}



}