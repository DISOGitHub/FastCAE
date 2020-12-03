#include "ThermalData.h"
#include "TFThickness.h"
#include "TFMaterial.h"
#include "ThermalBoundary.h"
#include "ThermalCoupling.h"
#include "ControlTemperature.h"
#include <QDomDocument>
#include "BoundaryTabTypeData.h"

namespace ThermalFluid{
	ThermalData::ThermalData()
	{
		initDataUnits();
	}


	ThermalData::~ThermalData()
	{
		clearBoundaryList();
		clearCouplingList();
		clearMaterialList();
		clearThicknessList();
		clearTControlList();

		_elementToMaterialHash.clear();
		_elementToThicknessHash.clear();
	}


	void ThermalData::setCoorDataUnits(double xLength, double xForce, double xTemper)
	{
		_coorDataUnits[0] = xLength;
		_coorDataUnits[1] = xForce;
		_coorDataUnits[2] = xTemper;
	}

	void ThermalData::setMaterialDataUnits(double yLength, double yForce, double yTemper)
	{
		_materialDataUnits[0] = yLength;
		_materialDataUnits[1] = yForce;
		_materialDataUnits[2] = yTemper;
	}

	double ThermalData::getXLength()
	{
		return _coorDataUnits[0];
	}

	double ThermalData::getXFroce()
	{
		return _coorDataUnits[1];
	}

	double ThermalData::getXTemper()
	{
		return _coorDataUnits[2];
	}

	double ThermalData::getYLength()
	{
		return _materialDataUnits[0];
	}

	double ThermalData::getYFroce()
	{
		return _materialDataUnits[1];
	}

	double ThermalData::getYTemper()
	{
		return _materialDataUnits[2];
	}


	void ThermalData::appendThermalBoundaryList(ThermalBoundary* tb)
	{
		_tBoundaryList.append(tb);
	}

	QList<ThermalBoundary*> ThermalData::getThermalBoundaryList()
	{
		return _tBoundaryList;
	}

	ThermalBoundary* ThermalData::getThermalBoundary(int id)
	{
		for (int i = 0; i < _tBoundaryList.size(); i++)
		{
			if (_tBoundaryList.at(i)->getID() == id)
				return _tBoundaryList.at(i);
		}

		return nullptr;
	}

	int ThermalData::getThermalBoundaryListCount()
	{
		return _tBoundaryList.count();
	}

	ThermalBoundary* ThermalData::getThermalBoundaryFromIndex(int index)
	{
		if (index < 0 || index >= _tBoundaryList.count())
			return nullptr;

		return _tBoundaryList.at(index);
	}

	void ThermalData::removeThermalBoundary(ThermalBoundary* data)
	{
		if (data == nullptr)
			return;

		_tBoundaryList.removeOne(data);
		delete data;
		data = nullptr;
	}

	void ThermalData::appendThermalCouplingList(ThermalCoupling* tc)
	{
		_tCouplingList.append(tc);
	}

	QList<ThermalCoupling*> ThermalData::getThermalCouplingList()
	{
		return _tCouplingList;
	}

	ThermalCoupling* ThermalData::getThermalCoupling(int id)
	{
		for (int i = 0; i < _tCouplingList.size(); i++)
		{
			if (_tCouplingList.at(i)->getID() == id)
				return _tCouplingList.at(i);
		}

		return nullptr;
	}

	int ThermalData::getThermalCouplingListCount()
	{
		return _tCouplingList.count();
	}

	ThermalCoupling* ThermalData::getThermalCouplingFromIndex(int index)
	{
		if (index < 0 || index >= _tCouplingList.count())
			return nullptr;

		return _tCouplingList.at(index);
	}

	void ThermalData::removeThermalCoupling(ThermalCoupling* data)
	{
		if (data == nullptr)
			return;

		_tCouplingList.removeOne(data);
		delete data;
		data = nullptr;
	}

	void ThermalData::appendMaterialList(TFMaterial* tm)
	{
		_tMaterialList.append(tm);
	}

	QList<TFMaterial*> ThermalData::getMaterialList()
	{
		return _tMaterialList;
	}

	TFMaterial* ThermalData::getMaterial(int id)
	{
		for (int i = 0; i < _tMaterialList.size(); i++)
		{
			if (_tMaterialList.at(i)->getID() == id)
				return _tMaterialList.at(i);
		}

		return nullptr;
	}

