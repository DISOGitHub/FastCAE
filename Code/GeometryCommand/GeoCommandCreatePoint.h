#ifndef GEOCOMMANDCREATEPOINT_H_
#define GEOCOMMANDCREATEPOINT_H_

#include "GeoCommandBase.h"
#include <QString>

namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommandCreatePoint : public GeoCommandBase
	{
	public:
		GeoCommandCreatePoint(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandCreatePoint() = default;
		
		void setName(QString name);
		void setCoordinate(double* c);
		void setCorner(double* c);
		void setPara(double* s);

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

	private:
		QString _name{};
		double _coordinate[3];
		double _corner[3];
		double _para[3];

		Geometry::GeometrySet* _result{};
	};


}


#endif 