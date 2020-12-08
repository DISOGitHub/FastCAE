/****************************
写出文件供求解器调用
*********************** */ 

#ifndef SOLVERINFOWRITER_H
#define SOLVERINFOWRITER_H

#include "moduleBase/IOBase.h"
#include "IOAPI.h"
#include <QList>

namespace ModelData
{
	class ModelDataBase;
}
namespace Setting
{
	class SolveOption;
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

namespace IO
{
	class IOAPI SolverInfoWriter : public ModuleBase::IOBase
	{
	public:
		SolverInfoWriter(const QString& fileName);
		SolverInfoWriter();
		~SolverInfoWriter() = default;

		///设置名称，不包含路径 eg 123.txt
//		void setName(const QString filenam);
		void appendModel(ModelData::ModelDataBase *model);
		bool write(QString dir = QString()) override;
//		void setFileType(FileType type);
		void xml2json_model(QString fileName);
		void xml2json_material(QString fileName);
		void xml2json_geometry(QString fileName);
		void xml2json_meshdata(QString fileName);
		void write_xml_model();
		void write_xml_material();
		void write_xml_geometry();
		void write_xml_meshdata();
	private:
		bool writeSingleModel();

	private:
		QList<ModelData::ModelDataBase*> _modelList{};
		QString _name{};
		bool  _isTextType{ true };
		bool isjson{ false };//判断用户选择的是xml文件还是json文件输出类型，默认为false就是xml
		Setting::SolveOption* _solveOption{};
	private:
		Geometry::GeometryData* _geoData{};
		MeshData::MeshData* _meshData{};
		ModelData::ModelDataSingleton* _modelData{};
		Material::MaterialSingleton* _materialData{};
		
		QDomDocument* _doc_model{};
		QDomDocument* _doc_material{};
		QDomDocument* _doc_geometry{};
		QDomDocument* _doc_meshdata{};
	};
}


#endif
