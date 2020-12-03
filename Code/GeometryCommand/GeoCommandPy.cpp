#include "GeoCommandPy.h"
#include <QMessageBox>
#include <QString>
#include <QDebug>
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include "GeoCommandList.h"
#include "GeoCommandCreateBox.h"
#include "GeoCommandCreateCylinder.h"
#include "GeoCommandCreateCone.h"
#include "GeoCommandCreateSphere.h"
#include "GeoCommandCreatePoint.h"
#include "GeoCommandCreateLine.h"
#include "GeoCommandCreateFace.h"
#include "GeoCommandCreateChamfer.h"
#include "GeoCommandCreateFillet.h"
#include "GeoCommandCreateVariableFillet.h"
#include "GeoCommandMirrorFeature.h"
#include "GeoCommandRotateFeature.h"
#include "GeoCommandMoveFeature.h"
#include "GeoCommandMakeMatrix.h"
#include"GeoCommandMakeExtrusion.h"
#include "GeoCommandMakeRevol.h"
#include "GeometryCommand/GeoCommandMakeLoft.h"
#include "GeometryCommand/GeoCommandMakeSweep.h"
#include "GeometryCommand/GeoCommandGeoSplitter.h"
#include "GeometryCommand/GeoCommandMakeFillHole.h"
#include "GeometryCommand/GeoCommandMakeRemoveSurface.h"
#include "GeometryCommand/GeoCommandFillGap.h"
#include <QMap>
#include <QPair>
namespace Command
{
    GUI::MainWindow* GeometryCommandPy::_mainWindow = nullptr;
    MainWidget::PreWindow* GeometryCommandPy::_preWindow = nullptr;

    void GeometryCommandPy::init(GUI::MainWindow* m, MainWidget::PreWindow* p)
    {
        _mainWindow = m;
        _preWindow = p;
    }

    void GeometryCommandPy::warning()
    {
        QMessageBox::warning(nullptr, QObject::tr("Warning"), QObject::tr("Create failed !"));
    }

