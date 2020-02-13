#ifndef CONFIGDATABASE_H
#define CONFIGDATABASE_H

#include "ConfigOptionsAPI.h"

namespace ConfigOption
{
	class BCConfig;
	class DataConfig;
	class GlobalConfig;
	class GeometryConfig;
	class MeshConfig;
	class PostConfig;
	class SolverOption;
	class ObserverConfig;
	class MaterialConfig;
	class ProjectTreeConfig;

	class CONFIGOPTIONSAPI ConfigDataBase
	{
	public:
		ConfigDataBase() = default;
		~ConfigDataBase() = default;

		virtual BCConfig* getBCConfig() = 0;
		virtual DataConfig* getDataConfig() = 0;
		virtual GeometryConfig* getGeometryConfig() = 0;
		virtual GlobalConfig* getGlobalConfig() = 0;
		virtual MeshConfig* getMeshConfig() = 0;
		virtual PostConfig* getPostConfig() = 0;
		virtual SolverOption* getSolverOption() = 0;
		virtual ObserverConfig* getObseverConfig() = 0;
		virtual MaterialConfig* getMaterialConfig() = 0;
		virtual ProjectTreeConfig* getProjectTreeConfig() = 0;

		virtual void clearAllConfig() = 0;
	};


}



#endif