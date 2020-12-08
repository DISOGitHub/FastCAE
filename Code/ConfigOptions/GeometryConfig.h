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
		void enableGeometryModeling(bool on);	//����Geometry modeling�Ƿ����
		bool isGeometryModelingEnabled();		//��ȡGeometry modeling�Ƿ����
		void enableGeometryOperations(bool on);	//����Geometry operations�Ƿ����
		bool isGeometryOperationsEnabled();		//��ȡGeometry operations�Ƿ����
		void enableCreateSketch(bool on);		//����Geometry sketch�Ƿ����
		bool isCreateSketchEnabled();			//��ȡGeometry sketch�Ƿ����
		void setImportSuffix(QString s);		//���õ��뼸�θ�ʽ
		QString getImportSuffix();				//��ȡ���뼸�θ�ʽ
		void setExportSuffix(QString s);		//���õ������θ�ʽ
		QString getExportSuffix();				//��ȡ�������θ�ʽ

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