#ifndef _CONSTANT_H
#define _CONSTANT_H

#include "PreDefineBase.h"

namespace Geant4 
{
	class Value;

	class Constant : public PreDefineBase
	{
	public:
		Constant();
		~Constant();

		void readGDML(QDomElement & ele) override;
		void writeGDML(QDomDocument& doc, QDomElement& ele) override;

		void setValue(QString v);
		void getValue(double* v) override;
		QString getValue();
//		QString getExpression();

	private:
		Value* _value{};
	};
}
#endif // _CONSTANT_H

