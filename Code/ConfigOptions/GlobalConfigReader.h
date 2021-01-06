#include "moduleBase/IOBase.h"
#include <QString>
#include <QFile>

class QDomElement;
class QDomDocument;

namespace ConfigOption
{
	class GeometryConfig;
	class GlobalConfig;
	class MeshConfig;
	class PostConfig;
	class ConfigDataBase;

	class GlobalConfigReader
	{
	public:
		GlobalConfigReader(QString fileName, ConfigDataBase* data);
		~GlobalConfigReader();

		bool read();

	private:
		bool readGlobal(QDomElement* e);
		bool readGeometry(QDomElement* e);
		bool readMesh(QDomElement* e);
		bool readPost(QDomElement* e);

	private:
		GlobalConfig* _globalConfig{};
		GeometryConfig* _geoConfig{};
		MeshConfig* _meshConfig{};
		PostConfig* _postConfig{};

		QFile _file{};
		QDomDocument* _doc{};
	};

}