    void GeometryCommandPy::createBox(QString name, double* loc, double* dir)
    {
        Command::CommandCreateBox* c = new Command::CommandCreateBox(_mainWindow, _preWindow);
        c->setName(name);
        c->setLocation(loc);
        c->setGeoPara(dir);
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

    void GeometryCommandPy::editBox(Geometry::GeometrySet* set, double* loc, double* dir)
    {
        Command::CommandCreateBox* c = new Command::CommandCreateBox(_mainWindow, _preWindow);
        c->setEditData(set);
        c->setLocation(loc);
        c->setGeoPara(dir);
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

    void GeometryCommandPy::createCylinder(QString name, double*corner, double*dir, double r, double l)
    {
        Command::GeoCommandCreateCylinder* c = new Command::GeoCommandCreateCylinder(_mainWindow, _preWindow);
        c->setName(name);
        c->setLocation(corner);
        c->setAxis(dir);
        c->setRadius(r);
        c->setLength(l);
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

    void GeometryCommandPy::editCylinder(Geometry::GeometrySet* set, double*corner, double*dir, double r, double l)
    {
        Command::GeoCommandCreateCylinder* c = new Command::GeoCommandCreateCylinder(_mainWindow, _preWindow);
        c->setEditData(set);
        c->setLocation(corner);
        c->setAxis(dir);
        c->setRadius(r);
        c->setLength(l);
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

    void GeometryCommandPy::createCone(QString name, double*corner, double*dir, double r, double r2, double l)
    {
        Command::GeoCommandCreateCone* c = new Command::GeoCommandCreateCone(_mainWindow, _preWindow);
        c->setName(name);
        c->setLocation(corner);
        c->setAxis(dir);
        c->setRadius(r,r2);
        c->setLength(l);
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

    void GeometryCommandPy::editCone(Geometry::GeometrySet* set, double*corner, double*dir, double r, double r2, double l)
    {
        Command::GeoCommandCreateCone* c = new Command::GeoCommandCreateCone(_mainWindow, _preWindow);
        c->setEditData(set);
        c->setLocation(corner);
        c->setAxis(dir);
        c->setRadius(r,r2);
        c->setLength(l);
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

    void GeometryCommandPy::createSphere(QString name, double* loc, double r)
    {
        Command::GeoCommandCreateSphere* c = new Command::GeoCommandCreateSphere(_mainWindow, _preWindow);
        c->setName(name);
        c->setLocation(loc);
        c->setRadius(r);
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

    void GeometryCommandPy::editSphere(Geometry::GeometrySet* set, double* loc, double r)
    {
        Command::GeoCommandCreateSphere* c = new Command::GeoCommandCreateSphere(_mainWindow, _preWindow);
        c->setEditData(set);
        c->setLocation(loc);
        c->setRadius(r);
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

    void GeometryCommandPy::createPoint(QString name, double* corner, double* para)
    {
        Command::GeoCommandCreatePoint* c = new Command::GeoCommandCreatePoint(_mainWindow, _preWindow);
        c->setName(name);
        c->setCorner(corner);
        c->setPara(para);
        double coor[3] = { 0.0 };
        for (int i = 0; i < 3; ++i)
            coor[i] = corner[i] + para[i];
        c->setCoordinate(coor);
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

    void GeometryCommandPy::editPoint(Geometry::GeometrySet* set, double* corner, double* para)
    {
        Command::GeoCommandCreatePoint* c = new Command::GeoCommandCreatePoint(_mainWindow, _preWindow);
        c->setEditData(set);
        c->setCorner(corner);
        c->setPara(para);
        double coor[3] = { 0.0 };
        for (int i = 0; i < 3; ++i)
            coor[i] = corner[i] + para[i];
        c->setCoordinate(coor);
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

    void GeometryCommandPy::createLine(QString name, double* startpoint, int index, double* coor, double len, double* dir, bool sreverse)
    {
        Command::GeoCommandCreateLine* c = new Command::GeoCommandCreateLine(_mainWindow, _preWindow);
		
		if(name!="") c->setName(name);
        c->setPoints(startpoint);
        c->setOptionIndex(index);
        c->setCoor(coor);
        c->setLength(len);
        c->setDir(dir);
        c->setReverse(sreverse);

        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

    void GeometryCommandPy::editLine(Geometry::GeometrySet* set, double* startpoint,  int index, double* coor, double len, double* dir, bool sreverse)
    {
        Command::GeoCommandCreateLine* c = new Command::GeoCommandCreateLine(_mainWindow, _preWindow);
        c->setEditData(set);
        c->setPoints(startpoint);
        c->setOptionIndex(index);
        c->setCoor(coor);
        c->setLength(len);
        c->setDir(dir);
        c->setReverse(sreverse);
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

    void GeometryCommandPy::createFace(QString name, int editid, QMultiHash<Geometry::GeometrySet*, int> hash)
    {
        Command::CommandCreateFace* c = new Command::CommandCreateFace(_mainWindow, _preWindow);
		if (name != "") c->setName(name);
        c->setShapeList(hash);
		if (editid>0)
		{
			Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(editid);
			if (set != nullptr) c->setEditData(set);
		}
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }


	void GeometryCommandPy::createChamfer(int editid, QMultiHash<Geometry::GeometrySet*, int> hash, double d1, double d2, int s)
    {
		bool sym{ true };
		if (s == 0)
			sym = true;
		else
			sym = false;
        Command::CommandCreateChamfer* c = new Command::CommandCreateChamfer(_mainWindow, _preWindow);
        c->setShapeList(hash);
        c->setSymmetrical(sym);
        c->setDistance(d1, d2);
		if (editid > 0)
		{
			Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(editid);
			if (set != nullptr) c->setEditData(set);
		}
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

    void GeometryCommandPy::editChamfer(Geometry::GeometrySet* set, QMultiHash<Geometry::GeometrySet*, int> hash, double d1, double d2, bool s)
    {
        Command::CommandCreateChamfer* c = new Command::CommandCreateChamfer(_mainWindow, _preWindow);
        c->setEditData(set);
        c->setShapeList(hash);
        c->setSymmetrical(s);
        c->setDistance(d1, d2);
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

    void GeometryCommandPy::createFillet(QMultiHash<Geometry::GeometrySet*, int> edges, double radius, int editID)
    {
        Command::CommandCreateFillet* c = new Command::CommandCreateFillet(_mainWindow, _preWindow);
        c->setShapeList(edges);
        c->setRadius(radius);
        if (editID > 0)
        {
            Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(editID);
            if (set != nullptr) c->setEditData(set);
        }

        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }



	void GeometryCommandPy::CreateVariableFillet(QMap<double, double> rad, double b, int editid, int setid, int edginex)
    {
        Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
        Command::CommandCreateVariableFillet* c = new Command::CommandCreateVariableFillet(_mainWindow, _preWindow);
        Geometry::GeometrySet*set = data->getGeometrySetByID(setid);
        c->setShape(set, edginex);
        c->setBasicRadius(b);
        c->setRaiudMap(rad);
		if (editid > 0)
		{
			Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(editid);
			if (set != nullptr) c->setEditData(set);
		}

        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }


	void GeometryCommandPy::CreateBooLOperation(BoolType t, int set1, int body1, int set2, int body2)
    {
		Geometry::GeometrySet* geo1 = Geometry::GeometryData::getInstance()->getGeometrySetByID(set1);
		if (geo1 == nullptr) return;
		Geometry::GeometrySet* geo2 = Geometry::GeometryData::getInstance()->getGeometrySetByID(set2);
		if (geo2 == nullptr) return;
		QPair <Geometry::GeometrySet*, int> solid1{ geo1, body1 };
		QPair <Geometry::GeometrySet*, int> solid2{ geo2, body2 };

        Command::CommandBool* c = new Command::CommandBool(_mainWindow, _preWindow);
        c->setType(t);
        c->setSolid1(solid1);
		c->setSolid2(solid2);
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

	void GeometryCommandPy::EditBooLOperation(Geometry::GeometrySet* set, BoolType t, int set1, int body1, int set2, int body2)
    {
		Geometry::GeometrySet* geo1 = Geometry::GeometryData::getInstance()->getGeometrySetByID(set1);
		if (geo1 == nullptr) return;
		Geometry::GeometrySet* geo2 = Geometry::GeometryData::getInstance()->getGeometrySetByID(set2);
		if (geo2 == nullptr) return;
		QPair <Geometry::GeometrySet*, int> solid1{ geo1, body1 };
		QPair <Geometry::GeometrySet*, int> solid2{ geo2, body2 };

        Command::CommandBool* c = new Command::CommandBool(_mainWindow, _preWindow);
        c->setEditData(set);
        c->setType(t);
		c->setSolid1(solid1);
		c->setSolid2(solid2);
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

	void GeometryCommandPy::CreateMirrorFeature(QMultiHash<Geometry::GeometrySet*, int> bodyhash, bool save, int typeindex, int faceindex, Geometry::GeometrySet* facebody, int planeindex, double * random, double* base)
    {
        Command::CommandMirrorFeature* c = new Command::CommandMirrorFeature(_mainWindow, _preWindow);
        c->setBodys(bodyhash);
        c->setSaveOrigin(save);
        c->setTypeIndex(typeindex);
		if (typeindex == 0)
		{
			c->setFaceIndex(faceindex);
			c->setFaceBody(facebody);
		}
		else if (typeindex == 1)
		{
			c->setPlaneIndex(planeindex);
		}
		else if (typeindex == 2)
		{
			c->setRandomDir(random);
			c->setBasePt(base);
		}
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

	void GeometryCommandPy::EditMirrorFeature(Geometry::GeometrySet* set, QMultiHash<Geometry::GeometrySet*, int> bodyhash, bool save, int typeindex, int faceindex, Geometry::GeometrySet* facebody, int planeindex, double * random, double* base)
    {
        Command::CommandMirrorFeature* c = new Command::CommandMirrorFeature(_mainWindow, _preWindow);
        c->setEditData(set);
		c->setBodys(bodyhash);
		c->setSaveOrigin(save);
		c->setTypeIndex(typeindex);
		if (typeindex == 0)
		{
			c->setFaceIndex(faceindex);
			c->setFaceBody(facebody);
		}
		else if (typeindex == 1)
		{
			c->setPlaneIndex(planeindex);
		}
		else if (typeindex == 2)
		{
			c->setRandomDir(random);
			c->setBasePt(base);
		}
        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }


	void GeometryCommandPy::RotateFeature(QMultiHash<Geometry::GeometrySet*, int> bodyhash, double* basicpt, int method,
                           Geometry::GeometrySet* edgeBody, int index, double* axis, int reverse, double angle, int saveOri)
    {
        auto c = new Command::CommandRotateFeature(_mainWindow, _preWindow);
		c->setBodys(bodyhash);
        c->setBasicPoint(basicpt);
        c->setMethod(method);
        c->setEdge(edgeBody, index);
        c->setVector(axis);
        c->setReverse(reverse == 1);
        c->setSaveOrigin(saveOri == 1);
        c->setDegree(angle);

        bool success = Command::GeoComandList::getInstance()->executeCommand(c);
        if (!success) warning();
    }

	void GeometryCommandPy::EditRotateFeature(Geometry::GeometrySet* editset, QMultiHash<Geometry::GeometrySet*, int> bodyHash, double* basicpt, int method,
		Geometry::GeometrySet* edgeBody, int index, double* axis, int reverse, double angle, int saveOri)
	{
		auto c = new Command::CommandRotateFeature(_mainWindow, _preWindow);
		c->setBodys(bodyHash);
		c->setEditData(editset);
		c->setBasicPoint(basicpt);
		c->setMethod(method);
		c->setEdge(edgeBody, index);
		c->setVector(axis);
		c->setReverse(reverse == 1);
		c->setSaveOrigin(saveOri == 1);
		c->setDegree(angle);

		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!success) warning();
	}

	void GeometryCommandPy::CreateMoveFeature(QMultiHash<Geometry::GeometrySet*, int> bodyhash, double* start, double* end, bool s, int typeindex, bool r, double l, double* dir)
	{
		Command::CommandMoveFeature* c = new Command::CommandMoveFeature(_mainWindow, _preWindow);
		c->setBodys(bodyhash);
		c->setSaveOrigin(s);
		c->setOptionIndex(typeindex);
		if (typeindex == 0)
		{
			c->setStartPt(start);
			c->setEndPt(end);
		}
		else
		{
			c->setReverse(r);
			c->setLength(l);
			c->setDir(dir);
		}
		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!success) warning();
	}

	void GeometryCommandPy::EditMoveFeature(Geometry::GeometrySet* set, QMultiHash<Geometry::GeometrySet*, int> bodyhash, double* start, double* end, bool s, int typeindex, bool r, double l, double* dir)
	{
		Command::CommandMoveFeature* c = new Command::CommandMoveFeature(_mainWindow, _preWindow);
		c->setEditData(set);
		c->setBodys(bodyhash);
		c->setSaveOrigin(s);
		c->setOptionIndex(typeindex);
		if (typeindex==0)
		{
			c->setStartPt(start);
			c->setEndPt(end);
		}
		else
		{
			c->setReverse(r);
			c->setLength(l);
			c->setDir(dir);
		}
	
		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!success) warning();
	}

	void GeometryCommandPy::MakeMatrix(QMultiHash<Geometry::GeometrySet*, int> bodyhash, int optionindex, double* dir1, bool reverse1, double dis1, int count1, bool showdir2, double*dir2, bool reverse2, double dis2, int count2, double* basept, double*axis, bool wirereverse, int wirecount, double degree)
	{
		Command::CommandMakeMatrix*c = new Command::CommandMakeMatrix(_mainWindow, _preWindow);
		c->setBodys(bodyhash);
		c->setOptionIndex(optionindex);
		c->setDir1(dir1);
		c->setReverse1(reverse1);
		c->setDir1Distance(dis1);
		c->setDir1Count(count1);
		c->setDir2IsChecked(showdir2);
		c->setDir2(dir2);
		c->setReverse2(reverse2);
		c->setDir2Distance(dis2);
		c->setDir2Count(count2);
		c->setBasicPoint(basept);
		c->setAxisDir(axis);
		c->setWireCount(wirecount);
		c->setDegree(degree);
		c->setWireRevers(wirereverse);
		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!success) warning();

	}


	void GeometryCommandPy::EditMatrix(Geometry::GeometrySet* set, QMultiHash<Geometry::GeometrySet*, int> bodyhash, int optionindex, double* dir1, bool reverse1, double dis1, int count1, bool showdir2, double*dir2, bool reverse2, double dis2, int count2, double* basept, double*axis, bool wirereverse, int wirecount, double degree)
	{
		Command::CommandMakeMatrix*c = new Command::CommandMakeMatrix(_mainWindow, _preWindow);
		c->setEditData(set);
		c->setBodys(bodyhash);
		c->setOptionIndex(optionindex);
		c->setDir1(dir1);
		c->setReverse1(reverse1);
		c->setDir1Distance(dis1);
		c->setDir1Count(count1);
		c->setDir2IsChecked(showdir2);
		c->setDir2(dir2);
		c->setReverse2(reverse2);
		c->setDir2Distance(dis2);
		c->setDir2Count(count2);
		c->setBasicPoint(basept);
		c->setAxisDir(axis);
		c->setWireCount(wirecount);
		c->setDegree(degree);
		c->setWireRevers(wirereverse);
		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!success) warning();

	}

	void GeometryCommandPy::CreateExtrusion(int editid,QString n, QMultiHash<Geometry::GeometrySet*, int> hash, double dis, double* dir, bool r, bool s)
	{
		Command::GeoCommandMakeExtrusion* c = new Command::GeoCommandMakeExtrusion(_mainWindow, _preWindow);
		c->setName(n);
		c->setShapeList(hash);
		c->setDistance(dis);
		c->setDirection(dir);
		c->setReverse(r);
		c->isSolid(s);

		if (editid > 0)
		{
			Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(editid);
			if (set != nullptr) c->setEditData(set); 
		}

		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!success) warning();
	}

	void GeometryCommandPy::CreateRevol(int editid, QString name, QMultiHash<Geometry::GeometrySet*, int> hash, double* basept, double degree, int optionindex, QPair<Geometry::GeometrySet*, int> axisetedge, double* coor, bool reverse, bool solid)
	{
		Command::GeoCommandMakeRevol*c = new Command::GeoCommandMakeRevol(_mainWindow, _preWindow);
		c->setName(name);
		c->setShapeList(hash);
		c->setDegree(degree);
		c->isSolid(solid);
		c->setBasicPoint(basept);
		c->setReverse(reverse);
		c->setOptionMethod(optionindex);
		if (optionindex==0)
			c->setAixSetEdge(axisetedge);
		else
			c->setCoor(coor);
			
		if (editid > 0)
		{
			Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(editid);
			if (set != nullptr) c->setEditData(set);
		}

		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!success) warning();
	}

	void GeometryCommandPy::CreateLoft(int id, QString name, bool solid, QList< QMultiHash<Geometry::GeometrySet*, int>> shapelist)
	{
		Command::GeoCommandMakeLoft* command = new Command::GeoCommandMakeLoft(_mainWindow, _preWindow);

		command->setShapeList(shapelist);
		command->isSolid(solid);
		command->setName(name);
		if (id > 0)
		{
			Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(id);
			if (set != nullptr) command->setEditData(set);
		}
		
		bool success = Command::GeoComandList::getInstance()->executeCommand(command);
		if (!success) warning();
	}

	void GeometryCommandPy::CreateSweep(int id, QMultiHash<Geometry::GeometrySet*, int> hash, QPair<Geometry::GeometrySet*, int> path, bool solid)
	{
		Command::GeoCommandMakeSweep* command = new Command::GeoCommandMakeSweep(_mainWindow, _preWindow);
		command->setSection(hash);
		command->setPath(path);
		command->isSolid(solid);

		if (id > 0)
		{
			Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(id);
			if (set != nullptr) command->setEditData(set);
		}

		bool success = Command::GeoComandList::getInstance()->executeCommand(command);
		if (!success) warning();
	}

	void GeometryCommandPy::MakeGeoSplitter(QMultiHash<Geometry::GeometrySet*, int> bodyhash, int typeindex, int faceindex, Geometry::GeometrySet* facebody, int planeindex, double * random, double* base)
	{
		Command::CommandGeoSplitter* command = new Command::CommandGeoSplitter(_mainWindow, _preWindow);
		command->setBodys(bodyhash);
		command->setTypeIndex(typeindex);
		if (typeindex == 0)
		{
			command->setFaceIndex(faceindex);
			command->setFaceBody(facebody);
		}
		else if (typeindex == 1)
		{
			command->setPlaneIndex(planeindex);
		}
		else if (typeindex == 2)
		{
			command->setRandomDir(random);
			command->setBasePt(base);
		}
		bool success = Command::GeoComandList::getInstance()->executeCommand(command);
		if (!success) warning();
	}

	void GeometryCommandPy::EditGeoSplitter(Geometry::GeometrySet* editset, QMultiHash<Geometry::GeometrySet*, int> bodyhash, int typeindex, int faceindex, Geometry::GeometrySet* facebody, int planeindex, double * random, double* base)
	{
		Command::CommandGeoSplitter* command = new Command::CommandGeoSplitter(_mainWindow, _preWindow);
		command->setEditData(editset);
		command->setBodys(bodyhash);
		command->setTypeIndex(typeindex);

		if (typeindex == 0)
		{
			command->setFaceIndex(faceindex);
			command->setFaceBody(facebody);
		}
		else if (typeindex == 1)
		{
			command->setPlaneIndex(planeindex);
		}
		else if (typeindex == 2)
		{
			command->setRandomDir(random);
			command->setBasePt(base);
		}

		bool success = Command::GeoComandList::getInstance()->executeCommand(command);
		if (!success) warning();
	}

	void GeometryCommandPy::MakeFillHole(QMultiHash<Geometry::GeometrySet*, int> faces, int editID)
	{
		Command::CommandMakeFillHole* c = new Command::CommandMakeFillHole(_mainWindow, _preWindow);
		c->setShapeList(faces);
		if (editID > 0)
		{
			Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(editID);
			if (set != nullptr) c->setEditData(set);
		}

		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!success) warning();
	}

	void GeometryCommandPy::MakeRemoveSurface(QMultiHash<Geometry::GeometrySet*, int> faces, int editID)
	{
		Command::CommandMakeRemoveSurface* c = new Command::CommandMakeRemoveSurface(_mainWindow, _preWindow);
		c->setShapeList(faces);
		if (editID > 0) 
		{
			Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(editID);
			if (set != nullptr) c->setEditData(set);
		}
		
		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!success) warning();
	}

	void GeometryCommandPy::MakeFillGap(int type, int set1, int body1, int set2, int body2)
	{
		Geometry::GeometrySet* geo1 = Geometry::GeometryData::getInstance()->getGeometrySetByID(set1);
		if (geo1 == nullptr) return;
		Geometry::GeometrySet* geo2 = Geometry::GeometryData::getInstance()->getGeometrySetByID(set2);
		if (geo2 == nullptr) return;
		QPair <Geometry::GeometrySet*, int> solid1{ geo1, body1 };
		QPair <Geometry::GeometrySet*, int> solid2{ geo2, body2 };
		QList <	QPair <Geometry::GeometrySet*, int>> shapelist{};
		shapelist.append(solid1); shapelist.append(solid2);

		Command::CommandFillGap* c = new Command::CommandFillGap(_mainWindow, _preWindow);
		c->setGapType(type);
		switch (type)
		{
		case 0:c->setEdges(shapelist); break;
		case 1:c->setFaces(shapelist); break;
		case 2:c->setSolids(shapelist); break;
		default:
			break;
		}
		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!success) warning();
	}

	void GeometryCommandPy::EditFillGap(Geometry::GeometrySet* set, int type, int set1, int body1, int set2, int body2)
	{
		Geometry::GeometrySet* geo1 = Geometry::GeometryData::getInstance()->getGeometrySetByID(set1);
		if (geo1 == nullptr) return;
		Geometry::GeometrySet* geo2 = Geometry::GeometryData::getInstance()->getGeometrySetByID(set2);
		if (geo2 == nullptr) return;
		QPair <Geometry::GeometrySet*, int> solid1{ geo1, body1 };
		QPair <Geometry::GeometrySet*, int> solid2{ geo2, body2 };
		QList <	QPair <Geometry::GeometrySet*, int>> shapelist{};
		shapelist.append(solid1); shapelist.append(solid2);

		Command::CommandFillGap* c = new Command::CommandFillGap(_mainWindow, _preWindow);
		c->setEditData(set);
		c->setGapType(type);
		switch (type)
		{
		case 0:c->setEdges(shapelist); break;
		case 1:c->setFaces(shapelist); break;
		case 2:c->setSolids(shapelist); break;
		default:
			break;
		}
		bool success = Command::GeoComandList::getInstance()->executeCommand(c);
		if (!success) warning();
	}

}

void GEOMETRYCOMMANDAPI CreateBox(char* name, double x, double y, double z, double l, double w, double h)
{
    QString sname = QString(name);
    double loc[3] = { x, y, z };
    double dir[3] = { l, w, h };
    Command::GeometryCommandPy::createBox(sname, loc, dir);
}

void GEOMETRYCOMMANDAPI EditBox(int id, double x, double y, double z, double l, double w, double h)
{
    Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(id);
    double loc[3] = { x, y, z };
    double dir[3] = { l, w, h };
    Command::GeometryCommandPy::editBox(set, loc, dir);
}

void GEOMETRYCOMMANDAPI CreateCylinder(char* name, double x, double y, double z, double l, double w, double h, double radius, double length)
{
    QString sname = QString(name);
    double corner[3] = { x, y, z };
    double dir[3] = { l, w, h };
    double rad = radius;
    double len = length;
    Command::GeometryCommandPy::createCylinder(sname, corner, dir, rad, len);

}

void GEOMETRYCOMMANDAPI EditCylinder(int id, double x, double y, double z, double l, double w, double h, double radius, double length)
{
    Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(id);
    double corner[3] = { x, y, z };
    double dir[3] = { l, w, h };
    double rad = radius;
    double len = length;
    Command::GeometryCommandPy::editCylinder(set, corner, dir, rad, len);

}

void GEOMETRYCOMMANDAPI CreateCone(char* name, double x, double y, double z, double l, double w, double h, double radius, double radius2, double length)
{
    QString sname = QString(name);
    double corner[3] = { x, y, z };
    double dir[3] = { l, w, h };
    double rad = radius;
    double rad2 = radius2;
    double len = length;
    Command::GeometryCommandPy::createCone(sname, corner, dir, rad, rad2, len);
}

void GEOMETRYCOMMANDAPI EditCone(int id, double x, double y, double z, double l, double w, double h, double radius, double radius2 ,double length)
{
    Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(id);
    double corner[3] = { x, y, z };
    double dir[3] = { l, w, h };
    double rad = radius;
    double rad2 = radius2;
    double len = length;
    Command::GeometryCommandPy::editCone(set, corner, dir, rad, rad2, len);
    
}

void GEOMETRYCOMMANDAPI CreateSphere(char* name, double x, double y, double z, double r)
{
    QString sname = QString(name);
    double corner[3] = { x, y, z };
    double rad = r;
    Command::GeometryCommandPy::createSphere(sname, corner, rad);
}

void GEOMETRYCOMMANDAPI EditSphere(int id, double x, double y, double z, double r)
{
    Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(id);
    double corner[3] = { x, y, z };
    double rad = r;
    Command::GeometryCommandPy::editSphere(set, corner, rad);
}

void GEOMETRYCOMMANDAPI CreatePoint(char* name, double x, double y, double z, double p1, double p2, double p3)
{
    QString sname = QString(name);
    double corner[3] = { x, y, z };
    double para[3] = { p1, p2, p3 };
    Command::GeometryCommandPy::createPoint(sname, corner, para);
}

void GEOMETRYCOMMANDAPI EditPoint(int id, double x, double y, double z, double p1, double p2, double p3)
{
    Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(id);
    double corner[3] = { x, y, z };
    double para[3] = { p1, p2, p3 };
    Command::GeometryCommandPy::editPoint(set, corner, para);
}

void GEOMETRYCOMMANDAPI CreateLine(char* name, double startpoint0, double startpoint1, double startpoint2,int method, double coor0, double coor1, double coor2, double len, double dir0, double dir1, double dir2, int reverse)
{
    QString sname = QString(name);
    double startPoint[3] = {startpoint0,startpoint1,startpoint2};
    double coor[3] = {coor0,coor1,coor2};
    double dir[3] = {dir0,dir1,dir2};
    bool sreverse;
    if (reverse == 0) { sreverse = false; }
    else { sreverse = true; }
    Command::GeometryCommandPy::createLine(sname, startPoint, method,coor,len,dir,sreverse);
}

void GEOMETRYCOMMANDAPI EditLine(int id, double startpoint0, double startpoint1, double startpoint2,  int method, double coor0, double coor1, double coor2, double len, double dir0, double dir1, double dir2, int reverse)
{
	Geometry::GeometrySet* set = new Geometry::GeometrySet();
	if (id>0)
		set = Geometry::GeometryData::getInstance()->getGeometrySetByID(id);
	if (set == nullptr) return;
	
    double startPoint[3] = { startpoint0, startpoint1, startpoint2 };
    double coor[3] = { coor0, coor1, coor2 };
    double dir[3] = { dir0, dir1, dir2 };
    bool sreverse;
    if (reverse == 0) { sreverse = false; }
    else { sreverse = true; }
    Command::GeometryCommandPy::editLine(set, startPoint, method, coor, len, dir, sreverse);
}

void GEOMETRYCOMMANDAPI CreateFace(char* edges, char* name, int editId)
{
    QString sname = QString(name);
	QString cedge = QString(edges);
	QStringList setInfos = cedge.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for(QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for(QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}

    Command::GeometryCommandPy::createFace(sname, editId, shapeHash);
}


void GEOMETRYCOMMANDAPI CreateChamfer(char* edges, int editId, double d1, double d2, int typeindex)
{
	QString cedge = QString(edges);
	QStringList setInfos = cedge.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for(QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for(QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}
	
	Command::GeometryCommandPy::createChamfer(editId, shapeHash,d1,d2,typeindex);
}

void GEOMETRYCOMMANDAPI EditChamfer(int id, char*setidStr, char* indexListStr, double d1, double d2, int sym)
{
    Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(id);
    QString setidlist = QString(setidStr);
    QStringList coorsl = setidlist.split(",");
    if (coorsl.size() < 1) return;
    QList<int> setidList;
    for (int i = 0; i < coorsl.size(); ++i)
    {
        setidList.append(coorsl.at(i).toInt());
    }

    QString indexlist = QString(indexListStr);
    QStringList startsl = indexlist.split(",");
    if (startsl.size() < 1) return;
    QList<int> indexList;
    for (int i = 0; i < startsl.size(); ++i)
    {
        indexList.append(startsl.at(i).toInt());
    }

    QMultiHash<Geometry::GeometrySet*, int> shapeHash;

    Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
    for (int i = 0; i < setidList.size(); ++i)
    {
        Geometry::GeometrySet*set = data->getGeometrySetByID(setidList[i]);
        shapeHash.insert(set, indexList[i]);
    }
    double dis1 = d1;
    double dis2 = d2;
    bool s;
    if (sym == 1) s = true;
    else { s = false; }

    Command::GeometryCommandPy::editChamfer(set,shapeHash, dis1, dis2, s);
}

void GEOMETRYCOMMANDAPI CreateFillet(char* edges, double rad, int editID)
{
    QString cedge = QString(edges);

    QStringList setInfos = cedge.split(";");
    QMultiHash<Geometry::GeometrySet*, int> shapeHash;
    Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
    for(QString setinfo : setInfos)
    {
        QStringList setin = setinfo.split(":");
        int setid = setin.at(0).toInt();
        auto set = data->getGeometrySetByID(setid);
        if (set == nullptr) continue;
        QStringList edges = setin.at(1).split(",");
        for(QString e : edges)
        {
            int index = e.toInt();
            shapeHash.insert(set, index);
        }
    }

    Command::GeometryCommandPy::createFillet(shapeHash, rad, editID);
}

void GEOMETRYCOMMANDAPI CreateVariableFillet(char*edges, double basicrad, int editId, int setid, int edgeindex)
{
	QString cedge = QString(edges);
	QStringList setInfos = cedge.split(";");
	//qDebug() << edges;
    QMap<double,double> radmap;
	for (QString setinfo : setInfos)
	{
		QStringList pointlist = setinfo.split(":");
		radmap.insert(pointlist[0].toDouble(), pointlist[1].toDouble());
	}

	Command::GeometryCommandPy::CreateVariableFillet(radmap, basicrad, editId, setid, edgeindex);
}


void GEOMETRYCOMMANDAPI CreateBooLOperation(char* booltype, int set1, int body1, int set2, int body2)
{
	QString str = QString(booltype);
	BoolType sbooltype{};
	if (str == "None") return;
	else if (str == "Cut") sbooltype = BoolCut;
	else if (str == "Fause") sbooltype = BoolFause;
	else if (str == "Common") sbooltype = BoolCommon;

	Command::GeometryCommandPy::CreateBooLOperation(sbooltype, set1, body1, set2, body2);
}

void GEOMETRYCOMMANDAPI EditBooLOperation(int id, char* booltype, int set1, int body1, int set2, int body2)
{
    Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(id);
	QString str = QString(booltype);
	BoolType sbooltype{};
	if (str == "None") return;
	else if (str == "Cut") sbooltype = BoolCut;
	else if (str == "Fause") sbooltype = BoolFause;
	else if (str == "Common") sbooltype = BoolCommon;
    
	Command::GeometryCommandPy::EditBooLOperation(set, sbooltype, set1, body1, set2, body2);
}

void GEOMETRYCOMMANDAPI CreateMirrorFeature(char* bodys, char* method, int faceindex, int facebody, char* planemethod, double random0, double random1, double random2, double base0, double base1, double base2, char* saveori)
{
   
	QString cedge = QString(bodys);
	QStringList setInfos = cedge.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for (QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for (QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}

	int typeindex{};
	QString methodstr = QString(method);
	if (methodstr == "SelectPlaneOnGeo") typeindex = 0;
	else if (methodstr == "Coordinate") typeindex = 1;
	else if (methodstr == "Random")typeindex = 2;

	int planeindex{};
	QString planemethodstr = QString(planemethod);
	if (planemethodstr == "XOY") planeindex = 0;
	else if (planemethodstr == "XOZ") planeindex = 1;
	else if (planemethodstr == "YOZ")planeindex = 2;
	QString savestr = QString(saveori);
	bool s;
    if (savestr == "Yes") s = true;
	else s = false;

    Geometry::GeometrySet* facebodyset = Geometry::GeometryData::getInstance()->getGeometrySetByID(facebody);
    double random[3] = { random0, random1, random2 };
    double basept[3] = { base0, base1, base2 };
	Command::GeometryCommandPy::CreateMirrorFeature(shapeHash, s, typeindex, faceindex, facebodyset, planeindex, random, basept);
}

void GEOMETRYCOMMANDAPI EditMirrorFeature(int id, char* bodys, char* method, int faceindex, int facebody, char* planemethod, double random0, double random1, double random2, double base0, double base1, double base2, char* saveori)
{
    Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(id);

	QString cedge = QString(bodys);
	QStringList setInfos = cedge.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for (QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for (QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}

	int typeindex{};
	QString methodstr = QString(method);
	if (methodstr == "SelectPlaneOnGeo") typeindex = 0;
	else if (methodstr == "Coordinate") typeindex = 1;
	else if (methodstr == "Random")typeindex = 2;

	int planeindex{};
	QString planemethodstr = QString(planemethod);
	if (planemethodstr == "XOY") planeindex = 0;
	else if (planemethodstr == "XOZ") planeindex = 1;
	else if (planemethodstr == "YOZ")planeindex = 2;
	QString savestr = QString(saveori);

	bool s;
	if (savestr == "Yes") s = true;
	else s = false;

	Geometry::GeometrySet* facebodyset = Geometry::GeometryData::getInstance()->getGeometrySetByID(facebody);
	double random[3] = { random0, random1, random2 };
	double basept[3] = { base0, base1, base2 };
    Command::GeometryCommandPy::EditMirrorFeature(set,shapeHash, s, typeindex, faceindex, facebodyset, planeindex, random, basept);
}

void GEOMETRYCOMMANDAPI RotateFeature(char* body, double basicx, double basicy, double basicz, /*体 & 基准点 */ 
                                       int method, int edgeBoby, int edgeIndex, double axisx, double axisy, double axisz, int reverse, /*轴线 */
                                       double angle, int saveOri)
{
	QString cedge = QString(body);
	QStringList setInfos = cedge.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for (QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for (QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}
    double basicPt[3] = { basicx, basicy, basicz };
    double axis[3] = { axisx, axisy, axisz };
    
    Geometry::GeometrySet* edge = data->getGeometrySetByID(edgeBoby);
	Command::GeometryCommandPy::RotateFeature(shapeHash, basicPt, method, edge, edgeIndex, axis, reverse, angle, saveOri);
	  
}

void GEOMETRYCOMMANDAPI EditRotateFeature(int bodyid, char* body, double basicx, double basicy, double basicz, /*体 & 基准点 */
	int method, int edgeBoby, int edgeIndex, double axisx, double axisy, double axisz, int reverse, /*轴线 */
	double angle, int saveOri)
{
	QString cedge = QString(body);
	QStringList setInfos = cedge.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for (QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for (QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}
	Geometry::GeometryData* gdata = Geometry::GeometryData::getInstance();
	Geometry::GeometrySet* editset = gdata->getGeometrySetByID(bodyid);
	double basicPt[3] = { basicx, basicy, basicz };
	double axis[3] = { axisx, axisy, axisz };

	Geometry::GeometrySet* edge = gdata->getGeometrySetByID(edgeBoby);
	Command::GeometryCommandPy::EditRotateFeature(editset, shapeHash,basicPt, method, edge, edgeIndex, axis, reverse, angle, saveOri);
}

void GEOMETRYCOMMANDAPI CreateMoveFeature(char * bodys, char* method, double startpt0, double startpt1, double startpt2, double endpt0, double endpt1, double endpt2, char* save, char* reverse,
											double length, double dir0, double dir1, double dir2)
{
	QString cedge = QString(bodys);
	QStringList setInfos = cedge.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for (QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for (QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}
	int optionindex{};
	QString methodstr = QString(method);
	if (methodstr == "Tow Points") optionindex = 0;
	else if (methodstr == "Distance") optionindex = 1;

	bool s;
	QString savestr = QString(save);
	if (savestr == "Yes") s = true;
	else s = false;

	bool r;                   
	QString reverstr = QString(reverse);
	if (reverstr == "Yes") r = true;
	else r = false;
	double start[3] = { startpt0, startpt1, startpt2 };
	double end[3] = { endpt0, endpt1, endpt2 };
	double dir[3] = { dir0, dir1, dir2 };
	Command::GeometryCommandPy::CreateMoveFeature(shapeHash, start, end, s, optionindex, r, length, dir);
}

void GEOMETRYCOMMANDAPI EditMoveFeature(int id, char * bodys, char* method, double startpt0, double startpt1, double startpt2, double endpt0, double endpt1, double endpt2, char* save, char* reverse,
											double length, double dir0, double dir1, double dir2)
{
	Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(id);
	QString cedge = QString(bodys);
	QStringList setInfos = cedge.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for (QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for (QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}
	int optionindex{};
	QString methodstr = QString(method);
	if (methodstr == "Tow Points") optionindex = 0;
	else if (methodstr == "Distance") optionindex = 1;

	bool s;
	QString savestr = QString(save);
	if (savestr == "Yes") s = true;
	else s = false;

	bool r{ false };
	QString reverstr = QString(reverse);
	if (reverstr == "Yes") r = true;
	else r = false;

	double start[3] = { startpt0, startpt1, startpt2 };
	double end[3] = { endpt0, endpt1, endpt2 };
	double dir[3] = { dir0, dir1, dir2 };

	Command::GeometryCommandPy::EditMoveFeature(set, shapeHash, start, end, s, optionindex, r, length, dir);
}

void GEOMETRYCOMMANDAPI MakeMatrix(char * bodys, int optionindex, double dir10, double dir11, double dir12, int reverse1, double dis1, int count1, int showdir2, double dir20, double dir21, double dir22, int reverse2, double dis2, int count2, double basept0, double basept1, double basept2, double axis0, double axis1, double axis2, int wirereverse, int wirecount, double degree)
{
	QString cedge = QString(bodys);
	QStringList setInfos = cedge.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for (QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for (QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}
	double dir1[3]{dir10, dir11, dir12};
	double dir2[3]{dir20, dir21, dir22};
	bool r1{ false };
	if (reverse1 == 1) r1 = true;
	bool r2{ false };
	if (reverse2 == 1) r2 = true;
	bool s{ false };
	if (showdir2 == 1) s = true;
	double basepoint[3]{basept0, basept1, basept2};
	double axis[3]{axis0, axis1, axis2};
	bool wr{ false };
	if (wirereverse == 1) wr = true;
	Command::GeometryCommandPy::MakeMatrix(shapeHash, optionindex,dir1,reverse1,dis1,count1,s,dir2,reverse2,dis2,count2,basepoint,axis,wr,wirecount,degree);
	
}

void GEOMETRYCOMMANDAPI EditMatrix(int id, char * bodys, int optionindex, double dir10, double dir11, double dir12, int reverse1, double dis1, int count1, int showdir2, double dir20, double dir21, double dir22, int reverse2, double dis2, int count2, double basept0, double basept1, double basept2, double axis0, double axis1, double axis2, int wirereverse, int wirecount, double degree)
{
	Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(id);
	QString cedge = QString(bodys);
	QStringList setInfos = cedge.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for (QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for (QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}
	double dir1[3]{dir10, dir11, dir12};
	double dir2[3]{dir20, dir21, dir22};
	bool r1{ false };
	if (reverse1 == 1) r1 = true;
	bool r2{ false };
	if (reverse2 == 1) r2 = true;
	bool s{ false };
	if (showdir2 == 1) s = true;
	double basepoint[3]{basept0, basept1, basept2};
	double axis[3]{axis0, axis1, axis2};
	bool wr{ false };
	if (wirereverse == 1) wr = true;
	Command::GeometryCommandPy::EditMatrix(set, shapeHash, optionindex, dir1, reverse1, dis1,count1, s, dir2, reverse2,dis2, count2, basepoint, axis, wr, wirecount, degree);
}

void GEOMETRYCOMMANDAPI CreateExtrusion(int id, char* name,char *edges, double dis, double pt0, double pt1, double pt2, char* reverse, char* solid)
{
	QString sname = QString(name);
	QString cedge = QString(edges); 
	QStringList setInfos = cedge.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for(QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for(QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}
	double dir[3]{pt0, pt1, pt2};
	QString re = QString(reverse);
	QString so = QString(solid);
	bool r,s;
	if (re == "Yes")r = true;
	else r = false;
	if (so == "Yes") s = true;
	else s = false;

	Command::GeometryCommandPy::CreateExtrusion(id,sname,shapeHash,dis,dir,r,s);
}

void GEOMETRYCOMMANDAPI CreateRevol(int id, char* name, char *edges, double basept0, double basept1, double basept2, double degree, char* optionindex, int axissetid, int edgeindex, double coor0, double coor1, double coor2, char* reverse, char* solid)
{
	QString sname = QString(name);
	QString cedge = QString(edges);
	QStringList setInfos = cedge.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for(QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for(QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}
	double basept[3]{basept0, basept1, basept2};
	double coor[3]{coor0, coor1, coor2};
	QString optionindexstr = QString(optionindex);
	int optionid{};
	if (optionindexstr == "Select On Geometry")optionid = 0;
	else optionid = 1;

	QString re = QString(reverse);
	QString so = QString(solid);
	bool r, s;
	if (re == "Yes")r = true;
	else r = false;
	if (so == "Yes") s = true;
	else s = false;
	QPair<Geometry::GeometrySet*, int> axisetedge{};
	axisetedge.first = data->getGeometrySetByID(axissetid);
	axisetedge.second = edgeindex;

	Command::GeometryCommandPy::CreateRevol(id, sname, shapeHash, basept, degree, optionid,axisetedge, coor, r, s);
}

void GEOMETRYCOMMANDAPI CreateLoft(int id, char* name, char* solid, char* sec)
{
	QString na(name);
	QString sl(solid);
	QString se(sec);
	bool s{};
	if (sl == "Yes") s = true;
	else s = false;
	QList< QMultiHash<Geometry::GeometrySet*, int>> shapelist;
	QStringList selist = se.simplified().split(" ");

	for(QString var : selist)
	{
		QMultiHash<Geometry::GeometrySet*, int> temphash{};
		var.replace(";", " ");
		QStringList varlist = var.simplified().split(" ");
		for(QString var : varlist)
		{
		
			var.replace(":", " ");
			QStringList hash = var.simplified().split(" ");
			QStringList indexlist = hash[1].simplified().split(",");
			for(QString var : indexlist)
			{
				Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
				Geometry::GeometrySet* geoset = data->getGeometrySetByID(hash[0].toInt());
				temphash.insert(geoset, var.toInt());
			}
		     
		}
		shapelist.push_back(temphash);
	}
	Command::GeometryCommandPy::CreateLoft(id, na, s, shapelist);
}

void GEOMETRYCOMMANDAPI CreateSweep(int id, char*edges, char* solid, int pathset, int pathedge)
{

	bool s{};
	QString solidstr = QString(solid);
	if (solidstr == "Yes") s = true;
	else s = false;
	QString cedge = QString(edges);
	QStringList setInfos = cedge.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for(QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for(QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}
	auto setpath = data->getGeometrySetByID(pathset);
	QPair<Geometry::GeometrySet*, int> pahtHash(setpath, pathedge);

	Command::GeometryCommandPy::CreateSweep(id, shapeHash, pahtHash, s);
	
}

void GEOMETRYCOMMANDAPI MakeGeoSplitter(char* bodystr, char* method, int faceid, int facebody, char* planemethod, double random0, double random1, double random2, double base0, double base1, double base2)
{

	QString cedge = QString(bodystr);
	QStringList setInfos = cedge.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for (QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for (QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		} 
	}

	int typeindex{};
	QString methodstr = QString(method);
	if (methodstr == "SelectPlaneOnGeo") typeindex = 0;
	else if (methodstr == "Coordinate") typeindex = 1;
	else if (methodstr == "Random")typeindex = 2;

	int planeindex{};
	QString planemethodstr = QString(planemethod);
	if (planemethodstr == "XOY") planeindex = 0;
	else if (planemethodstr == "XOZ") planeindex = 1;
	else if (planemethodstr == "YOZ")planeindex = 2;

	Geometry::GeometrySet* faceset = data->getGeometrySetByID(facebody);
	double random[3] = { random0, random1, random2 };
	double basept[3] = { base0, base1, base2 };
	Command::GeometryCommandPy::MakeGeoSplitter(shapeHash, typeindex, faceid, faceset, planeindex, random, basept);

}

void GEOMETRYCOMMANDAPI EditGeoSplitter(int editid, char* bodystr, char* method, int faceid, int facebody, char* planemethod, double random0, double random1, double random2, double base0, double base1, double base2)
{

	QString cedge = QString(bodystr);
	QStringList setInfos = cedge.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for (QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for (QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}
	int typeindex{};
	QString methodstr = QString(method);
	if (methodstr == "SelectPlaneOnGeo") typeindex = 0;
	else if (methodstr == "Coordinate") typeindex = 1;
	else if (methodstr == "Random")typeindex = 2;

	int planeindex{};
	QString planemethodstr = QString(planemethod);
	if (planemethodstr == "XOY") planeindex = 0;
	else if (planemethodstr == "XOZ") planeindex = 1;
	else if (planemethodstr == "YOZ")planeindex = 2;
	Geometry::GeometrySet* editset = data->getGeometrySetByID(editid);
	Geometry::GeometrySet* faceset = data->getGeometrySetByID(facebody);
	double random[3] = { random0, random1, random2 };
	double basept[3] = { base0, base1, base2 };
	Command::GeometryCommandPy::EditGeoSplitter(editset, shapeHash, typeindex, faceid, faceset, planeindex, random, basept);

}

void GEOMETRYCOMMANDAPI MakeFillHole(char* faces, int editID)
{
	QString cface = QString(faces);

	QStringList setInfos = cface.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for (QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for (QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}

	Command::GeometryCommandPy::MakeFillHole(shapeHash, editID);
}

void GEOMETRYCOMMANDAPI MakeRemoveSurface(char* faces, int editID)
{
	QString cface = QString(faces);

	QStringList setInfos = cface.split(";");
	QMultiHash<Geometry::GeometrySet*, int> shapeHash;
	Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
	for (QString setinfo : setInfos)
	{
		QStringList setin = setinfo.split(":");
		int setid = setin.at(0).toInt();
		auto set = data->getGeometrySetByID(setid);
		if (set == nullptr) continue;
		QStringList edges = setin.at(1).split(",");
		for (QString e : edges)
		{
			int index = e.toInt();
			shapeHash.insert(set, index);
		}
	}

	Command::GeometryCommandPy::MakeRemoveSurface(shapeHash, editID);
}

void GEOMETRYCOMMANDAPI CreateFillGap(char* type, int set1, int body1, int set2, int body2)
{
	QString str = QString(type);
	int indextype{};
	if (str == "Edge") indextype = 0;
	else if (str == "Surface") indextype = 1;
	else if (str == "Solid") indextype = 2;

	Command::GeometryCommandPy::MakeFillGap(indextype, set1, body1, set2, body2);
}

void GEOMETRYCOMMANDAPI EditFillGap(int id, char*type, int set1, int body1, int set2, int body2)
{
	Geometry::GeometrySet* set = Geometry::GeometryData::getInstance()->getGeometrySetByID(id);
	QString str = QString(type);
	int indextype{};
	if (str == "Edge") indextype = 0;
	else if (str == "Surface") indextype = 1;
	else if (str == "Solid") indextype = 2;
	Command::GeometryCommandPy::EditFillGap(set, indextype, set1, body1, set2, body2);
}

