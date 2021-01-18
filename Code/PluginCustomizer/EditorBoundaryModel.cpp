#include "EditorBoundaryModel.h"
#include "ui_EditorBoundaryModel.h"
#include "BoundaryModel.h"
#include <QDebug>
#include <QSignalMapper>


namespace FastCAEDesigner
{
	EditorBoundaryModel::EditorBoundaryModel(QTreeWidgetItem* treeItem, BoundaryModel* model, QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EditorBoundaryModel),
		_model(model),
		_treeItem(treeItem)
	{
		ui->setupUi(this);
		Init();
		//ui->groupBox->hide();
	}

	EditorBoundaryModel::~EditorBoundaryModel()
	{
		delete ui;
	}

	//系统初始化函数
	void EditorBoundaryModel::Init()
	{
		//隐藏根节点的基本信息
		ui->groupBox->setHidden(true);

		_commonConditionChkboxList.append(ui->chk_vt);
		_commonConditionChkboxList.append(ui->chk_press);
		_commonConditionChkboxList.append(ui->chk_temp);
		_commonConditionChkboxList.append(ui->chk_fix_support);
		_commonConditionChkboxList.append(ui->chk_displacement);
		_commonConditionChkboxList.append(ui->chk_angle_vt);
		_commonConditionChkboxList.append(ui->chk_rotation_angle);
		_commonConditionChkboxList.append(ui->chk_acc);
		_commonConditionChkboxList.append(ui->chk_acc_angle);

		/*
		ui->chk_vt->setWindowTitle("Velocity");
		ui->chk_press->setWindowTitle("Pressure");
		ui->chk_temp->setWindowTitle("Temperature");
		ui->chk_fix_support->setWindowTitle("FixSupport");
		ui->chk_displacement->setWindowTitle("Displacement");
		ui->chk_angle_vt->setWindowTitle("AngleVelocity");
		ui->chk_rotation_angle->setWindowTitle("Rotation");
		ui->chk_acc->setWindowTitle("Acceleration");
		ui->chk_acc_angle->setWindowTitle("AngleAcceleration");
		*/
		_keyDict.clear();
		_keyDict.insert(ui->chk_vt,"velocity");
		_keyDict.insert(ui->chk_press,"pressure");
		_keyDict.insert(ui->chk_temp,"temperature");
		_keyDict.insert(ui->chk_fix_support,"fixSupport");
		_keyDict.insert(ui->chk_displacement,"displacement");
		_keyDict.insert(ui->chk_angle_vt,"angleVelocity");
		_keyDict.insert(ui->chk_rotation_angle,"rotation");
		_keyDict.insert(ui->chk_acc,"acceleration");
		_keyDict.insert(ui->chk_acc_angle,"angleAcceleration");

		_commonBoundaryChkboxList.append(ui->chk_inlet);
		_commonBoundaryChkboxList.append(ui->chk_outlet);
		_commonBoundaryChkboxList.append(ui->chk_symmetry);
		_commonBoundaryChkboxList.append(ui->chk_wall);

		/*
		ui->chk_inlet->setWindowTitle("Inlet");
		ui->chk_outlet->setWindowTitle("Outlet");
		ui->chk_symmetry->setWindowTitle("Symmetry");
		ui->chk_wall->setWindowTitle("Wall");
		*/
		_keyDict.insert(ui->chk_inlet,"inlet");
		_keyDict.insert(ui->chk_outlet,"outlet");
		_keyDict.insert(ui->chk_symmetry,"symmetry");
		_keyDict.insert(ui->chk_wall,"wall");

		if (nullptr != _model)
		{
			UpdateDataToUi();
			SetIsEdit(_model->GetIsEdit());
			InitCheckBoxGroup();
			InitCheckBoxSlot();
		}

		connect(ui->btnOk, SIGNAL(clicked()), this, SLOT(OnBtnOkClicked()));
		connect(ui->btnCancel, SIGNAL(clicked()), this, SLOT(close()));
		connect(ui->chk_commonCondition, SIGNAL(clicked(bool)), this, SLOT(OnChkCommonConditionClicked(bool)));
		connect(ui->chk_commonboundary, SIGNAL(clicked(bool)), this, SLOT(OnChkCommonBoundaryClicked(bool)));
	}

