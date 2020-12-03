#include "Expression.h"
#include "Quantity.h"
#include <QDomElement>
#include <QStringList>

namespace Geant4 
{

	Expression::Expression()
	{
		_type = DataType::ExpressionType;
		Quantity* m_quantity = new Quantity;
	}

	Expression::~Expression()
	{
		delete m_quantity;
	}

	void Expression::setTimes(double times)
	{
		m_times = times;
	}

	double Expression::getTimes()
	{
		return m_times;
	}

	void Expression::setOperator(QString ope)
	{
		m_operator = ope;
	}

	QString Expression::getOperator()
	{
		return m_operator;
	}

	void Expression::setQuantity(Quantity* q)
	{
		m_quantity = q;
	}

	Geant4::Quantity* Expression::getQuantity()
	{
		return m_quantity;
	}

	void Expression::readGDML(QDomElement & ele)
	{
		QString name = ele.attribute("name");
		this->setName(name);
		QString text = ele.text();
		this->setExpression(text);
	}

	void Expression::writeGDML(QDomDocument& doc, QDomElement& ele)
	{

	}

	void Expression::getValue(double *v)
	{
		//TODO
	}

	void Expression::setExpression(QString exp)
	{
		m_expression = exp;
	}

	QString Expression::getExpression()
	{
		return m_expression;
	}

	void Expression::handleExpression(QString s)
	{
		QStringList list;
		list.append("+");
		list.append("-");
		list.append("*");
		list.append("/");

		for (auto opt : list)
		{
			if (s.contains(opt)) 
			{
				QStringList exp = s.split(opt);
				
			}
		}

	}

}