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

		void setBodys(QMultiHash<Geometry::GeometrySet*, int>  setidList);
		void setFaceIndex(int faceIndex);
		void setFaceBody(Geometry::GeometrySet*  faceBody);

		void setTypeIndex(int i);
		void setPlaneIndex(int planeindex);
		void setreverse(bool reverse);
		void setRandomDir(double* randomdir);
		void setBasePt(double* basepoint);
	private:
		void getDirection();

	private:

		QMultiHash<Geometry::GeometrySet*, int>_solidhash{};
		QHash<Geometry::GeometrySet*, Geometry::GeometrySet*> _inputOutputHash{};

		int _typeindex{ 0 };
		Geometry::GeometrySet* _faceBody{};
		int _planeindex{};
		int _faceIndex{ -1};
		double _basepoint[3];
		double _randomdir[3];
		bool _reverse{ false };


		double _planeLocation[3]{};
		double _planeDirection[3]{};

		bool _releasenew{ false };
		bool _releaseEdit{ false };

	};

}




#endif