#include "WriterBase.h"
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "DataManager.h"
#include "FileHelper.h"
#include <QFileInfo>

namespace FastCAEDesigner
{
	WriterBase::WriterBase()
	{
	}


	WriterBase::~WriterBase()
	{
	}

	QString WriterBase::GetFileName()
	{
		return QCoreApplication::applicationDirPath() + "//..//ConfigFiles//" + _fileName;
	}

	bool WriterBase::SaveFile(QDomDocument &doc)
	{
		qDebug() << GetFileName();
		QFile file(GetFileName());

		if (!file.open(QFile::WriteOnly | QFile::Truncate /*| QFile::Text*/)) 
		{
			qDebug() << "WriterBase::SaveFile():Create file failed.";
			return false;
		}

		QTextStream out_stream(&file);
		doc.save(out_stream, 4); //缩进1格
		file.close();
		return true;
	}

	QMap<QString, QString> WriterBase::insertQString(QStringList var, QStringList val)
	{
		QMap<QString, QString> map;

		for (int i = 0; i < var.count(); i++)
		{
			map.insert(var.at(i), val.at(i));
		}
		return map;
	}

	//保存图标到系统icon目录下，并返回不包含目录的文件名称。
	QString WriterBase::SaveIconToSystem(QString fileName)
	{
		QString destPath = FileHelper::GetSystemConfigPath() + "icon/";
		QFileInfo fileInfo(fileName);
		QString destFileName = fileInfo.fileName();
		DataManager::getInstance()->CopyFileToSystem(fileName, destPath);
		return destFileName;
	}
}