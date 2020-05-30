#ifndef _DATACONFIGREADER_H_
#define _DATACONFIGREADER_H_

#include <QString>
//#include "moduleBase/IOBase.h"
#include <QFile>
#include "DataProperty/modelTreeItemType.h"

class QDomElement;
class QDomDocument;

namespace ConfigOption
{
	class DataConfig;
	class PostConfig;
	
	class DataConfigReader //: public ModuleBase::IOBase
	{
	public:
		DataConfigReader(const QString fileName, DataConfig* dataconfig, PostConfig* postconfig);
		~DataConfigReader();

		bool read() /*override*/;

	private:
		void readDataBlock(ProjectTreeType t, QDomElement* e);
		void readPostConfig(ProjectTreeType t, QDomElement* e);

	private:
		DataConfig* _dataConfig{};
		PostConfig* _postConfig{};
		QDomDocument* _doc{};
		QFile _file{};

	};

}

#endif