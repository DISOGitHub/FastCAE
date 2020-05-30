#include "EditorSolverValue.h"
#include "ui_EditorSolverValue.h"
#include "EditorSolverManager.h"
#include "ConfigOptions/SolverConfig.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/SolverInfo.h"
#include "IO/IOConfig.h"
#include "EditorDependencyFiles.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QDebug>

namespace FastCAEDesigner
{

	EditorSolverValue::EditorSolverValue(GUI::MainWindow* mainwin, EditorSolverManager* p, ConfigOption::SolverInfo* solver)
		:_parent(p), _modifySolver(solver)
	{
		_ui = new Ui::EditorSolverValue;
		_ui->setupUi(this);
		if (_modifySolver != nullptr)  _isEdit = true;
		_solverOption = ConfigOption::ConfigOption::getInstance()->getSolverOption();

		connect(_ui->dependFileSelect, SIGNAL(clicked()), this, SLOT(OnDepenFileSelectClicked()));
		connect(_ui->sdRadioButton, SIGNAL(clicked()), this, SLOT(OnRadioBtnClicked()));
		connect(_ui->tpradioButton, SIGNAL(clicked()), this, SLOT(OnRadioBtnClicked()));
		this->init();
		
	}

	EditorSolverValue::~EditorSolverValue()
	{
		delete _ui;
	}
	
	void EditorSolverValue::init()
	{
		this->initCombox();
		initDependFiles();
		if (!_isEdit)
		{
			if (_ui->sdRadioButton->isChecked())
				initDependFiles(true);
			return;
		}

		this->setWindowTitle(QString(tr("Edit Solver")));

		ConfigOption::SolverType type = _modifySolver->getType();
		if (type == ConfigOption::ThirdParty) _ui->tpradioButton->setChecked(true);
		
		const QString name = _modifySolver->getName();
		_ui->nameLineEdit->setText(name);

		const QString path = _modifySolver->getExePath();
		_ui->pathlineEdit->setText(path);

		const QStringList dependencyfiles = _modifySolver->getDependences();
		/*QStringList nameList = path.split(".");*/
		if (_ui->sdRadioButton->isChecked())/* && (nameList.last() == "exe")*/
		{
			initDependFiles(true);
			_ui->dependFilesLineEdit->setText(dependencyfiles.join(";"));
		}

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

	void EditorSolverValue::on_out_OkButton_clicked()
	{

// 		QMessageBox::StandardButton result = QMessageBox::information(this, tr("prompt!"), tr("Whether to retain the modification?"),
// 			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		QMessageBox msgBox(QMessageBox::Information, tr("prompt!"), tr("Whether to retain the modification?"));
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setButtonText(QMessageBox::Yes, tr("Yes"));
		msgBox.setButtonText(QMessageBox::No, tr("No"));

		int result = msgBox.exec();

		switch (result)
		{
		case QMessageBox::Yes:
			break;
		default:
			return;
			break;
		}
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
		if (_ui->tpradioButton->isChecked())
		{
			type = ConfigOption::ThirdParty;
			_ui->dependFilesLineEdit->clear();
		}

		_modifySolver->setType(type);
		_modifySolver->setEXEPath(path);
		_modifySolver->setName(name);

		QStringList dependencyfiles = _ui->dependFilesLineEdit->text().trimmed().split(";");
		_modifySolver->setDependences(dependencyfiles);

		QString arg = _ui->arguementsLineEdit->text();
		_modifySolver->setParameter(arg);
		QString pro = _ui->processkeyLineEdit->text();
		_modifySolver->setProcessKeyWord(pro);

		QString tem, format;
		if (!_ui->templateRadioButton->isChecked())
		{
			tem.clear();
			_ui->templateLineEdit->setText("");
			format = _ui->FileComboBox->currentData(Qt::UserRole).toString();
		}
		else
		{
			tem = _ui->templateLineEdit->text();
			_ui->FileComboBox->setCurrentIndex(0);
			format.clear();
		}
		_modifySolver->setInputFormat(format);
		_modifySolver->setTemplate(tem);
		
		qDebug() << format;
		qDebug() << tem;

		const QString trans = _ui->transformerComboBox->currentData(Qt::UserRole).toString();
		_modifySolver->setTransfer(trans);

		qDebug() << trans;

		QStringList files;
		if (!_ui->dependFilesLineEdit->text().trimmed().isEmpty())
		{
			QString names = _ui->dependFilesLineEdit->text().trimmed();
			QStringList filenames = names.split(";");
			for (int i = 0; i < filenames.count(); i++)
			{
				files.append(filenames.at(i));
			}	
		}
			
		if (!tem.isEmpty())
			files.append(tem);

		if (_ui->tpradioButton->isChecked())
			files.clear();

		_parent->setDependencyFiles(files);
		_parent->update();
		this->close();
	}

	void EditorSolverValue::on_out_CancelButton_clicked()
	{
		this->close();
	}

	void EditorSolverValue::on_fileSelect_clicked()
	{
		QString fileName = QFileDialog::getOpenFileName(this, QString(tr("Solver")), "", QString(tr("All Files (*.*)")));
		if (fileName.isEmpty()) return;
		_ui->pathlineEdit->setText(fileName);

// 		QStringList nameList = fileName.split(".");
// 
// 		if ((_ui->sdRadioButton->isChecked()) && (nameList.last() == "exe"))
// 		{
// 			initDependFiles(true);
// 		}

	}

	void EditorSolverValue::on_fileSelect_1_clicked()
	{
		QString fileName = QFileDialog::getOpenFileName(this, QString(tr("Solver")), "", QString(tr("All Files (*.*)")));
		if (fileName.isEmpty()) return;
		_ui->templateLineEdit->setText(fileName);
	}

	void EditorSolverValue::initCombox()
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

	void EditorSolverValue::OnDepenFileSelectClicked()
	{
		QString names = _ui->dependFilesLineEdit->text().trimmed();

		EditorDependencyFiles dlg(names);
		int r = dlg.exec();

		if (r != QDialog::Accepted)
			return;

		QString filenams = dlg.getFiles();

		_ui->dependFilesLineEdit->setText(filenams);

	}

	void EditorSolverValue::OnRadioBtnClicked()
	{
		if (_ui->tpradioButton->isChecked())
		{
			//_ui->dependFilesLineEdit->clear();
			initDependFiles(false);
			return;
		}

// 		QString fileName = _ui->pathlineEdit->text().trimmed();
// 
// 		if (fileName.isEmpty())
// 		{
// 			initDependFiles(false);
// 			return;
// 		}
// 
// 		QStringList nameList = fileName.split(".");

		if ((_ui->sdRadioButton->isChecked()) /*&& (nameList.last() == "exe")*/)
		{
			initDependFiles(true);
		}
	}

	void EditorSolverValue::initDependFiles(bool visable)
	{
		_ui->label_6->setVisible(visable);
		_ui->dependFilesLineEdit->setVisible(visable);
		_ui->dependFileSelect->setVisible(visable);
		_ui->dependFileSelect->setMinimumSize(30, 24);
		_ui->dependFileSelect->setMaximumSize(30, 24);
	}


}