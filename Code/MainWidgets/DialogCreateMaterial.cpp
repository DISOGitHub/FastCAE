#include "DialogCreateMaterial.h"
#include "ui_DialogCreateMaterial.h"
#include "Material/Material.h"
#include "Material/MaterialSingletion.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/MaterialConfig.h"
#include <QDebug>
#include <QMessageBox>
#include "Material/MaterialFactory.h"
#include "python/PyAgent.h"

namespace MainWidget
{
	CreateMaterialDialog::CreateMaterialDialog(GUI::MainWindow* mainwindow) : QFDialog(mainwindow)
	{
		_ui = new Ui::DialogCreateMaterial;
		_ui->setupUi(this);
		init();
	}
	CreateMaterialDialog::~CreateMaterialDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void CreateMaterialDialog::init()
	{
		int maxid = Material::Material::getMaxID();
		_ui->materialName->setPlaceholderText(QString("Material_%1").arg(maxid + 1));
		QStringList materialtypes = ConfigOption::ConfigOption::getInstance()->getMaterialConfig()->getMaterialTypes();
		for (int i = 0; i < materialtypes.size(); ++i)
		{
			QString m = materialtypes.at(i);
			_ui->comboBox->addItem(m);
		}
		QStringList regs = Material::MaterialFactory::getRegTypes();
		for (QString m : regs)
		{
			_ui->comboBox->addItem(m);
		}
	}

	void CreateMaterialDialog::accept()
	{
		QString sname = _ui->materialName->text();
		QString name{};
		if (sname.isEmpty())
			sname = _ui->materialName->placeholderText();
		else if (sname.simplified().isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Material name can not be empty!"));
			return;
		}
		bool isequal = false;


		Material::MaterialSingleton* s = Material::MaterialSingleton::getInstance();
		const int nmodle = s->getMaterialCount();
		for (int i = 0; i < nmodle; ++i)
		{
			Material::Material*  data = s->getMaterialAt(i);
			
			QString modelName = data->getName();
			if (sname == modelName)
			{
				isequal = true;
				break;
			}
		}
		if (isequal)
		{
			QString info = tr("Material \"%1\" has already  existed !").arg(sname);
			QMessageBox::warning(this, tr("Warning"), info);
			return;
		}
		name = sname;
		const QString type = _ui->comboBox->currentText();
		if (type.isEmpty())
		{
			QMessageBox::warning(this, tr("Warning"), tr("Material type can not be empty!"));
			return;
		}

		QString code = QString("ControlPanel.CreateMaterial('%1','%2')").arg(name).arg(type);
		Py::PythonAagent::getInstance()->submit(code);

		/*Material::Material* material = nullptr;

		auto ma = ConfigOption::ConfigOption::getInstance()->getMaterialConfig()->getMaterialByType(type);
		if (ma != nullptr)
		{
			material = new Material::Material;
			material->copy(ma);
		}
		else
		{
			material = Material::MaterialFactory::createMaterial(type);
		}

		if (material == nullptr)
		{
			QMessageBox::warning(this, tr("Warning"), tr("Material create failed!"));
			return;
		}
		material->setName(name);
		
		Material::MaterialSingleton::getInstance()->appendMaterial(material);*/


		QDialog::accept();
	
	}

}