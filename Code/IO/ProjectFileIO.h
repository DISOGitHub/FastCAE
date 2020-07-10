/****************************
整个工程文件的读写
几何 网格 物理参数等
libaojun
*********************** */ 

#ifndef PROJECTFILEIO_H
#define PROJECTFILEIO_H

#include "moduleBase/IOBase.h"
#include "moduleBase/ThreadTask.h"
#include "IOAPI.h"

namespace GUI
{
	class MainWindow;
	class SignalHandler;
}

namespace Geometry
{
	class GeometryData;
}
namespace MeshData
{
	class MeshData;
}
namespace ModelData
{
	class ModelDataSingleton;
}
namespace Material
{
	class MaterialSingleton;
}
namespace Plugins
{
	class PluginManager;
}

class QDomNodeList;

namespace IO
{
	class IOAPI ProjectFileIO : public ModuleBase::ThreadTask, public ModuleBase::IOBase
	{
		Q_OBJECT
	public:
		ProjectFileIO(GUI::MainWindow* mw, GUI::SignalHandler*sh,  const QString& fileName, bool read = true);
		~ProjectFileIO() = default;

		void run() override;

	signals:
		void processFinished(QString f, bool, bool);

	private:
		bool read() override;
		bool write(QString s = QString()) override;
		
	
		bool writeXml(QString fileName);
		bool writeDiso(QString fileName);

		bool readXml(QString fileName);
		bool readDiso(QString fileName);

		void readModelData(QDomNodeList* nodeList);
		void readGeoData(QDomNodeList* nodeList, bool isdiso = false);
		void readMeshData(QDomNodeList* nodeList);
		void readMaterialData(QDomNodeList* nodelist);
		void readPluginData(QDomNodeList* nodeList);

	private:
		Geometry::GeometryData* _geoData{};
		MeshData::MeshData* _meshData{};
		ModelData::ModelDataSingleton* _modelData{};
		Material::MaterialSingleton* _materialData{};
		Plugins::PluginManager* _plugins{};

		bool _read{ true };
	};
}


#endif
