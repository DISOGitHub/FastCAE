#ifndef GEOCOMMANDMAKEEXTRUSION_H_
#define GEOCOMMANDMAKEEXTRUSION_H_

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
	class GEOMETRYCOMMANDAPI GeoCommandMakeExtrusion : public GeoCommandBase
	{
	public:
		GeoCommandMakeExtrusion(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~GeoCommandMakeExtrusion() = default;

		bool execute()override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

		void setAxis(double* axis);
		void setShapeList(QMultiHash<Geometry::GeometrySet*, int> s);
		void setName(QString name);
		void isSolid(bool ischecked);

		void setDirection(double * dir);
		void setDistance(double dis);
		void setReverse(bool reverse);
	private:
		void getAxis();

	private:

		QMultiHash<Geometry::GeometrySet*, int> _shapeHash{};
		double _axis[3];
		bool _isSolid{false};
		QString _extName{};

		double _direcion[3];
		bool _reverse{ false };
		double _distance{ 10 };
		Geometry::GeometrySet* _result{};
		
//		ModuleBase::SelectModel _mode{ModuleBase::None};
// 		ModuleBase::SelectBodyType _bodymode{ModuleBase::Shell};

	};














}
























#endif
