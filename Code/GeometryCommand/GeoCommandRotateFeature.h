#ifndef GEOCOMMANDROTATEFEATURE_H_
#define GEOCOMMANDROTATEFEATURE_H_

#include "GeoCommandBase.h"
#include <QList>
#include <QHash>
#include <QPair>

namespace Geometry
{
	class GeometrySet;
}

namespace Command
{

	class GEOMETRYCOMMANDAPI CommandRotateFeature : public GeoCommandBase
	{
	public:
		CommandRotateFeature(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CommandRotateFeature() = default;

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

		void setBodys(QMultiHash<Geometry::GeometrySet*, int> bodyhash);
		void setVector(double* vec);
		void setBasicPoint(double* bp);
		void setSaveOrigin(bool on);
		void setDegree(double d);
		void setMethod(int m);
		void setEdge(Geometry::GeometrySet* body, int edge);
		void setReverse(bool on);
	
	private:
		bool generateDir(double* d);

	private:
		//待删
		QList<Geometry::GeometrySet*> _bodys{};
		//新加
		QMultiHash<Geometry::GeometrySet*, int> _solidHash{};
		double _basicPoint[3];
		double _degree{ 0.0 };
		bool _saveOrigin{ false };
		int _method{ 0 };
		QPair<Geometry::GeometrySet*, int> _edge{};
		double _vector[3];
		bool _reverse{ false };

		QHash<Geometry::GeometrySet*, Geometry::GeometrySet*> _resultOriginHash{};
		bool _releasenew{ false };
		bool _releaseEdit{ false };
	};

}




#endif