#include "Geant4DataObserver.h"
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include "DataManager.h"
#include "Geant4DataModel.h"

namespace Geant4
{
	extern "C" bool getPathValue(QString & path, QWidget * wgt)
	{
		QLineEdit * line = qobject_cast<QLineEdit *>(wgt);
		if (line)
		{
			path = line->text();
			return true;
		}
		return false;
	}

	extern "C" bool getIntValue(int & val, QWidget * wgt)
	{
		QSpinBox * spin = qobject_cast<QSpinBox*>(wgt);
		if (spin)
		{
			val = spin->value();
			return true;
		}
		return false;
	}

	extern "C" bool getDoubleValue(double & val, QWidget * wgt)
	{
		QDoubleSpinBox * spin = qobject_cast<QDoubleSpinBox*>(wgt);
		if (spin)
		{
			val = spin->value();
			return true;
		}
		return false;
	}

	extern "C" int getEnumValue(QWidget * wgt)
	{
		QComboBox * box = qobject_cast<QComboBox *> (wgt);
		if (box)
		{
			return box->currentIndex();
		}
		return -1;
	}

	Geant4DataObserver::Geant4DataObserver(QWidget * wgt, QString sign, QString belong /*= QString()*/)
	{
		auto signArgs = sign.split('.', QString::SkipEmptyParts);
		if (signArgs.size() < 2) { return; }
		auto type = signArgs[0];
		auto dataModel = DataManager::getInstance()->getGeant4DataModel();

		if (type == "_G4SourceType")
		{
			_G4SourceType sourceType = dataModel->getSourceType();
			if (signArgs.size() == 2)
			{
				auto para = signArgs[1];
				if (para == "Location")
				{
					sourceType.location = (_G4Location)getEnumValue(wgt);
				}
				else if (para == "Shape")
				{
					sourceType.shape = (_G4Shape)getEnumValue(wgt);
				}
				else if (para == "Radius")
				{
					getDoubleValue(sourceType.radius, wgt);
				}
				else if (para == "Unit")
				{
					sourceType.unit = (_G4Unit)getEnumValue(wgt);
				}
			}
			else if (signArgs.size() == 3)
			{
				if (signArgs[1] == "_G4Position")
				{
					if (signArgs[2] == "X")
					{
						getIntValue(sourceType.position.x, wgt);
					}
					else if (signArgs[2] == "Y")
					{
						getIntValue(sourceType.position.y, wgt);
					}
					else if (signArgs[3] == "Z")
					{
						getIntValue(sourceType.position.z, wgt);
					}
				}
			}
			else
				Q_ASSERT(0);
			DataManager::getInstance()->getGeant4DataModel()->setSourceType(sourceType);
		}
		else if (type == "_G4AngleDistribution")
		{
			auto data = dataModel->getAngleDistribution();
			if (signArgs[1] == "DirectionDistribution")
			{
				data.directionDistribution = (_G4DirectionDistribution)getEnumValue(wgt);
			}
			else if (signArgs[1] == "XitaMin")
			{
				getDoubleValue(data.anXitaMin, wgt);
			}
			else if (signArgs[1] == "XitaMax")
			{
				getDoubleValue(data.anXitaMax, wgt);
			}
			else if (signArgs[1] == "FaiMin")
			{
				getDoubleValue(data.anFaiMin, wgt);
			}
			else if (signArgs[1] == "FaiMax")
			{
				getDoubleValue(data.anFaiMax, wgt);
			}
		}
		else if (type == "_G4SourceParameters")
		{
			auto data = dataModel->getSourceParameters();
			if (signArgs[1] == "SpectrumType")
			{
				data.spectrumType = (_G4SpectrumType)getEnumValue(wgt);
			}
			else if (signArgs[1] == "ParticleType")
			{
				data.particleType = (_G4ParticleType)getEnumValue(wgt);
			}
			else if (signArgs[1] == "EeqE0Mode")
			{
				getDoubleValue(data.E0, wgt);
			}
			else if (signArgs[1] == "EPowerLow")
			{
				getDoubleValue(data.powerLow, wgt);
			}
			else if (signArgs[1] == "_G4Unknown")
			{
				data.unknown = (_G4Unknown)getEnumValue(wgt);
			}
			else if (signArgs[1] == "EnvFile")
			{
				getPathValue(data.envFile, wgt);
			}
			else if (signArgs[1] == "HistogramFile")
			{
				getPathValue(data.histogramFile, wgt);
			}
			else if (signArgs[1] == "InputList")
			{
				data.inputList = (_G4HistogramInputList)getEnumValue(wgt);
			}
			else if (signArgs[1] == "InsertWay")
			{
				data.insertWay = (_G4HistogramInsertWay)getEnumValue(wgt);
			}
			else if (signArgs[1] == "IntegratedFlex")
			{
				getIntValue(data.integratedFlux, wgt);
			}
		}
		else if (type == "_G4PhysicalPart")
		{
			_G4PhysicalPart phyPart = dataModel->getPhysicalPart();
			if (signArgs[1] == "Mode")
			{
				phyPart.mode = (_G4PhysicalMode)getEnumValue(wgt);
			}
			else if (signArgs[1] == "ePhy")
			{
				phyPart.ePhy = (_G4ElectromagneticPhysics)getEnumValue(wgt);
			}
			else if (signArgs[1] == "hPhy")
			{
				phyPart.hPhy = (_G4HadronicPhysics)getEnumValue(wgt);
			}
			else if (signArgs[1] == "ION")
			{
				phyPart.ion = (_G4Ion_Ion)getEnumValue(wgt);
			}
			else if (signArgs[1] == "eIgnore")
			{
				getDoubleValue(phyPart.eIgnore, wgt);
			}
			DataManager::getInstance()->getGeant4DataModel()->setPhysicalPart(phyPart);
		}
		else if (type == "_G4GeometryEnergyThreshold")
		{
			auto data = dataModel->getGeometriesEnergyThreshold();
			auto geoEnTh = data.value(belong);
			if (signArgs[1] == tr("Mode"))
			{
				geoEnTh.type = (_G4GeometryEnergyThresholdType)getEnumValue(wgt);
			}
			else if (signArgs[1] == tr("e-"))
			{
				getDoubleValue(geoEnTh.eNegative, wgt);
			}
			else if (signArgs[1] == tr("e+"))
			{
				getDoubleValue(geoEnTh.ePlus, wgt);
			}
			else if (signArgs[1] == tr("grmma"))
			{
				getDoubleValue(geoEnTh.grmma, wgt);
			}
			else if (signArgs[1] == tr("proton"))
			{
				getDoubleValue(geoEnTh.proton, wgt);
			}
			data.insert(belong, geoEnTh);
			DataManager::getInstance()->getGeant4DataModel()->setGeometriesEnergyThreshold(data);
		}
		else if (type == "_G4AnalysisItem")
		{
			auto data = dataModel->getAnalysisItems();
			auto one = data.value(belong);
			one.name = belong;
			if (signArgs[1] == "Model")
			{
				one.model = (_G4CalculateModel)getEnumValue(wgt);
			}
			else if (signArgs[1] == "_G4Bool")
			{
				one.customizeMesh = (_G4Bool)getEnumValue(wgt);
			}
			else if (signArgs[1] == "_G4Unknown")
			{
				one.unknow = (_G4Unknown)getEnumValue(wgt);
			}
			data.insert(one.name, one);
			dataModel->setAnalysisItems(data);
		}
		else if (type == "_G4Histrogram")
		{
			if (signArgs[1] == "")
			{
			}
			else if (signArgs[1] == "")
			{
			}
			else if (signArgs[1] == "")
			{
			}
			else if (signArgs[1] == "")
			{
			}
			else if (signArgs[1] == "")
			{
			}
		}
		else if (type == "_G4Normalization")
		{
			auto phyPart = dataModel->getAnalysisItems();
			_G4AnalysisItem gItem = phyPart.value(belong);
			if (signArgs[1] == "_G4NormalizationType")
			{
				gItem.normalization.type = (_G4NormalizationType)getEnumValue(wgt);
			}
			else if (signArgs[1] == "_GSourceFlex")
			{
				int value;
				if (getIntValue(value, wgt))
					gItem.normalization.flex = value;
				
			}
			DataManager::getInstance()->getGeant4DataModel()->changeAnalysisItem(belong, gItem);
		}
	}

	Geant4DataObserver::~Geant4DataObserver()
	{
	}
}