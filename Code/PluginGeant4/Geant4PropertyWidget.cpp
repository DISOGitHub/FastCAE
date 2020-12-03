#include "Geant4PropertyWidget.h"
#include <QComboBox>
#include <QLineEdit>
#include <QSCrollBar>
#include <QObjectUserData>
#include <QPushButton>
#include <QFileDialog>
#include "Geant4DataModel.h"
#include "DataManager.h"
#include "Geant4ShipTreeItemType.h"
#include "Geant4DataObserver.h"
#include "QSpinBox"
#include "SolidsManager.h"
#include "SolidShapeBase.h"


namespace Geant4
{
	struct UserSignData : QObjectUserData {
		QString sign;
		QString belong;
	};

	Geant4PropertyWidget::Geant4PropertyWidget(QWidget* parent)
	{
		_ui.setupUi(this);
		_ui.propTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
		QHeaderView *header = _ui.propTable->verticalHeader();
		header->setHidden(true);
		_ui.propTable->resizeColumnsToContents();
	}

	Geant4PropertyWidget::~Geant4PropertyWidget()
	{

	}

	void Geant4PropertyWidget::updateWith(_G4SourceType sourceType)
	{
		init();
		addEnumLine(tr("Location"), _G4_LOCATION_ENUM_STRINGLIST_, sourceType.location, "_G4SourceType.Location");
		addEnumLine(tr("Shape"), _G4_SHAPE_ENUM_STRINGLIST_, sourceType.shape, "_G4SourceType.Shape");
		addIntLine(tr("Position X"), sourceType.position.x, "_G4SourceType._G4Position.X");
		addIntLine(tr("Position Y"), sourceType.position.y, "_G4SourceType._G4Position.Y");
		addIntLine(tr("Position Z"), sourceType.position.z, "_G4SourceType._G4Position.Z");
		addDoubleLine(tr("Radius"), sourceType.radius, "_G4SourceType.Radius");
		addEnumLine(tr("Unit"), _G4_UNIT_ENUM_STRINGLIST_, sourceType.unit, "_G4SourceType.Unit");
	}

	void Geant4PropertyWidget::updateWith(_G4AngleDistribution angleDistribution)
	{
		init();
		addEnumLine(tr("Direction"), _G4_DIRECTION_DISTRIBUTION_ENUM_STRINGLIST_, angleDistribution.directionDistribution, "_G4AngleDistribution.DirectionDistribution");
		addDoubleLine(QString::fromLocal8Bit("¦È") + tr("min"), angleDistribution.anXitaMin, "_G4AngleDistribution.XitaMin");
		addDoubleLine(QString::fromLocal8Bit("¦È") + tr("max"), angleDistribution.anXitaMax, "_G4AngleDistribution.XitaMax");
		addDoubleLine(QString::fromLocal8Bit("¦Õ") + tr("min"), angleDistribution.anFaiMin, "_G4AngleDistribution.FaiMin");
		addDoubleLine(QString::fromLocal8Bit("¦Õ") + tr("max"), angleDistribution.anFaiMax, "_G4AngleDistribution.FaiMax");
	}

