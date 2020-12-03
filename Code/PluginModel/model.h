#ifndef __MODEL_H_
#define __MODEL_H_

#include "ModelPluginAPI.h"
#include "ModelData/modelDataBaseExtend.h"


namespace ModelData
{
	class MODELPLUGINAPI ModelDataPlugin : public ModelDataBaseExtend
	{
	
	public:
		ModelDataPlugin(ProjectTreeType t);
		~ModelDataPlugin() = default;

	};
}


#endif