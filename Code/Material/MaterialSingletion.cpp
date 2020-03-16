#include "MaterialSingletion.h"
#include "Material.h"
#include <QMessageBox>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QFile>
#include <QDomProcessingInstruction>
#include <QTextStream>
#include <QApplication>
#include <QDebug>
#include "DialogLoadMaterial.h"
#include "MaterialFactory.h"
#include "DialogRemoveMaterial.h"

namespace Material
{
	MaterialSingleton* MaterialSingleton::_instance = nullptr;

	MaterialSingleton* MaterialSingleton::getInstance()
	{
		if (_instance == nullptr)
			_instance = new MaterialSingleton;
		return _instance;
	}

	void MaterialSingleton::clear()
	{
		const int n = _materialList.size();
		for (int i = 0; i < n; ++i)
		{
			Material* m = _materialList.at(i);
			delete m;
		}
		_materialList.clear();
	}

	int MaterialSingleton::getMaterialCount()
	{
		return _materialList.size();
	}

	Material* MaterialSingleton::getMaterialAt(const int i)
	{
		Material* m = nullptr;
		if (i>=0 && i<_materialList.size())
		{
			m = _materialList.at(i);
		}
		return m;
	}

	Material* MaterialSingleton::getMaterialByID(const int id)
	{
		Material* ma = nullptr;
		const int n = _materialList.size();
		for (int i = 0; i < n; ++i)
		{
			Material* m = _materialList.at(i);
			if (m->getID() == id)
				ma = m;
		}
		return ma;
	}

	void MaterialSingleton::appendMaterial(Material* m)
	{
		if (m != nullptr)
			_materialList.append(m);
	}

	QDomElement& MaterialSingleton::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement mroot = doc->createElement("Materials");
		const int n = _materialList.size();
		for (int i = 0; i < n; ++i)
		{
			Material* m = _materialList.at(i);
			m->writeToProjectFile(doc, &mroot);
		}
		parent->appendChild(mroot);
		return mroot;
	}

	void MaterialSingleton::readDataFromProjectFile(QDomElement* ele)
	{
		QDomNodeList malist = ele->elementsByTagName("Material");
		const int n = malist.size();
		for (int i = 0; i < n; ++i)
		{
			QDomElement maele = malist.at(i).toElement();
			QString type = maele.attribute("Type");
			Material* m = MaterialFactory::createMaterial(type);
			if (m == nullptr)
				m = new Material;
			m->readDataFromProjectFile(&maele);
			this->appendMaterial(m);
		}
	}

	void MaterialSingleton::removeMaterialByID(const int id)
	{
		const int n = _materialList.size();
		for (int i = 0; i < n; ++i)
		{
			Material* m = _materialList.at(i);
			if (m->getID() == id)
			{
				_materialList.removeAt(i);
				break;
			}
		}
	}

	void MaterialSingleton::appendToMaterialLib(const int id)
	{
		Material* m = this->getMaterialByID(id);
		if (m == nullptr) return;

		QHash<QString, Material*> mahash = this->loadMaterilLib();
		qDebug() << mahash.keys();
		qDebug() << m->getName();
		if (mahash.contains(m->getName()))
		{
			if (QMessageBox::Yes == QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Material has already exist, replace?"), QMessageBox::Yes | QMessageBox::No))
			{
				Material* tm = mahash.value(m->getName());
				if (tm != nullptr) delete tm;
				mahash.remove(m->getName());
			}
			else return;
		}
		Material* nm = new Material(false);
		nm->copy(m);
		mahash[m->getName()] = nm;
		this->writeToMaterialLib(mahash);
		
	}

	QHash<QString, Material*> MaterialSingleton::loadMaterilLib()
	{
		QHash<QString, Material*> malib;
		QString path = qApp->applicationDirPath() + "/MaterialLib.xml";

		QFile lib(path);
		if (!lib.open(QIODevice::Text | QIODevice::ReadOnly)) return malib;


		QDomDocument doc;
		if (!doc.setContent(&lib))
		{
			return malib;
		}
		QDomNodeList materiallist = doc.elementsByTagName("Material");
		const int nm = materiallist.size();
		for (int i = 0; i < materiallist.size(); ++i)
		{
			QDomElement ele = materiallist.at(i).toElement();
			Material* m = new Material(false);
			m->readDataFromProjectFile(&ele);
			QString name = m->getName();
			malib[name] = m;
		}

		lib.close();
		return malib;
	}

	void MaterialSingleton::writeToMaterialLib(QHash<QString, Material*> ms)
	{
		QString path = qApp->applicationDirPath() + "/MaterialLib.xml";
		QFile lib(path);
		if (!lib.open(QIODevice::Text | QIODevice::WriteOnly)) return;

		QDomDocument doc;
		QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(instruction);
		QDomElement root = doc.createElement("MaterialLib");
		doc.appendChild(root);
		QList<Material*> mlist = ms.values();
		for (int i = 0; i < mlist.size(); ++i)
		{
			Material*m = mlist.at(i);
			m->writeToProjectFile(&doc, &root);
			delete m;
		}

		QTextStream* stream = new QTextStream(&lib);
		doc.save(*stream, 4);
		lib.close();
		delete stream;
	}

	void MaterialSingleton::loadFromMaterialLib(GUI::MainWindow* m)
	{
		QHash<QString, Material*> mahash = this->loadMaterilLib();
		LoadMaterialDialog dlg(m, mahash);
		dlg.exec();
		QList<Material*> ml = mahash.values();
		for (int i = 0; i < ml.size(); ++i)
		{
			Material* m = ml.at(i);
			delete m;
		}
		mahash.clear();

	}

	void MaterialSingleton::removeFromMAterialLib(GUI::MainWindow* m)
	{
		QHash<QString, Material*> mahash = this->loadMaterilLib();
		const int no = mahash.size();
		RemoveMaterialDialog d(m, &mahash);
		d.exec();
		const int n = mahash.size();
		if (no != n)
		{
			this->writeToMaterialLib(mahash);
			return;
		}
		QList<Material*> ml = mahash.values();
		int nnn = ml.size();
		for (int i = 0; i < ml.size(); ++i)
		{
			Material* m = ml.at(i);
			delete m;
		}
		mahash.clear();
	}

}