	void Geant4PropertyWidget::updateWith(_G4SourceParameters sourceParameters)
	{
		init();
		addEnumLine(tr("Spectrum Type"), _G4_UNIT_SPECTRUM_TYPE_ENUM_STRINGLIST_, sourceParameters.spectrumType, "_G4SourceParameters.SpectrumType");
		addEnumLine(tr("Particle Type"), _G4_PARTICLE_TYPE_ENUM_STRINGLIST_, sourceParameters.particleType, "_G4SourceParameters.ParticleType");
		if (sourceParameters.spectrumType == EeqE0)
		{
			addDoubleLine(tr("EeqE0Mode"), sourceParameters.E0, "_G4SourceParameters.E0");
		}
		else if (sourceParameters.spectrumType == EPowerLow)
		{
			addDoubleLine(tr("EPowerLow"), sourceParameters.powerLow, "_G4SourceParameters.EPowerLow");
		}
		addEnumLine(tr("Energy Unit"), _G4_UNKNOWN_ENUM_STRINGLIST_, sourceParameters.unknown, "_G4SourceParameters._G4Unknown");
		addPathLine(tr("Environment File"), sourceParameters.envFile,"_G4SourceParameters.EnvFile");
		addPathLine(tr("Histogram File"), sourceParameters.histogramFile, "_G4SourceParameters.HistogramFile");
		addEnumLine(tr("Histogram Input Mode"), _G4_HISTOGRAM_INPUT_ENUM_STRINGLIST_,sourceParameters.inputList, "_G4SourceParameters.InputList");
		addEnumLine(tr("Histogram Insert Mode"), _G4_HISTOGRAM_INSERT_WAY_ENUM_STRINGLIST_, sourceParameters.insertWay, "_G4SourceParameters.InsertWay");
		addIntLine(tr("Integrated Flux"), sourceParameters.integratedFlux, "_G4SourceParameters.IntegratedFlex");
	}

	void Geant4PropertyWidget::updateWith(_G4PhysicalPart physicalPart)
	{
		init();
		addEnumLine(tr("Physical Mode"), _G4_PHYSICAL_MODE_ENUM_STRINGLIST_, physicalPart.mode, "_G4PhysicalPart.Mode", true);
		if (physicalPart.mode == SelfDefineMode)
		{
			addEnumLine(tr("Electromagnetic Physics"), _G4_ELECTROMAGNETIC_PHYSICS_ENUM_STRINGLIST_, physicalPart.ePhy, "_G4PhysicalPart.ePhy");
			addEnumLine(tr("Hadronic Physics"), _G4_HADRONIC_PHYSICS_ENUM_STRINGLIST_, physicalPart.hPhy, "_G4PhysicalPart.hPhy");
			addEnumLine(tr("ion-ion"), _G4_ION_ION_ENUM_STRINGLIST_, physicalPart.ion, "_G4PhysicalPart.ION");
			addDoubleLine(tr("cut"), physicalPart.eIgnore, "_G4PhysicalPart.eIgnore");
			addEnumLine(tr("Other Physics"), _G4_OTHER_PHYSICS_ENUM_STRINGLIST_, physicalPart.oPhy, "_G4PhysicalPart.oPhy");
		}
	}

