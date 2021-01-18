#ifndef GEOCOMMANDMOVEFEATURE_H_
#define GEOCOMMANDMOVEFEATURE_H_

#include "GeoCommandBase.h"
#include <QList>
#include <QHash>

namespace Geometry
{
	class GeometrySet;
}

namespace Command
{

	class GEOMETRYCOMMANDAPI CommandMoveFeature : public GeoCommandBase
	{
	public:
		CommandMoveFeature(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CommandMoveFeature() = default;

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

		void setBodys(QMultiHash<Geometry::GeometrySet*, int> bodyhash);
 		void setVector(double* vec);
 		void setSaveOrigin(bool on);
		void setOptionIndex(int i);
		void setStartPt(double * startpt);
		void setEndPt(double * endpt);
		void setDir(double * dir);
		void setReverse(bool r);
		void setLength(double length);
	private:
		void  getVec();


	protected:

	private:
		//待删
		QList<Geometry::GeometrySet*> _bodys{};
		//新加
		QMultiHash<Geometry::GeometrySet*, int> _solidhash{};
		double _vector[3];
		bool _saveOrigin{ false };
		int _optionindex{};
		double _startpt[3]{};
		double _endpt[3]{};
		double _dir[3]{};
		bool _reverse{};
		double _length{};
		QHash<Geometry::GeometrySet*, Geometry::GeometrySet*> _resultOriginHash{};

		bool _releasenew{ false };
		bool _releaseEdit{ false };
	};

}




#endif