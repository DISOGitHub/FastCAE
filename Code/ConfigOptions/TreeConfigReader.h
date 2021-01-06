/*读取模型树的Config*/

#ifndef TREECONFIGREADER_H
#define TREECONFIGREADER_H

#include <QString>
#include "ConfigOptionsAPI.h"
#include <QFile>
//#include "moduleBase/IOBase.h"

class QDomDocument;
class QDomElement;

namespace ConfigOption
{
	class ProjectTreeConfig;
	class ProjectTreeInfo;

	class CONFIGOPTIONSAPI TreeConfigReader /*: public ModuleBase::IOBase*/
	{
	public:
		TreeConfigReader(const QString &fileName, ProjectTreeConfig* treeconfig);
		~TreeConfigReader();
		bool read();

	private:
		void readTree(QDomElement*);
		void readChildNode(ProjectTreeInfo* tree, QDomElement* ele);

	private:
		ProjectTreeConfig* _info{};
		QDomDocument* _doc{};
		QFile _file{};
	};

} 

#endif