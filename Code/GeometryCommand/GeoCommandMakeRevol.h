#ifndef GEOCOMMANDMAKEREVOL_H_
#define GEOCOMMANDMAKEREVOL_H_

#include "geometryCommandAPI.h"
#include "GeoCommandBase.h"
#include <QMultiHash>
#include <QHash>
#include "moduleBase/ModuleType.h"

namespace Geometry
{
	class GeometrySet;
}
namespace Command
{
	class GEOMETRYCOMMANDAPI GeoCommandMakeRevol : public GeoCommandBase
	{
	public:
		GeoCommandMakeRevol(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandMakeRevol() = default;

		bool execute()override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

		void setShapeList(QMultiHash<Geometry::GeometrySet*, int> s);
		void setName(QString name);
		void isSolid(bool ischecked);
		void setDegree(double d);
		void setBasicPoint(double *bp);
		//void setVector(double* vec);
		void setOptionMethod(int i);
		void setAixSetEdge(QPair<Geometry::GeometrySet*, int>s);
		void setCoor(double* coor);
		void setReverse(bool s);

	private:
		void getVec(double * vec);


	private:

		QMultiHash<Geometry::GeometrySet*, int> _shapeHash{};
		QString _extName{};
		double _degree{ 0.0 };
		double _basicPoint[3];
		bool _reverse{};
		bool _isSolid{ true };
		int _optionmethod{ 0 };

		QPair<Geometry::GeometrySet*, int> _aixsetedge{};
		double _coor[3]{};
		Geometry::GeometrySet* _result{};

	};














}
























#endif
