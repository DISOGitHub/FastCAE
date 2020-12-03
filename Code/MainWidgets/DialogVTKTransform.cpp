#include "DialogVTKTransform.h"
#include "ui_DialogVTKTransform.h"
#include "ui_DialogSelectComponents.h"
#include "python/PyAgent.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshSet.h"
#include <QMenu>
#include <QDebug>

auto meshData = MeshData::MeshData::getInstance();

MainWidget::DialogVTKTransform::DialogVTKTransform(GUI::MainWindow* parent) :
															QFDialog(parent),
															_ui(new Ui::DialogVTKTransform),
															_mw(parent),
															_selectdlg(new DialogSelectComponents(parent))
{
	_ui->setupUi(this);
	_ui->geoSelectPoint->setToolTip(tr("Clicked Button Selected Components"));
	setWindowTitle(tr("Mesh Modeling"));
	_ui->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);

	disabledCustomRotateAxisSlot();
	connect(_ui->radioButtonX, SIGNAL(clicked()), this, SLOT(disabledCustomRotateAxisSlot()));
	connect(_ui->radioButtonY, SIGNAL(clicked()), this, SLOT(disabledCustomRotateAxisSlot()));
	connect(_ui->radioButtonZ, SIGNAL(clicked()), this, SLOT(disabledCustomRotateAxisSlot()));
	connect(_ui->radioButtonC, SIGNAL(clicked()), this, SLOT(enabledCustomRotateAxisSlot()));
	connect(_ui->geoSelectPoint, &QPushButton::clicked, [=]() { _selectdlg->clearSelectItems(); _selectdlg->exec(); });
	connect(_selectdlg, SIGNAL(selectedComponentsSig(QList<MeshData::MeshSet*>)), this, SLOT(selectedComponentsSlot(QList<MeshData::MeshSet*>)));
	connect(_ui->listWidget, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(customContextMenuRequestedSlot(const QPoint&)));
}

MainWidget::DialogVTKTransform::~DialogVTKTransform()
{
	delete _ui;
	_ui = NULL;
	delete _selectdlg;
	_selectdlg = NULL;
}

void MainWidget::DialogVTKTransform::disabledCustomRotateAxisSlot()
{
	_ui->customXDSB->setReadOnly(true);
	_ui->customYDSB->setReadOnly(true);
	_ui->customZDSB->setReadOnly(true);
}

void MainWidget::DialogVTKTransform::enabledCustomRotateAxisSlot()
{
	_ui->customXDSB->setReadOnly(false);
	_ui->customYDSB->setReadOnly(false);
	_ui->customZDSB->setReadOnly(false);
}

void MainWidget::DialogVTKTransform::accept()
{
	if (_components.size() == 0)	return;

	QString componentIds, rotate, moveLocation, scale;
	for (auto component : _components)
		componentIds.append(QString(",%1").arg(component->getID()));
	componentIds.remove(0, 1);

	rotate.append(_ui->rotateAngleDSB->text() + ",");
	if (_ui->radioButtonX->isCheckable())
		rotate.append("1,0,0");
	else if (_ui->radioButtonY->isCheckable())
		rotate.append("0,1,0");
	else if (_ui->radioButtonZ->isCheckable())
		rotate.append("0,0,1");
	else if (_ui->radioButtonC->isCheckable())
		rotate.append(_ui->customXDSB->text() + ',' + _ui->customYDSB->text() + ',' + _ui->customZDSB->text());

	moveLocation = _ui->directionXDSB->text() + ',' + _ui->directionYDSB->text() + ',' + _ui->directionZDSB->text();
	scale = _ui->scaleXDSB->text() + ',' + _ui->scaleYDSB->text() + ',' + _ui->scaleZDSB->text();

	QString code = QString("MainWindow.createVTKTransform(\"%1\",\"%2\",\"%3\",\"%4\")").arg(componentIds).arg(rotate).arg(moveLocation).arg(scale);
	Py::PythonAagent::getInstance()->submit(code);
	QFDialog::accept();
}

void MainWidget::DialogVTKTransform::selectedComponentsSlot(QList<MeshData::MeshSet*> components)
{
	for (MeshData::MeshSet* set : components)
	{
		if(_components.contains(set))	continue;
		_components.append(set);
		_ui->listWidget->addItem(set->getName());
	}
}

void MainWidget::DialogVTKTransform::customContextMenuRequestedSlot(const QPoint& point)
{
	QListWidgetItem* curItem = _ui->listWidget->itemAt(point);
	if (!curItem)	return;

	QMenu* menu = new QMenu(this);
	QAction* deleteItem = new QAction(tr("delete this item"));
	menu->addAction(deleteItem);
	connect(menu, &QMenu::triggered, [=]() {removeCurrentItem(curItem); });
	menu->exec(QCursor::pos());
}

void MainWidget::DialogVTKTransform::removeCurrentItem(QListWidgetItem* curItem)
{
	auto meshSet = meshData->getMeshSetByName(curItem->text());
	if (!meshSet)	return;
	_components.removeOne(meshSet);
	_ui->listWidget->removeItemWidget(curItem); 
	delete curItem;
}

MainWidget::DialogSelectComponents::DialogSelectComponents(GUI::MainWindow* parent) : 
																	QFDialog(parent),
																	_ui(new Ui::DialogSelectComponents)
{
	_ui->setupUi(this);
	setWindowTitle(tr("Select Components"));
	MeshData::MeshSet* meshSet = NULL;
	int nSet = meshData->getMeshSetCount();
	for (int i = 0; i < nSet; i++)
	{
		meshSet = meshData->getMeshSetAt(i);
		if (!meshSet)	continue;
		_ui->listWidget->addItem(meshSet->getName());
	}
}

MainWidget::DialogSelectComponents::~DialogSelectComponents()
{
	delete _ui;
	_ui = NULL;
}

void MainWidget::DialogSelectComponents::clearSelectItems()
{
	_ui->listWidget->clearSelection();
}

void MainWidget::DialogSelectComponents::accept()
{
	MeshData::MeshSet* meshSet = NULL;
	QList<MeshData::MeshSet*> components;

	auto items = _ui->listWidget->selectedItems();

	for (QListWidgetItem* item : items)
	{
		meshSet = meshData->getMeshSetByName(item->text());
		components.append(meshSet);
	}
	emit selectedComponentsSig(components);
	QFDialog::accept();
}
