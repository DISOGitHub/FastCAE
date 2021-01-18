#include "MaterialConfig.h"
#include "Material/Material.h"
#include <QStringList>
#include <QFile>
#include <QDomDocument>
#include <QDomNodeList>
#include <QDomElement>

namespace ConfigOption
{
	MaterialConfig::~MaterialConfig()
	{
		clearData();
	}

	QStringList MaterialConfig::getMaterialTypes()
	{
		return _materialHash.keys();
	}

	Material::Material* MaterialConfig::getMaterialByType(QString t)
	{
		return _materialHash.value(t);
	}

	bool MaterialConfig::readConfig(QString fileName)
	{
		QFile file(fileName);
		if (!file.open(QFile::ReadOnly | QFile::Text)) return false;
		QDomDocument doc;
		if (!doc.setContent(&file))
		{
			file.close();
			return false;
		}
		QDomNodeList materiallist = doc.elementsByTagName("Material");
		const int n = materiallist.size();
		for (int i = 0; i < n; ++i)
		{
			QDomElement maele = materiallist.at(i).toElement();
			Material::Material* m = new Material::Material(false);
			m->readDataFromProjectFile(&maele);
			QString type = m->getType();
			if (_materialHash.contains(type))
			{
				auto ma = _materialHash.value(type);
				delete ma;
			}
			_materialHash[type] = m;
		}

		file.close();
		return true;

	}

	void MaterialConfig::clearData()
	{
		QList<Material::Material*> materials = _materialHash.values();
		for (int i = 0; i < materials.size(); ++i)
		{
			Material::Material* m = materials.at(i);
			delete m;
		}
		_materialHash.clear();
	}




}