	int ThermalData::getMaterialListCount()
	{
		return _tMaterialList.count();
	}

	TFMaterial* ThermalData::getMaterialFromIndex(int index)
	{
		if (index < 0 || index >= _tMaterialList.count())
			return nullptr;

		return _tMaterialList.at(index);
	}

	void ThermalData::removeMaterial(TFMaterial* data)
	{
		if (data == nullptr)
			return;

		_tMaterialList.removeOne(data);
		delete data;
		data = nullptr;
	}

	void ThermalData::appendThicknessList(TFThickness* tn)
	{
		_tThicknessList.append(tn);
	}

	QList<TFThickness*> ThermalData::getThicknessList()
	{
		return _tThicknessList;
	}

	TFThickness* ThermalData::getThickness(int id)
	{
		for (int i = 0; i < _tThicknessList.size(); i++)
		{
			if (_tThicknessList.at(i)->getID() == id)
				return _tThicknessList.at(i);
		}

		return nullptr;
	}

	int ThermalData::getThicknessListCount()
	{
		return _tThicknessList.count();
	}

	TFThickness* ThermalData::getThicknessFromIndex(int index)
	{
		if (index < 0 || index >= _tThicknessList.count())
			return nullptr;

		return _tThicknessList.at(index);
	}

	void ThermalData::removeThickness(TFThickness* data)
	{
		if (data == nullptr)
			return;

		_tThicknessList.removeOne(data);
		delete data;
		data = nullptr;
	}

	void ThermalData::appendControlTemperatureList(ControlTemperature* tc)
	{
		_tControlList.append(tc);
	}

	QList<ControlTemperature*> ThermalData::getControlTemperatureList()
	{
		return _tControlList;
	}

	ControlTemperature* ThermalData::getControlTemperature(int id)
	{
		for (int i = 0; i < _tControlList.size(); i++)
		{
			if (_tCouplingList.at(i)->getID() == id)
				return _tControlList.at(i);
		}

		return nullptr;
	}

	int ThermalData::getControlTemperatureListCount()
	{
		return _tControlList.count();
	}

	ControlTemperature* ThermalData::getControlTemperatureFromIndex(int index)
	{
		if (index < 0 || index >= _tControlList.count())
			return nullptr;

		return _tControlList.at(index);
	}

	void ThermalData::removeControlTemperature(ControlTemperature* data)
	{
		if (data == nullptr)
			return;

		_tControlList.removeOne(data);
		delete data;
		data = nullptr;
	}

	void ThermalData::appendTabTypeDataList(BoundaryTabTypeData* td)
	{
		_bTabTypeDataList.append(td);
	}

	int ThermalData::getTabTypeDataListCount()
	{
		return _bTabTypeDataList.size();
	}

	BoundaryTabTypeData* ThermalData::getTabTypeDataFromIndex(int index)
	{
		if (index < 0 || index >= _bTabTypeDataList.count())
			return nullptr;

		return _bTabTypeDataList.at(index);
	}

	ThermalFluid::BoundaryTabTypeData* ThermalData::getTabTypeData(int id)
	{
		for (int i = 0; i < _bTabTypeDataList.size(); i++)
		{
			if (_bTabTypeDataList.at(i)->getID() == id)
				return _bTabTypeDataList.at(i);
		}

		return nullptr;
	}

	QList<BoundaryTabTypeData*>& ThermalData::getTabTypeDataList()
	{
		return _bTabTypeDataList;
	}

	void ThermalData::appendElementToMaterialHash(int eleId, int tmId)
	{
		_elementToMaterialHash.insert(eleId, tmId);
	}

	int ThermalData::getMaterialIDFromHash(int eleId)
	{
		return _elementToMaterialHash[eleId];
	}

	void ThermalData::appendElementToThicknessHash(int eleId, int tnId)
	{
		_elementToThicknessHash.insert(eleId, tnId);
	}

	int ThermalData::getThicknessIDFromHash(int eleId)
	{
		return _elementToThicknessHash[eleId];
	}

	void ThermalData::clearBoundaryList()
	{
		for (int i = 0; i < _tBoundaryList.size(); i++)
		{
			ThermalBoundary* tb = _tBoundaryList.at(i);
			//_tBoundaryList.removeOne(tb);
			delete tb;
			tb = nullptr;
		}

		_tBoundaryList.clear();
	}

