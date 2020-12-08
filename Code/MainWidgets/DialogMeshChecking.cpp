#include "DialogMeshChecking.h"
#include "ui_DialogMeshChecking.h"
#include "meshData/meshChecker.h"
#include "PostWidgets/PostCustomPlot.h"
#include "mainWindow/mainWindow.h"
#include <QDebug>
#include <QMovie>
#include <vtkDataSet.h>
#include <vtkDataArray.h>
#include <vtkCellData.h>
#include <vtkIdTypeArray.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkUnstructuredGrid.h>
#include <vtkAppendFilter.h>

namespace MainWidget
{
	QHash<VTKCellType, QColor> MeshCheckingDialog::TYPECOLORS = QHash<VTKCellType, QColor>();
	MeshCheckingDialog::MeshCheckingDialog(GUI::MainWindow* m)
		: QFDialog(m)
	{
		_ui = new Ui::MeshCheckingDialog;
		_ui->setupUi(this);
		_checker = new MeshData::MeshChecker;
		QSize size = _ui->gifLabel->size();
		QMovie* gif = new QMovie(":/QUI/icon/water_drop.gif");
		gif->setScaledSize(size);
		_ui->gifLabel->setMovie(gif);
		gif->start();
		_customplot = new Post::PostCustomPlot(this);
		_ui->tabWidget->insertTab(2,_customplot,"Res");
		connect(_checker, SIGNAL(finished()), this, SLOT(checkerFinished()));
		connect(_ui->checkBox, SIGNAL(stateChanged(int)), this, SLOT(on_checkbox_changed()));
		connect(this, SIGNAL(highLight(vtkDataSet*)), _mainWindow, SIGNAL(highLightDataSetSig(vtkDataSet*)));
		connect(this, SIGNAL(setDisplay(QString)), _mainWindow, SIGNAL(displayModeChangedSig(QString)));
		
		TYPECOLORS[VTK_TRIANGLE] = QColor(111, 9, 176);
		TYPECOLORS[VTK_QUAD] = QColor(250, 170, 20);
		TYPECOLORS[VTK_TETRA] = QColor(0, 168, 140);
		TYPECOLORS[VTK_HEXAHEDRON] = QColor(10, 132, 255);


		init();
	}
	void MeshCheckingDialog::init()
	{
		_ui->statusLabel->setText("Waitting");
		_ui->tabWidget->setCurrentIndex(0);
		_ui->tabWidget->widget(1)->setEnabled(false);
		const int w = this->width()-70;
		_ui->inputTab->setColumnWidth(0, w / 2);
		_ui->inputTab->setColumnWidth(1, w / 2);
		_ui->checkedTab->setColumnWidth(0, w / 2);
		_ui->checkedTab->setColumnWidth(1, w / 2);
		_ui->inputTab->verticalHeader()->hide();
		_ui->checkedTab->verticalHeader()->hide();
		_ui->inputTab->setRowCount(0);
		_ui->checkedTab->setRowCount(0);

// 		_ui->minBox->setMinimum(-1e32);
// 		_ui->maxBox->setMinimum(-1e32);
// 		_ui->maxBox->setMaximum(1e32);
// 		_ui->minBox->setMaximum(1e32);


		on_checkbox_changed();
	}

	MeshCheckingDialog::~MeshCheckingDialog()
	{
		delete _checker;
		if (_ui != nullptr) delete _ui;
	}

	void MeshCheckingDialog::on_checkButton_clicked()
	{
		init();
		int index = _ui->comboBox->currentIndex();
		_ui->statusLabel->setText("Checking...");
		MeshData::QualityMeasure m = (MeshData::QualityMeasure)index;
		_checker->setQualityMeasure(m);
		_checker->start();
	}

	void MeshCheckingDialog::on_closeButton_clicked()
	{
		this->close();
	}

	void MeshCheckingDialog::checkerFinished()
	{
		_ui->statusLabel->setText("Completed");
		_ui->tabWidget->setCurrentIndex(1);
		_ui->tabWidget->widget(1)->setEnabled(true);
		refreshBasicInfo();
		updatePlot();
	}

