#include "EditorPathValue.h"
#include "ui_EditorPathValue.h"
#include "DataProperty/ParameterPath.h"
#include "InputValidator.h"
#include <QDebug>
#include <QTimer>
#include <QFileDialog>

namespace FastCAEDesigner{
	EditorPathValue::EditorPathValue(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorPathValue)
	{
		ui->setupUi(this);
		Init();
	}
	EditorPathValue::EditorPathValue(DataProperty::ParameterPath* model, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorPathValue),
		_model(model)
	{
		ui->setupUi(this);
		Init();
	}
	EditorPathValue::~EditorPathValue()
	{
		delete ui;
		_usedNameList.clear();
	}

	void EditorPathValue::setNameUsedList(QList<QString> list)
	{
		_usedNameList = list;
	}
	void EditorPathValue::Init()
	{
		UpdateDataToUi();
	//	UpdateUiDisplay(false);
		InitErrorList();
		connect(ui->OkPBtn, SIGNAL(clicked()), this, SLOT(OnOkPBtnClicked()));
		connect(ui->CancelPBtn, SIGNAL(clicked()), this, SLOT(close()));
		connect(ui->TypeCB, SIGNAL(currentIndexChanged(int)), this, SLOT(OnComboxChanged(int)));
		connect(ui->PathPBtn, SIGNAL(clicked()), this, SLOT(OnPathPBtnClicked()));
	}
	void EditorPathValue::UpdateUiDisplay(bool l)
	{
		ui->label_3->setVisible(l);
		ui->FIleSuffixLE->setVisible(l);
		ui->label_4->setVisible(l);
		ui->ValueLE->setText("");//Added xvdongming 2020-02-13 在切换路径类型时，清除原来的数据，避免数据错误。
	}
	void EditorPathValue::InitErrorList()
	{
		_errorList.insert(NameIsEmpty, tr("Name is empty."));
		_errorList.insert(TheNameInUse, tr("The name is already in use."));
		_errorList.insert(FileSuffixIsEmpty, tr("File suffix is empty."));
		_errorList.insert(FileSuffixIsNotSure, tr("File suffix format is not correct,please follow *.dat"));
		_errorList.insert(IllegalName, tr("The name is illegal string."));
	}
	
	void EditorPathValue::UpdateDataToUi()
	{
		int type;
		QString s;
		ui->NameLE->setText(_model->getDescribe());

		switch (_model->getType())
		{
		case DataProperty::PathType::None:
		case DataProperty::PathType::Path:
			UpdateUiDisplay(false);
			type = 0;
			s = _model->getPath();
			break;
		case DataProperty::PathType::File:
			UpdateUiDisplay(true);
			type = 1;
			s = _model->getFile();
			break;
		case DataProperty::PathType::FileList:
			UpdateUiDisplay(true);
			type = 2;
			s = _model->getFileList().join(";");
			break;
		default:
			break;
		}

		ui->TypeCB->setCurrentIndex(type);

		int suffixError = IsFileSuffixSure(_model->getSuffix());
		QString suffix;
		if (suffixError == 0)
			suffix = _model->getSuffix();
		ui->FIleSuffixLE->setText(suffix);

		ui->ValueLE->setText(s);		

	}
	void EditorPathValue::UpdateUiToData()
	{
		_model->setDescribe(ui->NameLE->text());
		int type = ui->TypeCB->currentIndex();
		_model->setType(DataProperty::PathType(type + 1));
		if (type == 0)
		{
			//_model->setSuffix(ui->FIleSuffixLE->text());
			_model->setPath(ui->ValueLE->text());
		}
		else if (type == 1)
		{
			_model->setSuffix(ui->FIleSuffixLE->text());
			_model->setFile(ui->ValueLE->text());
		}
		else if (type == 2)
		{
			_model->setSuffix(ui->FIleSuffixLE->text());
			_model->setFileList(ui->ValueLE->text().split(";"));
		}
		
	}
	void EditorPathValue::OnComboxChanged(int index)
	{
		//qDebug() << index;
		if (index != 0)
		{
			UpdateUiDisplay(true);
		}
		else
		{
			UpdateUiDisplay(false);
		}
	}
	
