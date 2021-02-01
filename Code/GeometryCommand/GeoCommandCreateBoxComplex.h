#ifndef GEOCOMMANDCREATEBOXCOMPLEX_H_
#define GEOCOMMANDCREATEBOXCOMPLEX_H_

#include "GeoCommandBase.h"
#include <QString>

namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommandCreateBoxComplex : public GeoCommandBase
	{
	public:
		GeoCommandCreateBoxComplex(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandCreateBoxComplex() = default;
		
		void setName(QString name);
		void setLocation(double *d);
		void setRandH(double cyr1, double cyh1, double fr, double box1_w, double box2_w, double box2_h, double box3_w);


		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;
	private:
		QString _name{};
		double _location[3];
		double _cyr1;
		double _cyh1;
		double _fr;
		double _box1_w;
		double _box2_w;
		double _box2_h;
		double _box3_w;



		Geometry::GeometrySet* _result{};
	};


}


#endif 