	void MeshCheckingDialog::refreshBasicInfo()
	{
		//input
		QList<VTKCellType> types = _checker->getInputCellType();
		int row = types.size() + 1;
		_ui->inputTab->setRowCount(row);
		for (int i = 0; i < row - 1; ++i)
		{
			VTKCellType type = types.at(i);
			QString stype = MeshData::MeshChecker::VTKCellTypeToString(type);
			QTableWidgetItem* desitem = new QTableWidgetItem(stype);
			_ui->inputTab->setItem(i, 0, desitem);

			QString svalue = QString::number(_checker->getInputCellCount(type));
			QTableWidgetItem* valitem = new QTableWidgetItem(svalue);
			_ui->inputTab->setItem(i, 1, valitem);
		}
		QTableWidgetItem* desAllinput = new QTableWidgetItem(tr("All Count"));
		_ui->inputTab->setItem(row - 1, 0, desAllinput);
		QString allcount = QString::number(_checker->getInputCellCount());
		QTableWidgetItem* allInputNum = new QTableWidgetItem(allcount);
		_ui->inputTab->setItem(row - 1, 1, allInputNum);

		//output
		 types = _checker->getQualityCellType();
		row = types.size() + 4;
		_ui->checkedTab->setRowCount(row);
		for (int i = 0; i < row - 4; ++i)
		{
			VTKCellType type = types.at(i);
			QString stype = MeshData::MeshChecker::VTKCellTypeToString(type);
			QTableWidgetItem* desitem = new QTableWidgetItem(stype);
			_ui->checkedTab->setItem(i, 0, desitem);

			QString svalue = QString::number(_checker->gettQualityCellCount(type));
			QTableWidgetItem* valitem = new QTableWidgetItem(svalue);
			_ui->checkedTab->setItem(i, 1, valitem);
		}
		desAllinput = new QTableWidgetItem(tr("All Count"));
		_ui->checkedTab->setItem(row - 4, 0, desAllinput);
		allcount = QString::number(_checker->gettQualityCellCount());
		allInputNum = new QTableWidgetItem(allcount);
		_ui->checkedTab->setItem(row - 4, 1, allInputNum);

		if (_checker->gettQualityCellCount() == 0)
		{
			_ui->checkedTab->setRowCount(row - 3);
			return;
		}
		double range[2] = { 0 };
		_checker->getQualityRange(range);
		QTableWidgetItem* desMin = new QTableWidgetItem("Min");
		_ui->checkedTab->setItem(row - 3, 0, desMin);
		QTableWidgetItem* valueMin = new QTableWidgetItem(QString::number(range[0]));
		_ui->checkedTab->setItem(row - 3, 1, valueMin);
		QTableWidgetItem* desMax = new QTableWidgetItem("Max");
		_ui->checkedTab->setItem(row - 2, 0, desMax);
		QTableWidgetItem* valueMax = new QTableWidgetItem(QString::number(range[1]));
		_ui->checkedTab->setItem(row - 2, 1, valueMax);
		QTableWidgetItem* desAve = new QTableWidgetItem("Average");
		_ui->checkedTab->setItem(row - 1, 0, desAve);
		QTableWidgetItem* valueAve = new QTableWidgetItem(QString::number(_checker->getAverageQuality()));
		_ui->checkedTab->setItem(row - 1, 1, valueAve);
		_ui->minBox->setValue(range[0]);
		_ui->maxBox->setValue(range[1]);
	}

