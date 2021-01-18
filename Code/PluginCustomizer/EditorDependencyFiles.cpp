#include "EditorDependencyFiles.h"
#include "ui_EditorDependencyFiles.h"
#include <QFileDialog>
#include <QMessageBox>

namespace FastCAEDesigner{
	EditorDependencyFiles::EditorDependencyFiles(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorDependencyFiles)
	{
		ui->setupUi(this);

		//ui->listWidget->setCurrentRow(0);

		connect(ui->SelectFilesPBtn, SIGNAL(clicked()), this, SLOT(OnSelectPBtnClicked()));
		connect(ui->deletePBtn, SIGNAL(clicked()), this, SLOT(OnDeletePBtnClicked()));
		connect(ui->ClearPBtn, SIGNAL(clicked()), this, SLOT(OnClearAllPBtnClicked()));
		connect(ui->OkPBtn, SIGNAL(clicked()), this, SLOT(OnOkPBtnClicked()));
		connect(ui->CancelPBtn, SIGNAL(clicked()), this, SLOT(close()));
	}

	EditorDependencyFiles::EditorDependencyFiles(QString fileName, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorDependencyFiles),
		_fileNames(fileName)
	{
		ui->setupUi(this);

		//ui->listWidget->setCurrentRow(0);

		init();

		connect(ui->SelectFilesPBtn, SIGNAL(clicked()), this, SLOT(OnSelectPBtnClicked()));
		connect(ui->deletePBtn, SIGNAL(clicked()), this, SLOT(OnDeletePBtnClicked()));
		connect(ui->ClearPBtn, SIGNAL(clicked()), this, SLOT(OnClearAllPBtnClicked()));
		connect(ui->OkPBtn, SIGNAL(clicked()), this, SLOT(OnOkPBtnClicked()));
		connect(ui->CancelPBtn, SIGNAL(clicked()), this, SLOT(close()));
	}

	EditorDependencyFiles::~EditorDependencyFiles()
	{
		_selectedFiles.clear();
		delete ui;
	}

	void EditorDependencyFiles::init()
	{
		if (_fileNames.isEmpty())
			return;

		QStringList names = _fileNames.split(";");
		
		for (int i = 0; i < names.count(); i++)
		{
			QListWidgetItem* item = new QListWidgetItem;
			item->setText(names.at(i));
			ui->listWidget->addItem(item);
		}

		UpdateFileNames();//Added xvdongming 2020-02-14
	}

	void EditorDependencyFiles::OnSelectPBtnClicked()
	{
		QFileDialog *selectFile = new QFileDialog(this);

		selectFile->setWindowTitle(tr("Dependency Files Select"));
		selectFile->setDirectory("");
		selectFile->setNameFilter(tr("All Files (*.*)"));
		selectFile->setFileMode(QFileDialog::ExistingFiles);
		selectFile->setViewMode(QFileDialog::Detail);

		QStringList fileNames;
		if (selectFile->exec())
		{
			fileNames = selectFile->selectedFiles();
		}

		for (int i = 0; i < fileNames.count(); i++)
		{
			//if (_selectedFiles.contains(fileNames.at(i))) //Modified:xvdongming 2020-02-14
			if (_fileNames.contains(fileNames.at(i)))
				continue;
			
			QListWidgetItem* item = new QListWidgetItem;
			item->setText(fileNames.at(i));
			ui->listWidget->addItem(item);
			_selectedFiles.append(fileNames.at(i));
		}
	}

	void EditorDependencyFiles::OnDeletePBtnClicked()
	{
		//int rowIndex = ui->listWidget->currentRow();
		ui->listWidget->takeItem(ui->listWidget->currentRow());
		UpdateFileNames();//Added xvdongming 2020-02-14
	}

	void EditorDependencyFiles::OnClearAllPBtnClicked()
	{
// 		QMessageBox::StandardButton result = QMessageBox::information(this, tr("Clear All!"), tr("Whether to clear all of the list?"),
// 			QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

		QMessageBox msgBox(QMessageBox::Information, tr("Clear All!"), tr("Whether to clear all of the list?"));
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
		msgBox.setButtonText(QMessageBox::Yes, tr("Yes"));
		msgBox.setButtonText(QMessageBox::No, tr("No"));

		int result = msgBox.exec();

		switch (result)
		{
		case QMessageBox::Yes:
			ui->listWidget->clear();
			break;
		default:
			break;
		}

		UpdateFileNames();//Added xvdongming 2020-02-14
	}

	void EditorDependencyFiles::OnOkPBtnClicked()
	{
		/*
		const int n = ui->listWidget->count();
		QStringList filenames;
		for (int i = 0; i < n; i++)
		{
			QString name = ui->listWidget->item(i)->text().trimmed();
			filenames.append(name);
		}

		_fileNames = filenames.join(";");
		*/
		UpdateFileNames();//Added xvdongming 2020-02-14
		this->accept();
		close();
	}

	QString EditorDependencyFiles::getFiles()
	{
		return _fileNames;
	}

	//Added xvdongming 2020-02014 刷新依赖文件列表
	void EditorDependencyFiles::UpdateFileNames()
	{
		int num = ui->listWidget->count();
		
		if (0 == num)
		{
			_fileNames = "";
			return;
		}
		
		QStringList filenames;

		for (int i = 0; i < num; i++)
		{
			QString name = ui->listWidget->item(i)->text().trimmed();
			
			if (0 == name.length())
				continue;

			filenames.append(name);
		}

		_fileNames = filenames.join(";");
	}

}