	void Geant4PropertyWidget::updateWith(int type, QTreeWidgetItem * item)
	{
		init();
		QString itemName = item->text(0);
		if (type == Geant4PhEnThItem)
		{
			auto geoEnTh = DataManager::getInstance()->getGeant4DataModel()->getGeometriesEnergyThreshold().value(itemName);
			updateWith(geoEnTh);
			if (geoEnTh.belong.isEmpty())
			{
				geoEnTh.belong = itemName;
				DataManager::getInstance()->getGeant4DataModel()->setGeometryEnergyThreahold(geoEnTh);
			}
		}
		else if (type == Geant4CaAnItem)
		{
			auto anInfo = DataManager::getInstance()->getGeant4DataModel()->getAnalysisItems().value(itemName);
			addEnumLine(tr("Customize Mesh"), _G4_BOOL_ENUM_STRINGLIST_, anInfo.customizeMesh, "_G4AnalysisItem._G4Bool", false, itemName);
			addEnumLine(tr("Unit"), _G4_UNKNOWN_ENUM_STRINGLIST_, anInfo.unknow, "_G4AnalysisItem._G4Unknown", false, itemName);
		}
		else if (type == Geant4CaAnHiItem)
		{
			QTreeWidgetItem * parItem = item->parent();
			if (parItem)
			{
				QString anName = parItem->text(0);
				auto anInfo = DataManager::getInstance()->getGeant4DataModel()->getAnalysisItems().value(anName);
				QString belong = anName + "." + itemName;
				auto data = anInfo.hiss.value(itemName);
				data.name = itemName;
				addIntLine(tr("Zone number"), data.zoneNum, "_G4Histogram.ZoneNum", 0, 0x0FFFFFFF, belong);
				addIntLine(tr("X Min"), data.xMin, "_G4Histogram.XMin", 0, 0x0FFFFFFF, belong);
				addIntLine(tr("X Max"), data.xMax, "_G4Histogram.XMax", 0, 0x0FFFFFFF, belong);
				addPathLine(tr("Root Path"), data.rootPath, "_G4Histogram.RootPath", belong);
				addEnumLine(tr("Unit"), _G4_HISTOGRAM_UNITLIST_, data.unit, "_G4Histogram._G4Unit", false, belong);
			}
		}
		else if (type == Geant4CaAnNoItem)
		{
			QTreeWidgetItem * parItem = item->parent();
			if (parItem)
			{
				QString anName = parItem->text(0);
				auto anInfo = DataManager::getInstance()->getGeant4DataModel()->getAnalysisItems().value(itemName);
				addEnumLine(tr("Normalization Type"), _G4_NORMALIZATION_TYPE_ENUM_STRINGLIST_, anInfo.normalization.type, "_G4Normalization._G4NormalizationType", true, anName);
			}
		}
		else if (type == Geant4CaAnRelationItem)
		{
			QStringList lables; lables << tr("ObjectA") << tr("ObjectB");
			_ui.propTable->setHorizontalHeaderLabels(lables);
			QTreeWidgetItem * parItem = item->parent();
			if (parItem)
			{
				QString anName = parItem->text(0);
				auto anInfo = DataManager::getInstance()->getGeant4DataModel()->getRelationsItem(anName);
				for (int i = 0; i < anInfo.size(); i++)
				{
					_G4Relation relation = anInfo.at(i);
					addRelationLine(relation.objA, relation.objB);
				}
			}
		}
	}

	void Geant4PropertyWidget::updateWith(_G4GeometryEnergyThreshold enTh)
	{
		init();
		addEnumLine(tr("Mode"), _G4_GEO_EN_TH_TYPE_ENUM_STRINGLIST_, enTh.type, "_G4GeometryEnergyThreshold.Mode", true, enTh.belong);
		if (enTh.type == Customize)
		{
			addDoubleLine(tr("e-"), enTh.eNegative, "_G4GeometryEnergyThreshold.e-", 2, 0, 0xFFFFFFFF, enTh.belong);
			addDoubleLine(tr("e+"), enTh.ePlus, "_G4GeometryEnergyThreshold.e+", 2, 0, 0xFFFFFFFF, enTh.belong);
			addDoubleLine(tr("grmma"), enTh.grmma, "_G4GeometryEnergyThreshold.grmma", 2, 0, 0xFFFFFFFF, enTh.belong);
			addDoubleLine(tr("proton"), enTh.proton, "_G4GeometryEnergyThreshold.proton", 2, 0, 0xFFFFFFFF, enTh.belong);
		}
	}

	void Geant4PropertyWidget::updateWith(_G4AnalysisItem analysisItem, const QString belong)
	{
		init();
		addEnumLine(tr("Normalization Type"), _G4_NORMALIZATION_TYPE_ENUM_STRINGLIST_, analysisItem.normalization.type, "_G4Normalization._G4NormalizationType", true, belong);
		if (analysisItem.normalization.type == FLUENCY || analysisItem.normalization.type == TO_FLUENCY)
		{
			addIntLine(tr("Source Flex"), analysisItem.normalization.flex, "_G4Normalization._GSourceFlex");
		}
	}

	void Geant4PropertyWidget::updateProperty(int type)
	{
		switch (type)
		{
		case SourceType:
			updateWith(DataManager::getInstance()->getGeant4DataModel()->getSourceType());
			break;
		case SourceAngle:
			updateWith(DataManager::getInstance()->getGeant4DataModel()->getAngleDistribution());
			break;
		case SourcePara:
			updateWith(DataManager::getInstance()->getGeant4DataModel()->getSourceParameters());
			break;

		case Geant4Physical:
			updateWith(DataManager::getInstance()->getGeant4DataModel()->getPhysicalPart());
			break;

		default:
			break;
		}
	}