	void MeshCheckingDialog::updatePlot()
	{
		_customplot->clearPlottables();
		/*_customplot->addGraph();*/
// 		_customplot->legend->clear();
// 		double range[2] = { 0 };
// 		_checker->getQualityRange(range);
// 		double em = (range[1] - range[0])*0.1;
// 		_customplot->xAxis->setRange(range[0]-em, range[1]+em);
		_customplot->xAxis->grid()->setVisible(false);

		QVector<double> ticks = _checker->getTicks();

		QHash<VTKCellType, QCPBars*> bars;
		QList<VTKCellType> typeList = _checker->getQualityCellType();
		for (int i = 0; i < typeList.size(); ++i)
		{
			VTKCellType type = typeList.at(i);
			QCPBars* bar = new QCPBars(_customplot->xAxis, _customplot->yAxis);
			bars[type] = bar;
			bar->setAntialiased(false);
			bar->setStackingGap(1);
			QString name = MeshData::MeshChecker::VTKCellTypeToString(type);
			bar->setName(name);
			QColor color = TYPECOLORS.value(type);
			bar->setPen(QPen(color.lighter()));
			bar->setBrush(color);
			bar->setWidthType(QCPBars::wtAxisRectRatio);
			bar->setWidth(0.05);
			if (i > 0)
			{
				QCPBars* b = bars.value(typeList.at(i - 1));
				bar->moveAbove(b);
			}

			QVector<double> data = _checker->getCountList(type);
			if (data.size() == ticks.size())
				bar->setData(ticks, data);
		}
// 		int ymax = _checker->getMaxCellCount();
// 		_customplot->yAxis->setRange(0,ymax*1.05);
		_customplot->rescaleAxes();
		_customplot->legend->setVisible(true);
		_customplot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop | Qt::AlignRight);
		_customplot->legend->setBrush(QColor(255, 255, 255, 100));
		_customplot->legend->setBorderPen(Qt::NoPen);
		_customplot->replot();
	}
	void MeshCheckingDialog::on_checkbox_changed()
	{
		bool checked = _ui->checkBox->isChecked();
		_ui->minlabel->setVisible(checked);
		_ui->maxlabel->setVisible(checked);
		_ui->maxBox->setVisible(checked);
		_ui->minBox->setVisible(checked);
	}


	void MeshCheckingDialog::closeEvent(QCloseEvent *e)
	{
		bool checked = _ui->checkBox->isChecked();
		if (!checked) return;

		double min = _ui->minBox->value();
		double max = _ui->maxBox->value();
		
		if (max < min) return;

		QList<vtkDataSet*> datasetList = _checker->getResultList();
		const int n = datasetList.size();

		vtkSmartPointer<vtkAppendFilter> appf = vtkSmartPointer<vtkAppendFilter>::New();
		for (int i = 0; i < n; ++i)
		{
			vtkDataSet* dataset = datasetList.at(i);
			vtkDataArray* qua = dataset->GetCellData()->GetScalars("CellQuality");
			if (qua == nullptr) continue;
			const int nv = qua->GetNumberOfValues();
			if(nv != dataset->GetNumberOfCells()) continue;
			vtkSmartPointer<vtkIdTypeArray> array = vtkSmartPointer<vtkIdTypeArray>::New();
			for (int j = 0; j < nv; ++j)
			{
				double q = qua->GetComponent(j, 0);
				if (q<min || q>max)
					array->InsertNextValue(j);
			}

			vtkSmartPointer<vtkSelectionNode> selectionNode = vtkSmartPointer<vtkSelectionNode>::New();

			selectionNode->SetFieldType(vtkSelectionNode::CELL);
			selectionNode->SetContentType(vtkSelectionNode::INDICES);
			selectionNode->SetSelectionList(array);

			vtkSmartPointer<vtkSelection> selection = vtkSmartPointer<vtkSelection>::New();
			selection->AddNode(selectionNode);

			vtkSmartPointer<vtkExtractSelection> extractionSelection = vtkSmartPointer<vtkExtractSelection>::New();
			extractionSelection->SetInputData(0, dataset);
			extractionSelection->SetInputData(1, selection);
			extractionSelection->Update();
			
			vtkSmartPointer<vtkUnstructuredGrid>  DataSet = vtkSmartPointer<vtkUnstructuredGrid>::New();
			DataSet->DeepCopy(extractionSelection->GetOutput());
			appf->AddInputData(DataSet);
		}
		appf->Update();
		vtkDataSet *bad = appf->GetOutput();

		emit setDisplay("wireframe");
		emit highLight(bad);

		QDialog::closeEvent(e);
	}

	

}