#include "ParaPath.h"
#include "DataProperty/ParameterPath.h"
#include <QFileDialog>
#include <QDebug>

namespace SelfDefObj
{

	ParaPath::ParaPath(GUI::MainWindow * m, DataProperty::ParameterBase* p)
		: ParaMore(m, p), SelfDefParaWidgetBase(p)
	{
		_data = (DataProperty::ParameterPath*)p;
		updateLineEdit();
		_inited = true;
	}

	void ParaPath::on_moreButton_clicked()
	{
		DataProperty::PathType t = _data->getType();
		_file.clear();
		_fileList.clear();
		switch (t)
		{
		case DataProperty::File:
			_file = QFileDialog::getOpenFileName(this, tr("Select a file"), "", _data->getSuffix());
			if (_file.isEmpty()) return;
//			_data->setFile(file);
			break;
		case  DataProperty::Path:
			_file = QFileDialog::getExistingDirectory(this, tr("Select a directory"), "");
			if (_file.isEmpty()) return;
//			_data->setPath(file);
			break;
		case DataProperty::FileList:
			_fileList = QFileDialog::getOpenFileNames(this, tr("Select files"), "", _data->getSuffix());
			if (_fileList.isEmpty()) return;
//			_data->setFileList(filelist);
			break;
		default: break;
		}

		submitCode();
		emit dataChanged(_data);
		updateLineEdit();
	}

	void ParaPath::updateLineEdit()
	{
		DataProperty::PathType t = _data->getType();
		QString path;
		QStringList filelist;
		switch (t)
		{
		case DataProperty::File:
			path = _data->getFile();
			break;
		case  DataProperty::Path:
			path = _data->getPath();
			break;
		case  DataProperty::FileList:
			filelist = _data->getFileList();
			for (int i = 0; i < filelist.size(); ++i)
			{
				path.append(filelist.at(i));
				path.append(";");
			}
			path.resize(path.size() - 1);
			break;
		}
		qDebug() << path;
		this->setText(path);
		
	}

	QString ParaPath::generateCode()
	{
		QString code = SelfDefParaWidgetBase::generateCode();
		QString text;
		if (_fileList.isEmpty())
			text = _file;
		else
			text = _fileList.join(",");
		code.replace("%Value%", text);
		return code;
	}

	
}