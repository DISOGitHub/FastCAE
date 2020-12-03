#ifndef _CONFIGOPTIONS_H_
#define _CONFIGOPTIONS_H_

#include "ConfigDataBase.h"

namespace ConfigOption
{
	class CONFIGOPTIONSAPI ConfigOption : public ConfigDataBase
	{
	public:
		static ConfigOption* getInstance();

		bool isGeometryEnabled();
		bool isMeshEnabled();
		bool isPostEnabled();

		BCConfig* getBCConfig() override;
		DataConfig* getDataConfig() override;
		GeometryConfig* getGeometryConfig() override;
		GlobalConfig* getGlobalConfig() override;
		MeshConfig* getMeshConfig() override;
		PostConfig* getPostConfig() override;
		SolverOption* getSolverOption() override;
		ObserverConfig* getObseverConfig()override;
		MaterialConfig* getMaterialConfig()override;
		ProjectTreeConfig* getProjectTreeConfig()override;
//		NodeFormConfig* getNodeFormConfig()override;

		void clearAllConfig()override;
//		QString readConfig();

	private:
		ConfigOption();
		~ConfigOption();
	
	private:
		static ConfigOption* _instance;

		BCConfig* _bcConfig{};
		DataConfig* _dataConfig{};
		GlobalConfig* _globalConfig{};
		GeometryConfig* _geoConfig{};
		MeshConfig* _meshConfig{};
		PostConfig* _postConfig{};
		SolverOption* _solverOption{};
		ObserverConfig* _observerConfig{};
		MaterialConfig* _materialConfig{};
		ProjectTreeConfig* _projectTreeConfig{};
//		NodeFormConfig* _nodeFormConfig{};
	};

}



#endif