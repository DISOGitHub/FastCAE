#ifndef _GEOMETRYPARABOOLOPERATION_H_
#define _GEOMETRYPARABOOLOPERATION_H_

#include "geometryModelParaBase.h"
#include "GeometryCommand/GeoCommandBool.h"
#include <QString>

namespace Geometry
{
	class GEOMETRYAPI GeometryParaBoolOperation : public GeometryModelParaBase
	{
	public:
		GeometryParaBoolOperation();
		~GeometryParaBoolOperation() = default;

		void setInputBody1(QPair <Geometry::GeometrySet*, int> body1);
		QPair <Geometry::GeometrySet*, int> getBody1();

		void setInputBody2(QPair <Geometry::GeometrySet*, int> body2);
		QPair <Geometry::GeometrySet*, int> getBody2();

		void setType(BoolType b);
		BoolType getType();
		///����д�빤���ļ�
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///�ӹ����ļ���������
		virtual void readDataFromProjectFile(QDomElement* e) override;
		QString BoolTypeToString(BoolType b);
		BoolType StringToBoolType(QString s);

	private:
		BoolType _booltype;

		QPair <Geometry::GeometrySet*, int> _body1;
		QPair <Geometry::GeometrySet*, int> _body2;
	};
}


#endif