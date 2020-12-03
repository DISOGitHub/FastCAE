#include <ui_DialogGeometryRename.h>
#include <QTreeWidgetItem>
#include <QMessageBox>
#include "DataProperty/modelTreeItemType.h"
#include "geometry/GeoComponent.h"
#include "DialogGeometryRename.h"
#include "DataProperty/DataBase.h"
#include "mainWindow/mainWindow.h"

namespace MainWidget
{
	GeometryRenameDialog::GeometryRenameDialog(GUI::MainWindow* m, DataProperty::DataBase* data, QTreeWidgetItem* item)
		:QFDialog(m), _data(data), _treeItem(item)
	{
		_ui = new Ui::DialogRename;
		_ui->setupUi(this);
		_ui->newNamelineEdit->setText(_data->getName());
		connect(this, SIGNAL(disPlayProp(DataProperty::DataBase*)), m, SIGNAL(updateProperty(DataProperty::DataBase*)));
	}

	GeometryRenameDialog::~GeometryRenameDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void GeometryRenameDialog::accept()
	{
		QString strNew = _ui->newNamelineEdit->text();
		if (strNew.isEmpty() || _data == nullptr) return;
		QString strOld = _data->getName();
		if (strNew==strOld)
		{
			QMessageBox::warning(this, tr("Warning"), tr("The Same Name!"));
			return;
		}
		else
		{
			_data->setName(strNew);
			if (_treeItem->type() != TreeItemType::DatumPlane)
				strNew += QString("(%1)").arg(_data->getID());
			_treeItem->setText(0, strNew);
		}
		emit disPlayProp(_data);
		QDialog::accept();
	}
}
