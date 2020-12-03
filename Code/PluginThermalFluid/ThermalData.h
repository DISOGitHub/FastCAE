#ifndef THERMALDATA_H
#define THERMALDATA_H

#include <QList>
#include <QHash>
#include <QString>

enum ThermalType{
	NONE = 0,
	QNODE,
	INTERP,
	SINK,
	AREA,
	LENG,
	TOTA,
	VOLU,
	ABSO
};

class QDomDocument;
class QDomElement;

namespace ThermalFluid{

	class ThermalBoundary;
	class ThermalCoupling;
	class TFMaterial;
	class TFThickness;
	class ControlTemperature;
	class BoundaryTabTypeData;

	class ThermalData
	{
		friend class TFCouplingTree;
	public:
		ThermalData();
		~ThermalData();

		void setCoorDataUnits(double xLength, double xForce, double xTemper);
		double getXLength();
		double getXFroce();
		double getXTemper();

		void setMaterialDataUnits(double yLength, double yForce, double yTemper);
		double getYLength();
		double getYFroce();
		double getYTemper();

		void appendThermalBoundaryList(ThermalBoundary* tb);
		QList<ThermalBoundary*> getThermalBoundaryList();
		ThermalBoundary* getThermalBoundary(int id);
		int getThermalBoundaryListCount();
		ThermalBoundary* getThermalBoundaryFromIndex(int index);
		void removeThermalBoundary(ThermalBoundary* data);

		void appendThermalCouplingList(ThermalCoupling* tc);
		QList<ThermalCoupling*> getThermalCouplingList();
		ThermalCoupling* getThermalCoupling(int id);
		int getThermalCouplingListCount();
		ThermalCoupling* getThermalCouplingFromIndex(int index);
		void removeThermalCoupling(ThermalCoupling* data);

		void appendMaterialList(TFMaterial* tm);
		QList<TFMaterial*> getMaterialList();
		TFMaterial* getMaterial(int id);
		int getMaterialListCount();
		TFMaterial* getMaterialFromIndex(int index);
		void removeMaterial(TFMaterial* data);

		void appendThicknessList(TFThickness* tn);
		QList<TFThickness*> getThicknessList();
		TFThickness* getThickness(int id);
		int getThicknessListCount();
		TFThickness* getThicknessFromIndex(int index);
		void removeThickness(TFThickness* data);

		void appendControlTemperatureList(ControlTemperature* tc);
		QList<ControlTemperature*> getControlTemperatureList();
		ControlTemperature* getControlTemperature(int id);
		int getControlTemperatureListCount();
		ControlTemperature* getControlTemperatureFromIndex(int index);
		void removeControlTemperature(ControlTemperature* data);

		void appendTabTypeDataList(BoundaryTabTypeData* td);
		int getTabTypeDataListCount();
		BoundaryTabTypeData* getTabTypeDataFromIndex(int index);
		BoundaryTabTypeData* getTabTypeData(int id);
		QList<BoundaryTabTypeData*>& getTabTypeDataList();


		void appendElementToMaterialHash(int eleId, int tmId);
		int getMaterialIDFromHash(int eleId);

		void appendElementToThicknessHash(int eleId, int tnId);
		int getThicknessIDFromHash(int eleId);

		ThermalType stringToThermalType(QString s);
		QString	thermalTypeToString(ThermalType t);

		void writeThermalData(QDomDocument* doc, QDomElement* tf);
		void readThermalData(QDomElement* e);

		void setMeshKernalID(int id);
		int getMeshKernalID();

		void clear();


	private:
		void initDataUnits();
		void clearBoundaryList();
		void clearCouplingList();
		void clearMaterialList();
		void clearThicknessList();
		void clearTControlList();
		void clearTabTypeDataList();
		void clearHash();

	private:
		double _coorDataUnits[3];
		double _materialDataUnits[3];

		int _mkID;

		QList<ThermalBoundary*> _tBoundaryList{};
		QList<ThermalCoupling*> _tCouplingList{};
		QList<TFMaterial*> _tMaterialList{};
		QList<TFThickness*> _tThicknessList{};
		QList<ControlTemperature*> _tControlList{};
		QList<BoundaryTabTypeData*> _bTabTypeDataList{};

		QHash<int, int> _elementToMaterialHash{};
		QHash<int, int> _elementToThicknessHash{};
	};
}
#endif // _DEBUG

