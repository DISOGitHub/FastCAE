#include "DialogMeshRename.h"
#include <QTreeWidgetItem>
#include <ui_DialogGeometrysetRename.h>
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "meshData/meshSet.h"
#include "DataProperty/DataBase.h"
#include <QMessageBox>

namespace MainWidget
{
	MeshRenameDialog::MeshRenameDialog(GUI::MainWindow* mw, QTreeWidgetItem* item)
		:QFDialog(mw), _item(item)
	{
		_ui = new Ui::DialogSetRename;
		_ui->setupUi(this);
		_meshdata = MeshData::MeshData::getInstance();

		_type = (TreeItemType)item->type();
		int id = item->data(0, Qt::UserRole).toInt();

		if (_type == MeshChild)
			_data = _meshdata->getKernalByID(id);
		else
			_data = _meshdata->getMeshSetByID(id);

		QString name = _data->getName();
		_ui->newNamelineEdit->setText(name);
	}

	MeshRenameDialog::~MeshRenameDialog()
	{
		if (_ui == nullptr) delete _ui;
	}

	void MeshRenameDialog::accept()
	{
		QString strNew = _ui->newNamelineEdit->text();
		if (strNew.isEmpty() || _data == nullptr) return;
		QString strOld = _data->getName();
		if (strNew == strOld)
		{
			QMessageBox::warning(this, tr("Warning"), tr("The Same Name!"));
			return;
		}
		else
		{
			_data->setName(strNew);
			_item->setText(0, strNew);
		}
		QDialog::accept();
	}


}