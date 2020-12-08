#include "DialogLoadMaterial.h"
#include "ui_DialogLoadMaterial.h"
#include "Material.h"
#include "MaterialSingletion.h"
#include <QMessageBox>
#include "python/PyAgent.h"
#include <QDebug>
namespace Material
{
	LoadMaterialDialog::LoadMaterialDialog(GUI::MainWindow* m,QHash<QString, Material*> ma)
		: _ui(new Ui::LoadMaterialDialog), _maHash(ma), QFDialog(m)
	{
		_ui->setupUi(this);
		init();
	}
	LoadMaterialDialog::~LoadMaterialDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void LoadMaterialDialog::init()
	{
		QList<Material*> materialList = _maHash.values();
		const int n = materialList.size();

		for (int i = 0; i < n; ++i)
		{
			Material* m = materialList.at(i);
			QListWidgetItem* item = new QListWidgetItem(QIcon("://QUI/icon/material.png"), m->getName());
			_ui->availableList->addItem(item);
		}

	}

	void LoadMaterialDialog::accept()
	{
		QStringList materiallist = this->getSelectedMaterials();
		QString namelist = materiallist.join(",");
		QString code = QString("ControlPanel.loadFromMaterialLib('%1')").arg(namelist);
		Py::PythonAagent::getInstance()->submit(code);
/*
		const int n = materiallist.size();
		if (n <= 0) return;
		MaterialSingleton* s = MaterialSingleton::getInstance();
		for (int i = 0; i < n; ++i)
		{
			QString ms = materiallist.at(i);
			Material* ori = _maHash.value(ms);
			if (ori == nullptr)  continue;
			for (int i = 0; i < s->getMaterialCount(); i++){
				if (ori->getName() == s->getMaterialAt(i)->getName())
				{
					QMessageBox::warning(NULL, tr("Warning"), tr("The material that you want to load from lib already exists!"));
					return;
				}
			}
			Material* nma = new Material;
			nma->copy(ori);
			s->appendMaterial(nma);
		}*/

		QDialog::accept();

	}

	void LoadMaterialDialog::on_addButton_clicked()
	{
		int iSelectListItemNumber = _ui->selectedList->count();
		
		QList<QListWidgetItem*> itemList = _ui->availableList->selectedItems();
		const int n = itemList.size();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* availitem = itemList.at(i);
			const int row = _ui->availableList->row(availitem);
			QListWidgetItem* selectitem = new QListWidgetItem(availitem->icon(), availitem->text());
			const int id = availitem->data(Qt::UserRole).toInt();
			selectitem->setData(Qt::UserRole, id);
			_ui->selectedList->addItem(selectitem);
			_ui->availableList->takeItem(row);
		}
	}

	void LoadMaterialDialog::on_removeButton_clicked()
	{
		QList<QListWidgetItem*> itemList = _ui->selectedList->selectedItems();
		const int n = itemList.size();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* selectitem = itemList.at(i);
			const int row = _ui->selectedList->row(selectitem);
			QListWidgetItem* availitem = new QListWidgetItem(selectitem->icon(), selectitem->text());
			const int id = selectitem->data(Qt::UserRole).toInt();
			availitem->setData(Qt::UserRole, id);
			_ui->availableList->addItem(availitem);

			_ui->selectedList->takeItem(row);
		}
	}

	QStringList LoadMaterialDialog::getSelectedMaterials()
	{
		QStringList namelist;
		const int n = _ui->selectedList->count();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem*item = _ui->selectedList->item(i);
			const QString text = item->text();
			namelist.append(text);
		}
		return namelist;
	}

}