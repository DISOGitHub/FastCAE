#ifndef _EXPRESSION_H
#define _EXPRESSION_H

#include "PreDefineBase.h"

namespace Geant4 
{
	class Quantity;

	class Expression : public PreDefineBase
	{
	public:
		Expression();
		~Expression();

		void setTimes(double times);
		double getTimes();
		void setOperator(QString ope);
		QString getOperator();
		void setQuantity(Quantity* q);
		Quantity* getQuantity();

		void readGDML(QDomElement & ele) override;
		void writeGDML(QDomDocument& doc, QDomElement& ele) override;
		void getValue(double *v);

		void setExpression(QString exp);
		QString getExpression();

	private:
		void handleExpression(QString s);

	private:
		double m_times;
		QString m_operator;
		Quantity* m_quantity;

		QString m_expression;
	};
}
#endif // _EXPRESSION_H

