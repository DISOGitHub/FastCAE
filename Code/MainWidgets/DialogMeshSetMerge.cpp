#include "DialogMeshSetMerge.h"
#include "ui_DialogMeshSetMerge.h"
#include "meshData/meshSet.h"
#include "meshData/meshSingleton.h"
#include "MeshWidget.h"
#include <QMessageBox>
 
namespace MainWidget
{
	MeshSetMergeDialog::MeshSetMergeDialog(GUI::MainWindow* mw,  MeshWidget* w)
		:QFDialog(mw), _meshwidget(w)
	{
		_ui = new Ui::MeshSetMergeDialog;
		_ui->setupUi(this);
		_meshData = MeshData::MeshData::getInstance();

		_ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
		_ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
		_ui->AvailableListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
		_ui->MergeListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);
		_ui->CutListWidget->setSelectionMode(QAbstractItemView::ExtendedSelection);

		connect(_ui->TypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(on_typeChanged()));
		init();

	}

	MeshSetMergeDialog::~MeshSetMergeDialog()
	{
		delete _ui;
	}

	void MeshSetMergeDialog::init()
	{
		int maxID = MeshData::MeshSet::getMaxID();
		QString name = QString("set_%1").arg(maxID + 1);
		_ui->NameLineEdit->setPlaceholderText(name);

		on_typeChanged();
	}

	void MeshSetMergeDialog::on_typeChanged()
	{
		_ui->AvailableListWidget->clear();
		_ui->MergeListWidget->clear();
		_ui->CutListWidget->clear();

		MeshData::SetType type = MeshData::Node;
		QString icon = "://QUI/icon/nodeset.png";
		if (_ui->TypeComboBox->currentIndex() == 1)
		{
			type = MeshData::Element;
			icon = "://QUI/icon/eleset.png";
		}

		const int n = _meshData->getMeshSetCount();
		for (int i = 0; i < n; ++i)
		{
			auto set = _meshData->getMeshSetAt(i);
			if (set->getSetType() != type) continue;
			QString name = set->getName();
			int id = set->getID();
			QListWidgetItem* item = new QListWidgetItem(QIcon(icon), name);
			item->setData(Qt::UserRole, id);
			_ui->AvailableListWidget->addItem(item);
		}
	}

	QString MeshSetMergeDialog::getNameFromUi()
	{
		QString text = _ui->NameLineEdit->text();
		if (text.simplified().isEmpty())
			text = _ui->NameLineEdit->placeholderText();
		return text.simplified();
	}

	void MeshSetMergeDialog::on_addButton_clicked()
	{
		QList<QListWidgetItem*> itemList = _ui->AvailableListWidget->selectedItems();
		const int n = itemList.size();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* availitem = itemList.at(i);
			const int row = _ui->AvailableListWidget->row(availitem);
			QListWidgetItem* selectitem = new QListWidgetItem(availitem->icon(), availitem->text());
			const int id = availitem->data(Qt::UserRole).toInt();
			selectitem->setData(Qt::UserRole, id);
			_ui->MergeListWidget->addItem(selectitem);
			_ui->AvailableListWidget->takeItem(row);
		}
	}

	void MeshSetMergeDialog::on_removeButton_clicked()
	{
		QList<QListWidgetItem*> itemList = _ui->MergeListWidget->selectedItems();
		const int n = itemList.size();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* selectitem = itemList.at(i);
			const int row = _ui->MergeListWidget->row(selectitem);
			QListWidgetItem* availitem = new QListWidgetItem(selectitem->icon(), selectitem->text());
			const int id = selectitem->data(Qt::UserRole).toInt();
			availitem->setData(Qt::UserRole, id);
			_ui->AvailableListWidget->addItem(availitem);

			_ui->MergeListWidget->takeItem(row);
		}
	}

	void MeshSetMergeDialog::on_btn_add_clicked()
	{
		QList<QListWidgetItem*> itemList = _ui->AvailableListWidget->selectedItems();
		const int n = itemList.size();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* availitem = itemList.at(i);
			const int row = _ui->AvailableListWidget->row(availitem);
			QListWidgetItem* selectitem = new QListWidgetItem(availitem->icon(), availitem->text());
			const int id = availitem->data(Qt::UserRole).toInt();
			selectitem->setData(Qt::UserRole, id);
			_ui->CutListWidget->addItem(selectitem);
			_ui->AvailableListWidget->takeItem(row);
		}
	}

	void MeshSetMergeDialog::on_btn_del_clicked()
	{
		QList<QListWidgetItem*> itemList = _ui->CutListWidget->selectedItems();
		const int n = itemList.size();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem* selectitem = itemList.at(i);
			const int row = _ui->CutListWidget->row(selectitem);
			QListWidgetItem* availitem = new QListWidgetItem(selectitem->icon(), selectitem->text());
			const int id = selectitem->data(Qt::UserRole).toInt();
			availitem->setData(Qt::UserRole, id);
			_ui->AvailableListWidget->addItem(availitem);

			_ui->CutListWidget->takeItem(row);
		}
	}

	QList<int> MeshSetMergeDialog::getMergeIDs()
	{
		QList<int>  ids;
		const int n = _ui->MergeListWidget->count();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem*item = _ui->MergeListWidget->item(i);
			const int id = item->data(Qt::UserRole).toInt();
			const QString text = item->text();
			ids.append(id);
		}
		return ids;
	}

	QList<int> MeshSetMergeDialog::getCutIDs()
	{
		QList<int>  ids;
		const int n = _ui->CutListWidget->count();
		for (int i = 0; i < n; ++i)
		{
			QListWidgetItem*item = _ui->CutListWidget->item(i);
			const int id = item->data(Qt::UserRole).toInt();
			const QString text = item->text();
			ids.append(id);
		}
		return ids;
	}

	void MeshSetMergeDialog::accept()
	{
		QList<int> mergeids = this->getMergeIDs();
		if (mergeids.isEmpty())
		{
			QMessageBox::warning(this, QString(tr("Warning")), QString(tr("No merge object selected !")));
			return;
		}
		QList<int> cutids = this->getCutIDs();
		
		MeshData::SetType type = MeshData::Node;
		if (_ui->TypeComboBox->currentIndex() == 1) type = MeshData::Element;

		QString name = this->getNameFromUi();
		MeshData::MeshSet* set = new MeshData::MeshSet(name, type);
		for (int id : mergeids)
		{
			auto s = _meshData->getMeshSetByID(id);
			if (s == nullptr) continue;
			set->merge(s);
		}
		for (int id : cutids)
		{
			auto s = _meshData->getMeshSetByID(id);
			if (s == nullptr) continue;
			set->cut(s);
		}
		set->generateDisplayDataSet();
		_meshData->appendMeshSet(set);
		_meshwidget->updateMeshSetTree();

		QDialog::accept();
	}

}



