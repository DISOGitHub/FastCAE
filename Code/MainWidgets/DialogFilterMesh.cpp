#include "DialogFilterMesh.h"
#include "ui_DialogFilterMesh.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "mainWindow/mainWindow.h"
#include "preWindow.h"
#include <vtkDataSet.h>
#include <vtkUnstructuredGrid.h>
#include <vtkIdList.h>
#include <vtkCell.h>
#include <QListWidgetItem>
#include <QListWidget>

namespace MainWidget{
	FilterMeshDialog::FilterMeshDialog(GUI::MainWindow* m, PreWindow* p):
		QDialog(m), _data(MeshData::MeshData::getInstance())
	{
		_ui = new Ui::FilterMeshDialog;
		_ui->setupUi(this);

		Qt::WindowFlags flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;
		setWindowFlags(flags);

		_cellTypeList = { 0, 1, 2, 3 };

		connect(this, SIGNAL(updateMeshTree()), m, SIGNAL(updateMeshTreeSig()));
		connect(this, SIGNAL(updateSetTree()), m, SIGNAL(updateSetTreeSig()));
		connect(this, SIGNAL(updateActions()), m, SIGNAL(updateActionStatesSig()));
		connect(this, SIGNAL(updateMeshActor()), p, SIGNAL(updateMeshActorSig()));

		init();
	}

	FilterMeshDialog::~FilterMeshDialog()
	{
		delete _ui;
	}

	void FilterMeshDialog::accept()
	{
// 		if (_cellTypeList.size() == 4 || _cellTypeList.size() == 0)
// 			return;

		setCellFilters();
		setFilterMeshs();

		int num = _kernalList.size();

		for (int i = 0; i < num; i++)
		{
			MeshData::MeshKernal* kernal = _kernalList.at(i);
			if (kernal ==nullptr)continue;
			vtkDataSet* dataset = kernal->getMeshData();
			vtkDataSet* filterdata = filterMesh(dataset);
			kernal->setMeshData(filterdata);
		}

		emit updateMeshTree();
		emit updateSetTree();
		emit updateActions();
		emit updateMeshActor();

		QDialog::accept();
		this->close();
	}

	void FilterMeshDialog::init()
	{
	/*	MeshData::MeshData* _data = MeshData::MeshData::getInstance();*/

		int num = _data->getKernalCount();

		for (int i = 0; i < num; i++)
		{
			MeshData::MeshKernal* kernal = _data->getKernalAt(i);
			if (kernal == nullptr)continue;
			QString name = kernal->getName();

			QListWidgetItem* item = new QListWidgetItem();
			QCheckBox* check = new QCheckBox();
			check->setText(name);

			//item->setCheckState(Qt::Unchecked);
			_ui->listWidget->addItem(item);
			_ui->listWidget->setItemWidget(item,check);
		}
	}

	vtkDataSet* FilterMeshDialog::filterMesh(vtkDataSet* dataset)
	{
		if (dataset == nullptr)return dataset;
	/*	if (_cellTypeList.size() <= 0) return dataset;*/
		vtkUnstructuredGrid* ung = vtkUnstructuredGrid::New();
		vtkPoints* points = vtkPoints::New();

		const int nNode = dataset->GetNumberOfPoints();
		for (int i = 0; i < nNode; i++)
		{
			double* coor = dataset->GetPoint(i);
			points->InsertNextPoint(coor);
		}
		ung->SetPoints(points);

		const int ncell = dataset->GetNumberOfCells();
		for (int i = 0; i < ncell; i++)
		{
			vtkCell* cell = dataset->GetCell(i);
			if (cell == nullptr) continue;
			VTKCellType type = (VTKCellType)cell->GetCellType();
			vtkIdList* idlist = vtkIdList::New();
			if (isSpecifiedCell(type))
			{
				idlist = cell->GetPointIds();
				ung->InsertNextCell(type, idlist);
			}
		}

		return ung;
	}

	bool FilterMeshDialog::isSpecifiedCell(VTKCellType type)
	{
		int flag = -1;
		switch (type)
		{
		case VTKCellType::VTK_VERTEX:
		case VTKCellType::VTK_POLY_VERTEX:
			flag = 0;
			break;
		case VTKCellType::VTK_LINE:
		case VTKCellType::VTK_POLY_LINE:
		case VTKCellType::VTK_QUADRATIC_EDGE:
			flag = 1;
			break;
		case VTKCellType::VTK_TRIANGLE:
		case VTKCellType::VTK_QUAD:
		case VTKCellType::VTK_TRIANGLE_STRIP:
		case VTKCellType::VTK_PIXEL:
		case VTKCellType::VTK_POLYGON:
		case VTKCellType::VTK_QUADRATIC_TRIANGLE:
		case VTKCellType::VTK_QUADRATIC_LINEAR_QUAD:
		case VTKCellType::VTK_QUADRATIC_QUAD:
		case VTKCellType::VTK_BIQUADRATIC_QUAD:
			flag = 2;
			break;
		case VTKCellType::VTK_TETRA:
		case VTKCellType::VTK_HEXAHEDRON:
		case VTKCellType::VTK_VOXEL:
		case VTKCellType::VTK_WEDGE:
		case VTKCellType::VTK_PYRAMID:
		case VTKCellType::VTK_PENTAGONAL_PRISM:
		case VTKCellType::VTK_HEXAGONAL_PRISM:
		case VTKCellType::VTK_QUADRATIC_TETRA:
		case VTKCellType::VTK_QUADRATIC_PYRAMID:
		case VTKCellType::VTK_QUADRATIC_HEXAHEDRON:
		case VTKCellType::VTK_BIQUADRATIC_QUADRATIC_HEXAHEDRON:
		case VTKCellType::VTK_TRIQUADRATIC_HEXAHEDRON:
		case VTKCellType::VTK_QUADRATIC_LINEAR_WEDGE:
		case VTKCellType::VTK_QUADRATIC_WEDGE:
		case VTKCellType::VTK_BIQUADRATIC_QUADRATIC_WEDGE:
			flag = 3;
			break;
		default:
			break;
		}

		if (_cellTypeList.contains(flag))
			return true;

		return false;
	}

	void FilterMeshDialog::setCellFilters()
	{
		if (_ui->checkBox_0d->isChecked())
			_cellTypeList.removeOne(0);
		if (_ui->checkBox_1d->isChecked())
			_cellTypeList.removeOne(1);
		if (_ui->checkBox_2d->isChecked())
			_cellTypeList.removeOne(2);
		if (_ui->checkBox_3d->isChecked())
			_cellTypeList.removeOne(3);
	}

	void FilterMeshDialog::setFilterMeshs()
	{
		int num = _ui->listWidget->count();
		for (int i = 0; i < num; i++)
		{
			QListWidgetItem* item = _ui->listWidget->item(i);
			QWidget* widget = _ui->listWidget->itemWidget(item);
			QCheckBox* check = (QCheckBox*)widget;
			if (check->isChecked())
			{
				MeshData::MeshKernal* k = _data->getKernalAt(i);
 				_kernalList.append(k);
			}
// 			if (item->checkState() == Qt::Checked)
// 			{
// 				MeshData::MeshKernal* k = _data->getKernalAt(i);
// 				_kernalList.append(k);
// 			}
		}
	}

}