	void ThermalData::clearCouplingList()
	{
		for (int i = 0; i < _tCouplingList.size(); i++)
		{
			ThermalCoupling* tc = _tCouplingList.at(i);
			//_tCouplingList.removeOne(tc);
			delete tc;
			tc = nullptr;
		}

		_tCouplingList.clear();
	}

	void ThermalData::clearMaterialList()
	{
		for (int i = 0; i < _tMaterialList.size(); i++)
		{
			TFMaterial* tm = _tMaterialList.at(i);
			//_tMaterialList.removeOne(tm);
			delete tm;
			tm = nullptr;
		}

		_tMaterialList.clear();
	}

	void ThermalData::clearThicknessList()
	{
		for (int i = 0; i < _tThicknessList.size(); i++)
		{
			TFThickness* tn = _tThicknessList.at(i);
			//_tThicknessList.removeOne(tn);
			delete tn;
			tn = nullptr;
		}

		_tThicknessList.clear();
	}

	void ThermalData::clearTControlList()
	{
		for (int i = 0; i < _tControlList.size(); i++)
		{
			ControlTemperature* tc = _tControlList.at(i);
			//_tControlList.removeOne(tc);
			delete tc;
			tc = nullptr;
		}

		_tControlList.clear();
	}

	void ThermalData::clearTabTypeDataList()
	{
		for (int i = 0; i < _bTabTypeDataList.size(); i++)
		{
			BoundaryTabTypeData* td = _bTabTypeDataList.at(i);
			delete td;
			td = nullptr;
		}

		_bTabTypeDataList.clear();
	}

	ThermalType ThermalData::stringToThermalType(QString s)
	{
		s = s.toUpper();
		if (s == "QNODE")
			return ThermalType::QNODE;
		else if (s == "INTERP")
			return ThermalType::INTERP;
		else if (s == "SINK")
			return ThermalType::SINK;
		else if (s == "AREA")
			return ThermalType::AREA;
		else if (s == "LENG")
			return ThermalType::LENG;
		else if (s == "TOTAL")
			return ThermalType::TOTA;
		else if (s == "VOLU")
			return ThermalType::VOLU;
		else if (s == "ABSO")
			return ThermalType::ABSO;
		else
			return ThermalType::NONE;
	}

	QString ThermalData::thermalTypeToString(ThermalType t)
	{
		if (t == ThermalType::QNODE)
			return "QNODE";
		else if (t == ThermalType::INTERP)
			return "INTERP";
		else if (t == ThermalType::SINK)
			return "SINK";
		else if (t == ThermalType::AREA)
			return "AREA";
		else if (t == ThermalType::LENG)
			return "LENG";
		else if (t == ThermalType::TOTA)
			return "TOTAL";
		else if (t == ThermalType::VOLU)
			return "VOLU";
		else if (t == ThermalType::ABSO)
			return "ABSO";
		else
			return "NULL";

	}

	void ThermalData::writeThermalData( QDomDocument* doc, QDomElement* tf )
	{
		tf->setAttribute("xLength", _coorDataUnits[0]);
		tf->setAttribute("xForce",  _coorDataUnits[1]);
		tf->setAttribute("xTemper", _coorDataUnits[2]);
		tf->setAttribute("yLength", _materialDataUnits[0]);
		tf->setAttribute("yForce",  _materialDataUnits[1]);
		tf->setAttribute("yTemper", _materialDataUnits[2]);

		QDomElement tb     = doc->createElement("ThermalBoundary");
		QDomElement tc     = doc->createElement("ThermalCoupling");
		QDomElement mater  = doc->createElement("Material");
		QDomElement temper = doc->createElement("ControlTemperature");
		QDomElement thick  = doc->createElement("Thickness");
		QDomElement bttd   = doc->createElement("BoundaryTabTypeData");
		tf->appendChild(tb);
		tf->appendChild(tc);
		tf->appendChild(mater);
		tf->appendChild(temper);
		tf->appendChild(thick);
		tf->appendChild(bttd);

		for (int i = 0; i < _tCouplingList.size(); i++)
			_tCouplingList[i]->writeTcData(doc,&tc);

		for (int j = 0; j < _tMaterialList.size();j++)
			_tMaterialList[j]->writeMaterialData(doc,&mater);

		for (int k = 0; k < _tBoundaryList.size();k++)
			_tBoundaryList[k]->writeTbData(doc,&tb);

		for (int m = 0; m < _tThicknessList.size(); m++)
			_tThicknessList[m]->wirteThickData(doc,&thick);

		for (int n = 0; n < _tControlList.size();n++)
			_tControlList[n]->writeConTemperData(doc,&temper);

		for (int x = 0; x < _bTabTypeDataList.size(); x++)
			_bTabTypeDataList[x]->writeBoundaryTabTypeData(doc, &bttd);
	}

