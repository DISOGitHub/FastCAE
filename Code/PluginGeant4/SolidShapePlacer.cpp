#include "SolidShapePlacer.h"
#include "mainWindow/mainWindow.h"
#include "DataManager.h"
#include "StructureManager.h"
#include "SolidsManager.h"
#include "Physvol.h"
#include "SolidShapeBase.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "mainWindow/SubWindowManager.h"
#include "MainWidgets/preWindow.h"
#include <TopoDS_Shape.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <QDebug>

namespace Geant4
{
	SolidShapePlacer::SolidShapePlacer(GUI::MainWindow* m)
	{
		_structureManager = DataManager::getInstance()->getStructManager();
		_solidManager = DataManager::getInstance()->getSolidsManager();
		_geoData = Geometry::GeometryData::getInstance();

		auto pw = m->getSubWindowManager()->getPreWindow();

		connect(this, SIGNAL(showSet(Geometry::GeometrySet*, bool)), pw, SIGNAL(showGeoSet(Geometry::GeometrySet*, bool)));
		connect(this, SIGNAL(updateGeoTree()), m, SIGNAL(updateGeometryTreeSig()));
	}

	void SolidShapePlacer::place()
	{
		QList<Physvol*> phyList = _structureManager->getAllPhysvol();
		qDebug() << phyList.size();
		for (auto phy : phyList)
		{
			qDebug() << phy->getName();
			QString solidref = phy->getSolidRef();
			qDebug() << solidref;
			SolidShapeBase* solidshape = _solidManager->getSolidByName(solidref);
			if(solidshape ==  nullptr) continue;
			TopoDS_Shape* shape = solidshape->getShape();
			if(shape == nullptr) continue;
			double r[3] = { 0 }, p[3] = { 0 };
			phy->getRotation(r);
			phy->getPosition(p);

			auto set = Transform(shape, r, p);
			if(set == nullptr) continue;
			set->setName(solidref);
			_geoData->appendGeometrySet(set);
			phy->connectToGeometrySet(set);
			emit showSet(set, true);
		}
		emit updateGeoTree();
	}

	Geometry::GeometrySet* SolidShapePlacer::Transform(TopoDS_Shape* s, double * r, double * p)
	{
		TopoDS_Shape shape = (*s);

		//ÒÆ¶¯
		gp_Vec vec(p[0], p[1], p[2]);
		gp_Trsf aTrsf;
		aTrsf.SetTranslation(vec);
		BRepBuilderAPI_Transform aBRespTrsf(shape, aTrsf, true);
		aBRespTrsf.Build();
		if (!aBRespTrsf.IsDone()) return nullptr;
		shape = aBRespTrsf.Shape();

		TopoDS_Shape *newshape = new TopoDS_Shape;
		*newshape = shape;
		auto set = new Geometry::GeometrySet;
		set->setShape(newshape);

		return set;
	}

}



