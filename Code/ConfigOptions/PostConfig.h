#ifndef _POSTCONFIG_H_
#define _POSTCONFIG_H_

#include "ConfigOptionsAPI.h"
#include "DataProperty/modelTreeItemType.h"
#include <QHash>

namespace ConfigOption
{
	class PostConfigInfo;

	class CONFIGOPTIONSAPI PostConfig
	{
	public:
		PostConfig();
		~PostConfig() = default;

		void enablePost(bool on);
		bool isPostEnabled();
		void enablePost2D(bool on);
		bool isPost2DEnabled();
		void enablePost3D(bool on);
		bool isPost3DEnabled();
		void setPostConfigInfo(ProjectTreeType type, PostConfigInfo* info);
		PostConfigInfo* getPostConfigInfo(ProjectTreeType type);
		void clearData();

	private:
		bool _enablePost{ false };
		bool _enablePost2D{ false };
		bool _enablePost3D{ false };

		QHash<ProjectTreeType, PostConfigInfo*> _postConfigInfo{};
//		QHash<ProjectTreeType, double> _post3DConfig{};

	};


}

#endif
