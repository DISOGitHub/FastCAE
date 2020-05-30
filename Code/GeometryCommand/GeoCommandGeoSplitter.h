#ifndef GEOCOMMANDGEOSPLITTER_H_
#define GEOCOMMANDGEOSPLITTER_H_

#include "GeoCommandBase.h"
#include <QHash>
namespace Geometry
{
	class GeometrySet;
}

namespace Command
{

	class GEOMETRYCOMMANDAPI CommandGeoSplitter : public GeoCommandBase
	{
	public:
		CommandGeoSplitter(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CommandGeoSplitter() = default;

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

		void setBody(Geometry::GeometrySet* b);
		void setFaceIndex(int faceIndex);
		void setFaceBody(Geometry::GeometrySet*  faceBody);

	private:
		Geometry::GeometrySet* _body{};
		Geometry::GeometrySet* _faceBody{};
		int _faceIndex{ -1};

		bool _releasenew{ false };
		bool _releaseEdit{ false };
		//Geometry::GeometrySet* _result{};

		QHash<Geometry::GeometrySet*, Geometry::GeometrySet*> _inputOutputHash{};

	};

}




#endif