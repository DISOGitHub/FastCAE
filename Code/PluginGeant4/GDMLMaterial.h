#ifndef _GDMLMATERIAL_H_
#define _GDMLMATERIAL_H_

#include "DataProperty/DataBase.h" 
#include <QString>

namespace  Geant4
{
	struct Fraction
	{
		double n;
		QString ref;
	};
	struct MaterialType
	{
		QString type;
		QString unit;
		double value;
	};
	struct Composite
	{
		double n;
		QString ref;
	};

	class Value;
	
	class GDMLMaterial : public DataProperty::DataBase
	{
		Q_OBJECT
	public:
		GDMLMaterial() ;
		~GDMLMaterial() ;

		void setName(const QString &name);

		void setState(QString state);
		QString getState();
		void setFormula(QString f);
		QString getFormula();
		void setZ(QString Z);
		QString getZ();

		void appendMaterialType(MaterialType m);
		void appendFraction(Fraction fra);
		void appendComposite(Composite com);

		QList<MaterialType> getMaterialTypeList();
		MaterialType getMaterialTypeByIndex(int index);
		int getMaterialTypeCount();

		QList<Fraction> getFractionList();
		Fraction getFractionByIndex(int index);
		int getFractionCount();

		QList<Composite> getCompositeList();
		Composite getCompositeByIndex(int index);
		int getCompositeCount();

		void setValue(double v);
		double getValue();
		
		void clearMaterial();
		void writeGDML(QDomDocument& doc, QDomElement& materil);
		void readGDML(QDomElement& ele);
		

	private:
		QString _state{};
		QString _formula{};
		QString _Z{};
		Value* _value{};
		QList<Fraction> _fracList{};
		QList<MaterialType> _materialtypelist{};
		QList<Composite> _compositelist{};
	

	};
}


#endif