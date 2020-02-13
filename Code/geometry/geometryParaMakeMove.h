#ifndef _GEOMETRYPARAMAKEMOVE_H_
#define _GEOMETRYPARAMAKEMOVE_H_

#include "geometryModelParaBase.h"
#include <QString>

namespace Geometry
{
	class GeometrySet;
	class GEOMETRYAPI GeometryParaMakeMove : public GeometryModelParaBase
	{
	public:
		GeometryParaMakeMove();
		~GeometryParaMakeMove() = default;

		void setOptionIndex(int index);
		int getOptionIndex();

		void setBodyList(QList<Geometry::GeometrySet*> bodylist);
		QList<Geometry::GeometrySet*> getBodyList();

		void setSaveOrigion(bool s);
		bool getSaveOrigion();

		void setBasePoint(double* basept);
		void getBasePoint(double* basept);

		void setEndPoint(double* endpt);
		void getsetEndPoint(double* endpt);

		void setReverse(bool s);
		bool getReverse();

		void setLength(double l);
		double getLength();

		void setDirection(double* dir);
		void getDirection(double* dir);

		void setOriSet(Geometry::GeometrySet* s);
		Geometry::GeometrySet* getOriSet();

		///数据写入工程文件
		QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		///从工程文件读入数据
		virtual void readDataFromProjectFile(QDomElement* e) override;


	private:
		QList<Geometry::GeometrySet*> _bodylist{};
		int _optionindex{};
		
		Geometry::GeometrySet* _oriset{};
		double _startpoint[3]{};
		double _endpoint[3]{};

		double _length{10};
		double _direction[3]{};
		bool _reverse{};

		bool _saveorigion{};


	};
}


#endif