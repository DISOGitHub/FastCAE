#include "dialogModelImport.h"
#include "ui_dialogModelImport.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <QMessageBox>
#include "GeoCommandMakeHullCut.h"
#include "GeometryCommand/GeoCommandList.h"
#include <QTreeWidget>
#include "mainWindow/mainWindow.h"
#include <QDebug>
#include "DataProperty/modelTreeItemType.h"
#include "settings/busAPI.h"
#include <QFileDialog>
#include <QDomDocument>
#include <QHeaderView>
#include <cmath>
#include "dialogSingleCurve.h"
namespace PluginShip
{
	ModelImportDialog::ModelImportDialog(GUI::MainWindow* m)
	{
		_ui = new Ui::ModelImportDialog;
		_ui->setupUi(this);
		_customplotsection = new QCustomPlot;
		_ui->customPlotLayout->addWidget(_customplotsection);
		
		_customplotwater = new QCustomPlot;
		_ui->customPlotLayout->addWidget(_customplotwater);
		//initwater();
		connect(_customplotwater, SIGNAL(plottableClick(QCPAbstractPlottable*, int, QMouseEvent*)), this, SLOT(graphClicked(QCPAbstractPlottable*, int)));
	}

	ModelImportDialog::~ModelImportDialog()
	{
		
	}

	void ModelImportDialog::updateDialog()
	{
		_ui->lineEdit_nsect->setText(QString::number(_pointlist.size()));
		_ui->lcdNumber->display(_pointlist.size());
		_ui->tableWidget_import->clearContents();
		_ui->tableWidget_section->clearContents();
		updateTableImport();
		updateTableSection();
	}

	void ModelImportDialog::updateTableImport()
	{
		//填表格1
		int length{};
		for (int i = 0; i < _pointlist.size(); i++)
		{
			int size = _pointlist[i].size();
			if (size > length) length = size;
		}
		int colcount = 3 * length;
		int rowcount = _pointlist.size()+1;
		//填表格
		_ui->lineEdit_mp_maxcp0->setText(QString::number(length));
		_ui->tableWidget_import->setRowCount(rowcount);
		_ui->tableWidget_import->setColumnCount(colcount);
		_ui->tableWidget_import->setEditTriggers(QAbstractItemView::NoEditTriggers);
		//设置表头
		QStringList header,sideheader;
		for (int i = 0; i < length; i++)
		{
			header << "Pt" + QString::number(i) << "Pt" + QString::number(i) << "Pt" + QString::number(i);
		}
		sideheader << "Coordinate";
		for (size_t i = 1; i < rowcount; i++)
		{
			sideheader << "StaNo" + QString::number(i);
		}
		_ui->tableWidget_import->setHorizontalHeaderLabels(header);
		_ui->tableWidget_import->setVerticalHeaderLabels(sideheader);
		//填充数据
		for (int i = 0; i < colcount; i = i + 3)
		{
			_ui->tableWidget_import->setItem(0, i, new QTableWidgetItem("X"));
			_ui->tableWidget_import->setItem(0, i+1, new QTableWidgetItem("Y"));
			_ui->tableWidget_import->setItem(0, i+2, new QTableWidgetItem("Z"));
		}
		int k = 1;
		for (QList<double*>  var : _pointlist)
		{
			int i = 0;
			for (double* varr : var)
			{
				
				_ui->tableWidget_import->setItem(k, i, new QTableWidgetItem(QString::number(varr[0])));
				_ui->tableWidget_import->setItem(k, i + 1, new QTableWidgetItem(QString::number(varr[1])));
				_ui->tableWidget_import->setItem(k, i + 2, new QTableWidgetItem(QString::number(varr[2])));
				i = i + 3;
			}
			k = k + 1;
		}
		
	}

	void ModelImportDialog::updateTableSection()
	{
		_ui->tableWidget_section->setEditTriggers(QAbstractItemView::NoEditTriggers);
		_ui->tableWidget_section->setRowCount(_pointlist.size());
		_ui->tableWidget_section->setColumnCount(3);
		QStringList header;
		header << "StaNo"<< "PtNum"  << "Length";
		_ui->tableWidget_section->setHorizontalHeaderLabels(header);
		_ui->tableWidget_section->verticalHeader()->hide();
		//填充数据
		for (int i = 0; i < _pointlist.size();i++)
		{
			_ui->tableWidget_section->setItem(i, 0, new QTableWidgetItem(QString::number(i+1)));
			int num = _pointlist[i].size();
			_ui->tableWidget_section->setItem(i, 1, new QTableWidgetItem(QString::number(num)));
			double len = getLength(i);
			_ui->tableWidget_section->setItem(i, 2, new QTableWidgetItem(QString::number(len)));
		}

	}

