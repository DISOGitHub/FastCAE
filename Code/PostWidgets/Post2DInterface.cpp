#include "Post2DInterface.h"
#include "ui_Post2DInterface.h"
#include "Post2DWidget.h"
#include <QFile>
#include <QTextStream>

namespace Post
{
	Post2DWindowInterface::Post2DWindowInterface(GUI::MainWindow* m, int proid) :
		PostWindowBase(m, proid, ModuleBase::Post2D, D2), _ui(new Ui::Post2DInterface)
	{
		_ui->setupUi(this);
		_post2DWindow = new Post2DWindow(m, proid);
		appendGraphWindow(_post2DWindow);
		_ui->plotLayout->addWidget(_post2DWindow);
		setPropWidget(_post2DWindow->getPropWidget());
		setTreeWidget(_post2DWindow->getTreeWidget());
	}
	Post2DWindowInterface::~Post2DWindowInterface()
	{
	//	delete _post2DWindow;
		delete _ui;
	}

	Post2DWindow* Post2DWindowInterface::getWindow()
	{
		return _post2DWindow;
	}
	void Post2DWindowInterface::reTranslate()
	{
		_post2DWindow->reTranslate();
	}

	QString Post2DWindowInterface::readLine()
	{
		if (_stream != nullptr)
		{
			while (!_stream->atEnd())
			{
				QString l = _stream->readLine().simplified();
				if (!l.isEmpty()) return l;
			}

		}
		return QString();
	}
	QVector<double> Post2DWindowInterface::getData(QString f, QString va)
	{
		QList<QString> filenames = _values.keys();
		QString filenamekey;
		for (int i = 0; i < filenames.size(); ++i)
		{
			QString file = filenames.at(i);
			if (file.endsWith(f))
			{
				filenamekey = file;
				break;
			}
		}
		if (filenamekey.isEmpty())
			return QVector<double>();

		return _values[filenamekey].value(va);
	}
	void Post2DWindowInterface::openfile(QString f)
	{
		QFile file(f);
		if (!file.exists()) return;
		if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
			return;
		if (_values.contains(f))
			_values.remove(f);
		_stream = new QTextStream(&file);
		QString line = this->readLine();
		QStringList variableName = line.split(" ");

		while (!_stream->atEnd())
		{
			line = this->readLine();
			QStringList sdata = line.split(" ");
			if (sdata.size() != variableName.size()) continue;
			for (int i = 0; i < sdata.size(); ++i)
			{
				QString name = variableName.at(i);
				QString sd = sdata.at(i);
				double d = sd.toDouble();
				_values[f][name].append(d);
			}
		}
		file.close();
		delete _stream;
		_stream = nullptr;
	}
	void Post2DWindowInterface::addCurve(QString name, QString f, QString x, QString y)
	{
		_post2DWindow->addCurve(getData(f,x), getData(f,y), name);
	}
	void Post2DWindowInterface::removeCurve(QString name)
	{
		_post2DWindow->delCurve(name,1);
	}

	void Post2DWindowInterface::saveImage(QString fileName, int width, int heigh, bool showDlg)
	{
		_post2DWindow->saveImage(fileName, width, heigh, showDlg);
	}

}