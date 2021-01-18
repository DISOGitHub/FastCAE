#ifndef GETMODELITEMICONFACTORY_H
#define GETMODELITEMICONFACTORY_H

#include <QIcon>


namespace FastCAEDesigner
{
	class GetModelItemIconFactory
	{
	public:
		static QIcon Get(int modelItemType);
	};
}

#endif
