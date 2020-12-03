#include "dialogSingleCurve.h"
#include "ui_dialogSingleCurve.h"
#include "dialogInputData.h"


namespace PluginShip
{
	SingleCurveDialog::SingleCurveDialog(int index, QList<QList<double*>> r)
	{
		_ui = new Ui::SingleCurveDialog;
		_ui->setupUi(this);
		_curveindex = index;
		_waterlist = r;
		_ui->lineEdit->setText(QString::number(_curveindex));
		_ui->lineEdit->setReadOnly(true);
		updateTable();
		updateCustom();
	}

	SingleCurveDialog::~SingleCurveDialog()
	{
		
	}
	
	void SingleCurveDialog::updateTable()
	{

		_ui->tableSingleCurve->setRowCount(_waterlist[_curveindex].size());
		_ui->tableSingleCurve->setColumnCount(2);
		QStringList header,sideheader;
		header << "1" << "2";
		_ui->tableSingleCurve->setHorizontalHeaderLabels(header);
		
		//Ìî³äÊý¾Ý
		for (int i = 0; i < _waterlist[_curveindex].size(); i++)
		{
			_ui->tableSingleCurve->setItem(i, 0, new QTableWidgetItem(QString::number(_waterlist[_curveindex][i][1])));
			
			_ui->tableSingleCurve->setItem(i, 1, new QTableWidgetItem(QString::number(_waterlist[_curveindex][i][2])));
			sideheader << QString::number(i);
			
		}
		_ui->tableSingleCurve->setVerticalHeaderLabels(sideheader);
	}

	void SingleCurveDialog::updateCustom()
	{
		_singlecustom = new QCustomPlot;
		_ui->verticalLayout->addWidget(_singlecustom);

		QVector<double> x(_waterlist[_curveindex].size()), y(_waterlist[_curveindex].size());
		for (int j = 0; j < _waterlist[_curveindex].size(); j++)
		{
			double* temp = _waterlist[_curveindex][j];
			x[j] = temp[1];
			y[j] = temp[2];
		}
		_singlecustom->addGraph();
		//ÉèÖÃ»­±Ê
		QPen p;
		p.setColor(Qt::green);
		p.setWidth(1.5);
		_singlecustom->graph()->setPen(p);
		_singlecustom->graph()->setLineStyle(QCPGraph::lsLine);
		_singlecustom->graph()->setData(x, y);
		_singlecustom->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, p, QColor(Qt::black), 5));
		qSort(x.begin(), x.end());
		qSort(y.begin(), y.end());
		double minx= x.front();
		double maxx = x.back();
		double miny = y.front();
		double maxy = y.back();
		_singlecustom->xAxis->setRange(minx-10, maxx+10);
		_singlecustom->yAxis->setRange(miny-10, maxy+10);
		_singlecustom->replot();

	}
	
	void SingleCurveDialog::updateData()
	{
		int rowcount = _ui->tableSingleCurve->rowCount();
		int colcount = _ui->tableSingleCurve->columnCount();
		double x = _waterlist[_curveindex].front()[0];
		_waterlist[_curveindex].clear();
		for (int i = 0; i < rowcount; i++)
		{
			double* one = new double;
			one[0] = x;
			for (int j = 0; j < colcount; j++)
			{
				double  temp = _ui->tableSingleCurve->item(i, j)->text().toDouble();
				if (j != colcount - 1) one[1] = temp;
				else one[2] = temp;
			}
			_waterlist[_curveindex].push_back(one);
		}
	}

	void SingleCurveDialog::accept()
	{
		for each (double* var in _waterlist[_curveindex])
		{
			qDebug() << var[0] << " " << var[1] << "" << var[2] << endl;
		}
		emit signal_dataChanged(_curveindex, _waterlist);

		QDialog::accept();
		this->close();
	}

	void SingleCurveDialog::on_pushButtonRedraw_clicked()
	{
		
		int rowcount = _ui->tableSingleCurve->rowCount();
		int colcount = _ui->tableSingleCurve->columnCount();
		for (int i = 0; i < rowcount; i++)
		{
			for (int j = 0; j <colcount; j++)
			{
				
				double  temp = _ui->tableSingleCurve->item(i, j)->text().toDouble();
				if (j != colcount - 1) _waterlist[_curveindex][i][1]=temp;
				else _waterlist[_curveindex][i][2] = temp;
				
			}
		}
		delete _singlecustom;
		_singlecustom = nullptr;
		updateCustom();
	}

	void SingleCurveDialog::on_pushButtonAdd_clicked()
	{

		InputDataDialog* dlg = new InputDataDialog();
		connect(dlg, SIGNAL(sendAxis(double, double)), this, SLOT(getAxis(double,double)));
		dlg->exec();
		delete dlg;
		dlg = nullptr;
	}

	void SingleCurveDialog::on_pushButtonCancel_clicked()
	{
		int rowIndex = _ui->tableSingleCurve->currentRow();
		if (rowIndex != -1)
			_ui->tableSingleCurve->removeRow(rowIndex);
		updateData();
		updateTable();
		delete _singlecustom;
		_singlecustom = nullptr;
		updateCustom();

	}

	void SingleCurveDialog::getAxis(double axis1, double axis2)
	{
		int rowcountbefor = _ui->tableSingleCurve->rowCount();
		_ui->tableSingleCurve->setRowCount(rowcountbefor + 1);
		_ui->tableSingleCurve->setItem(rowcountbefor, 0, new QTableWidgetItem(QString::number(axis1)));
		_ui->tableSingleCurve->setItem(rowcountbefor, 1, new QTableWidgetItem(QString::number(axis2)));
		updateData();
		updateTable();
		delete _singlecustom;
		_singlecustom = nullptr;
		updateCustom();

	}

}