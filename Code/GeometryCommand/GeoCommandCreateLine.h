#ifndef GEOCOMMANDCREATELINE_H_
#define GEOCOMMANDCREATELINE_H_

#include "GeoCommandBase.h"
#include <QString>

namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommandCreateLine : public GeoCommandBase
	{
	public:
		GeoCommandCreateLine(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandCreateLine() = default;
		
		void setName(QString name);
		void setPoints(double* start);
		void setOptionIndex(int i);
		void setCoor(double* c);
		void setLength(double l);
		void setDir(double *d);
		void setReverse(bool b);
		bool getEndPoint(double* endpt);

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

	private:
		QString _name{};
		double _startPoint[3];
		double _endPoint[3];
		int _index;
		double _coor[3];
		double _length;
		double _dir[3];
		bool _reverse;

		Geometry::GeometrySet* _result{};
	};


}


#endif 