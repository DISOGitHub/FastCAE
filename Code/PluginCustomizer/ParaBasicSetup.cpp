#include "ParaBasicSetup.h"
#include "ui_ParaBasicSetup.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/GlobalConfig.h"
#include "settings/busAPI.h"
#include <QDebug>
#include <QValidator>
#include <QIntValidator>
#include <QRegExp>
#include <QString>
#include <QFileDialog>
#include <QSize>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QMessageBox>
#include <QFileInfo>
#include <QMainWindow>
#include <QTimer>
#include "mainWindow/SubWindowManager.h"
#include "InputValidator.h"
#include "DataManager.h"
#include <QFileInfo>



namespace FastCAEDesigner
{
	ParaBasicSetup::ParaBasicSetup(ConfigOption::GlobalConfig* globalConfig, QMainWindow *parent):
		_globalConfig(globalConfig), _mainWindow(parent),
		QDialog(parent),
		ui(new Ui::ParaBasicSetup)
	{
		ui->setupUi(this);
		InitErrorList();
		//Init();
	}
	//xuxinwei 20200305
	ParaBasicSetup::ParaBasicSetup(ConfigOption::GlobalConfig* globalConfig, QMainWindow *parent, GUI::SubWindowManager* sub) :
		_globalConfig(globalConfig), _mainWindow(parent), _subWindow(sub),
		QDialog(parent),
		ui(new Ui::ParaBasicSetup)
	{
		ui->setupUi(this);
		InitErrorList();
		//Init();
	}

	ParaBasicSetup::~ParaBasicSetup()
	{
		delete ui;
	}
	
	void ParaBasicSetup::UpdateUiToData()
	{
		if (nullptr == _globalConfig)
		{
			qDebug() << "ParaBasicSetup::UpdateUiToData1():_globalConfig == nullptr,update data fail.";
		}

		_globalConfig->setSoftName(ui->txtNameEn->text());
		_globalConfig->setChineseName(ui->txtNameCn->text());
		_globalConfig->setCorporation(ui->txtCorporation->text());
		_globalConfig->setWebsite(ui->txtWedSite->text());
		_globalConfig->setVersion(ui->txtVer->text());
		_globalConfig->setEMail(ui->txtEmail->text());
		
		QFileInfo logoFileInfo(_logoFileName);
		QString logoFileName = logoFileInfo.fileName();
		QFileInfo welcomeFileInfo(_welcomeFileName);
		QString welcomeFileName = welcomeFileInfo.fileName();

		_globalConfig->setLogo(logoFileName);
		_globalConfig->setWelcome(welcomeFileName);


		qDebug() << logoFileName << "," << welcomeFileName;
	}

	void ParaBasicSetup::UpdateDataToUi()
	{
		if (nullptr == _globalConfig)
		{
			qDebug() << "ParaBasicSetup::UpdateDataToUi1():_globalConfig == nullptr,update data fail.";
		}

		ui->txtNameCn->setText(_globalConfig->getChineseName());
		ui->txtNameEn->setText(_globalConfig->getSoftName());
		ui->txtVer->setText(_globalConfig->getVersion());
		ui->txtEmail->setText(_globalConfig->getEMail());
		ui->txtWedSite->setText(_globalConfig->getWebsite());
		ui->txtCorporation->setText(_globalConfig->getCorporation());
		
		ui->lbl_logo->setText(" ");
		ui->lbl_welcome->setText(" ");

		if (!_logoFileName.isEmpty())
			ShowImage(ui->lbl_logo, _logoFileName);
			
		if (!_welcomeFileName.isEmpty())
			ShowImage(ui->lbl_welcome, _welcomeFileName);
		
		//ui->txtLogo->setText(_globalConfig->getLogo());
		//ui->txtWelcom->setText(_globalConfig->getWelcome());
	}

	void ParaBasicSetup::showEvent(QShowEvent *)
	{
		Init();
	}

	void  ParaBasicSetup::InitErrorList()
	{
		_errorList.insert(ChnNameIsEmpty, tr("Chinese name is empty."));
		_errorList.insert(EngNameIsEmpty, tr("English name is empty."));
		_errorList.insert(EmailNameInvalid, tr("Email format error."));
		_errorList.insert(WebsiteNameInvalid, tr("Website format error."));
		_errorList.insert(ChnNameIsError, tr("Chinese name contain illegal characters."));
		_errorList.insert(EngNameIsError, tr("English name contain illegal characters."));
	}
	
