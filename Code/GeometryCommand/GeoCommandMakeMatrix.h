#ifndef GEOCOMMANDMAKEMATRIX_H_
#define GEOCOMMANDMAKEMATRIX_H_

#include "GeoCommandBase.h"
#include <QList>
#include <QHash>

namespace Geometry
{
	class GeometrySet;
}

namespace Command
{

	class GEOMETRYCOMMANDAPI CommandMakeMatrix : public GeoCommandBase
	{
	public:
		CommandMakeMatrix(GUI::MainWindow* m, MainWidget::PreWindow* p);
		~CommandMakeMatrix() = default;

		bool execute() override;
		void undo() override;
		void redo() override;
		void releaseResult() override;

		void setBodys(QMultiHash<Geometry::GeometrySet*, int> b);
		void setSelectLinear(bool s);
		void setSelectWire(bool s);
		void setOptionIndex(int i);

		//dir1
		void setDir1(double *dir);
		void setReverse1(bool s);
		void setDir1Distance(double d);
		void setDir1Count(int count);
		//dir2
		void setDir2IsChecked(bool k);
		void setDir2(double *dir);
		void setReverse2(bool s);
		void setDir2Distance(double d);
		void setDir2Count(int count);
		//wire
		void setBasicPoint(double* bp);
		void setAxisDir(double* dir);
		void setWireCount(int s);
		void setDegree(double d);
		void setWireRevers(bool s);
		void getVecWire();
		QList<double*> getAngleList();

	private:
		QList<double*> getDir1Vector();
		QList<double*> getDir2Vector();
		QList<double*> getVecSumlist(QList<double*>dir1list, QList<double*>dir2list);

	private:
		//
		QList<Geometry::GeometrySet*> _bodys{};
		//
		QMultiHash<Geometry::GeometrySet*, int> _solidhash{};
		bool _selectLinear{ true };
		bool _selectWire{ false };
		
		double _dir1[3] ;
		bool _reverse1{};
		double _dir1Distance;
		int _dir1Count;
		
		bool _selectDir2{ false };
		double _dir2[3];
		bool _reverse2{};
		double _dir2Distance;
		int _dir2Count;

		int _wireCount;
		double _degree;
		double _basicPoint[3];
		double _axisdir[3];
		bool _reversewire{};
		
		QHash<Geometry::GeometrySet*, Geometry::GeometrySet*> _resultHash{};

		bool _releasenew{ false };
		bool _releaseEdit{ false };
	};

}




#endif