#ifndef _MATERIALCONFIG_H
#define _MATERIALCONFIG_H_

#include "ConfigOptionsAPI.h"
#include <QHash>
#include <QStringList>

namespace Material
{
	class Material;
}

namespace ConfigOption
{
	class CONFIGOPTIONSAPI MaterialConfig
	{
	public:
		MaterialConfig() = default;
		~MaterialConfig();

		QStringList getMaterialTypes();
		Material::Material* getMaterialByType(QString t);
		void clearData();

		bool readConfig(QString file);

	private:
		QHash<QString, Material::Material*> _materialHash{};

	};

}




#endif