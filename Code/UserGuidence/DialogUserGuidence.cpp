#include "DialogUserGuidence.h"
#include "mainWindow/mainWindow.h"
#include "ui_DialogUserGuidence.h"
#include "settings/busAPI.h"
#include "ExampleWidget.h"
#include <QDesktopServices>
#include <QUrl>
#include <QDir>
#include <QFileInfo>
#include <QImage>
#include <QDebug>
#include <QAction>

namespace Guidence
{
	UserGuidenceDialog::UserGuidenceDialog(GUI::MainWindow* mw, QAction* ac)
		:QDialog(mw), _action(ac)
	{
		_ui = new Ui::UserGuidenceDialog;
		_ui->setupUi(this);
		init();
	}

	UserGuidenceDialog::~UserGuidenceDialog()
	{
		for (auto w : _examples) delete w;
		_examples.clear();

		if (_ui != nullptr) delete _ui;
	}

	void UserGuidenceDialog::init()
	{
		_ui->backButton->setVisible(false);
		_ui->stackedWidget->setCurrentIndex(0);

		QString examplePath = QApplication::applicationDirPath() + "/../examples/";
		qDebug() << examplePath;
		QDir exampledri(examplePath);
		if (!exampledri.exists()) return;
		exampledri.setFilter(QDir::Dirs);
		QFileInfoList folderlist = exampledri.entryInfoList();
		const int n = folderlist.size();
		
		int exampleCount = 0;
		for (int i = 0; i < n; ++i)
		{
			QFileInfo fileInfo = folderlist.at(i);
			if (fileInfo.fileName() == "." | fileInfo.fileName() == "..") continue;
			
			QString path = fileInfo.absoluteFilePath();
			QLayout* l = _ui->example_L;
			if (0 != exampleCount % 2) l = _ui->example_R;
			exampleCount++;
			QWidget* w = new ExampleWidget(path, this);
			l->addWidget(w);
			_examples.append(w);
		}

	}

	void UserGuidenceDialog::closeEvent(QCloseEvent * e)
	{
		bool show = !_ui->notshowBox->isChecked();
		if (_action != nullptr)
			_action->setChecked(show);
		Setting::BusAPI::instance()->isEnableUserGuidence(show);
		QDialog::closeEvent(e);
		delete this;
	}

	void UserGuidenceDialog::showEvent(QShowEvent * e)
	{
		QDialog::showEvent(e);
		QSize s = _ui->picLabel->size();
		QImage image;
		image.load("://QUI/icon/FastCAEFrame.png");
		QPixmap pixmap = QPixmap::fromImage(image);
		QPixmap fit = pixmap.scaled(s, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		_ui->picLabel->setPixmap(fit);
	}

	void UserGuidenceDialog::on_backButton_clicked()
	{
		_ui->stackedWidget->setCurrentIndex(0);
		_ui->notshowBox->setVisible(true);
		_ui->backButton->setVisible(false);
	}

	void UserGuidenceDialog::on_closeButton_clicked()
	{
		this->close();
	}

	void UserGuidenceDialog::on_documentButton_clicked()
	{
		QDesktopServices::openUrl(QUrl("http://www.fastcae.com/document.html"));
	}

	void UserGuidenceDialog::on_exampleButton_clicked()
	{
		_ui->stackedWidget->setCurrentIndex(1);
		_ui->notshowBox->setVisible(false);
		_ui->backButton->setVisible(true);
	}

}



