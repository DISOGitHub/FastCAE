#ifndef _VALUE3D_H_
#define _VALUE3D_H_

#include <QObject> 
#include <QString>

namespace  Geant4
{
	class Value;

	class Value3D : public QObject
	{
		Q_OBJECT
	public:
		Value3D();
		~Value3D();

		void getValue(double* v);
		void getValue(QStringList& list);

		void setName(QString name);
		QString getName();

		void setUnit(QString unit);
		QString getUnit();

		void setValue(double x, double y, double z);
		void setValue(QString x, QString y, QString z);

		

		Value* getValueX();
		Value* getValueY();
		Value* getValueZ();

	private:
		Value* _x{};
		Value* _y{};
		Value* _z{};
		QString  _name{};
		QString  _unit{};

	};
}


#endif