	void ParaBasicSetup::Init()
	{
		UpdateDataToUi();																		

		setTabOrder(ui->txtNameCn, ui->txtNameEn);
		setTabOrder(ui->txtNameEn, ui->txtVer);
		setTabOrder(ui->txtVer, ui->txtCorporation);
		setTabOrder(ui->txtCorporation, ui->txtEmail);
		setTabOrder(ui->txtEmail, ui->txtWedSite);
		//setTabOrder(ui->txtWedSite, ui->txtNameCn);

		QFileInfo fi1(_logoFileName);
		QFileInfo fi2(_welcomeFileName);
		DataManager::getInstance()->removeIconNameFromList(fi1.fileName());
		DataManager::getInstance()->removeIconNameFromList(fi2.fileName());
		
		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(OnBtnCancelClicked()));
		connect(ui->btnLogo, SIGNAL(clicked()), this, SLOT(OnBtnOpenLogoClicked()));
		connect(ui->btnWelcom, SIGNAL(clicked()), this, SLOT(OnBtnOpenWelcomeClicked()));
		connect(ui->txtEmail, SIGNAL(textChanged(QString)), this, SLOT(OnTxtEmailTextChanged(QString)));
		connect(ui->txtWedSite, SIGNAL(textChanged(QString)), this, SLOT(OnTxtWebSiteTextChanged(QString)));

