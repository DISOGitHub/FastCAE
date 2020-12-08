#include "modelDataSingleton.h"
#include "modelDataBase.h"
#include <QString>
#include <assert.h>
#include <QDataStream>
#include <QCryptographicHash>
#include <QDomElement>
#include <QDomDocument>
#include <QDebug>
#include <QFile>
#include <QTextCodec>
namespace ModelData
{
	ModelDataSingleton* ModelDataSingleton::_instance = nullptr;

	ModelDataSingleton::ModelDataSingleton()
	{
	}

	ModelDataSingleton::~ModelDataSingleton()
	{
		clear();
	}

	ModelDataSingleton* ModelDataSingleton::getinstance()
	{
		if (_instance == nullptr)
		{
			_instance = new ModelDataSingleton;
		}
		return _instance;
	}

	void ModelDataSingleton::appendModel(ModelDataBase* model)
	{
		_modelList.append(model);
	}

	int ModelDataSingleton::getModelCount()
	{
		return _modelList.size();
	}

	ModelDataBase* ModelDataSingleton::getModelAt(const int index)
	{
		assert(index >= 0 && index < _modelList.size());
		return _modelList.at(index);
	}

	QString ModelDataSingleton::getMD5()
	{
		const int n = _modelList.size();
		if (n < 1) return "";
		
// 		QFile file("test.dat");
// 		if (!file.open(QIODevice::WriteOnly))
// 		{
// 			printf("can't open file for write\n");
// 			exit(1);
// 		}
		
		QDataStream stream/*(&file)*/;
		for (int i = 0; i < _modelList.size(); ++i)
		{
			_modelList[i]->dataToStream(&stream);
		}
		
		char* s;
//		QString a;
		stream >> s;

// 		QByteArray string = s;
// 		QCryptographicHash *hash = new QCryptographicHash(QCryptographicHash::Md5);
// 		hash->addData(string);
// 		QByteArray string1 = hash->result();
// 		QTextCodec *codec = QTextCodec::codecForName("KOI8-R");
// 		QString string2 = codec->toUnicode(string1);
// 
// 		qDebug() << "baisuishankuangquanshui" << sizeof(stream);
// 		file.close();

		QString md5 = QCryptographicHash::hash(s, QCryptographicHash::Md5);
		//return md5;
		return md5;
	}

	ModelDataBase* ModelDataSingleton::getModelByID(const int id)
	{
		const int n = _modelList.size();
		for (int i = 0; i < n; ++i)
		{
			ModelDataBase* m = _modelList.at(i);
			if (m->getID() == id)
				return m;
		}
		return nullptr;
	}

	ModelData::ModelDataBase* ModelDataSingleton::getModelByName(QString name)
	{
		for (auto m : _modelList)
		{
			if (m->getName() == name)
				return m;
		}
		return nullptr;
	}

	QDomElement& ModelDataSingleton::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement modelElement = doc->createElement("ModelData");
		const int nmodel = _modelList.size();
		for (int i = 0; i < nmodel; ++i)
		{
			ModelDataBase* model = _modelList.at(i);
#ifdef Q_OS_WIN
			model->writeToProjectFile(doc, &modelElement);
#else
			QDomElement modelEle = doc->createElement("Model");
			model->writeToProjectFile1(doc,&modelEle);
			modelElement.appendChild(modelEle);
#endif
		}

		parent->appendChild(modelElement);
		return modelElement;
	}

	void ModelDataSingleton::clear()
	{
		const int n = _modelList.size();
		for (int i = 0; i < n; ++i)
		{
			ModelDataBase* d = _modelList.at(i);
			delete d;
		}
		_modelList.clear();
		ModelDataBase::resetMaxID();
	}

	void ModelDataSingleton::removeModelByID(const int id)
	{
		ModelDataBase* dataBase = getModelByID(id);
		if (dataBase == nullptr) return;
		const int index = _modelList.indexOf(dataBase);
		_modelList.removeAt(index);
		delete dataBase;
	}

	int ModelDataSingleton::getModelIDByIndex(const int index)
	{
		const int n = _modelList.size();
		if (index >= n || index < 0) return -1;
		ModelDataBase* data = _modelList.at(index);
		const int id = data->getID();
		return id;
	}
}