	void Geant4PropertyWidget::updateProperty(int type, QTreeWidgetItem * item)
	{
		switch (type)
		{
		case Geant4PhEnThItem:
		case Geant4CaAnItem:
		case Geant4CaAnHiItem:
		case Geant4CaAnNoItem:
		case Geant4CaAnRelationItem:
			updateWith(type, item);
			break;
		}
	}

	void Geant4PropertyWidget::slotItemValueChanged()
	{
		QWidget * wgt = dynamic_cast<QWidget *>(sender());
		if (wgt)
		{
			UserSignData * data = dynamic_cast<UserSignData *> (wgt->userData(0));
			if (data)
			{
				QString sign = data->sign;
				QString belong = data->belong;
				Geant4DataObserver observer(wgt, sign, belong);
			}
		}
	}

	void Geant4PropertyWidget::slotUpdateSelf()
	{
		QWidget * wgt = dynamic_cast<QWidget *>(sender());
		if (wgt)
		{
			UserSignData * data = dynamic_cast<UserSignData *> (wgt->userData(0));
			if (data)
			{
				QString sign = data->sign;
				if (sign.contains("_G4PhysicalPart"))
				{
					updateWith(DataManager::getInstance()->getGeant4DataModel()->getPhysicalPart());
				}
				else if (sign.contains("_G4GeometryEnergyThreshold"))
				{
					QString belong = data->belong;
					auto data = DataManager::getInstance()->getGeant4DataModel()->getGeometriesEnergyThreshold().value(belong);
					updateWith(data);
				}
				else if (sign.contains("_G4Normalization"))
				{
					QString belong = data->belong;
					updateWith(DataManager::getInstance()->getGeant4DataModel()->getAnalysisItem(belong), belong);
				}
			}
		}
	}

	void Geant4PropertyWidget::cleanWidget()
	{
		init();
	}

	void Geant4PropertyWidget::resizeEvent(QResizeEvent * e)
	{
		int w = this->width();
		int totalwidth = w;
		if (w < 0) return;
		if (_ui.propTable->verticalScrollBar()->isVisible())
		{
			totalwidth -= _ui.propTable->verticalScrollBar()->width();
		}
		init();
		_ui.propTable->setColumnWidth(0, totalwidth / 2 - 4);
		_ui.propTable->setColumnWidth(1, totalwidth / 2 - 4);
	}

	void Geant4PropertyWidget::init()
	{
		_ui.propTable->clear(); 
		_ui.propTable->setRowCount(0);
		_ui.propTable->setColumnCount(2);

		QStringList lables; lables << tr("Name") << tr("Value");
		_ui.propTable->setHorizontalHeaderLabels(lables);
	}

	void Geant4PropertyWidget::addEnumLine(QString name, QStringList items, int currentIndex, QString sign, bool updateSelf /*= false*/, QString belong /*= QString()*/)
	{
		int rowCount = _ui.propTable->rowCount();
		_ui.propTable->setRowCount(rowCount + 1);
		QTableWidgetItem * itemName = new QTableWidgetItem(name);
		_ui.propTable->setItem(rowCount, 0, itemName);
		QComboBox * box = new QComboBox();
		box->addItems(items);
		box->setCurrentIndex(currentIndex);
		_ui.propTable->setCellWidget(rowCount, 1, box);
		UserSignData * signWith = new UserSignData;
		signWith->sign = sign;
		signWith->belong = belong;
		box->setUserData(0, signWith);
		connect(box, SIGNAL(currentIndexChanged(int)), this, SLOT(slotItemValueChanged()));
		if (updateSelf)
		{
			connect(box, SIGNAL(currentIndexChanged(int)), this, SLOT(slotUpdateSelf()));
		}
	}

