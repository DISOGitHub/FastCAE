#ifndef _MESHCONFIG_H_
#define _MESHCONFIG_H_

#include "ConfigOptionsAPI.h"
#include <QString>

namespace ConfigOption
{

	class CONFIGOPTIONSAPI MeshConfig
	{
	public:
		MeshConfig();
		~MeshConfig() = default;
		void clearData();

		bool isMeshEnabled();

		bool isImportMeshEnabled();
		void setImportSuffix(QString s);
		QString getImportSuffix();

		bool isExportMeshEnabled();
		void setExportSuffix(QString s);
		QString getExportSuffix();

		bool isMeshGenerationEnabled();
		void enableSurfaceMesh(bool on);
		bool isSurfaceMeshEnabled();
		void enableSolidMesh(bool on);
		bool isSolidMeshEnabled();

		void enableComponent(bool on);
		bool isComponentEnabled();

		void setCheckMesh(bool on);		//设置检测网格质量
		bool getCheckMeshEnabled();			//获取检测网格质量状态

		void enableFluidMesh(bool on);
		bool isFluidMeshEnabled();

		void enableFilterMesh(bool on);
		bool isFilterMeshEnabled();

		void enableMeshModeling(bool on);
		bool isMeshModelingEnabled();
		
	private:
// 		bool _enableMesh{ false };
// 		bool _importMesh{ false };
		QString _importSuffix{};
//		bool _exportMesh{ false };
		QString _exportSuffix{};

// 		bool _enableMeshGeneration{ false };
// 		bool _isGmsher{ false };
		bool _enableSurfaceMesh{ false };
		bool _enableSolidMesh{ false };
//		bool _enableFluidMesh{ false };
		bool _enableComponent{ false };

		bool _checkMesh{ false };		//check mesh

		bool _fluidMesh{ false };
		bool _filterMesh{ false };
		bool _meshModeling{ false };
		

	};

}


#endif
