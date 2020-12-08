#ifndef _GEOMETRYCONFIGDATA_H_
#define _GEOMETRYCONFIGDATA_H_

#include "ConfigOptionsAPI.h"
#include <QString>

namespace ConfigOption
{
	class CONFIGOPTIONSAPI GeometryConfig
	{
	public:
		GeometryConfig();
		~GeometryConfig() = default;

		void clearData();

		bool isGeometryEnabled();
		bool isImportGeometryEnabled();
		bool isExportGeometryEnabled();
		//Geometry modeling operations sketch
		void enableGeometryModeling(bool on);	//设置Geometry modeling是否可用
		bool isGeometryModelingEnabled();		//获取Geometry modeling是否可用
		void enableGeometryOperations(bool on);	//设置Geometry operations是否可用
		bool isGeometryOperationsEnabled();		//获取Geometry operations是否可用
		void enableCreateSketch(bool on);		//设置Geometry sketch是否可用
		bool isCreateSketchEnabled();			//获取Geometry sketch是否可用
		void setImportSuffix(QString s);		//设置导入几何格式
		QString getImportSuffix();				//获取导入几何格式
		void setExportSuffix(QString s);		//设置导出几何格式
		QString getExportSuffix();				//获取导出几何格式

	protected:
	private:
// 		bool _isGeoOn{ false };
// 		bool _isimportGeo{ false };
// 		QString _suffix{};
// 		bool _isCreateGeo{ false };

		bool _isGeoModeling{ false };	//Geometry modeling
		bool _isGeoOperations{ false };	//Geometry operations
		bool _isCreateSketch{ false };	//Create Sketch

		QString _importSuffix{};		//importSuffix
		QString _exportSuffix{};		//exportSuffix
	};

}


#endif