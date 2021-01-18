#ifndef GEOCOMMANDCREATECYLINDRICALCOMPLEX_H_
#define GEOCOMMANDCREATECYLINDRICALCOMPLEX_H_

#include "GeoCommandBase.h"
#include <QString>

namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommandCreateCylindricalComplex : public GeoCommandBase
	{
	public:
		GeoCommandCreateCylindricalComplex(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandCreateCylindricalComplex() = default;
		
		void setName(QString name);
		void setLocation(double *d);
		void setRandH(double cyr1,double cyh1, double cor, double coh, double cyh2);


		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

	private:
		QString _name{};
		double _location[3];
		double _cyr1;
		double _cyh1;
		double _cor;
		double _coh;
		double _cyh2;

		Geometry::GeometrySet* _result{};
	};


}


#endif 