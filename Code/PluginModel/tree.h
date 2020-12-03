#ifndef PROJECTTREEPLUG_IN_H_
#define PROJECTTREEPLUG_IN_H_

#include "ProjectTree/ProjectTreeWithBasicNode.h"
#include "ModelPluginAPI.h"

namespace GUI
{
	class MainWindow;
}

namespace ProjectTree
{
	class MODELPLUGINAPI PluginTree : public ProjectTreeWithBasicNode
	{
	public:
		PluginTree(GUI::MainWindow* m);
		~PluginTree() = default;
	};

}


#endif