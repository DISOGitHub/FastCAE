#include "EditorCurveModel.h"
#include "ui_EditorCurveModel.h"
#include <QPushButton>
#include "Post2DCurveModel.h"
#include "Post3DFileModel.h"
#include "EditorCurveInfo.h"
#include "Editor3DFileValue.h"
#include "DataManager.h"
#include "InputValidator.h"
#include "ConfigOptions/PostCurve.h"
#include <QStringList>
#include <QPixmap>
#include <QBitmap>
#include <QTimer>
#include <QIcon>
#include <QToolButton>
#include <QDebug>
#include <QMessageBox>

#pragma execution_character_set("utf-8")

namespace FastCAEDesigner{
	EditorCurveModel::EditorCurveModel(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorCurveModel)
	{
		ui->setupUi(this);
	}
	//curve信息编辑入口
	EditorCurveModel::EditorCurveModel(QTreeWidgetItem* treeItem, Post2DCurveModel* curveModel, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorCurveModel),
		_curveModel(curveModel),
		_treeWidgetItem(treeItem)
	{
		ui->setupUi(this);
		this->setWindowTitle(tr("Curve Parameter"));

		Init();
	}
	//vector信息编辑入口
	EditorCurveModel::EditorCurveModel(QTreeWidgetItem* treeItem,Post3DFileModel* fileModel, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorCurveModel),
		_fileModel(fileModel),
		_treeWidgetItem(treeItem)
	{
		ui->setupUi(this);
		this->setWindowTitle(tr("3D Graph Parameter"));

		Init_3D();
	}

	EditorCurveModel::~EditorCurveModel()
	{
		delete ui;
	}

	//curve operation//
	//curve信息初始化
	void EditorCurveModel::Init()
	{
		//ui->NameLE->setText(_curveModel->GetEngName());
		ui->comboBox->insertItem(0, tr(".dat"));
		ui->tableWidget->setColumnCount(4);
		ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		ui->tableWidget->setWindowFlags(Qt::FramelessWindowHint);
		ui->tableWidget->setShowGrid(false);
		ui->tableWidget->verticalHeader()->setVisible(false);
		ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);  //单击选择一行


		_curveInfoList = _curveModel->GetCurveList();

		InitFileName();
		updateCurveDataToUi();

