#include "meshChecker.h"
#include "meshKernal.h"
#include "meshSingleton.h"
#include <vtkDataSet.h>
#include <vtkCell.h>
#include <vtkCellQuality.h>
#include <vtkCellData.h>
#include <vtkDoubleArray.h>
#include <math.h>
#include <vtkUnstructuredGrid.h>
#include <QDebug>

namespace MeshData
{
	MeshChecker::MeshChecker()
	{
		_meshData = MeshData::getInstance();
		_cellQuality = vtkSmartPointer<vtkCellQuality>::New();
		_qualityArray = vtkSmartPointer<vtkDoubleArray>::New();
		_cellQuality->SetUndefinedQuality(-1e66);
		_cellQuality->SetUnsupportedGeometry(-1e66);
	}
	MeshChecker::~MeshChecker()
	{
// 		const int n = _resultList.size();
// 		for (int i = 0; i < n; ++i)
// 		{
// 			vtkDataSet* d = _resultList.at(i);
// 			d->Delete();
// 		}
// 		_resultList.clear();
	}

	void MeshChecker::run()
	{
		_qualityArray->SetNumberOfValues(0);
		_inputCount.clear();
		_checkedCount.clear();
// 		const int nc = _resultList.size();
// 		for (int i = 0; i < nc; ++i)
// 		{
// 			vtkDataSet* d = _resultList.at(i);
// 			d->Delete();
// 		}
 		_resultList.clear();

		if (_measure == NONE) return;
		const int n = _meshData->getKernalCount();
		for (int i = 0; i < n; ++i)
		{
			MeshKernal* k = _meshData->getKernalAt(i);
			this->checkKernal(k);
		}
		_qualityArray->Modified();
		countResult();
	}

	void MeshChecker::setQualityMeasure(QualityMeasure m)
	{
		_measure = m;
		_cellQuality->SetQualityMeasure(m);
	}

	void MeshChecker::checkKernal(MeshKernal* k)
	{
		vtkDataSet* checkingDataset = k->getMeshData();
		_cellQuality->SetInputData(checkingDataset);
		_cellQuality->Update();
		vtkDataSet* res = _cellQuality->GetOutput();

// 		vtkUnstructuredGrid* g = vtkUnstructuredGrid::New();
// 		g->DeepCopy(res);
 		_resultList.append(res);

		this->collectInfo(res);
		
	}
	
	void MeshChecker::collectInfo(vtkDataSet* res)
	{
		vtkDataArray* qua = res->GetCellData()->GetScalars("CellQuality");

		const int n = res->GetNumberOfCells();

		for (int i = 0; i < n; ++i)
		{
			vtkCell* cell = res->GetCell(i);
			VTKCellType type = (VTKCellType)cell->GetCellType();
			++_inputCount[type];

			double q = qua->GetComponent(i, 0);
			if (q > -1e65)
			{
				_qualityArray->InsertNextValue(q);
				_checkedCount[type].append(q);
			}
		}
	}

	QString MeshChecker::VTKCellTypeToString(VTKCellType type)
	{
		QString stype("Undefined");

		switch (type)
		{
		case VTK_TRIANGLE:
			stype = "Tri3"; break;
		case  VTK_QUAD:
			stype = "Quad4"; break;
		case VTK_TETRA:
			stype = "Tet4"; break;
		case VTK_HEXAHEDRON:
			stype = "Hex8"; break;
		default:
			break;
		}

		return stype;
	}

	QList<VTKCellType> MeshChecker::getInputCellType()
	{
		return _inputCount.keys();
	}

	int MeshChecker::getInputCellCount(VTKCellType type)
	{
		int count = 0;
		if (_inputCount.contains(type))
			count = _inputCount.value(type);
		return count;
	}

	int MeshChecker::getInputCellCount()
	{
		int count = 0;
		QList<VTKCellType> typelist = this->getInputCellType();
		const int n = typelist.size();
		for (int i = 0; i < n; ++i)
		{
			VTKCellType t = typelist.at(i);
			count += this->getInputCellCount(t);
		}
		return count;
	}

	QList<VTKCellType> MeshChecker::getQualityCellType()
	{
		return _checkedCount.keys();
	}

	int MeshChecker::gettQualityCellCount(VTKCellType type)
	{
		int count = 0;
		if (_checkedCount.contains(type))
			count = _checkedCount.value(type).size();
		return count;
	}

	int MeshChecker::gettQualityCellCount()
	{
		int count = 0;
		QList<VTKCellType> typelist = this->getQualityCellType();
		const int n = typelist.size();
		for (int i = 0; i < n; ++i)
		{
			VTKCellType t = typelist.at(i);
			count += this->gettQualityCellCount(t);
		}
		return count;
	}

	void MeshChecker::getQualityRange(double* range)
	{
		_qualityArray->GetRange(range);
// 		range[0] = r[0];
// 		range[1] = r[1];
	}

	double MeshChecker::getAverageQuality()
	{
		const int n = _qualityArray->GetNumberOfValues();
		if (n == 0) return -1e66;
		double sum = 0;
		for (int i = 0; i < n; ++i)
		{
			sum += _qualityArray->GetValue(i);
		}
		return sum / n;
	}

	void MeshChecker::countResult()
	{
		_ticks.clear();
		_cellCount.clear();
		const int count = gettQualityCellCount();
		if (count == 0) return;

		double range[2];
		this->getQualityRange(range);

		if (range[1] - range[0] < 1e-8)
		{
			_ticks.append((range[0] + range[1]) / 2);
			QList<VTKCellType> types = this->getQualityCellType();
			for (int i = 0; i < types.size(); ++i)
			{
				VTKCellType t = types.at(i);
				int n = this->gettQualityCellCount(t);
				_cellCount[t].append(n);
			}
			return;
		}

		QList<VTKCellType> types = this->getQualityCellType();
		for (int i = 0; i < types.size(); ++i)
		{
			VTKCellType t = types.at(i);
			int n = this->gettQualityCellCount(t);
			_cellCount[t].resize(10);
		}
		double ran = range[1] - range[0];
		double step = ran / 10.0;
		for (int i = 0; i < 10; ++i)
			_ticks.append(range[0] + step / 2 + step*i);

		for (int i = 0; i < types.size(); ++i)
		{
			VTKCellType t = types.at(i);
			QList<double> qual = _checkedCount[t];
			int cellcount = qual.size();
			for (int c = 0; c < cellcount; ++c)
			{
				double v = qual.at(c);
				for (int j = 0; j < 10; ++j)
				{
					double ti = _ticks.at(j);
					if (v >= ti - step / 2 && v < ti + step / 2)
					{
						++_cellCount[t][j];
						continue;
					}
				}
			}

		}
	}

	QVector<double> MeshChecker::getTicks()
	{
		return _ticks;
	}

	QVector<double> MeshChecker::getCountList(VTKCellType t)
	{
		QVector<double> d;
		if (_cellCount.contains(t))
			d = _cellCount.value(t);
		return d;
	}

	QList<vtkDataSet*> MeshChecker::getResultList()
	{
		return _resultList;
	}



}