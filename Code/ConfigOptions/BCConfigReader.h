#ifndef _BCCONFIGREADER_H_
#define _BCCONFIGREADER_H_

#include <QFile>
#include <QString>
//#include "moduleBase/IOBase.h"
#include "DataProperty/modelTreeItemType.h"

class QDomDocument;
class QDomElement;


namespace ConfigOption
{
	class BCConfig;

	class BCConfigReader //: public ModuleBase::IOBase
	{
	public:
		BCConfigReader(const QString fileName, BCConfig* bcconfig);
		~BCConfigReader();
		
		bool read()/* override*/;

	private:
		void readBCType(QDomElement* ele, ProjectTreeType t);
		void readBC(QDomElement* ele, ProjectTreeType t);
		
	private:
		BCConfig* _bcconfig{};
		QDomDocument* _doc{};
		
		QFile _file;
	};

}

#endif