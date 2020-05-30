/****************************
���������ļ��Ķ�д
���� ���� ���������
libaojun
*********************** */ 

#ifndef PROJECTFILEIO_H
#define PROJECTFILEIO_H

#include "moduleBase/IOBase.h"
#include "IOAPI.h"





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
	class IOAPI ProjectFileIO : public ModuleBase::IOBase
	{
	public:
		ProjectFileIO(const QString& fileName);
		ProjectFileIO();
		~ProjectFileIO() = default;

		bool read() override;
		bool write(QString s = QString()) override;
		
	private:
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
	};
}


#endif
