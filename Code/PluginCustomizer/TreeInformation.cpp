#include "TreeInformation.h"
#include "ui_TreeInformation.h"
#include "ModelBase.h"
#include <QFileDialog>
#include <QTimer>
#include "InputValidator.h"-
#include "DataManager.h"
#include <QFileInfo>


namespace FastCAEDesigner{
	TreeInformation::TreeInformation(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::TreeInformation)
	{
		ui->setupUi(this);
		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
		connect(ui->btnLoadIcon, SIGNAL(clicked()), this, SLOT(OnBtnLoadIconClicked()));
		InitErrorList();

		QFileInfo icon(ui->txtIcon->text().trimmed());
		DataManager::getInstance()->removeIconNameFromList(icon.fileName());

		//_iconNameList = DataManager::getInstance()->getIconNameList();
		//Init();
	}

	TreeInformation::TreeInformation(QTreeWidgetItem* treeItem, ModelBase* model, QWidget* parent) :
		QDialog(parent),
		ui(new Ui::TreeInformation),
		_model(model),
		_treeItem(treeItem)
	{
		ui->setupUi(this);
		Init();
		InitErrorList();
	}

	TreeInformation::~TreeInformation()
	{
		delete ui;
		_errorList.clear();
		_usedChnNameList.clear();
		_usedEngNameList.clear();
	}

	void TreeInformation::Init()
	{
		UpdateDataToUi();
		SetIsEdit(_model->GetIsEdit());

		//20200324 xuxinwei
		QFileInfo icon(ui->txtIcon->text().trimmed());
		DataManager::getInstance()->removeIconNameFromList(icon.fileName());
		//20200324 xuxinwei

		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
		connect(ui->btnLoadIcon, SIGNAL(clicked()), this, SLOT(OnBtnLoadIconClicked()));
	}

	void TreeInformation::UpdateDataToUi()
	{
		if (nullptr == _model)
			return;

		ui->txtChineseName->setText(_model->GetChnName());
		ui->txtEnglishName->setText(_model->GetEngName());
		ui->txtIcon->setText(_model->GetIconName());
	}

	void TreeInformation::UpdateUiToData()
	{
		if (nullptr == _model)
			return;

		_model->SetChnName(ui->txtChineseName->text());
		_model->SetEngName(ui->txtEnglishName->text());
		_model->SetIconName(ui->txtIcon->text());
	}

	void TreeInformation::SetIsEdit(bool b)
	{
		ui->txtChineseName->setEnabled(b);
		ui->txtEnglishName->setEnabled(b);
		ui->txtIcon->setEnabled(b);
		ui->btnLoadIcon->setEnabled(b);
	}