	void ThermalData::readThermalData( QDomElement* e )
	{
		QDomNodeList tbList         = e->elementsByTagName("aThermalBoundary");
		QDomNodeList tcList         = e->elementsByTagName("aThermalCoupling");
		QDomNodeList materialList   = e->elementsByTagName("aMaterial");
		QDomNodeList thickList      = e->elementsByTagName("aThick");
		QDomNodeList conTemperList  = e->elementsByTagName("aTemper");
		QDomNodeList bttdList       = e->elementsByTagName("aBoundaryTabTypeData");

		for (int i = 0; i < bttdList.size(); i++)
		{
			BoundaryTabTypeData *pBttd = new BoundaryTabTypeData();
			if (pBttd == nullptr)		delete pBttd;
			else
			{
				QDomElement aBttd = bttdList.at(i).toElement();
				pBttd->readBoundaryTabTypeData(&aBttd);
				appendTabTypeDataList(pBttd);
			}
		}

		for (int i = 0; i < tbList.size(); i++)
		{
			ThermalBoundary* pTb = new ThermalBoundary( this );
			if ( pTb == nullptr )		delete pTb;			
			else
			{
				QDomElement aTb = tbList.at(i).toElement();
				pTb->readTbData( &aTb );
				appendThermalBoundaryList( pTb );
			}	
		}
	
		for (int i = 0; i < tcList.size(); i++)
		{
			ThermalCoupling* pTc = new ThermalCoupling();
			if ( pTc == nullptr )		delete pTc;
			else
			{
				QDomElement aTc = tcList.at(i).toElement();
				pTc->readTcData( &aTc );
				appendThermalCouplingList( pTc );
			}	
		}
 		
		for (int i = 0; i < materialList.size(); i++)
		{
			TFMaterial* pMat = new TFMaterial();
			if ( pMat == nullptr )		delete pMat;
			else
			{
				QDomElement aMater = materialList.at(i).toElement();
				pMat->readMaterialData( &aMater );
				appendMaterialList( pMat );
			}			
		}
		
		for (int i = 0; i < thickList.size(); i++)
		{
			TFThickness *pThk = new TFThickness();
			if ( pThk == nullptr )		delete pThk;
			else
			{
				QDomElement aThick = thickList.at(i).toElement();
				pThk->readThickData( &aThick );
				appendThicknessList( pThk );
			}		
		}
		
		for (int i = 0; i < conTemperList.size(); i++)
		{
			ControlTemperature* pCt = new ControlTemperature( this );
			if ( pCt == nullptr )		delete pCt;
			else
			{
				QDomElement aConTemper = conTemperList.at(i).toElement();
				pCt->readConTemperData(&aConTemper);
				appendControlTemperatureList( pCt );
			}		
		}		
	}

	void ThermalData::setMeshKernalID(int id)
	{
		_mkID = id;
	}

	int ThermalData::getMeshKernalID()
	{
		return _mkID;
	}

	void ThermalData::initDataUnits()
	{
		_coorDataUnits[0] = 0;
		_coorDataUnits[1] = 0;
		_coorDataUnits[2] = 0;

		_materialDataUnits[0] = 0;
		_materialDataUnits[1] = 0;
		_materialDataUnits[2] = 0;
	}

	void ThermalData::clearHash()
	{
		_elementToMaterialHash.clear();
		_elementToThicknessHash.clear();
	}

	void ThermalData::clear()
	{
		initDataUnits();
		clearBoundaryList();
		clearCouplingList();
		clearMaterialList();
		clearThicknessList();
		clearTControlList();
		clearTabTypeDataList();
		clearHash();
	}
}
