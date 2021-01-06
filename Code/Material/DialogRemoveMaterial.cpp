#include "DialogRemoveMaterial.h"
#include <ui_DialogRemoveMaterial.h>
#include <QMessageBox>
#include <QDebug>
#include "python/PyAgent.h"
namespace Material
{
	RemoveMaterialDialog::RemoveMaterialDialog(GUI::MainWindow* m, QHash<QString, Material*>* ms) :
		QFDialog(m), _data(ms)
	{
		_ui = new Ui::RemoveMaterialDialog;
		_ui->setupUi(this);
		this->init();
		connect(_ui->selectAllCheckBox, SIGNAL(stateChanged(int)), this, SLOT(on_selectAll()));
		connect(_ui->clearSelectCheckBox, SIGNAL(stateChanged(int)), this, SLOT(on_clearSelect()));
		connect(_ui->listWidget, SIGNAL(itemChanged(QListWidgetItem *)), this, SLOT(itemChanged(QListWidgetItem *)));
	}

	RemoveMaterialDialog::~RemoveMaterialDialog()
	{
		if (_ui != nullptr) delete _ui;
	}

	void RemoveMaterialDialog::init()
	{
		blockSignals(true);
		QStringList mnames = _data->keys();
		for (QString mn : mnames)
		{
			QListWidgetItem* item = new QListWidgetItem(QIcon(), mn);
			item->setCheckState(Qt::Unchecked);
			_ui->listWidget->addItem(item);
		}
		blockSignals(false);
	}

	bool RemoveMaterialDialog::isAllChecked()
	{
		const int n = _ui->listWidget->count();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* item = _ui->listWidget->item(i);
			if (item->checkState() == Qt::Unchecked)
				return false;
		}
		return true;
	}

	bool RemoveMaterialDialog::isAllUnChecked()
	{
		const int n = _ui->listWidget->count();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* item = _ui->listWidget->item(i);
			if (item->checkState() == Qt::Checked)
				return false;
		}
		return  true;
	}

	void RemoveMaterialDialog::on_cancelPushButton_clicked()
	{
		this->close();
	}

	void RemoveMaterialDialog::on_removePushButton_clicked()
	{
		QStringList names;
		const int n = _ui->listWidget->count();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* item = _ui->listWidget->item(i);
			if (item->checkState() == Qt::Checked)
				names.append(item->text());
		}
		if (names.isEmpty())
		{
			QMessageBox::warning(this, QString(tr("Warning")), QString(tr("No one has been Selected !")));
			return;
		}
		
		int nn = names.size();
		QMessageBox::StandardButton b = (QMessageBox::StandardButton)QMessageBox::warning(this, QString(tr("Warning")), QString(tr("%1 object(s) will be removed, continue ?").arg(nn)), QMessageBox::Yes, QMessageBox::No);
		if (b == QMessageBox::No) return;

		QString namestr = names.join(",");
		QString code = QString("ControlPanel.RemoveFromMaterialLib('%1')").arg(namestr);
		Py::PythonAagent::getInstance()->submit(code);


		/*for (QString name : names)
		{
			auto ma = _data->value(name);
			qDebug() << name;
			if (ma == nullptr) continue;
			_data->remove(name);
			delete ma;
		}*/
		this->close();
	}

	void RemoveMaterialDialog::on_selectAll()
	{
		blockSignals(true);
		if (!_ui->selectAllCheckBox->isChecked())  return;
		_ui->clearSelectCheckBox->setCheckState(Qt::Unchecked);
		const int n = _ui->listWidget->count();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* item = _ui->listWidget->item(i);
			item->setCheckState(Qt::Checked);
		}
		blockSignals(false);

	}

	void RemoveMaterialDialog::on_clearSelect()
	{
		blockSignals(true);

		if (!_ui->clearSelectCheckBox->isChecked()) return;
		_ui->selectAllCheckBox->setCheckState(Qt::Unchecked);
		const int n = _ui->listWidget->count();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* item = _ui->listWidget->item(i);
			item->setCheckState(Qt::Unchecked);
		}
		blockSignals(false);
	}

	void RemoveMaterialDialog::itemChanged(QListWidgetItem * item)
	{
		blockSignals(true);
		if (item->checkState() == Qt::Unchecked)
		{
			if (isAllUnChecked())
			{
				_ui->clearSelectCheckBox->setChecked(true);
				_ui->selectAllCheckBox->setChecked(false);
			}
			else
			{
				_ui->clearSelectCheckBox->setChecked(false);
				_ui->selectAllCheckBox->setChecked(false);
			}
		}
		else
		{
			if (isAllChecked())
			{
				_ui->clearSelectCheckBox->setChecked(false);
				_ui->selectAllCheckBox->setChecked(true);
			}
			else
			{
				_ui->clearSelectCheckBox->setChecked(false);
				_ui->selectAllCheckBox->setChecked(false);
			}
		}
		blockSignals(false);
	}

}



