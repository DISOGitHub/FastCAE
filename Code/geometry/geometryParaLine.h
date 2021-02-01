#ifndef _GEOMETRYPARALINE_H_
#define _GEOMETRYPARALINE_H_

#include "geometryModelParaBase.h"
#include <QString>

namespace Geometry
{
	class GEOMETRYAPI GeometryParaLine : public GeometryModelParaBase
	{
	public:
		GeometryParaLine();
		~GeometryParaLine() = default;

		void setName(QString name);
		QString getName();

		void setStartPoint(double* startpt);
		void getStartPoint(double* startpt);

		void setOptionIndex(int index);
		int getOptionIndex();

		void setCoor(double* coor);
		void getCoor(double* coor);

		void setLength(double l);
		double getLength();

		void setDirection(double* dir);
		void getDirection(double* dir);

		void setReverse(bool b);
		bool getReverse();

		///数据写入工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e) override;

	private:
		QString _name{};
		int _index;
		double _startpoint[3];
		double _endpoint[3];

		double _length;
		double _direction[3];
		double _coor[3];
		bool _reverse;

	};
}


#endif