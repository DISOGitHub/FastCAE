#pragma once
#include <QStringList>
#include <QMap>

/*
	Page Source
*/
enum _G4Location
{
	Line,
	Surface,
	Body,
};
#define _G4_LOCATION_ENUM_STRINGLIST_ QStringList{tr("Line"),tr("Surface"),tr("Body")}

enum _G4Shape
{
	Cube,
	Cuboid,
	Sphere,
	Cone,
	Cylinder,
};
#define _G4_SHAPE_ENUM_STRINGLIST_ QStringList{tr("Cube"),tr("Cuboid"),tr("Sphere"),tr("Cone"),tr("Cylinder")}

enum _G4Unit
{
	M,
	cm,
	mm,
};
#define _G4_UNIT_ENUM_STRINGLIST_ QStringList{tr("m"),tr("cm"),tr("mm")}

struct _G4Position
{
	int x{0};
	int y{0};
	int z{0};
};

struct _G4SourceType
{
	_G4Location location{ Surface };
	_G4Shape shape{ Sphere };
	_G4Position position;
	double radius{ 5 };
	_G4Unit unit{ cm };
};

enum _G4DirectionDistribution
{
	sin_law,
	cos_law,
};
#define _G4_DIRECTION_DISTRIBUTION_ENUM_STRINGLIST_ QStringList{tr("sin"),tr("cos")}

struct _G4AngleDistribution
{
	_G4DirectionDistribution directionDistribution{ cos_law };
	double anXitaMin{ 0.0 };
	double anXitaMax{ 90.0 };
	double anFaiMin{ 0.0 };
	double anFaiMax{ 360.0 };
};

enum _G4SpectrumType
{
	EeqE0,
	EPowerLow,
};
#define _G4_UNIT_SPECTRUM_TYPE_ENUM_STRINGLIST_ QStringList{tr("E=E0"),tr("EPowerLow")}

enum _G4ParticleType
{
	e_negative,
	e_plus,
	grmma,
	proton,
};
#define _G4_PARTICLE_TYPE_ENUM_STRINGLIST_ QStringList{tr("e-"),tr("e+"),tr("gramma"),tr("proton")}

enum _G4Unknown
{
	Mev,
	Exp,
};
#define _G4_UNKNOWN_ENUM_STRINGLIST_ QStringList{tr("Mev"),tr("Exp")}

enum _G4HistogramInputList
{
	Differential,
	Integral,
};
#define _G4_HISTOGRAM_INPUT_ENUM_STRINGLIST_ QStringList{tr("Differential"),tr("Integral")}

enum _G4SourceTableSpecification
{
	SPENVIS_trapped_ptotons,
};
#define _G4_SOURCE_TABLE_SPECIFICATION_ENUM_STRINGLIST_ QStringList{tr("SPENVIS_trapped_ptotons")}

enum _G4HistogramInsertWay
{
	Lin,
	Exp2,
};
#define _G4_HISTOGRAM_INSERT_WAY_ENUM_STRINGLIST_ QStringList{tr("Lin"),tr("Exp")}

enum _G4SourceSpecification
{
	to_fluence,
};
#define _G4_SOURCE_SOURCE_SPECIFICATION_ENUM_STRINGLIST_ QStringList{tr("to_fluence")}

struct _G4SourceParameters
{
	_G4SpectrumType spectrumType{ EeqE0 };
	_G4ParticleType particleType{ e_negative };
	_G4Unknown unknown{ Mev };
	double Emin{ 0.5 };
	double Emax{ 15.0 };
	double E0{ 5 };
	double powerLow{ 5 };

	QString envFile;
	QString histogramFile;

	_G4HistogramInputList inputList{ Differential };
	_G4HistogramInsertWay insertWay{ Exp2 };

	int integratedFlux;
//	_G4SourceTableSpecification tableSpecification;
};

/*
	Page Source End
*/





/*
	Page Physical 
*/

enum _G4PhysicalMode
{
	DefaultMode,
	SelfDefineMode,
};
#define _G4_PHYSICAL_MODE_ENUM_STRINGLIST_ QStringList{tr("Default"),tr("Customize")}