	void Geant4PropertyWidget::addIntLine(QString name, int val, QString sign, int min /*= 0*/, int max /*= 0xFFFFFFFF*/, QString belong /*= QString()*/)
	{
		int rowCount = _ui.propTable->rowCount();
		_ui.propTable->setRowCount(rowCount + 1);
		QTableWidgetItem * itemName = new QTableWidgetItem(name);
		_ui.propTable->setItem(rowCount, 0, itemName);
		QSpinBox * spin = new QSpinBox();
		spin->setMinimum(min);
		spin->setMaximum(max);
		spin->setValue(val);
		_ui.propTable->setCellWidget(rowCount, 1, spin);
		UserSignData * signWith = new UserSignData;
		signWith->sign = sign;
		signWith->belong = belong;
		spin->setUserData(0, signWith);
		connect(spin, SIGNAL(valueChanged(int)), this, SLOT(slotItemValueChanged()));
	}

	void Geant4PropertyWidget::addDoubleLine(QString name, double val, QString sign, int accuracy /*= 2*/, double min /*= 0*/, double max /*= 0xFFFFFFFF*/, QString belong /*= QString()*/)
	{
		int rowCount = _ui.propTable->rowCount();
		_ui.propTable->setRowCount(rowCount + 1);
		QTableWidgetItem * itemName = new QTableWidgetItem(name);
		_ui.propTable->setItem(rowCount, 0, itemName);
		QDoubleSpinBox * spin = new QDoubleSpinBox();
		spin->setDecimals(accuracy);
		spin->setMinimum(min);
		spin->setMaximum(max);
		spin->setValue(val);
		_ui.propTable->setCellWidget(rowCount, 1, spin);
		UserSignData * signWith = new UserSignData;
		signWith->sign = sign;
		signWith->belong = belong;
		spin->setUserData(0, signWith);
		connect(spin, SIGNAL(valueChanged(double)), this, SLOT(slotItemValueChanged()));
	}

	void Geant4PropertyWidget::addPathLine(QString name, QString val, QString sign, QString belong /*= QString()*/)
	{
		int rowCount = _ui.propTable->rowCount();
		_ui.propTable->setRowCount(rowCount + 1);
		QTableWidgetItem * itemName = new QTableWidgetItem(name);
		_ui.propTable->setItem(rowCount, 0, itemName);
		QWidget * wgt = new QWidget(this);
		QLineEdit * line = new QLineEdit(wgt);
		line->setDisabled(true);
		line->setText(val);
		QPushButton * btn = new QPushButton(wgt);
		btn->setObjectName("addButton");
		btn->setText("...");
		QHBoxLayout * lay = new QHBoxLayout(this);
		wgt->setLayout(lay);
		lay->setSpacing(0);
		lay->setMargin(0);
		lay->addWidget(line,4);
		lay->addWidget(btn, 1);
		connect(btn, &QPushButton::clicked, [=](){
			QString filePath = QFileDialog::getExistingDirectory();
			line->setText(filePath);
		});
		UserSignData * signWith = new UserSignData;
		signWith->sign = sign;
		signWith->belong = belong;
		line->setUserData(0, signWith);
		connect(line, SIGNAL(textChanged(const QString &)), this, SLOT(slotItemValueChanged()));
		_ui.propTable->setCellWidget(rowCount, 1, wgt);
	}

	void Geant4PropertyWidget::addRelationLine(const QString& objA, const QString& objB)
	{
		int rowCount = _ui.propTable->rowCount();
		_ui.propTable->setRowCount(rowCount + 1);
		QTableWidgetItem * objAItem = new QTableWidgetItem(objA);
		_ui.propTable->setItem(rowCount, 0, objAItem);
		QTableWidgetItem * objBItem = new QTableWidgetItem(objB);
		_ui.propTable->setItem(rowCount, 1, objBItem);
	}

}