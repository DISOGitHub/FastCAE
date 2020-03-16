#include "DialogCreatePhysicsModel.h"
#include <ui_DialogCreatePhysicsModel.h>
#include "PhysicsWidget.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/ProjectTreeConfig.h"
#include "ConfigOptions/ProjectTreeInfo.h"
#include "ModelData/modelDataBase.h"
#include "ModelData/modelDataBaseExtend.h"
#include "ModelData/modelDataSingleton.h"
#include "ModelData/modelDataFactory.h"
#include "mainWindow/mainWindow.h"
#include "settings/busAPI.h"
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <assert.h>
#include <QTextEdit>
#include "ConfigOptions/DataConfig.h"
#include <QRegExp>
#include "python/PyAgent.h"
//#include "TestDemos/TestModelData.h"

namespace MainWidget
{
	CreatePhysicsModel::CreatePhysicsModel(PhysicsWidget* pw, GUI::MainWindow* m) : QFDialog(m),
		_physicsWidget(pw)
	{
		_ui = new Ui::DialogCreateModel;
		_ui->setupUi(this);
		Qt::WindowFlags flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;
		setWindowFlags(flags);
		
		_ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
		_ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
		init();
	}

// 	CreatePhysicsModel::CreatePhysicsModel(const VecAddItem & items, PhysicsWidget* pw, GUI::MainWindow* m) : QFDialog(m),
// 		_physicsWidget(pw)
// 	{
// 		_ui = new Ui::DialogCreateModel;
// 		_ui->setupUi(this);
// 		connect(this, SIGNAL(updateActionStates()), m, SIGNAL(updateActionStatesSig()));
// 		init(items);
// 	}

	CreatePhysicsModel::~CreatePhysicsModel()
	{
//		_subWidgets.clear();
		delete _ui;
	}
// 
// 	QString CreatePhysicsModel::getControlText(const QString & name)
// 	{
// 		return _subWidgetsText[name];
// 	}

// 	QString CreatePhysicsModel::getSelectedTreeTypeName() const
// 	{
// 		return _ui->comboBox->currentText();
// 	}

	ProjectTreeType CreatePhysicsModel::getSelectedTreeType() const
	{
//		ModuleBase::ProjectTreeInfo* info = ModuleBase::ProjectTreeInfo::getInstance();
//		ConfigOption::ProjectTreeConfig* treeconfig = ConfigOption::ConfigOption::getInstance()->getTreeConfig();
		ConfigOption::ProjectTreeConfig* treeconfig = ConfigOption::ConfigOption::getInstance()->getProjectTreeConfig();
		const QList<ProjectTreeType> types = treeconfig->getTreeTypes();
		const int index = _ui->comboBox->currentIndex();
		assert(index >= 0 && index < types.size());
		return types.at(index);
	}

	void CreatePhysicsModel::init(/*const VecAddItem & items = VecAddItem()*/)
	{
		int id = ModelData::ModelDataBase::getMaxID() + 1;
		_ui->projectName->setPlaceholderText(QString("Case_%1").arg(id));
//		ConfigOption::ProjectTreeConfig* treeconfig = ConfigOption::ConfigOption::getInstance()->getTreeConfig();
		ConfigOption::ProjectTreeConfig* treeconfig = ConfigOption::ConfigOption::getInstance()->getProjectTreeConfig();
		const QList<ProjectTreeType> types = treeconfig->getTreeTypes();
		QString lang = Setting::BusAPI::instance()->getLanguage();

		const int nctype = types.size();
		for (int i = 0; i < nctype; ++i)
		{
			ProjectTreeType type = types.at(i);
			ConfigOption::ProjectTreeInfo* tree = treeconfig->getProjectTree(type);
			QString text;
			if (lang == "Chinese") text = tree->getChinese();
			else if (lang == "English") text = tree->getName();
			if (text.isEmpty()) text = tree->getName();

			_ui->comboBox->insertItem(i, text, type);
		}
		QHash<int, QString> regType = ModelData::ModelDataFactory::getRegedType();
		QList<int> typeList = regType.keys();
		for (int i = 0; i < typeList.size(); ++i)
		{
			const int t = typeList.at(i);
			QString text = regType.value(t);

			_ui->comboBox->insertItem(i+nctype, text, t);
		}
	}

	void CreatePhysicsModel::accept()
	{
		QString sname = _ui->projectName->text();
		QString name;
		if (sname.isEmpty())
		{
			name = _ui->projectName->placeholderText();
		}
		else if (sname.simplified().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Name can not be empty"));
			return;
		}
		if (name.isEmpty())
		{
			name = sname.simplified();
		}
		bool nameexist = false;
		ModelData::ModelDataSingleton* s = ModelData::ModelDataSingleton::getinstance();
		const int nmodle = s->getModelCount();
		for (int i = 0; i < nmodle; ++i)
		{
			ModelData::ModelDataBase* data = s->getModelAt(i);
			QString modelName = data->getName();
			if (name == modelName)
			{
				nameexist = true;
				break;
			}
		}
		if (nameexist)
		{
			QString info = tr("Project \"%1\" has already  existed !").arg(name);
			QMessageBox::warning(this, tr("Warning"), info);
			return;
		}
		if (name.toLocal8Bit().length() > 20)
		{
			QString info = tr("Name contains too many characters !");
			QMessageBox::warning(this, tr("Warning"), info);
			return;
		}

		QRegExp rx("[//\*:<>?\"\|]");
//		bool match = rx.exactMatch(name);
		int pos = rx.indexIn(name);
		if (pos>-1 || name.contains("\\"))
		{
			QString info = tr("Name can not contains fellowing char: \\/:*?\"<>|!");
			QMessageBox::warning(this, tr("Warning"), info);
			return;
		}


//		qDebug() << _ui->comboBox->currentIndex();
		if (_ui->comboBox->currentIndex() < 0)
		{
			QDialog::reject();
			return;
		}

		ConfigOption::ProjectTreeConfig* treeconfig = ConfigOption::ConfigOption::getInstance()->getProjectTreeConfig();
		ProjectTreeType type = (ProjectTreeType)_ui->comboBox->currentData().toInt();
		QString stype = treeconfig->getNameByType(type);

		if (stype.isEmpty())
		{
			QHash<int, QString> regType = ModelData::ModelDataFactory::getRegedType();
			stype = regType.value(type);
		}

		QString code = QString("ControlPanel.createCase(\"%1\",\"%2\")").arg(name).arg(stype);
		Py::PythonAagent::getInstance()->submit(code);
// 
// 
// 
// 		
// 		ModelData::ModelDataBase* modelBase = new ModelData::ModelDataBaseExtend(type);
// 
// 		assert(modelBase);
// 		modelBase->setName(name);
// 		modelBase->copyFormConfig();
// 		s->appendModel(modelBase);
// 
// 		_physicsWidget->updatePhysicsTree();
// 		emit updateActionStates();

		QDialog::accept();
	}


}