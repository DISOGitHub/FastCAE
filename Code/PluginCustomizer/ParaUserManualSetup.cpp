#include "ParaUserManualSetup.h"
#include "ui_ParaUserManualSetup.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/GlobalConfig.h"
#include "DataManager.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QTime>

#ifdef Q_OS_WIN

#include <Windows.h>
#include <QAxWidget>
#include <QAxSelect>
#include <QAxObject>

#endif

#pragma execution_character_set("utf-8")

namespace FastCAEDesigner
{
	ParaUserManualSetup::ParaUserManualSetup(ConfigOption::GlobalConfig* globalConfig, QWidget *parent) :
		_globalConfig(globalConfig),
		QDialog(parent),
		ui(new Ui::ParaUserManualSetup)
	{
		ui->setupUi(this);

		this->setFixedSize(1050, 650);
		//ui->widget->resize(900, 600);
		//ui->gridLayout_2->
		connect(ui->btnLoadDoc, SIGNAL(clicked()), this, SLOT(OnBtnLoadDocClicked()));
		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
		connect(ui->btnPreview, SIGNAL(clicked()), this, SLOT(OnBtnPreviewClicked()));

		_fileName = DataManager::getInstance()->GetUserManual();
		ui->txtFileName->setText(_fileName);
		//ui->txtFileName->setEnabled(false);
		_globalConfig = globalConfig;

		/*
		HRESULT result = OleInitialize(0);
		
		if (result != S_OK && result != S_FALSE)
		{
			qWarning("Qt: could not initialize OLE (error %x)", (unsigned int)result);
		}
		*/
		ui->btnPreview->hide();
	}

	ParaUserManualSetup::~ParaUserManualSetup()
	{
		delete ui;
		//OleUninitialize();
		CloseOffice();
	}

	void ParaUserManualSetup::resizeEvent(QResizeEvent *e)
	{
		/*
		auto rect = ui->widget->geometry();
		
		if (_officeContent != nullptr)
			_officeContent->setGeometry(rect);

		QWidget::resizeEvent(e);
		*/
	}

	void ParaUserManualSetup::showEvent(QShowEvent *e)
	{
		OpenFile(_fileName);
		QDialog::showEvent(e);
	}

	void ParaUserManualSetup::Sleep(int msec)
	{
		QTime dieTime = QTime::currentTime().addMSecs(msec);

		while (QTime::currentTime() < dieTime)
		{
			QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
		}
	}
	
	void ParaUserManualSetup::UpdataDlg()
	{
		int ox = this->x();
		int oy = this->y();
		int w = this->width() + 1;
		int h = this->height() + 1;
		Sleep(200);
		this->setGeometry(ox, oy, w, h);
		Sleep(200);
		w--;
		h--;
		this->setGeometry(ox, oy, w, h);
	}

	void ParaUserManualSetup::OpenFile(QString fileName)
	{
		if (fileName.isEmpty())
			return;

		if (fileName.endsWith(".xlsx"))
		{
			this->OpenExcel(fileName);
		}
		else if (fileName.endsWith(".docx") || fileName.endsWith(".doc"))
		{
			this->OpenWord(fileName);
		}
		else if (fileName.endsWith(".pdf"))
		{
			this->OpenPdf(fileName);
		}

	}

	void ParaUserManualSetup::OnBtnLoadDocClicked()
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open user doc file"), "", tr("doc (*.doc *.pdf *.docx)"));
		
		if (fileName.isEmpty())
			return;
		
		ui->txtFileName->setText(fileName);
		OpenFile(fileName);
		return;
	}
	
	void ParaUserManualSetup::OnBtnPreviewClicked()
	{
		QString fileName = ui->txtFileName->text().trimmed();
		
		if (fileName.isEmpty())
			return;
		
		OpenFile(_fileName);
	}

	void ParaUserManualSetup::OnBtnOkClicked()
	{
		QString fileFullName = ui->txtFileName->text();
		DataManager::getInstance()->SetUserManual(fileFullName);

		QFileInfo fileInfo(fileFullName);
		QString fileName = fileInfo.fileName();
		_globalConfig->SetUserManual(fileName);
		close();
	}

	void ParaUserManualSetup::OnBtnCancelClicked()
	{
		close();
	}

	void ParaUserManualSetup::OpenExcel(QString &fileName)
	{
#ifdef Q_OS_WIN

		this->CloseOffice();
		_officeContent = new QAxWidget("Excel.Application", this->ui->widget);
		_officeContent->dynamicCall("SetVisible (bool Visible)", "false");//不显示窗体
		_officeContent->setProperty("DisplayAlerts", false);
		auto rect = this->ui->widget->geometry();
		_officeContent->setGeometry(rect);
		_officeContent->setControl(fileName);
		_officeContent->show();
#endif
	}

	void ParaUserManualSetup::OpenWord(QString &fileName)
	{
#ifdef Q_OS_WIN

		this->CloseOffice();
		//_officeContent = new QAxWidget("Word.Application", this);
		//_officeContent = new QAxWidget("Word.Application");
		_officeContent = new QAxWidget("Word.Application",this->ui->widget);
		_officeContent->dynamicCall("SetVisible (bool Visible)", "false");//不显示窗体
		_officeContent->setProperty("DisplayAlerts", false);
		ui->widget->hide();
		auto rect =  this->ui->widget->geometry();
		_officeContent->setGeometry(rect);
		qDebug() << fileName;
		_officeContent->setControl(fileName);
		_officeContent->show();
		ui->widgetLayout->addWidget(_officeContent);
		//UpdataDlg();
		/**/
		/*
		CloseOffice();
		_officeContent = new QAxWidget("Word.Application");
		_officeContent->dynamicCall("SetVisible (bool Visible)", "false");//不显示窗体
		_officeContent->setProperty("DisplayAlerts", false);
		auto rect = ui->widget->geometry();
		_officeContent->setGeometry(rect);
		_officeContent->setControl(fileName);
		_officeContent->show();
		_officeContent->raise();
		ui->widgetLayout->addWidget(_officeContent);
		UpdataDlg();
		*/
#endif
	}

	void ParaUserManualSetup::OpenPdf(QString &fileName)
	{
#ifdef Q_OS_WIN
		/*
		this->CloseOffice();
		//_officeContent = new QAxWidget(this);
		_officeContent = new QAxWidget;

		if (!_officeContent->setControl("Adobe PDF Reader"))
			QMessageBox::critical(this, "Error", "没有安装pdf！");

		//ui->widget->hide();
	    QVariant v = _officeContent->dynamicCall("LoadFile(const QString&)", fileName);
		bool b = v.toBool();
		QString str = (b) ? "Load ok.": "Load fail.";
		qDebug() << str;
		_officeContent->show();
		ui->widgetLayout->addWidget(_officeContent);
		*/
		this->CloseOffice();
		_officeContent = new QAxWidget();

		if (!_officeContent->setControl("Adobe PDF Reader"))
			QMessageBox::critical(this, "Error", tr("Not found pdf reader"));
		
		//auto rect = ui->widget->geometry();
		//_officeContent->setGeometry(rect);
		ui->widget->hide();
		ui->widgetLayout->addWidget(_officeContent);
		_officeContent->dynamicCall("LoadFile(const QString&)", fileName);
		_officeContent->show();
		//_officeContent->raise();
#endif
	}

	void ParaUserManualSetup::CloseOffice()
	{
#ifdef Q_OS_WIN
		if (this->_officeContent)
		{
			_officeContent->close();
			_officeContent->clear();
			delete _officeContent;
			_officeContent = nullptr;
		}
#endif
	}

}