	double ModelImportDialog::getLength(int k)
	{
		QList<double* > temp = _pointlist[k];
		double length{};
		for (int i = 0; i < temp.size()-1; i++)
		{
			double dis = sqrt(pow((temp[i][0] - temp[i + 1][0]), 2) + pow((temp[i][1] - temp[i + 1][1]), 2) + pow((temp[i][2] - temp[i + 1][2]), 2));
			length = length + dis;
		}
		return length;
	}

	void ModelImportDialog::initsection()
	{
		//XY qlist<double*> sectionlist;
		QList<QList<double*>> resultlist;//竖型列表
		QList<QList<double*>> treelist;//横向列表
		for (int i = 0; i < _pointlist.size(); i++)
		{
			int k = _pointlist[i].size();
			QList<double*>temp{};
			temp << _pointlist[i].front() << _pointlist[i][(k - 1) / 2] << _pointlist[i].back();
			resultlist.push_back(temp);

		}

		QList<double*> temp1, temp2, temp3;
		for (int i = 0; i < resultlist.size(); i++)
		{
			temp1 << resultlist[i][0];
			temp2 << resultlist[i][1];
			temp3 << resultlist[i][2];
		}
		treelist << temp1 << temp2 << temp3;

		for (int i = 0; i < resultlist.size(); i++)
		{
			QVector<double> x(3), y(3);
			for (int j = 0; j < 3; j++)
			{
				x[j] = resultlist[i][j][0];
				y[j] = resultlist[i][j][1];
			}
			_customplotsection->addGraph();
			
			//设置画笔
			QPen p;
			p.setWidth(1);
			p.setColor(Qt::green);
			_customplotsection->graph(i)->setPen(p);
			_customplotsection->graph(i)->setData(x, y);
			_customplotsection->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle,p,QColor(Qt::black) ,5));

		}

		for (int i = 0; i < treelist.size(); i++)
		{
			QVector<double> x(_pointlist.size()), y(_pointlist.size());
			for (int j = 0; j < _pointlist.size(); j++)
			{
				double* temp = treelist[i][j];
				x[j] = temp[0];
				y[j] = temp[1];
			}
			_customplotsection->addGraph();
			//设置画笔
			QPen p;
			p.setWidth(1);
			p.setColor(Qt::green);
			_customplotsection->graph(i+resultlist.size())->setPen(p);
			_customplotsection->graph(i+resultlist.size())->setLineStyle(QCPGraph::lsLine);
			_customplotsection->graph(i+resultlist.size())->setData(x, y);
			_customplotsection->graph(i+resultlist.size())->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, p, QColor(Qt::black), 5));

		}
		_customplotsection->xAxis->setRange(_minX-10, _maxX+10);
		_customplotsection->yAxis->setRange(_minY-10, _maxY+10);
		_customplotsection->xAxis->setVisible(false);
		_customplotsection->yAxis->setVisible(false);
		_customplotsection->replot();
	}

	void ModelImportDialog::initwater()
	{
		QList<QList<double*>> resultlist;//XZ
		for (int i = 0; i < _pointlist.size(); i++)
		{
			
			if (i < (_pointlist.size() + 1) / 2)
			{
				resultlist << _pointlist[i].mid(0,((_pointlist[i].size()-1)/2)+1 );//保存需要画出的一半数据
				_symwaterlist << _pointlist[i].mid((((_pointlist[i].size() - 1) / 2)+1), _pointlist[i].size()-1 );//保留对称的数据。
			
			}
			else
			{
				resultlist << _pointlist[i].mid(((_pointlist[i].size() - 1) / 2), _pointlist[i].size() - 1);
				_symwaterlist << _pointlist[i].mid(0, ((_pointlist[i].size() - 1) / 2));

			}
		}
		_waterlist = resultlist;
		for (int i = 0; i < resultlist.size(); i++)
		{
			QVector<double> x(resultlist[i].size()), y(resultlist[i].size());
			for (int j = 0; j < resultlist[i].size(); j++)
			{
				double* temp = resultlist[i][j];
				x[j] = temp[1];
				y[j] = temp[2];
			}
			_customplotwater->addGraph();
			//设置画笔
			QPen p;
			p.setColor(Qt::green);
			p.setWidth(1.5);
			_customplotwater->graph(i)->setPen(p);
			_customplotwater->graph(i)->setLineStyle(QCPGraph::lsLine);
			_customplotwater->graph(i)->setData(x, y);
			_customplotwater->graph(i)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, p, QColor(Qt::black), 5));
		}
		//添加两条线
		QVector<double>x0(2) , y0(2), x1(2), y1(2);
		x0[0] = 0; x0[1] = 0;
		y0[0] = 0; y0[1] = _maxZ;
		x1[0] = _minY; x1[1] = _maxY;
		y1[0] = _maxZ; y1[1] = _maxZ;
		
		_customplotwater->addGraph();
		//设置画笔
		QPen p0;
		p0.setColor(Qt::black);
		p0.setWidth(2.5);
		_customplotwater->graph(resultlist.size() )->setPen(p0);
		_customplotwater->graph(resultlist.size())->setLineStyle(QCPGraph::lsLine);
		_customplotwater->graph(resultlist.size())->setData(x0, y0);
		_customplotwater->addGraph();

		QPen p1;
		p1.setColor(Qt::black);
		p1.setWidth(2.5);
		_customplotwater->graph(resultlist.size() + 1)->setPen(p1);
		_customplotwater->graph(resultlist.size() + 1)->setLineStyle(QCPGraph::lsLine);
		_customplotwater->graph(resultlist.size() + 1)->setData(x1, y1);
		
		_customplotwater->xAxis->setRange(_minY - 10, _maxY + 10);
		_customplotwater->yAxis->setRange(_minZ - 10, _maxZ + 10);
		_customplotwater->xAxis->setVisible(false);
		_customplotwater->yAxis->setVisible(false);
		_customplotwater->replot();
	}

	void ModelImportDialog::getMinMaxValue()
	{
		if (_pointlist.size() < 1)return;
		QList<double> Xlist, Ylist, Zlist;
		for (int i = 0; i < _pointlist.size(); i++)
		{
			for (double* var : _pointlist[i])
			{
				Xlist << var[0];
				Ylist << var[1];
				Zlist << var[2];
			}
		}
		qSort(Xlist.begin(), Xlist.end());
		qSort(Ylist.begin(), Ylist.end()); 
		qSort(Zlist.begin(), Zlist.end());
		_minX = Xlist.front(); _maxX = Xlist.back();
		_minY = Ylist.front(); _maxY = Ylist.back(); 
		_minZ = Zlist.front(); _maxZ = Zlist.back();
	}

	void ModelImportDialog::on_pushButton_import_clicked()
	{
		QString dir = Setting::BusAPI::instance()->getWorkingDir();
		QString fillter = tr("Project file(*.xml)");
		QString title = tr("Open a project");
		QString filePath = QFileDialog::getOpenFileName(this, title, dir, fillter);
		if (filePath.isEmpty()) return;
		QFile file(filePath);
		if (!file.open(QFile::ReadOnly)) 
		{
			QMessageBox::warning(this, tr("error"), tr("open file failed!"));
			return;
		}
		QDomDocument doc;
		if (!doc.setContent(&file))
		{
			file.close();
			return;
		}
		 file.close();
		 QDomElement root = doc.documentElement(); //返回根节点
		 QDomNode node = root.firstChild(); //获得第一个子节点
		  while (!node.isNull())  //如果节点不空
		  {
			  if (node.isElement()) //如果节点是元素
			  {
				  QDomElement e = node.toElement(); 
				  QDomNodeList list = e.childNodes();
				  QList<double*>templist{};
				  for (int i = 0; i < list.count(); i++) //遍历子元素，count和size都可以用,可用于标签数计数
				  {
					 
					  QDomNode n = list.at(i);
					  QDomElement ne = n.toElement();
					  double* temp = new double[3];
					  temp[0] = ne.attribute("X").toDouble();
					  temp[1] = ne.attribute("Y").toDouble();
					  temp[2] = ne.attribute("Z").toDouble();
					  templist.push_back(temp);
			
					  if (i == list.count() - 1)
						  _pointlist.push_back(templist);
				  }
			  }
			  node = node.nextSibling();
			 
		  }
		  updateDialog();
		  getMinMaxValue();
		  initsection();
		  initwater();
	}

	void ModelImportDialog::on_pushButton_save_clicked()
	{
		QString dir = Setting::BusAPI::instance()->getWorkingDir();
		QString fillter = tr("Project file(*.xml)");
		QString title = tr("Open a project");
		QString filePath = QFileDialog::getSaveFileName(this, title, dir, fillter);
		if (filePath.isEmpty()) return;
		QFile file(filePath);
		if (!file.open(QIODevice::WriteOnly | QFile::Truncate))
		{
			QMessageBox::warning(this, tr("error"), tr("save file failed!"));
			return;
		}

		QDomDocument doc;
		//写入xml头部
		QDomProcessingInstruction instruction; //
		instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(instruction);
		QDomElement root = doc.createElement("SplitResult");
		doc.appendChild(root);
		QDomAttr time = doc.createAttribute("time");
		QDateTime current_date_time = QDateTime::currentDateTime();
		QString current_date = current_date_time.toString("yyyy.MM.dd hh:mm:ss");
		time.setValue(current_date);
		root.setAttributeNode(time);
		int i = 1;
		for (QList<double*> var : _pointlist)
		{

			QList<double*> lastlist{};
			for (QList<double*>::iterator it = var.begin(); it != var.end(); ++it)
			{
				lastlist.push_back(*it);
			}
			//边里有几个点

			QDomElement book = doc.createElement("sta");
			root.appendChild(book);
			book.setAttribute("index", i);
			book.setAttribute("ptNum", lastlist.size());
		
			for (int i = 0; i < lastlist.size(); ++i)
			{
				QDomElement pt = doc.createElement("pt");
				pt.setAttribute("index", i);
				pt.setAttribute("X", QString::number(lastlist[i][0], 'f', 1));
				pt.setAttribute("Y", QString::number(lastlist[i][1], 'f', 1));
				pt.setAttribute("Z", QString::number(lastlist[i][2], 'f', 1));
				book.appendChild(pt);
			}
			++i;
		}
		QTextStream out_stream(&file);
		doc.save(out_stream, 4); //缩进4格
		file.close();
	}

	void ModelImportDialog::graphClicked(QCPAbstractPlottable*plottable, int dataIndex)
	{
		double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
		double datakey=plottable->interface1D()->dataMainKey(dataIndex);
		double findpt[2]{datakey, dataValue};

		QString message = QString("Clicked on graph '%1' at data point #%2 with value %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue);
		int index=findPointlistIndex(dataIndex,findpt);
		if (index == -1) return;

		SingleCurveDialog* dlg = new SingleCurveDialog(index,_waterlist);
		connect(dlg, SIGNAL(signal_dataChanged(int, QList<QList<double*>>)), this, SLOT(getCurveData(int, QList<QList<double*>>)));
		dlg->exec();
		delete dlg;
		dlg = nullptr;
	}

	void ModelImportDialog::getCurveData(int i, QList<QList<double*>> waterlist)
	{

		qDebug() << "New:" << endl;
		for (double* var : waterlist[i])
		{
			qDebug() << var[0] << " " << var[1] << "" << var[2] << endl;
		}
		qDebug() << "Old:" << endl;
		for (double* var : _waterlist[i])
		{
			qDebug() << var[0] << " " << var[1] << "" << var[2] << endl;
		}
		_waterlist.replace(i, waterlist[i]);
		qDebug() << "AfterUpdate：" << endl;
		for (double* var : _waterlist[i])
		{
			qDebug() << var[0] << " " << var[1] << "" << var[2] << endl;
		}
		QList<double*> newlist{};
		int mid = (_waterlist.size() + 1) / 2;
		if (i<=mid) newlist = _waterlist[i] + _symwaterlist[i];
		else newlist = _symwaterlist[i] + _waterlist[i];
		qDebug() << "pointlist[i]" << endl;
		for (double* var : _pointlist[i])
		{
			qDebug() << var[0] << " " << var[1] << "" << var[2] << endl;
		}
		qDebug() << "newlist:" << endl;
		for (double* var : newlist)
		{
			qDebug() << var[0] << " " << var[1] << "" << var[2] << endl;
		}
		_pointlist.replace(i, newlist);
		qDebug() << "NEWpointlist[i]" << endl;
		for (double* var :_pointlist[i])
		{
			qDebug() << var[0] << " " << var[1] << "" << var[2] << endl;
		}

		updateDialog();
		getMinMaxValue();
		initsection();
		initwater();
	}

	int ModelImportDialog::findPointlistIndex(int index, double* pt)
	{
		int k = -1;
		for (int i = 0; i < _waterlist.size(); i++)
		{
			if (_waterlist[i][index][1] == pt[0] && _waterlist[i][index][2] == pt[1]) k = i;
		}
		return k;
	}

}