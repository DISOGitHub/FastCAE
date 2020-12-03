#include "SolidShapeBase.h"
#include <TopoDS_Shape.hxx>

namespace Geant4 
{	

	SolidShapeBase::~SolidShapeBase()
	{
		if (_shape != nullptr) delete _shape;
	}

	void SolidShapeBase::setName(QString name)
	{
		_name = name;
		this->appendProperty("Name", _name);
	}

	QString SolidShapeBase::getName()
	{
		return _name;
	}

	SolidType SolidShapeBase::getType()
	{
		return _type;
	}

	void SolidShapeBase::setType(SolidType type)
	{
		_type = type;
		QString s = TypeToString(_type);
		this->appendProperty("Type", s);
	}

	QString SolidShapeBase::getParameterUnit()
	{
		return QString();
	}

	void SolidShapeBase::setParameterUnit(QString unit)
	{

	}

	void SolidShapeBase::getShapeParameter(double * p)
	{
	}

	void SolidShapeBase::setShapeParameter(double* p)
	{

	}

	TopoDS_Shape* SolidShapeBase::getShape()
	{
		return _shape;
	}

	

	void SolidShapeBase::readGDML(QDomElement& ele)
	{

	}

	void SolidShapeBase::writeGDML(QDomDocument& doc, QDomElement& ele)
	{

	}

	QString SolidShapeBase::TypeToString(SolidType t)
	{
		QString st("UnDefine");
		switch (t)
		{
		case SolidBox: st = "Box"; break;
		case SolidSphere:st = "Sphere"; break;
		case SolidTube:st = "Tube"; break;
		case SolidUnionBoolean:st = "Union"; break;
		case SolidSubtractionBoolean:st = "Subtraction"; break;
		case SolidIntersectionBoolean:st = "Intersection"; break;
		default: break;
		}
		return st;
	}

	SolidType SolidShapeBase::SringToType(QString s)
	{
		s = s.toLower();
		if ("box" == s)
			return  SolidBox;
		else if ("sphere" == s)
			return SolidSphere;
		else if ("tube" == s)
			return SolidTube;
		else if ("union" == s)
			return SolidUnionBoolean;
		else if ("subtraction" == s)
			return SolidSubtractionBoolean;
		else if ("intersection" == s)
			return SolidIntersectionBoolean;
		
		
		return UnDefine;
	}

}