#ifndef _GEOMETRYDATABASE_H
#define _GEOMETRYDATABASE_H

#include <QString>
#include "DataProperty/DataBase.h"

enum SolidType
{
	UnDefine = 0,
	SolidBox,
	SolidSphere,
	SolidTube,
	SolidUnionBoolean,
	SolidSubtractionBoolean,
	SolidIntersectionBoolean,
};

class TopoDS_Shape;
class QDomElement;

namespace Geant4 
{
	class SolidShapeBase : public DataProperty::DataBase
	{
		Q_OBJECT
	public:
		SolidShapeBase() = default;
		~SolidShapeBase();

		void setName(QString name);
		QString getName();
		
		SolidType getType();
		void setType(SolidType type);

		virtual QString getParameterUnit();
		virtual void setParameterUnit(QString unit);
		virtual void getShapeParameter(double* p);
		virtual void setShapeParameter(double* p);
		
		TopoDS_Shape*  getShape();
		virtual void generateShape() =  0;
		virtual void readGDML(QDomElement& ele);
		virtual void writeGDML(QDomDocument& doc, QDomElement& ele);


		static QString TypeToString(SolidType t);
		static SolidType SringToType(QString s);

	protected:
		QString _name{};
		SolidType _type{UnDefine};
		TopoDS_Shape* _shape{};

	};
}
#endif // _GEOMETRYBASE_H