	//初始化所有复选框控件点击的槽函数
	void EditorBoundaryModel::InitCheckBoxSlot()
	{
		QSignalMapper *signalMapper = new QSignalMapper(this);
		ConnectCheckBoxGroupSlot(1, _commonConditionChkboxList, signalMapper);
		ConnectCheckBoxGroupSlot(2, _commonBoundaryChkboxList, signalMapper);
		connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(OnCheckboxGroupItemClicked(int)));
	}
	
	//绑定指定组复选框的槽响应函数到收发器。1、groupIndex：组的所有。2、itemList：复选框列表。3、signalMapper：收发器指针。
	void EditorBoundaryModel::ConnectCheckBoxGroupSlot(int groupIndex, QList<QCheckBox*> itemList, QSignalMapper *signalMapper)
	{
		foreach(QCheckBox* item, itemList)
		{
			if (nullptr == item)
				continue;

			connect(item, SIGNAL(clicked(bool)), signalMapper, SLOT(map()));
			signalMapper->setMapping(item, groupIndex);
		}
	}

	//所有复选框控件响应的槽函数：index为组的索引，1为基本条件。2为边界条件
	void EditorBoundaryModel::OnCheckboxGroupItemClicked(int index)
	{
		bool b = GetCheckBoxGroupState(index);

		if (index == 1)
			ui->chk_commonCondition->setChecked(b);
		else
			ui->chk_commonboundary->setChecked(b);
	}

	//槽函数---Ok按钮
	void EditorBoundaryModel::OnBtnOkClicked()
	{
		UpdateUiToData();
		
		if (nullptr != _treeItem)
		{
			_treeItem->setText(0, ui->txtChineseName->text());
		}

		QString paraStr = CreateBoundaryConditionParameter();
		qDebug() << paraStr;
		_model->SetCommonCondition(paraStr);
		this->accept();
		close();
	}

	//刷新数据到UI
	void EditorBoundaryModel::UpdateDataToUi()
	{
		if (nullptr == _model)
			return;

		ui->txtChineseName->setText(_model->GetChnName());
		ui->txtEnglishName->setText(_model->GetEngName());
		ui->txtIcon->setText(_model->GetIconName());
	}

	//刷新UI信息到数据
	void EditorBoundaryModel::UpdateUiToData()
	{
		if (nullptr == _model)
			return;

		_model->SetChnName(ui->txtChineseName->text());
		_model->SetEngName(ui->txtEnglishName->text());
		_model->SetIconName(ui->txtIcon->text());
	}

	//初始化各个复选框控件选中状态
	void EditorBoundaryModel::InitCheckBoxGroup()
	{
		if (nullptr == _model)
			return;

		QStringList list = _model->GetCommanCondition().split(',');
		bool b1 = InitCheckboxState(list, _commonConditionChkboxList);
		bool b2 = InitCheckboxState(list, _commonBoundaryChkboxList);
		ui->chk_commonCondition->setChecked(b1);
		ui->chk_commonboundary->setChecked(b2);
	}

	//初始化给定参数参数列表复选框选中状态，只要有一个复选框为选中状态，返回true，否则返回false
	bool EditorBoundaryModel::InitCheckboxState(QStringList list, QList<QCheckBox*> chkList)
	{
		bool b = false;
		int num = chkList.count();
		
		for (int i = 0; i < num; i++)
		{
			QCheckBox* item = chkList[i];
			
			if (nullptr == item)
				continue;

			QString name = _keyDict[item].toLower();
			
			if (0 == name.length())
				continue;

			if (list.contains(name))
			{
				item->setChecked(true);
				b = true;
			}
		}

		return b;
	}

	//设定基本参数是否可以编辑状态
	void EditorBoundaryModel::SetIsEdit(bool b)
	{
		ui->txtChineseName->setEnabled(b);
		ui->txtEnglishName->setEnabled(b);
		ui->txtIcon->setEnabled(b);
		ui->btnLoadIcon->setEnabled(b);
	}

	//建立所有参数列表的字符串数组
	QString EditorBoundaryModel::CreateBoundaryConditionParameter()
	{
		QStringList list;
		list.clear();
		CreateConditionParameterQStringList(list, _commonConditionChkboxList);
		CreateConditionParameterQStringList(list, _commonBoundaryChkboxList);
		return list.join(',');
	}

	//建立给定参数组条件选中的字符串列表
	void EditorBoundaryModel::CreateConditionParameterQStringList(QStringList &pList, QList<QCheckBox*> chkList)
	{
		for (int i = 0; i < chkList.count(); i++)
		{
			QCheckBox* item = chkList[i];

			if (nullptr == item)
				continue;
			
			if (item->checkState() == Qt::Checked)
			{
				QString keyValue = _keyDict[item];

				if (0 == keyValue.length())
					continue;

				pList.append(keyValue.toLower());
			}
		}
	}

	//槽函数-通用条件checkbox被点击
	void EditorBoundaryModel::OnChkCommonConditionClicked(bool b)
	{
		SetCheckBoxListState(_commonConditionChkboxList, b);
	}

	//槽函数-边界条件checkbox被点击
	void EditorBoundaryModel::OnChkCommonBoundaryClicked(bool b)
	{
		SetCheckBoxListState(_commonBoundaryChkboxList, b);
	}

	//设定指定组复选框控件的选中状态
	void EditorBoundaryModel::SetCheckBoxListState(QList<QCheckBox*> itemList, bool b)
	{
		int num = itemList.count();

		for (int i = 0; i < num; i++)
		{
			QCheckBox *item = itemList.at(i);

			if (nullptr == item)
				continue;

			item->setChecked(b);
		}
	}

	//得到指定组是否有控件复选框被设置选中状态
	bool EditorBoundaryModel::GetCheckBoxGroupState(int groupIndex)
	{
		if (1 == groupIndex)
			return GetCheckBoxGroupState(_commonConditionChkboxList);
		else
			return GetCheckBoxGroupState(_commonBoundaryChkboxList);
	}

	//指定组的复选框控件只要有一个被选中，则返回true，否则返回false
	bool EditorBoundaryModel::GetCheckBoxGroupState(QList<QCheckBox*> itemList)
	{
		bool b = false;

		foreach(QCheckBox* item, itemList)
		{
			if (nullptr == item)
				continue;

			if (item->checkState() == Qt::Checked)
				return true;
		}

		return b;
	}
}