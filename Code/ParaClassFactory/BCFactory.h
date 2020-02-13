#ifndef _BCFACTORY_H_
#define _BCFACTORY_H_

#include "ParaClassFactoryAPI.h"
#include "BCBase/BCType.h"
#include "DataProperty/modelTreeItemType.h"

namespace BCBase
{
	class BCBase;
}

namespace ParaClassFactory
{

	class PARACLASSFACTORYAPI BCFactory
	{
	public:
		BCFactory() = default;
		~BCFactory() = default;

		static BCBase::BCBase* createBCByText(QString text);
		static BCBase::BCBase* createBCByType(BCBase::BCType type, QString name, ProjectTreeType t);


	};


}


#endif