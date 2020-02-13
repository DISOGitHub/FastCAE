#include "DialogGeometrysetRename.h"
#include <ui_DialogGeometrysetRename.h>
#include "geometry/geometrySet.h"
#include <QMessageBox>
#include <QTreeWidgetItem>

namespace MainWidget
{
	GeometrysetRenameDialog::GeometrysetRenameDialog(GUI::MainWindow* m, Geometry::GeometrySet* gset, QTreeWidgetItem* item)
		:QFDialog(m), _gset(gset), _treeItem(item)
	{
		_ui = new Ui::DialogSetRename;
		_ui->setupUi(this);
		_ui->newNamelineEdit->setText(_gset->getName());
	}

	GeometrysetRenameDialog::~GeometrysetRenameDialog()
	{
		if (_ui != nullptr) delete _ui;
	}


	void GeometrysetRenameDialog::accept()
	{
		QString strNew = _ui->newNamelineEdit->text();
		if (strNew.isEmpty() || _gset == nullptr) return;
		QString strOld = _gset->getName();
		if (strNew==strOld)
		{
				QMessageBox::warning(this, tr("Warning"), tr("The Same Name!"));
				return;
		}
		else
		{
			_gset->setName(strNew);
			if (_gset->getType() != Geometry::DATUM)
				strNew += QString("(%1)").arg(_gset->getID());
			_treeItem->setText(0, strNew);
		}
		QDialog::accept();
	}

}