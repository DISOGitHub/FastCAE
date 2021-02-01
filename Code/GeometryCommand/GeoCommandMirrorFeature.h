#ifndef GEOCOMMANDMIRRORFEATURE_H_
#define GEOCOMMANDMIRRORFEATURE_H_

#include "GeoCommandBase.h"
#include <QList>
#include <QHash>

namespace Geometry
{
	class GeometrySet;
}

namespace Command
{

	class GEOMETRYCOMMANDAPI CommandMirrorFeature : public GeoCommandBase
	{
	public:
		CommandMirrorFeature(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CommandMirrorFeature() = default;

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

		void setBodys(QMultiHash<Geometry::GeometrySet*, int>  b);
		void setPlane(double* loc, double* dir);
		void setSaveOrigin(bool on);

		void setTypeIndex(int i);
		void setFaceIndex(int faceIndex);
		void setFaceBody(Geometry::GeometrySet*  faceBody);
		void setPlaneIndex(int planeindex);
		void setreverse(bool reverse);
		void setRandomDir(double* randomdir);
		void setBasePt(double* basepoint);
	private:
		void getDirection();

	private:
		//待删
		QList<Geometry::GeometrySet*> _bodys{};
		//新加
		QMultiHash<Geometry::GeometrySet*, int> _solidhash{};

		double _planeLocation[3]{};
		double _planeDirection[3]{};
		bool _saveOrigin{ false };
	

		QHash<Geometry::GeometrySet*, Geometry::GeometrySet*> _resultOriginHash{};
		int _planeindex;
		Geometry::GeometrySet* _faceBody{};
		int _typeindex{ 0 };
		int _faceIndex{ 0 };
		double _basepoint[3];
		double _randomdir[3];
		bool _reverse{ false };

		bool _releasenew{ false };
		bool _releaseEdit{ false };
	};

}




#endif