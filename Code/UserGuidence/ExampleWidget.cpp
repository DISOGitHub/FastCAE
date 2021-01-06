#include "ExampleWidget.h"
#include "ui_ExampleWidget.h"
#include "python/PyAgent.h"
#include <QFile>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>
#include <QDebug>

namespace Guidence
{
	ExampleWidget::ExampleWidget(QString path, QWidget* p)
		:QWidget(p), _path(path), _parent(p)
	{
		_ui = new Ui::ExampleWidget;
		_ui->setupUi(this);
		_agent = Py::PythonAagent::getInstance();
		this->init();
		_ui->detailsButton->setVisible(false);
	}

	ExampleWidget::~ExampleWidget()
	{
		if (_ui != nullptr) delete _ui;
	}

	void ExampleWidget::init()
	{
		QFile tipfile(_path + "/tip.txt");
		if (!tipfile.exists()) return;
		if (!tipfile.open(QIODevice::ReadOnly)) return;
		QString s = tipfile.readAll();
		tipfile.close();
		_ui->piclabel->setToolTip(s);
	}

	void ExampleWidget::showEvent(QShowEvent *e)
	{
		QWidget::showEvent(e);
		QSize s = _ui->piclabel->size();
		QImage image;
		image.load(_path+"/pic.png");
		QPixmap pixmap = QPixmap::fromImage(image);
		QPixmap fit = pixmap.scaled(s, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		_ui->piclabel->setPixmap(fit);

	}

	void ExampleWidget::on_detailsButton_clicked()
	{
		QFile detail(_path + "/details.pdf");
		if (!detail.exists())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Detail File is not exist !"));
			return;
		}
		QDesktopServices::openUrl(QUrl::fromLocalFile(_path + "/details.pdf"));
	}

	void ExampleWidget::on_viewButton_clicked()
	{
		QFile viewpy(_path + "/view.py");
		if (!viewpy.exists())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Script File is not exist !"));
			return;
		}
		
		if (!viewpy.open(QIODevice::ReadOnly)) return;

		QString line;
		while (!viewpy.atEnd())
		{
			line = viewpy.readLine().simplified();
			if (line.isEmpty()) continue;
			if (line.contains("%examplesPath%"))
				line.replace("%examplesPath%", _path);

			_agent->submit(line);
		}

		if (_parent != nullptr)
			_parent->close();
	}



	void ExampleWidget::on_openButton_clicked()
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile(_path));
	}

}