#include "DialogAddBC.h"
#include "ui_DialogAddBC.h"
#include "DataProperty/ParameterDouble.h"
#include "ModelData/modelDataBaseExtend.h"
#include "BCBase/BCBase.h"
#include "BCBase/BCUserDef.h"
#include "ParaClassFactory/BCFactory.h"
// #include "BCBase/BCTemperature.h"
// #include "BCBase/BCPressure.h"
// #include "BCBase/BCDisplacement.h"
#include "ModelData/modelDataBaseExtend.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshSet.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/BCConfig.h"
#include <assert.h>
#include "python/PyAgent.h"
#include <QDebug>
#include "ModelData/modelDataBase.h"
#include "ModelData/modelDataSingleton.h"
namespace ProjectTree
{
	
	AddBCDialog::AddBCDialog(GUI::MainWindow* m,ModelData::ModelDataBaseExtend* data) 
		: QFDialog(m), _data(data)
	{
		_ui = new Ui::AddBCDialog();
		_ui->setupUi(this); 
		init();
	}
	AddBCDialog::~AddBCDialog()
	{

	}
	void AddBCDialog::init()
	{
		initBCType();
		initMeshSet();
	}
	void AddBCDialog::initMeshSet()
	{
		MeshData::MeshData* mesh = MeshData::MeshData::getInstance();
		QList<int> comList = _data->getMeshSetList();
		for (int i = 0; i < comList.size(); ++i)
		{
			int setID = comList.at(i);
			MeshData::MeshSet* set = mesh->getMeshSetByID(setID);
			assert(set != nullptr);
			QString name = set->getName();
			_ui->setComboBox->addItem(name, setID);
		}
	}
	void AddBCDialog::initBCType()
	{
		int beg = BCBase::BCType::None;
		int end = BCBase::BCType::End;
		ConfigOption::BCConfig* userdefbcs = ConfigOption::ConfigOption::getInstance()->getBCConfig();
		for (int i = beg; i < end; ++i)
		{
			QString stype = BCBase::BCTypeToString((BCBase::BCType)i);
			if (!userdefbcs->isEnable(stype,_data->getTreeType())) continue;
			_ui->typeComboBox->addItem(stype, i);
		}
//		ConfigOption::BCConfig* userdefbcs = ConfigOption::ConfigOption::getInstance()->getBCConfig();
		const int n = userdefbcs->getBCCount(_data->getTreeType());
		for (int i = 0; i < n; ++i)
		{
			BCBase::BCUserDef* bc = userdefbcs->getBCAt(i, _data->getTreeType());
			_ui->typeComboBox->addItem(bc->getName(), BCBase::UserDef);
		}
	}
	/*void AddBCDialog::accept()
	{
		const int index = _ui->typeComboBox->currentIndex();
		BCBase::BCType bcType = (BCBase::BCType)(_ui->typeComboBox->itemData(index).toInt());
		QString name = _ui->typeComboBox->currentText();
		int setindex = _ui->setComboBox->currentIndex();
		QList<int> setidlist = _data->getMeshSetList();
		for (int i = 0; i < setidlist.size(); ++i)
		{ 
			qDebug() << QString::number(setidlist.at(i));
		}
		if (setindex < 0) return;
		const int id = setidlist.at(setindex);
		qDebug() << QString::number(id);
		MeshData::MeshSet* set = MeshData::MeshData::getInstance()->getMeshSetByID(id);
		assert(set != nullptr);
		QString bcTypeToString = BCBase::BCTypeToString(bcType);//liu
		BCBase::BCBase* bc = ParaClassFactory::BCFactory::createBCByType(bcType, name, _data->getTreeType());

		if (bc != nullptr)
		{
			//QString code = QString("Case.addBC(%1,%2,\"%3\")").arg(caseid).arg(id).arg(addIdValues.join(" "));//
			//qDebug() << code;
			//Py::PythonAagent::getInstance()->submit(code);
			bc->setType(bcType);
			bc->setMeshSetID(id);
			_data->appeendBC(bc);
		}*/
	void AddBCDialog::accept()
	{
//		const int index = _ui->typeComboBox->currentIndex();
//		BCBase::BCType bcType = (BCBase::BCType)(_ui->typeComboBox->itemData(index).toInt());  //Commented-Out By Baojun 
		int caseid = _data->getID();
		QString name = _ui->typeComboBox->currentText();
		int setindex = _ui->setComboBox->currentIndex();
		QList<int> setidlist = _data->getMeshSetList();
		if (setindex < 0) return;
		const int id = setidlist.at(setindex);
//		MeshData::MeshSet* set = MeshData::MeshData::getInstance()->getMeshSetByID(id); //Commented-Out By Baojun
//		if (set == nullptr) return;
//		int caseid = _data->getID();
//		assert(set != nullptr);
//		QString bctypetostring = BCBase::BCTypeToString(bcType);//liu
	
		//submit py code
		QString code = QString("Case.addBC(%1,%2,\"%3\")").arg(caseid).arg(id).arg(name);//
		qDebug() << code;
		Py::PythonAagent::getInstance()->submit(code);
		 
		QDialog::accept();
	}

}