	void TreeInformation::OnBtnOkClicked()
	{
		/*
		if (_workMode == EditMode)
		{
			UpdateUiToData();
			_treeItem->setText(0, ui->txtChineseName->text());
		}
		
		this->accept();
		close();
		*/

		//int errorCode = IsDataOk();
		QString nameChn = ui->txtChineseName->text().trimmed();
		QString nameEng = ui->txtEnglishName->text().trimmed();
		QString msg = "";
		int errorCode = InputValidator::getInstance()->ValidateName(nameChn, nameEng, _usedChnNameList, _usedEngNameList, msg);

		if (0 != errorCode)
		{
			//QString errorMsg = _errorList[errorCode];
			//ui->lbl_info->setText(errorMsg);
			ui->lbl_info->setText(msg);
			ui->lbl_info->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		//xuxinwei 20200324
		QFileInfo iconName(ui->txtIcon->text().trimmed());
		if (!DataManager::getInstance()->getIconNameIsAvailable(iconName.fileName()))
		{
			ui->lbl_info->setText(tr("The icon file is already existed."));
			ui->lbl_info->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}
		
		DataManager::getInstance()->setIconNameList(iconName.fileName());
		//xuxinwei 20200324

		UpdateUiToData();
		
		if (nullptr != _treeItem)
		{
			_treeItem->setText(0, ui->txtChineseName->text());
		}
		
		this->accept();
		close();
	}
	/*
	void TreeInformation::CreateRootOkClicked()
	{
		int errorCode = IsDataOk();

		if (0 != errorCode)
		{
			QString errorMsg = _errorList[errorCode];
			ui->lbl_info->setText(errorMsg);
			ui->lbl_info->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		this->accept();
		close();
	}

	void TreeInformation::CreateChildOkClicked()
	{

	}

	void TreeInformation::EditOkClicked()
	{

	}
	*/
	void TreeInformation::OnBtnLoadIconClicked()
	{
		QString suffixName = QString("*.*");
		QString file_name = QFileDialog::getOpenFileName(NULL, tr("Open image file"), ".", suffixName);
		ui->txtIcon->setText(file_name);
	}

	void TreeInformation::setNodeName(QString ChineseName, QString EnglishName)
	{
		/*ui->ChineseName->setText(ChineseName);
		ui->EnglishName->setText(EnglishName);*/
	}
	void TreeInformation::setNodeIcon(QString Icon)
	{
		/*ui->Icon->setText(Icon);*/
	}

	void TreeInformation::setNodeEnable()
	{
		/*ui->label->setEnabled(false);
		ui->label_3->setEnabled(false);
		ui->label_2->setEnabled(false);
		ui->ChineseName->setEnabled(false);
		ui->EnglishName->setEnabled(false);
		ui->Icon->setEnabled(false);*/
	}

	QString TreeInformation::GetChineseName()
	{
		return ui->txtChineseName->text().trimmed();
	}

	QString TreeInformation::GetEnglishName()
	{
		return ui->txtEnglishName->text().trimmed();
	}

	QString TreeInformation::GetIconName()
	{
		return ui->txtIcon->text().trimmed();
	}

	void TreeInformation::SetWorkModel(EWorkMode mode)
	{
		_workMode = mode;
		
		if (mode == EWorkMode::CreateRootNodeMode)
		{
			ui->lbl_Icon->hide();
			ui->txtIcon->hide();
			ui->btnLoadIcon->hide();
		}
	}

	void TreeInformation::SetUsedChnNameList(QList<QString> nameList)
	{
		_usedChnNameList = nameList;
	}

	void TreeInformation::SetUsedEngNameList(QList<QString> nameList)
	{
		_usedEngNameList = nameList;
	}
	//初始化错误代码对应的错误信息列表
	void  TreeInformation::InitErrorList()
	{
		_errorList.insert(ChnNameIsEmpty, tr("Chinese name is empty."));
		_errorList.insert(EngNameIsEmpty, tr("English name is empty."));
		_errorList.insert(TheChnNameInUse, tr("Chinese name is already in use"));
		_errorList.insert(TheEngNameInUse, tr("English name is already in use"));
		_errorList.insert(NameNotAllow, tr("Special characters in name"));
		_errorList.insert(ChnNameNotAllow, tr("Special characters in  chinese name"));
		_errorList.insert(EngNameNotAllow, tr("Special characters in  english name"));

	}

	//校验数据设定是否正确，根据错误的状况返回响应的错误代码
	int TreeInformation::IsDataOk()
	{
		QString nameChn = ui->txtChineseName->text().trimmed();
		QString nameEng = ui->txtEnglishName->text().trimmed();

		if (nameChn.isEmpty())
			return ChnNameIsEmpty;

		if (nameEng.isEmpty())
			return EngNameIsEmpty;

		if (_usedChnNameList.contains(nameChn))
			return TheChnNameInUse;

		if (_usedEngNameList.contains(nameEng))
			return TheEngNameInUse;

		if (!FileNameIsAllow(nameChn))
			return ChnNameNotAllow;
		
		if (!FileNameIsAllow(nameEng))
			return EngNameNotAllow;

		return 0;
	}
	//定时器槽函数
	void TreeInformation::OnTimeout()
	{
		ui->lbl_info->setText("");
		ui->lbl_info->hide();
	}

	bool TreeInformation::FileNameIsAllow(QString fileName)
	{
		if (fileName.isEmpty())
			return false;

		bool isOk = true;
		QString pattern = "[\\\\/:*?\"<>., ;'{|`~!@#$%&*()=+\\^}\\[\\]]";

		QRegExp rx(pattern);
		int match = fileName.indexOf(rx);

		if (match >= 0)
			isOk = false;

		return isOk;
	}
}

