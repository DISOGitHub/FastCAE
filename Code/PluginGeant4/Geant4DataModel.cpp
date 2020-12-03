#include "Geant4DataModel.h"
namespace Geant4
{
	_G4SourceType Geant4DataModel::getSourceType() const
	{
		return m_sourceType;
	}

	void Geant4DataModel::setSourceType(const _G4SourceType & type)
	{
		m_sourceType = type;
	}

	_G4AngleDistribution Geant4DataModel::getAngleDistribution() const
	{
		return m_angleDistribution;
	}

	void Geant4DataModel::setAngleDistribution(const _G4AngleDistribution & distribution)
	{
		m_angleDistribution = distribution;
	}

	_G4SourceParameters Geant4DataModel::getSourceParameters() const
	{
		return m_sourceParameters;
	}

	void Geant4DataModel::setSourceParameters(const _G4SourceParameters & parameters)
	{
		m_sourceParameters = parameters;
	}

	_G4SourceSpecification Geant4DataModel::getSourceSpecification() const
	{
		return m_sourceSpecification;
	}

	void Geant4DataModel::setSourceSpecification(const _G4SourceSpecification & specification)
	{
		m_sourceSpecification = specification;
	}

	_G4PhysicalPart Geant4DataModel::getPhysicalPart() const
	{
		return m_physicalPart;
	}

	void Geant4DataModel::setPhysicalPart(const _G4PhysicalPart & physicalPart)
	{
		m_physicalPart = physicalPart;
	}

	QMap<QString, _G4GeometryEnergyThreshold> Geant4DataModel::getGeometriesEnergyThreshold() const
	{
		return m_geoEnTh;
	}

	void Geant4DataModel::setGeometriesEnergyThreshold(const QMap<QString, _G4GeometryEnergyThreshold> & geoEnTh)
	{
		m_geoEnTh = geoEnTh;
	}

	void Geant4DataModel::deleteGeometriesEnergyThreshold(const QString& itemName)
	{
		m_geoEnTh.remove(itemName);
	}

	void Geant4DataModel::setGeometryEnergyThreahold(const _G4GeometryEnergyThreshold & geoEnTh)
	{
		m_geoEnTh.insert(geoEnTh.belong, geoEnTh);
	}

	QMap<QString, _G4AnalysisItem> Geant4DataModel::getAnalysisItems() const
	{
		return m_analysisItems;
	}

	_G4AnalysisItem Geant4DataModel::getAnalysisItem(const QString& key) const
	{
		return m_analysisItems.value(key);
	}

	void Geant4DataModel::changeAnalysisItem(const QString& itemName, const _G4AnalysisItem& item)
	{
		m_analysisItems[itemName] = item;
	}

	void Geant4DataModel::setAnalysisItems(const QMap<QString, _G4AnalysisItem> & items)
	{
		m_analysisItems = items;
	}

	void Geant4DataModel::addAnalysisItem(_G4AnalysisItem item)
	{
		m_analysisItems.insert(item.name, item);
	}

	void Geant4DataModel::deleteAnalysisItem(QString itemName)
	{
		m_analysisItems.remove(itemName);
	}

	void Geant4DataModel::deleteAnalysisItemVolume(QString itemName, QString volName)
	{
		auto item = m_analysisItems.value(itemName);
		item.volumes.removeOne(volName);
		m_analysisItems.insert(itemName, item);
	}

	QString Geant4DataModel::getGDMLFile() const
	{
		return m_gdmlFile;
	}

	void Geant4DataModel::setGDMLFile(const QString & gdmlFile)
	{
		m_gdmlFile = gdmlFile;
	}

	bool Geant4DataModel::isAnalysisExistVolume(const QString & anaName)
	{
		if (m_analysisItems.contains(anaName))
		{
			auto info = m_analysisItems.value(anaName);
			if (info.volumes.isEmpty())
			{
				return false;
			}
			return true;
		}
		return false;
	}

	bool Geant4DataModel::isAnalysisExistVolumeInterface(const QString & anaName)
	{
		if (m_analysisItems.contains(anaName))
		{
			auto info = m_analysisItems.value(anaName);
			if (info.volumeInterfaces.isEmpty())
			{
				return false;
			}
			return true;
		}
		return false;
	}

	QMap<QString, QList<_G4Relation> > Geant4DataModel::getRelationsItems() const
	{
		return m_Relations;
	}

	QList<_G4Relation> Geant4DataModel::getRelationsItem(const QString& key) const
	{
		return m_Relations.value(key);
	}

	void Geant4DataModel::setRelations(const QString itemName, const _G4Relation& relation)
	{
		m_Relations[itemName].append(relation);
	}

	void Geant4DataModel::addRelationsItem(const QString& item, const _G4Relation& relation)
	{
		m_Relations[item].append(relation);
	}

	void Geant4DataModel::deleteRelationsItem(const QString& item, const _G4Relation& relation)
	{
		m_Relations[item].removeOne(relation);
	}

	void Geant4DataModel::deleteAllRelationsItem()
	{
		m_Relations.clear();
	}

}