enum _G4ElectromagneticPhysics
{
	EmStandard,
	EmStandard_Opt1,
	EmStandard_Opt2,
	EmStandard_Opt3,
	RmcEmStandard,
};
#define _G4_ELECTROMAGNETIC_PHYSICS_ENUM_STRINGLIST_ QStringList{tr("em_standard"),tr("em_standard_opt1"),tr("em_standard_opt2"),tr("em_standard_opt3"),tr("rmc_em_standard")}

enum _G4HadronicPhysics
{
	QBBC,
	QGSP_BERT,
	QGSP_BERT_HP,
	Binary,
	Binary_had,
};
#define _G4_HADRONIC_PHYSICS_ENUM_STRINGLIST_ QStringList{tr("QBBC"),tr("QGSP_BERT"),tr("QGSP_BERT_HP"),tr("binary"),tr("binary_had")}

enum _G4Ion_Ion
{
	Binary_Ion,
	Qmd_Ion,
	Incl_Ion,
};
#define _G4_ION_ION_ENUM_STRINGLIST_ QStringList{tr("binary_ion"),tr("qmd_ion"),tr("incl_ion")}

enum _G4Other_Physics
{
	Sopping,
	Dacay,
	Raddecay,
	Gamma_Nac,
};
#define _G4_OTHER_PHYSICS_ENUM_STRINGLIST_ QStringList{tr("sopping"),tr("dacay"),tr("raddecay"),tr("gamma_nac")}

struct _G4PhysicalPart
{
	_G4PhysicalMode mode{ DefaultMode };
	_G4ElectromagneticPhysics ePhy{ EmStandard };
	_G4HadronicPhysics hPhy{ QBBC };
	_G4Ion_Ion ion{ Binary_Ion };
	_G4Other_Physics oPhy{ Dacay };
	double eIgnore{ 0.5 };
};

/*
	Page Physical End
*/

/*
	Page Geometry Energy Threshold
*/

enum _G4GeometryEnergyThresholdType
{
	Default,
	Customize,
};
#define _G4_GEO_EN_TH_TYPE_ENUM_STRINGLIST_ QStringList{tr("default"),tr("customize")}

struct _G4GeometryEnergyThreshold
{
	_G4GeometryEnergyThresholdType type{ Default };
	double 	eNegative{0};
	double ePlus{0};
	double grmma{0};
	double proton{0};
	QString belong;
};

/*
	Page Geometry Energy Threshold
*/

/*
	Page Calculate
*/
//Geant4CaAnItem,
//Geant4CaAnHiItem,
//Geant4CaAnNoItem,

enum _G4CalculateModel
{
	Ionising_Dose,
};
#define _G4_CALCULATE_MODEL_ENUM_STRINGLIST_ QStringList{tr("ionising_Dose"),tr(""),tr(""),tr("")}

enum _G4Bool
{
	True,
	False,
};
#define _G4_BOOL_ENUM_STRINGLIST_ QStringList{tr("True"),tr("False")}

enum _G4Histogram_Unit
{
	Mev_CM,
	Mev_Histogram,
	Mev_Mg_CM2,
};
#define _G4_HISTOGRAM_UNITLIST_ QStringList{tr("Mev/cm"),tr("Mev"),tr("Mev*mg/cm2")}

struct _G4Histogram
{
	QString name;
	int zoneNum;
	int xMin;
	int xMax;
	QString rootPath;
	_G4Histogram_Unit unit;
};

enum _G4NormalizationType
{
	NONE,
	FLUENCY,
	TO_FLUENCY,
	NONORMALIZATION,
	GEOMETRIC_FACTOR,
	GEOFACTOR,
	TO_SOURCE_SURFACE,
	PEREVT,
	PER_NB_EVENTS,
	PER_NB_EVT,
	PEREVENT,
};
#define _G4_NORMALIZATION_TYPE_ENUM_STRINGLIST_ QStringList{tr("NONE"),tr("FLUENCY"),tr("TO_FLUENCY"),tr("NONORMALIZATION"),tr("GEOMETRIC_FACTOR"),tr("GEOFACTOR"),tr("TO_SOURCE_SURFACE"),tr("PEREVT"),tr("PER_NB_EVENTS"),tr("PER_NB_EVT"),tr("PEREVENT")}

