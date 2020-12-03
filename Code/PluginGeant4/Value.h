#ifndef VALUEBASE_H
#define VALUEBASE_H

#include <QObject>
#include <QString>


namespace Geant4
{
	class Value : public QObject
	{
		Q_OBJECT
	public:
		Value() = default;
		Value(double v);
		~Value();

		double getValue();
		void setValue(double v);

		void setName(QString name);
		QString getName();

		void setUnit(QString unit);
		QString getUnit();

		void setReferance(QString ref);
		QString getReference();

		void isReference(bool is);
		bool isReference();

		void setType(QString t);
		QString getType();

	private:
		double transformPI();

	private:
		double _value{ 0 };
		bool _isRef{ false };
		QString  _constantRef{};
		QString _name{};
		QString _unit{};
		QString _type{};
	};

}


#endif