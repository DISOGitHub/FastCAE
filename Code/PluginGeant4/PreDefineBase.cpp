#include "PreDefineBase.h"

namespace Geant4
{
	
	void PreDefineBase::setName(const QString &name)
	{
		_name = name;
		this->appendProperty("Name", name);
	}

	int PreDefineBase::getDim()
	{
		return _dim;
	}

	DataType PreDefineBase::getType()
	{
		return _type;
	}

	 
}