	int EditorPathValue::IsNameSure()
	{
		QString name = ui->NameLE->text().trimmed();

		if (_usedNameList.contains(name))
			return TheNameInUse;

		if (name.isEmpty())
			return NameIsEmpty;

		if (InputValidator::getInstance()->FileNameIsAllow(name) == false)
			return IllegalName;

		return 0;
	}
	int EditorPathValue::IsFileSuffixSure()
	{
		QString suffix = ui->FIleSuffixLE->text().trimmed();

		if (suffix.isEmpty())
			return FileSuffixIsEmpty;

		if ((suffix.length() < 3) || (suffix.toLatin1().data()[0] != '*') || (suffix.toLatin1().data()[1] != '.'))
		{
			//qDebug() << suffix.length() << suffix.toLatin1().data()[0] << suffix.toLatin1().data()[1];
			return FileSuffixIsNotSure;
		}
			
		return 0;
	}
	int EditorPathValue::IsFileSuffixSure(QString suffix)
	{
		if (suffix.isEmpty())
			return FileSuffixIsEmpty;

		if ((suffix.length() < 3) || (suffix.toLatin1().data()[0] != '*') || (suffix.toLatin1().data()[1] != '.'))
		{
			//qDebug() << suffix.length() << suffix.toLatin1().data()[0] << suffix.toLatin1().data()[1];
			return FileSuffixIsNotSure;
		}

		return 0;
	}
	void EditorPathValue::OnOkPBtnClicked()
	{
		int type = ui->TypeCB->currentIndex();
		int nameError = IsNameSure();
		
		if (nameError != 0)
		{
			QString errorMsg = _errorList[nameError];
			ui->ErrorText->setText(errorMsg);
			ui->ErrorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		if (type != 0)
		{
			int suffixError = IsFileSuffixSure();

			if (suffixError != 0)
			{
				QString errorMsg = _errorList[suffixError];
				ui->ErrorText->setText(errorMsg);
				ui->ErrorText->show();
				QTimer::singleShot(3000, this, SLOT(OnTimeout()));
				return;
			}
		}

		UpdateUiToData();
		this->accept();
		close();
	}
	void EditorPathValue::OnPathPBtnClicked()
	{
		if ((ui->TypeCB->currentIndex() == 1) || (ui->TypeCB->currentIndex() == 2))
		{
			int suffixError = IsFileSuffixSure();

			if (suffixError != 0)
			{
				QString errorMsg = _errorList[suffixError];
				ui->ErrorText->setText(errorMsg);
				ui->ErrorText->show();
				QTimer::singleShot(3000, this, SLOT(OnTimeout()));
				return;
			}
		}

		QFileDialog *selectFile = new QFileDialog(this);
		QString suffixName = QString("*.*");

		selectFile->setWindowTitle(tr("Select File"));
		selectFile->setDirectory(QCoreApplication::applicationDirPath());

		if (ui->FIleSuffixLE->text() != "")
			suffixName = ui->FIleSuffixLE->text();

		selectFile->setNameFilter(suffixName);

		if (ui->TypeCB->currentIndex() == 0)
			selectFile->setFileMode(QFileDialog::Directory);
		else if (ui->TypeCB->currentIndex() == 1)
			selectFile->setFileMode(QFileDialog::ExistingFile);
		else if (ui->TypeCB->currentIndex() == 2)
			selectFile->setFileMode(QFileDialog::ExistingFiles);
		
		selectFile->setViewMode(QFileDialog::Detail);

		QStringList fileNames;
		if (selectFile->exec())
		{
			fileNames = selectFile->selectedFiles();
			ui->ValueLE->setText(fileNames.join(";"));
		}

	}
	void EditorPathValue::OnTimeout()
	{
		ui->ErrorText->setText("");
		ui->ErrorText->hide();
	}
}