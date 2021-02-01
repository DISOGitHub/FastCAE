#include "MaterialFactory.h"
#include "Material.h"

namespace Material
{
	QHash<QString, CREATEMATERIAL> MaterialFactory::_typeFunHash = QHash<QString, CREATEMATERIAL>();

	void MaterialFactory::registerType(QString type, CREATEMATERIAL f)
	{
		_typeFunHash.insert(type, f);
	}

	void MaterialFactory::removeType(QString type)
	{
		_typeFunHash.remove(type);
	}

	Material* MaterialFactory::createMaterial(QString type)
	{
		auto fun = _typeFunHash.value(type);
		if (fun == nullptr) return nullptr;
		Material* ma = fun(type);
		if (ma == nullptr) return ma;
		ma->setType(type);
		return  ma;
	}

	QStringList MaterialFactory::getRegTypes()
	{
		return _typeFunHash.keys();
	}

}


