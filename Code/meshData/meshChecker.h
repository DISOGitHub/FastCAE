#ifndef MESHCHECKER_H
#define MESHCHECKER_H

#include <QThread>
#include "meshDataAPI.h"
#include <vtkCellType.h>
#include <vtkSmartPointer.h>
#include <QHash>
#include <QList>
#include <QString>
#include <QVector>

#define BARCOUNT 10;

class vtkDataSet;
class vtkCell;
class vtkCellQuality;
class vtkDoubleArray;

namespace MeshData
{
	class MeshData;
	class MeshKernal;

	enum QualityMeasure
	{
		NONE = 0,
		AREA,
		ASPECT_BETA,
		ASPECT_FROBENIUS,
		ASPECT_GAMMA,
		ASPECT_RATIO,
		COLLAPSE_RATIO,
		CONDITION,
		DIAGONAL,
		DIMENSION,
		DISTORTION,
		EDGE_RATIO,
		JACOBIAN,
		MAX_ANGLE,
		MAX_ASPECT_FROBENIUS,
		MAX_EDGE_RATIO,
		MED_ASPECT_FROBENIUS,
		MIN_ANGLE,
		NORMAL,
		ODDY,
		RADIUS_RATIO,
		RELATIVE_SIZE_SQUARED,
		SCALED_JACOBIAN,
		SHAPE,
		SHAPE_AND_SIZE,
		SHEAR,
		SHEAR_AND_SIZE,
		SKEW,
		STRETCH,
		TAPER,
		VOLUME,
		WARPAGE
	};

	class MESHDATAAPI MeshChecker :public QThread
	{
	public:
		MeshChecker();
		~MeshChecker();
		static QString VTKCellTypeToString(VTKCellType type);

		virtual void run();
		void setQualityMeasure(QualityMeasure m);
		
		QList<VTKCellType> getInputCellType();
		int getInputCellCount(VTKCellType type);
		int getInputCellCount();

		QList<VTKCellType> getQualityCellType();
		int gettQualityCellCount(VTKCellType type);
		int gettQualityCellCount();

		void getQualityRange(double* r);
		double getAverageQuality();

		QVector<double> getTicks();
		QVector<double> getCountList(VTKCellType t);

		QList<vtkDataSet*> getResultList();

	private:
		void checkKernal(MeshKernal* k);
		void collectInfo(vtkDataSet* outInfo);
		void countResult();


	private:
		vtkSmartPointer<vtkCellQuality> _cellQuality{};
		MeshData* _meshData{};
		QualityMeasure _measure{ NONE };
		
		vtkSmartPointer<vtkDoubleArray> _qualityArray{};
		QHash<VTKCellType, int> _inputCount{};
		QHash<VTKCellType, QList<double>> _checkedCount{};
		QList<vtkDataSet*> _resultList{};

		QVector<double> _ticks{};
		QHash<VTKCellType, QVector<double>> _cellCount{};


	};


}



#endif