		//_styleOk = QString("QLineEdit {border-width: 1px;padding: 1px; border-style: solid;border-color: #e1e1e1;border-radius:1px; background-color:white; selection-color: #0a214c;selection-background-color: #C19A6B;min-height:24px; color:rgb(126,126,126);}QLineEdit:!enabled {border-width: 1px;padding: 1px; border-style: solid;border-color: #e1e1e1;border-radius:1px;  background-color: rgb(216,216,216); selection-color: #0a214c;selection-background-color: #C19A6B;min-height:24px; color:rgb(126,126,126);}");//±³¾°É«
		//_styleError = QString("QLineEdit {border-width: 1px;padding: 1px; border-style: solid;border-color: #e1e1e1;border-radius:1px; background-color:red; selection-color: #0a214c;selection-background-color: #C19A6B;min-height:24px; color:rgb(225,225,225);}QLineEdit:!enabled {border-width: 1px;padding: 1px; border-style: solid;border-color: #e1e1e1;border-radius:1px;  background-color: rgb(216,216,216); selection-color: #0a214c;selection-background-color: #C19A6B;min-height:24px; color:rgb(126,126,126);}");//±³¾°É«
		_styleOk = QString("QLineEdit {border-width: 1px;padding: 1px; border-style: solid;border-color: #e1e1e1;border-radius:1px; background-color:white; selection-color: #0a214c;selection-background-color: #C19A6B;min-height:24px; color:rgb(126,126,126);}QLineEdit:!enabled {border-width: 1px;padding: 1px; border-style: solid;border-color: #e1e1e1;border-radius:1px;  background-color: rgb(216,216,216); selection-color: #0a214c;selection-background-color: #C19A6B;min-height:24px; color:rgb(126,126,126);}");//±³¾°É«
		_styleError = QString("QLineEdit {border-width: 1px;padding: 1px; border-style: solid;border-color: #e1e1e1;border-radius:1px; background-color:red; selection-color: #0a214c;selection-background-color: #C19A6B;min-height:24px; color:rgb(225,225,225);}QLineEdit:!enabled {border-width: 1px;padding: 1px; border-style: solid;border-color: #e1e1e1;border-radius:1px;  background-color: rgb(216,216,216); selection-color: #0a214c;selection-background-color: #C19A6B;min-height:24px; color:rgb(126,126,126);}");//±³¾°É«
	}

	void ParaBasicSetup::SetControlValidStyle(QLineEdit *edit, bool b)
	{
		QPalette paRed;
		paRed.setColor(QPalette::WindowText, Qt::red);
		QPalette paBlack;
		paBlack.setColor(QPalette::WindowText, Qt::black);

		if (b)
			//edit->setPalette(paBlack);
			edit->setStyleSheet("color: black;");
		else
			//edit->setPalette(paRed);
			edit->setStyleSheet("color: red;");
	}

	void ParaBasicSetup::OnTxtEmailTextChanged(QString text)
	{
		QString emailRegexp = "^[A-Za-z0-9\u4e00-\u9fa5]+@[a-zA-Z0-9_-]+(\.[a-zA-Z0-9_-]+)+$";
		//QString emailRegexp = "^[a-z0-9A-Z]+[- | a-z0-9A-Z . _]+@([a-z0-9A-Z]+(-[a-z0-9A-Z]+)?\\.)+[a-z]{2,}$";
		QRegExp regExp(emailRegexp);
		
		if (regExp.exactMatch(text))
		{
			//ui->txtEmail->setStyleSheet(_styleOk);
			SetControlValidStyle(ui->txtEmail, true);
			_emailIsValid = true;
		}
		else
		{
			//ui->txtEmail->setStyleSheet(_styleError);
			SetControlValidStyle(ui->txtEmail, false);
			_emailIsValid = false;
		}
	}
	
	void ParaBasicSetup::OnTxtWebSiteTextChanged(QString text)
	{
		QString strUrlExp = "((http|https|ftp)://|(www)\\.)(\\w+)(\\.?[\\.a-z0-9/:?%&=\\-_+#;]*)";
		QRegExp regExp(strUrlExp);
		
		if (regExp.exactMatch(text))
		{
			//ui->txtWedSite->setStyleSheet(_styleOk);
			SetControlValidStyle(ui->txtWedSite, true);
			_websiteisValid = true;
		}
		else
		{
			//ui->txtWedSite->setStyleSheet(_styleError);
			SetControlValidStyle(ui->txtWedSite, false);
			_websiteisValid = false;
		}
	}

	QString ParaBasicSetup::GetLogoFileName()
	{
		return _logoFileName;
	}

	QString ParaBasicSetup::GetWelcomeFileName()
	{
		return _welcomeFileName;
	}

	void ParaBasicSetup::SetLogoFileName(QString fileName)
	{
		_logoFileName = fileName;
		ShowImage(ui->lbl_logo, _logoFileName);
	}

	void ParaBasicSetup::SetWelcomeFileName(QString fileName)
	{
		_welcomeFileName = fileName;
		ShowImage(ui->lbl_welcome, _welcomeFileName);
	}

	int ParaBasicSetup::IsDataOk()
	{
		QString chnName = ui->txtNameCn->text();
		QString engName = ui->txtNameEn->text();
		if (chnName.isEmpty())
			return ChnNameIsEmpty;
		
		if (engName.isEmpty())
			return EngNameIsEmpty;

		if (!_emailIsValid)
			return EmailNameInvalid;

		if (!_websiteisValid)
			return WebsiteNameInvalid;

		if (!InputValidator::getInstance()->FileChnNameIsAllow(chnName))
			return ChnNameIsError;

		if (!InputValidator::getInstance()->FileEngNameIsAllow(engName))
			return EngNameIsError;

		return 0;
	}

	void ParaBasicSetup::OnBtnOkClicked()
	{
		int errorCode = IsDataOk();
		
		if (0 != errorCode)
		{
			//QString errorMsg = _errorList[errorCode];
			//QMessageBox messageBox(QMessageBox::NoIcon, tr("Error"), errorMsg, QMessageBox::Yes, NULL);;
			//messageBox.exec();
			//return;
			QString errorMsg = _errorList[errorCode];
			ui->lbl_info->setText(errorMsg);
			ui->lbl_info->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}
		UpdateUiToData();

		//xuxinwei 20200324
		QFileInfo logoIcon(_logoFileName);
		QFileInfo welcomeIcon(_welcomeFileName);
		//qDebug() << logoIcon.fileName() << welcomeIcon.fileName();
		if (!DataManager::getInstance()->getIconNameIsAvailable(logoIcon.fileName()))
		{
			ui->lbl_info->setText(tr("Logo icon file is already existed."));
			ui->lbl_info->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		DataManager::getInstance()->setIconNameList(logoIcon.fileName());

		if (!DataManager::getInstance()->getIconNameIsAvailable(welcomeIcon.fileName()))
		{
			ui->lbl_info->setText(tr("Welcome page icon file is already existed."));
			ui->lbl_info->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}
	
		DataManager::getInstance()->setIconNameList(welcomeIcon.fileName());
		//xuxinwei 20200324
		
		// added by libaojun @ 2019/11/26
		QString title = ui->txtNameEn->text();
		const QString lang = Setting::BusAPI::instance()->getLanguage();
		if (lang.toLower() == "chinese") title = ui->txtNameCn->text();
		//_mainWindow->setWindowTitle(title);
		_mainWindow->setWindowTitle(title + " ---- " + tr("Customizing"));
		QFileInfo logoFileInfo(_logoFileName);
		
		if (logoFileInfo.exists())
		{
			_mainWindow->setWindowIcon(QIcon(_logoFileName));
			_subWindow->setIcon(_logoFileName);
		}

		//xuxinwei 20200305
		QString web = ui->txtWedSite->text().trimmed();
		//qDebug() << web;
		_subWindow->openUrl(web);


		this->accept();
		close();
	}

	//¶¨Ê±Æ÷²Ûº¯Êý
	void ParaBasicSetup::OnTimeout()
	{
		ui->lbl_info->setText("");
		ui->lbl_info->hide();
	}


	void ParaBasicSetup::OnBtnCancelClicked()
	{
		close();
	}

	void ParaBasicSetup::OnBtnOpenLogoClicked()
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open logo file"), "", tr("Images (*.png *.jpg)"));
		if (fileName.isEmpty())
			return;
		ShowImage(ui->lbl_logo, fileName);
		_logoFileName = fileName;
	}

	void ParaBasicSetup::OnBtnOpenWelcomeClicked()
	{
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open welcom file"), "", tr("Images (*.png *.jpg)"));
		if (fileName.isEmpty())
			return;
		ShowImage(ui->lbl_welcome, fileName);
		_welcomeFileName = fileName;
	}

	void ParaBasicSetup::ShowImage(QLabel *widget, QString fileName)
	{
		QImage Image;
		bool b = Image.load(fileName);
		
		if (!b)
		{
			return;
		}

		QPixmap pixmap = QPixmap::fromImage(Image);
		int with = widget->width();
		int height = widget->height();
		QPixmap fitpixmap = pixmap.scaled(with, height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);  // ±¥ÂúÌî³ä
		//QPixmap fitpixmap = pixmap.scaled(with, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // °´±ÈÀýËõ·Å
		widget->setPixmap(fitpixmap);
	}

}