#ifndef _PREDEFINEBASE_H_
#define _PREDEFINEBASE_H_

#include "DataProperty/DataBase.h"

class QDomElement;

namespace Geant4
{
	enum DataType
	{
		UnKnow = 0,
		ConstantType,
		PositionType,
		RotationType,
		QuantityType,
		ExpressionType,
	};


	class PreDefineBase : public DataProperty::DataBase
	{
	public:
		PreDefineBase() = default;
		~PreDefineBase() = default;

		void setName(const QString &name) override;
		int getDim();
		DataType getType();

		virtual void readGDML(QDomElement & ele) = 0;
		virtual void writeGDML(QDomDocument& doc, QDomElement& ele) = 0;
		virtual void getValue(double* v) = 0;

	protected:
		int _dim{ 1 };
		DataType _type{ UnKnow };
		
	};
}


#endif