struct _G4Normalization
{
	_G4NormalizationType type;
	int flex{0};
};

enum _G4AnalysisType
{
	TID_Analysis,
	NIEL_Analysis,
	Fluence_Analysis,
	LET_Analysis,
};

struct _G4VolumeInterface
{
	QString volumeA;
	QString volumeB;
	bool isEqual(QString Name1, QString Name2)
	{
		if ((Name1 == volumeA && Name2 == volumeB) ||
			(Name1 == volumeB && Name2 == volumeA))
		{
			return true;
		}
		return false;
	}
};

struct _G4AnalysisItem
{
	_G4AnalysisType type;
	QString name;
	_G4CalculateModel model;
	QVector<_G4VolumeInterface> volumeInterfaces;
	QVector<QString> volumes;
	_G4Bool customizeMesh{ False };
	_G4Unknown unknow{ Mev };
	QMap<QString, _G4Histogram> hiss;
	_G4Normalization normalization;
};

struct _G4Relation
{
	QString objA;
	QString objB;

	bool operator== (const _G4Relation & an)
	{
		if ((an.objA == objA && an.objB == objB)||
			(an.objB == objA && an.objA == objB))
		{
			return true;
		}
		return false;
	}

};

/*
	Page Calculate End
*/

namespace Geant4
{
	class Geant4DataModel
	{
	public:
		Geant4DataModel() = default;
		~Geant4DataModel() = default;

		_G4SourceType getSourceType() const;
		void setSourceType(const _G4SourceType & type);

		_G4AngleDistribution getAngleDistribution() const;
		void setAngleDistribution(const _G4AngleDistribution & distribution);

		_G4SourceParameters getSourceParameters() const;
		void setSourceParameters(const _G4SourceParameters & parameters);

		_G4SourceSpecification getSourceSpecification() const;
		void setSourceSpecification(const _G4SourceSpecification & specification);

		_G4PhysicalPart getPhysicalPart() const;
		void setPhysicalPart(const _G4PhysicalPart & physicalPart);

		QMap<QString, _G4GeometryEnergyThreshold> getGeometriesEnergyThreshold() const;
		void setGeometriesEnergyThreshold(const QMap<QString, _G4GeometryEnergyThreshold> & geoEnTh);
		void deleteGeometriesEnergyThreshold(const QString& itemName);
		void setGeometryEnergyThreahold(const  _G4GeometryEnergyThreshold & geoEnTh);

		QMap<QString, _G4AnalysisItem> getAnalysisItems() const;
		_G4AnalysisItem getAnalysisItem(const QString& key) const;
		void changeAnalysisItem(const QString& itemName, const _G4AnalysisItem& item);
		void setAnalysisItems(const QMap<QString, _G4AnalysisItem> & items);
		void addAnalysisItem(_G4AnalysisItem item);
		void deleteAnalysisItem(QString itemName);
		void deleteAnalysisItemVolume(QString itemName, QString volName);

		QString getGDMLFile() const;
		void setGDMLFile(const QString & gdmlFile);

		bool isAnalysisExistVolume(const QString & anaName);
		bool isAnalysisExistVolumeInterface(const QString & anaName);

		QMap<QString, QList<_G4Relation> > getRelationsItems() const;
		QList<_G4Relation> getRelationsItem(const QString& key) const;
		void setRelations(const QString itemName, const _G4Relation& relation);
		void addRelationsItem(const QString& item, const _G4Relation& relation);
		void deleteRelationsItem(const QString& item, const _G4Relation& relation);
		void deleteAllRelationsItem();

	private:
		_G4SourceType m_sourceType;
		_G4AngleDistribution m_angleDistribution;
		_G4SourceParameters m_sourceParameters;
		_G4SourceSpecification m_sourceSpecification;

		_G4PhysicalPart m_physicalPart;

		QMap<QString, _G4GeometryEnergyThreshold> m_geoEnTh;

		QMap<QString, _G4AnalysisItem> m_analysisItems;

		QMap<QString, QList<_G4Relation> > m_Relations;

		QString m_gdmlFile;
	};
}