		connect(ui->OKPBtn, SIGNAL(clicked()), this, SLOT(OnOkPBtnClicked()));
		connect(ui->CancelPBtn, SIGNAL(clicked()), this, SLOT(close()));
		connect(ui->InsertPBtn, SIGNAL(clicked()), this, SLOT(OnInsertPBtnClicked()));
	}
	//文件名初始化
	void EditorCurveModel::InitFileName()
	{
		QString fileName = _curveModel->getFileName();
		QStringList filenamelist = fileName.split(".");
		filenamelist.removeLast();
		QString name = filenamelist.join(".");

		ui->NameLE->setText(name);
	}
	//将curve数据显示到界面上
	void EditorCurveModel::updateCurveDataToUi()
	{
		_nameUsedList.clear();

		ui->tableWidget->reset();
		ui->tableWidget->clear();
		ui->tableWidget->setRowCount(0);

		QStringList headers;
		headers << QString(tr("Curve Name")) << QString(tr("X Variable")) << QString(tr("Y Variable")) << QString(tr("Operation"));
		//headers << "曲线名称" << "X轴变量" << "Y轴变量" << "操作";
		ui->tableWidget->setHorizontalHeaderLabels(headers);

		for (int i = 0; i < _curveInfoList.count(); i++)
		{
			ConfigOption::PostCurve* postCurve = _curveInfoList.at(i);
			qDebug() << postCurve->getDescribe();

			if (postCurve->getDescribe().isEmpty())
				continue;

			int rowIndex = ui->tableWidget->rowCount();
			ui->tableWidget->insertRow(rowIndex);
// 			ConfigOption::PostCurve* postCurve = _curveInfoList.at(i);
// 			qDebug() << postCurve->getDescribe();

// 			if (postCurve->getDescribe().isEmpty())
// 				continue;

			insertRowtoTableWidget(ui->tableWidget, rowIndex, postCurve);
			//insertRowtoTableWidget(ui->tableWidget, rowIndex, _curveInfoList.at(i).getDescribe(), _curveInfoList.at(i).XAxisName, _curveInfoList.at(i).YAxisName);
			//insertRowtoTableWidget(ui->tableWidget, i, _curveInfoList.at(i).Name, _curveInfoList.at(i).XAxisName, _curveInfoList.at(i).YAxisName);
			_nameUsedList.append(_curveInfoList.at(i)->getDescribe());
		}
	}
	//插入一行数据到tablewidget中
	void EditorCurveModel::insertRowtoTableWidget(QTableWidget* TableWidget, int rowIndex, ConfigOption::PostCurve* postCurve)
	{
		QTableWidgetItem* Name = new QTableWidgetItem();
		QTableWidgetItem* XVariable = new QTableWidgetItem();
		QTableWidgetItem* YVariable = new QTableWidgetItem();

		Name->setText(postCurve->getDescribe());
		XVariable->setText(postCurve->getXVariable());
		YVariable->setText(postCurve->getYVariable());

		Name->setFlags(Name->flags() & ~Qt::ItemIsEditable);
		XVariable->setFlags(XVariable->flags() & ~Qt::ItemIsEditable);
		YVariable->setFlags(YVariable->flags() & ~Qt::ItemIsEditable);
		Name->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		XVariable->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		YVariable->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);


		TableWidget->setItem(rowIndex, 0, Name);
		TableWidget->setItem(rowIndex, 1, XVariable);
		TableWidget->setItem(rowIndex, 2, YVariable);
		Name->setFlags(Name->flags() & ~Qt::ItemIsEditable);
		
		createPushbuttonGroup(rowIndex);
	}
	//获取curvelist
	QList<ConfigOption::PostCurve*> EditorCurveModel::getCurveInfo()
	{
		return _curveInfoList;
	}

	QToolButton* EditorCurveModel::CreateButton(QString text, QString iconName)
	{
		QToolButton* btn = new QToolButton(this);
		btn->setText("");
		btn->setFixedSize(16, 16);
		QIcon icon/*(iconName)*/;
		icon.addFile(iconName,QSize(16,16));
		//btn->setFlat(true);
		btn->setStyleSheet("QToolButton{background: transparent;border:none}");
		btn->setIcon(icon);

		return btn;
	}

	QPushButton* EditorCurveModel::createEditButton()
	{
		QPushButton* btn = new QPushButton(this);
		btn->setObjectName("btn_add");
		btn->setText("");
		//btn->setFixedSize(QSize(16, 16));
		btn->setFlat(true);
		btn->setStyleSheet("QPushButton{border-image: url(:/btn_edit_normal.png);}"
			"QPushButton:pressed{border-image: url(:/btn_edit_clicked.png);}");

		return btn;
	}

	QPushButton* EditorCurveModel::createDeleteButton()
	{
		QPushButton* btn = new QPushButton(this);
		btn->setObjectName("btn_add");
		btn->setText("");
		//btn->setFixedSize(QSize(16, 16));
//		btn->setStyleSheet("background-image: url(:/QUI/geometry/selectwire.png);");
		btn->setStyleSheet("QPushButton{border-image: url(:/btn_delete_normal.png);border: 11px}"
			"QPushButton:pressed{border-image: url(:/btn_delete_clicked.png);}");

		return btn;
	}
	//创建按钮组
	void EditorCurveModel::createPushbuttonGroup(int rowIndex)
	{
		QSignalMapper* deleteMapper = new QSignalMapper(this);
		QSignalMapper* editMapper = new QSignalMapper(this);
		QHBoxLayout *h_box_layout = new QHBoxLayout();
		QWidget *widget = new QWidget();
		QToolButton* deletePBtn = CreateButton("", ":/delete.png");
		QToolButton* editPBtn = CreateButton("", ":/editor.png");
// #ifdef Q_OS_WIN
// 		QToolButton* deletePBtn = CreateButton("", ":/delete.png");
// 		QToolButton* editPBtn = CreateButton("", ":/editor.png");
// #endif
// #ifdef Q_OS_LINUX
// 		QPushButton* deletePBtn = new QPushButton;
// 		QPushButton* editPBtn = new QPushButton;
// 		deletePBtn->setText(tr("delete"));
// 		editPBtn->setText(tr("edit"));
// #endif 

		//QPushButton* deletePBtn = createDeleteButton();
		//QPushButton* editPBtn = createEditButton();

		connect(deletePBtn, SIGNAL(clicked()), deleteMapper, SLOT(map()));
		deleteMapper->setMapping(deletePBtn, rowIndex);
		connect(deleteMapper, SIGNAL(mapped(int)), this, SLOT(deleteRow(int)));

		connect(editPBtn, SIGNAL(clicked()), editMapper, SLOT(map()));
		editMapper->setMapping(editPBtn, rowIndex);
		connect(editMapper, SIGNAL(mapped(int)), this, SLOT(editRowInfo(int)));

		//h_box_layout->addStretch();
		h_box_layout->addWidget(editPBtn);
		h_box_layout->addWidget(deletePBtn);
		h_box_layout->setContentsMargins(0, 0, 0, 0);
		h_box_layout->setSpacing(1);
		widget->setLayout(h_box_layout);

		ui->tableWidget->setCellWidget(rowIndex, 3, widget);

	}
	//相关槽函数
	void EditorCurveModel::OnOkPBtnClicked()
	{
		if (_curveInfoList.isEmpty())
		{
			QMessageBox::warning(this, tr("prompt"), tr("There is no curve!"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return;
		}
		QString fileName = ui->NameLE->text().trimmed();
		fileName.append(ui->comboBox->currentText().trimmed());
		QList<QString> nameUsedList = getNameUsedList();
		nameUsedList.removeOne(_curveModel->getFileName());

		if (ui->NameLE->text().trimmed() == "")
		{
			QString errorMsg = tr("The name is empty.");
			ui->ErrorText->setText(errorMsg);
			ui->ErrorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}
		else if (nameUsedList.contains(fileName))
		{
			QString errorMsg = tr("The name is already in used.");
			ui->ErrorText->setText(errorMsg);
			ui->ErrorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}
		else if (InputValidator::getInstance()->FileNameIsAllow(ui->NameLE->text().trimmed()) == false)
		{
			QString errorMsg = tr("The name is illegal string.");
			ui->ErrorText->setText(errorMsg);
			ui->ErrorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		_curveModel->setFileName(fileName);
		_curveModel->setCurveList(_curveInfoList);

		if (_treeWidgetItem != nullptr)
		{
			_treeWidgetItem->setText(0, fileName);
		}

		this->accept();
		close();
	}

	void EditorCurveModel::OnInsertPBtnClicked()
	{
		EditorCurveInfo info;

		info.setNameUsedList(_nameUsedList);
		int r = info.exec();

		if (r != QDialog::Accepted)
			return;

		/*int rowIndex = ui->tableWidget->rowCount();*/
		//ui->tableWidget->insertRow(rowIndex);
		//SPost2DCurve spc；
		ConfigOption::PostCurve* postCurve = new ConfigOption::PostCurve;
		postCurve->setDescribe(info.getCurveName());
		postCurve->setXVariable(info.getXVariable());
		postCurve->setYVariable(info.getYVariable());
		//SPost2DCurve spc = SPost2DCurve{ rowIndex, info.getCurveName(), info.getXVariable(), info.getYVariable() };
		//_curveInfoList.append(spc);
		_curveInfoList.append(postCurve);
		updateCurveDataToUi();
		//insertRowtoTableWidget(ui->tableWidget, rowIndex, info.getCurveName(), info.getXVariable(), info.getYVariable());	

	}

	void EditorCurveModel::deleteRow(int rowIndex)
	{

		_curveInfoList.removeAt(rowIndex);

		updateCurveDataToUi();

	}

	void EditorCurveModel::editRowInfo(int rowIndex)
	{
		ConfigOption::PostCurve* currentCurve = _curveInfoList.at(rowIndex);

		QString curvename = currentCurve->getDescribe();
		QString xvariable = currentCurve->getXVariable();
		QString yvariable = currentCurve->getYVariable();

		EditorCurveInfo info(curvename, xvariable, yvariable);
		QList<QString> nameUsedList = _nameUsedList;

		nameUsedList.removeOne(curvename);
		info.setNameUsedList(nameUsedList);
		//info.InitCurveInfo(curvename, xvariable, yvariable);

		int r = info.exec();

		if (r != QDialog::Accepted)
			return;

		ConfigOption::PostCurve* postCurve = new ConfigOption::PostCurve;
		postCurve->setDescribe(info.getCurveName());
		postCurve->setXVariable(info.getXVariable());
		postCurve->setYVariable(info.getYVariable());
		//{ rowIndex, info.getCurveName(), info.getXVariable(), info.getYVariable() };
		_curveInfoList.replace(rowIndex, postCurve);

		updateCurveDataToUi();
	}


	//3d operation//
	//vector初始化
	void EditorCurveModel::Init_3D()
	{
		ui->comboBox->insertItem(0, tr(".vtk"));
		ui->comboBox->insertItem(1, tr(".sol"));
		ui->comboBox->insertItem(2, tr(".cgns"));
		ui->comboBox->insertItem(3, tr(".dat"));

		ui->tableWidget->setColumnCount(5);
		ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		ui->tableWidget->setWindowFlags(Qt::FramelessWindowHint);
		ui->tableWidget->setShowGrid(false);
		ui->tableWidget->verticalHeader()->setVisible(false);

		//ui->NameLE->setText(_fileModel->GetEngName());
// 		QString name = _fileModel->getFileName();
// 		QStringList nameList = name.split(".");
// 		
// 		if (nameList.last() == "vtk")
//  			ui->comboBox->setCurrentIndex(0);
//  		else
//  			ui->comboBox->setCurrentIndex(1);
		InitFileName_3D();
		InitVariableList();
		updateCurveDataToUi_3D();

		connect(ui->OKPBtn, SIGNAL(clicked()), this, SLOT(OnOkPBtnClicked_3D()));
		connect(ui->CancelPBtn, SIGNAL(clicked()), this, SLOT(close()));
		connect(ui->InsertPBtn, SIGNAL(clicked()), this, SLOT(OnInsertPBtnClicked_3D()));
	}
	//vector文件名初始化
	void EditorCurveModel::InitFileName_3D()
	{
		QString fileName = _fileModel->getFileName();
		QStringList filenamelist = fileName.split(".");

		if (filenamelist.last() == "vtk")
			ui->comboBox->setCurrentIndex(0);
		else if (filenamelist.last() == "sol")
			ui->comboBox->setCurrentIndex(1);
		else if (filenamelist.last() == "cgns")
			ui->comboBox->setCurrentIndex(2);
		else if (filenamelist.last() == "dat")
			ui->comboBox->setCurrentIndex(3);
		else
			ui->comboBox->setCurrentIndex(0);

		filenamelist.removeLast();
		QString name = filenamelist.join(".");

		ui->NameLE->setText(name);
	}
	//初始化4中类型列表
	void EditorCurveModel::InitVariableList()
	{
		_nodeScalarList = _fileModel->getNodeScalarList();
		_cellScalarList = _fileModel->getCellScalarList();
		_nodeVectorList = _fileModel->getNodeVectorList();
		_cellVectorList = _fileModel->getCellVectorList();
	}
	//更新vector数据到界面
	void EditorCurveModel::updateCurveDataToUi_3D()
	{
		_nameUsedList.clear();

		ui->tableWidget->reset();
		ui->tableWidget->clear();
		ui->tableWidget->setRowCount(0);

		QStringList headers;
		headers << QString(tr("ID")) << QString(tr("Name")) << QString(tr("Type")) << QString(tr("DataType")) << QString(tr("Operation"));
		ui->tableWidget->setHorizontalHeaderLabels(headers);

		//note：xvdongming
		//insertNodeScalar();
		//insertCellScalar();
		//insertNodeVector();
		//insertCellVector();
		//note：xvdongming	

		//Added xvdongming 2020-02012
		insertCurve(_nodeScalarList, EDataType::ScalarNode);
		insertCurve(_cellScalarList, EDataType::ScalarCell);
		insertCurve(_nodeVectorList, EDataType::VectorNode);
		insertCurve(_cellVectorList, EDataType::VectorCell);
		//Added xvdongming 2020-02012
	}
	//插入数据到tablewidget
	void EditorCurveModel::insertRowtoTableWidget_3D(int rowIndex, QString name,QString type,QString dataType)
	{
		QTableWidgetItem* Id = new QTableWidgetItem;
		QTableWidgetItem* Name = new QTableWidgetItem;
		QTableWidgetItem* Type = new QTableWidgetItem;
		QTableWidgetItem* DataType = new QTableWidgetItem;

		Id->setText(QString::number(rowIndex+1));
		Name->setText(name);
		Type->setText(type);
		DataType->setText(dataType);

		Id->setFlags(Id->flags() & ~Qt::ItemIsEditable);
		Name->setFlags(Name->flags() & ~Qt::ItemIsEditable);
		Type->setFlags(Type->flags() & ~Qt::ItemIsEditable);
		DataType->setFlags(DataType->flags() & ~Qt::ItemIsEditable);

		Id->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		Name->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		Type->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		DataType->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		ui->tableWidget->setItem(rowIndex, 0, Id);
		ui->tableWidget->setItem(rowIndex, 1, Name);
		ui->tableWidget->setItem(rowIndex, 2, Type);
		ui->tableWidget->setItem(rowIndex, 3, DataType);
		createPushbuttonGroup_3D(rowIndex);
	}
	//更新编辑过完成的数据
	void EditorCurveModel::updateRowtoTableWidget_3D(int rowIndex, QString name, QString type, QString dataType)
	{
		QTableWidgetItem* Id = new QTableWidgetItem;
		QTableWidgetItem* Name = new QTableWidgetItem;
		QTableWidgetItem* Type = new QTableWidgetItem;
		QTableWidgetItem* DataType = new QTableWidgetItem;

		Id->setText(QString::number(rowIndex + 1));
		Name->setText(name);
		Type->setText(type);
		DataType->setText(dataType);

		Id->setFlags(Id->flags() & ~Qt::ItemIsEditable);
		Name->setFlags(Name->flags() & ~Qt::ItemIsEditable);
		Type->setFlags(Type->flags() & ~Qt::ItemIsEditable);
		DataType->setFlags(DataType->flags() & ~Qt::ItemIsEditable);

		Id->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		Name->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		Type->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
		DataType->setTextAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

		ui->tableWidget->setItem(rowIndex, 0, Id);
		ui->tableWidget->setItem(rowIndex, 1, Name);
		ui->tableWidget->setItem(rowIndex, 2, Type);
		ui->tableWidget->setItem(rowIndex, 3, DataType);
		createPushbuttonGroup_3D(rowIndex);
	}
	//创建按钮组
	void EditorCurveModel::createPushbuttonGroup_3D(int rowIndex)
	{
		QSignalMapper* deleteMapper = new QSignalMapper(this);
		//QSignalMapper* editMapper = new QSignalMapper(this);
		QHBoxLayout *h_box_layout = new QHBoxLayout();
		QWidget *widget = new QWidget();


		QToolButton* deletePBtn = CreateButton("", ":/delete.png");
	//	QToolButton* editPBtn = CreateButton("", ":/editor.png");
// #ifdef Q_OS_WIN
// 		QToolButton* deletePBtn = CreateButton("", ":/delete.png");
// 		QToolButton* editPBtn = CreateButton("", ":/editor.png");
// #endif
// #ifdef Q_OS_LINUX
// 		QPushButton* deletePBtn = new QPushButton;
// 		QPushButton* editPBtn = new QPushButton;
// 		deletePBtn->setText(tr("delete"));
// 		editPBtn->setText(tr("edit"));
// #endif 

		connect(deletePBtn, SIGNAL(clicked()), deleteMapper, SLOT(map()));
		deleteMapper->setMapping(deletePBtn, rowIndex);
		connect(deleteMapper, SIGNAL(mapped(int)), this, SLOT(deleteRow_3D(int)));

// 		connect(editPBtn, SIGNAL(clicked()), editMapper, SLOT(map()));
// 		editMapper->setMapping(editPBtn, rowIndex);
// 		connect(editMapper, SIGNAL(mapped(int)), this, SLOT(editRowInfo_3D(int)));

		//h_box_layout->addWidget(editPBtn);//Noted:xvdongming 2020-02-12 三维后处理文件--曲线不允许编辑，只允许删除和添加。
		h_box_layout->addWidget(deletePBtn);
	//	h_box_layout->setContentsMargins(0, 0, 0, 0);
	//	h_box_layout->setSpacing(1);
		widget->setLayout(h_box_layout);

		ui->tableWidget->setCellWidget(rowIndex, 4, widget);
	}
	//相关槽函数
	void EditorCurveModel::deleteRow_3D(int rowIndex)
	{
		QString name = ui->tableWidget->item(rowIndex, 1)->text().trimmed();
		QString type = ui->tableWidget->item(rowIndex, 2)->text().trimmed();
		QString dataType = ui->tableWidget->item(rowIndex, 3)->text().trimmed();

		removeNameFromList(name, type, dataType);
		
		updateCurveDataToUi_3D();
	}

	void EditorCurveModel::editRowInfo_3D(int rowIndex)
	{
		QString name = ui->tableWidget->item(rowIndex, 1)->text().trimmed();
		QString type = ui->tableWidget->item(rowIndex, 2)->text().trimmed();
		QString dataType = ui->tableWidget->item(rowIndex, 3)->text().trimmed();
		
		QList<QString> nameUsedList = _nameUsedList;
		Editor3DFileValue info(name, type, dataType);

		nameUsedList.removeOne(name);
		info.setNameUsedList(nameUsedList);

		int r = info.exec();

		if (r != QDialog::Accepted)
			return;

		if (info.getName() != name)
		{
			if ((info.getType() != type) || (info.getDataType() != dataType))
				insertNameToList(info.getName(), info.getType(), info.getDataType());
			else
				replaceNameFromList(name, info.getName(), info.getType(), info.getDataType());
		}

		updateRowtoTableWidget_3D(rowIndex, info.getName(), info.getType(), info.getDataType());

	}

	void EditorCurveModel::OnInsertPBtnClicked_3D()
	{
		Editor3DFileValue info;

		info.setNameUsedList(_nameUsedList);
		int r = info.exec();

		if (r != QDialog::Accepted)
			return;

		insertNameToList(info.getName(), info.getType(), info.getDataType());
		//_nameUsedList.append(info.getName());
		updateCurveDataToUi_3D();
		
	}

	void EditorCurveModel::OnOkPBtnClicked_3D()
	{
		QString name = ui->NameLE->text().trimmed();
		name.append(ui->comboBox->currentText().trimmed());
		QList<QString> nameUsedList = getNameUsedList();
		nameUsedList.removeOne(_fileModel->getFileName());

		if (ui->NameLE->text().trimmed() == "")
		{
			QString errorMsg = tr("The name is empty.");
			ui->ErrorText->setText(errorMsg);
			ui->ErrorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}
		else if (nameUsedList.contains(name))
		{
			QString errorMsg = tr("The name is already in used.");
			ui->ErrorText->setText(errorMsg);
			ui->ErrorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}
		else if (InputValidator::getInstance()->FileNameIsAllow(ui->NameLE->text().trimmed()) == false)
		{
			QString errorMsg = tr("The name is illegal string.");
			ui->ErrorText->setText(errorMsg);
			ui->ErrorText->show();
			QTimer::singleShot(3000, this, SLOT(OnTimeout()));
			return;
		}

		_fileModel->setDataList(_nodeScalarList, _nodeVectorList, _cellScalarList, _cellVectorList);
		_fileModel->setFileName(name);

		//getNameUsedList();

		if (_treeWidgetItem != nullptr)
		{
			_treeWidgetItem->setText(0, name);
		}

		close();
	}
	//获取cell,sarlar类型数据列表
	QStringList EditorCurveModel::getCellScalarList()
	{
		return _cellScalarList;
	}
	//获取cell,vector类型数据列表
	QStringList EditorCurveModel::getCellVectorList()
	{
		return _cellVectorList;
	}
	//获取node,sarlar类型数据列表
	QStringList EditorCurveModel::getNodeScalarList()
	{
		return _nodeScalarList;
	}
	//获取node,vector类型数据列表
	QStringList EditorCurveModel::getNodeVectorList()
	{
		return _nodeVectorList;
	}

	//Added xvdongming 2020-02-12
	//根据给定的曲线类型，初始化表单中应用的字符串标识
	void EditorCurveModel::InitCurveTypeStringName(EDataType type,QString& sDataType, QString& sType)
	{
		if (EDataType::ScalarNode == type)
		{
			sType = QString("scalar");
			sDataType = QString("node");
		}
		else if (EDataType::VectorNode == type)
		{
			sType = QString("vector");
			sDataType = QString("node");
		}
		else if (EDataType::ScalarCell == type)
		{
			sType = QString("scalar");
			sDataType = QString("cell");
		}
		else if (EDataType::VectorCell == type)
		{
			sType = QString("vector");
			sDataType = QString("cell");
		}

		return;
	}

	//Added xvdongming 2020-02-12
	//插入曲线记录
	void  EditorCurveModel::insertCurve(QStringList curveList, EDataType type)
	{
		if (curveList.isEmpty())
			return;

		QString sDataType = "";
		QString sType = "";
		InitCurveTypeStringName(type, sDataType, sType);

		for (int i = 0; i < curveList.count(); i++)
		{
			QString name = curveList.at(i);

			if (0 == name.length())
				continue;
			
			int rowIndex = ui->tableWidget->rowCount();
			ui->tableWidget->insertRow(rowIndex);
			insertRowtoTableWidget_3D(rowIndex, name, sType, sDataType);
			_nameUsedList.append(name);
		}
	}

	//插入node,scalar类型数据
	void EditorCurveModel::insertNodeScalar()
	{
		if (_nodeScalarList.isEmpty())
			return;
		else
		{
			for (int i = 0; i < _nodeScalarList.count(); i++)
			{
				int rowIndex = ui->tableWidget->rowCount();
				ui->tableWidget->insertRow(rowIndex);

				QString name = _nodeScalarList.at(i);

				//Added xvdongming 2020-02-12
				//当曲线名称为空时，不填充曲线
				if (name.isEmpty())
					continue;
				//Added xvdongming 2020-02-12

				QString type = QString("scalar");
				QString dataType = QString("node");

				insertRowtoTableWidget_3D(rowIndex, name, type, dataType);
				_nameUsedList.append(name);
			}
		}
	}
	//插入node,vector类型数据
	void EditorCurveModel::insertNodeVector()
	{
		if (_nodeVectorList.isEmpty())
			return;
		else
		{
			for (int i = 0; i < _nodeVectorList.count(); i++)
			{
				int rowIndex = ui->tableWidget->rowCount();
				ui->tableWidget->insertRow(rowIndex);

				QString name = _nodeVectorList.at(i);
				
				//Added xvdongming 2020-02-12
				//当曲线名称为空时，不填充曲线
				if (name.isEmpty())
					continue;
				//Added xvdongming 2020-02-12

				QString type = QString("vector");
				QString dataType = QString("node");

				insertRowtoTableWidget_3D(rowIndex, name, type, dataType);
				_nameUsedList.append(name);
			}
		}
	}
	//插入cell,scalar类型数据
	void EditorCurveModel::insertCellScalar()
	{
		if (_cellScalarList.isEmpty())
			return;
		else
		{
			for (int i = 0; i < _cellScalarList.count(); i++)
			{
				int rowIndex = ui->tableWidget->rowCount();
				ui->tableWidget->insertRow(rowIndex);

				QString name = _cellScalarList.at(i);
				
				//Added xvdongming 2020-02-12
				//当曲线名称为空时，不填充曲线
				if (name.isEmpty())
					continue;
				//Added xvdongming 2020-02-12

				QString type = QString("scalar");
				QString dataType = QString("cell");

				insertRowtoTableWidget_3D(rowIndex, name, type, dataType);
				_nameUsedList.append(name);
			}
		}
	}
	//插入cell,vector类型数据
	void EditorCurveModel::insertCellVector()
	{
		if (_cellVectorList.isEmpty())
			return;
		else
		{
			for (int i = 0; i < _cellVectorList.count(); i++)
			{
				int rowIndex = ui->tableWidget->rowCount();
				ui->tableWidget->insertRow(rowIndex);

				QString name = _cellVectorList.at(i);

				//Added xvdongming 2020-02-12
				//当曲线名称为空时，不填充曲线
				if (name.isEmpty())
					continue;
				//Added xvdongming 2020-02-12
				
				QString type = QString("vector");
				QString dataType = QString("cell");

				insertRowtoTableWidget_3D(rowIndex, name, type, dataType);
				_nameUsedList.append(name);
			}
		}
	}
	//插入数据到相关类型的链表中
	void EditorCurveModel::insertNameToList(QString name,QString type, QString dataType)
	{
		if ((type == "scalar") && (dataType == "node"))
			_nodeScalarList.append(name);
		else if ((type == "scalar") && (dataType == "cell"))
			_cellScalarList.append(name);
		else if ((type == "vector") && (dataType == "node"))
			_nodeVectorList.append(name);
		else if ((type == "vector") && (dataType == "cell"))
			_cellVectorList.append(name);
	}
	//更新相关列表中的数据
	void EditorCurveModel::replaceNameFromList(QString oldname, QString name, QString type, QString dataType)
	{
		if ((type == "scalar") && (dataType == "node"))
			_nodeScalarList.replaceInStrings(oldname, name);
		else if ((type == "scalar") && (dataType == "cell"))
			_cellScalarList.replaceInStrings(oldname, name);
		else if ((type == "vector") && (dataType == "node"))
			_nodeVectorList.replaceInStrings(oldname, name);
		else if ((type == "vector") && (dataType == "cell"))
			_cellVectorList.replaceInStrings(oldname, name);
	}
	//删除相关链表中的数据
	void EditorCurveModel::removeNameFromList(QString name, QString type, QString dataType)
	{
		if ((type == "scalar") && (dataType == "node"))
			 _nodeScalarList.removeOne(name);
		else if ((type == "scalar") && (dataType == "cell"))
			_cellScalarList.removeOne(name);
		else if ((type == "vector") && (dataType == "node"))
			_nodeVectorList.removeOne(name);
		else if ((type == "vector") && (dataType == "cell"))
			_cellVectorList.removeOne(name);
	}


	//获取已经使用过的文件名
	QList<QString> EditorCurveModel::getNameUsedList()
	{
		QList<QString> nameList;
		ModelBase* parentModel = DataManager::getInstance()->GetModelFromDict(_treeWidgetItem->parent());
		QList<ModelBase*> childList = parentModel->GetChildList();

		if (childList.count() == 0)
			return nameList;

		for (int i = 0; i < childList.count(); i++)
		{
			ModelBase* model = childList.at(i);

			if (nullptr == model)
				continue;

		//	qDebug() << model->getFileName();

			nameList.append(model->getFileName());
		}

		return nameList;
	}

	//定时器槽函数
	void EditorCurveModel::OnTimeout()
	{
		ui->ErrorText->setText("");
		ui->ErrorText->hide();
	}
}