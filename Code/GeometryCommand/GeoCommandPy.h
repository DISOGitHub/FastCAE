#ifndef GEOCOMMANDPY_H_
#define GEOCOMMANDPY_H_

#include "geometryCommandAPI.h"
#include "GeoCommandBool.h"
#include <QString>
#include <QMultiHash>
#include <QMap>

namespace GUI
{
    class MainWindow;
}
namespace MainWidget
{
    class PreWindow;
}
namespace Geometry
{
    class GeometrySet;
}


namespace Command
{
    class GEOMETRYCOMMANDAPI GeometryCommandPy
    {
    public:
        GeometryCommandPy() = default;
        ~GeometryCommandPy() = default;

        static void init(GUI::MainWindow* m, MainWidget::PreWindow* p);
        static void createBox(QString name, double* loc, double* dir);
        static void editBox(Geometry::GeometrySet* set, double* loc, double* dir);
        static void createCylinder(QString name,double*corner,double*dir,double r,double l);
        static void editCylinder(Geometry::GeometrySet* set, double*corner, double*dir, double r, double l);
        static void createCone(QString name, double*corner, double*dir, double r,double r2, double l);
        static void editCone(Geometry::GeometrySet* set, double*corner, double*dir, double r,double r2, double l);
        static void createSphere(QString name, double* loc, double r);
        static void editSphere(Geometry::GeometrySet* set, double* loc, double r);
        static void createPoint(QString name, double* corner, double* para);
        static void editPoint(Geometry::GeometrySet* set, double* corner, double* para);
        static void createLine(QString name, double* startpoint,int index,double* coor,double len,double* dir,bool sreverse);
        static void editLine(Geometry::GeometrySet* set, double* startpoint, int index, double* coor, double len, double* dir, bool sreverse);
        static void createFace(QString name, int editid, QMultiHash<Geometry::GeometrySet*, int> hash);
		static void createChamfer(int editid, QMultiHash<Geometry::GeometrySet*, int> hash,double d1, double d2, int s);
        static void editChamfer(Geometry::GeometrySet* set, QMultiHash<Geometry::GeometrySet*, int> hash, double d1, double d2, bool s);
        static void createFillet(QMultiHash<Geometry::GeometrySet*, int> edges, double radius, int editID);
		static void CreateVariableFillet(QMap<double, double> rad, double b, int editid,int setid, int edginex);
		
		static void CreateBooLOperation(BoolType t, int set1, int body1, int set2, int body2);
        static void EditBooLOperation(Geometry::GeometrySet* set, BoolType t, int set1,int body1,int set2, int body2);
		static void CreateMirrorFeature(QMultiHash<Geometry::GeometrySet*, int> bodyhash, bool save, int typeindex, int faceindex, Geometry::GeometrySet* facebody, int planeindex, double * random, double* base);
		static void EditMirrorFeature(Geometry::GeometrySet* set, QMultiHash<Geometry::GeometrySet*, int> bodyhash, bool save, int typeindex, int faceindex, Geometry::GeometrySet* facebody, int planeindex, double * random, double* base);
		static void RotateFeature(QMultiHash<Geometry::GeometrySet*, int> bodyhash, double* basicpt, int method, Geometry::GeometrySet* edgeBody, int index, double* axis, int reverse, double angle, int saveOri);
		static void EditRotateFeature(Geometry::GeometrySet* editset, QMultiHash<Geometry::GeometrySet*, int> bodyHash,double* basicpt, int method, Geometry::GeometrySet* edgeBody, int index, double* axis, int reverse, double angle, int saveOri);

		static void CreateMoveFeature(QMultiHash<Geometry::GeometrySet*, int> bodyhash, double* start, double* end, bool s, int typeindex, bool r, double l, double* dir);
		static void EditMoveFeature(Geometry::GeometrySet* set, QMultiHash<Geometry::GeometrySet*, int> bodyhash, double* start, double* end, bool s, int typeindex, bool r, double l, double* dir);
		
		static void MakeMatrix(QMultiHash<Geometry::GeometrySet*, int> bodyhash, int optionindex, double* dir1, bool reverse1, double dis1, int count1, bool showdir2, double*dir2, bool reverse2, double dis2, int count2,
									double* basept,double*axis,bool wirereverse,int wirecount,double degree );
		static void EditMatrix(Geometry::GeometrySet* set, QMultiHash<Geometry::GeometrySet*, int> bodyhash, int optionindex, double* dir1, bool reverse1, double dis1, int count1, bool showdir2, double*dir2, bool reverse2, double dis2,
									 int count2, double* basept, double*axis, bool wirereverse, int wirecount, double degree);
		static void CreateExtrusion(int editid,QString n, QMultiHash<Geometry::GeometrySet*, int> hash, double dis, double* dir, bool r, bool s);
		static void CreateRevol(int editid, QString name, QMultiHash<Geometry::GeometrySet*, int> hash, double* basept, double degree, int optionindex, QPair<Geometry::GeometrySet*, int> axisetedge, double* coor, bool reverse, bool solid);
		static void CreateLoft(int id, QString name, bool solid, QList< QMultiHash<Geometry::GeometrySet*, int>> shapelist);
		static void CreateSweep(int id, QMultiHash<Geometry::GeometrySet*, int> hash, QPair<Geometry::GeometrySet*, int> path, bool solid);
		static void MakeGeoSplitter(QMultiHash<Geometry::GeometrySet*, int> bodyhash, int typeindex, int faceindex, Geometry::GeometrySet* facebody, int planeindex, double * random, double* base);
		static void EditGeoSplitter(Geometry::GeometrySet* editset, QMultiHash<Geometry::GeometrySet*, int> bodyhash, int typeindex, int faceindex, Geometry::GeometrySet* facebody, int planeindex, double * random, double* base);
		static void MakeFillHole(QMultiHash<Geometry::GeometrySet*, int> faces,  int editID);
		static void MakeRemoveSurface(QMultiHash<Geometry::GeometrySet*, int> faces, int editID);
		static void MakeFillGap(int type, int set1, int body1, int set2, int body2);
		static void EditFillGap(Geometry::GeometrySet* set, int type, int set1, int body1, int set2, int body2);

    private:
        static void warning();

    private:
        static GUI::MainWindow* _mainWindow;
        static MainWidget::PreWindow* _preWindow;
    };
}

//声明为C接口，供Python脚本调用
extern "C"
{
	void GEOMETRYCOMMANDAPI CreateBox(char* name, double x, double y, double z, double l, double w, double h);
	void GEOMETRYCOMMANDAPI EditBox(int id, double x, double y, double z, double l, double w, double h);
	void GEOMETRYCOMMANDAPI CreateCylinder(char* name, double x, double y, double z, double l, double w, double h, double radius, double length);
	void GEOMETRYCOMMANDAPI EditCylinder(int id, double x, double y, double z, double l, double w, double h, double radius, double length);
	void GEOMETRYCOMMANDAPI CreateCone(char* name, double x, double y, double z, double l, double w, double h, double radius, double radius2, double length);
	void GEOMETRYCOMMANDAPI EditCone(int id, double x, double y, double z, double l, double w, double h, double radius, double radius2, double length);
	void GEOMETRYCOMMANDAPI CreateSphere(char* name, double x, double y, double z, double r);
	void GEOMETRYCOMMANDAPI EditSphere(int id, double x, double y, double z, double r);
	void GEOMETRYCOMMANDAPI CreatePoint(char* name, double x, double y, double z, double p1, double p2, double p3);
	void GEOMETRYCOMMANDAPI EditPoint(int id, double x, double y, double z, double p1, double p2, double p3);
	void GEOMETRYCOMMANDAPI CreateLine(char* name, double startpoint0, double startpoint1, double startpoint2,
										int method, double coor0, double coor1, double coor2, double len, double dir0, double dir1, double dir2, int reverse);
	void GEOMETRYCOMMANDAPI EditLine(int id, double startpoint0, double startpoint1, double startpoint2, 
										int method, double coor0, double coor1, double coor2, double len, double dir0, double dir1, double dir2, int reverse);
	void GEOMETRYCOMMANDAPI CreateFace(char* edges,char* name, int editId);
	void GEOMETRYCOMMANDAPI CreateChamfer(char* edges, int editId,double d1,double d2,int typeindex);
	void GEOMETRYCOMMANDAPI EditChamfer(int id, char*setidStr, char* indexListStr, double d1, double d2, int sym);
	void GEOMETRYCOMMANDAPI CreateFillet(char* edges, double rad, int editID);
	void GEOMETRYCOMMANDAPI CreateVariableFillet(char*edges, double basicrad, int editId, int setid, int edgeindex);
	void GEOMETRYCOMMANDAPI CreateBooLOperation(char* booltype, int set1, int body1, int set2, int body2);
	void GEOMETRYCOMMANDAPI EditBooLOperation(int id, char* booltype, int set1, int body1, int set2, int body2);
	void GEOMETRYCOMMANDAPI CreateMirrorFeature(char* bodys, char* method,int faceindex, int facebody,char* planemethod, double random0, double random1, double random2, double base0, double base1, double base2,char* saveori);
	void GEOMETRYCOMMANDAPI EditMirrorFeature(int id, char* bodys, char* method, int faceindex, int facebody, char* planemethod, double random0, double random1, double random2, double base0, double base1, double base2, char* saveori);
	void GEOMETRYCOMMANDAPI RotateFeature(char* body, double basicx, double basicy, double basicz, //体 & 基准点
										   int method, int edgeBoby, int edgeIndex, double axisx, double axisy, double axisz, int reverse, //轴线 
											double angle, int saveOri);
	void GEOMETRYCOMMANDAPI EditRotateFeature(int bodyid, char* body, double basicx, double basicy, double basicz, //体 & 基准点
											   int method, int edgeBoby, int edgeIndex, double axisx, double axisy, double axisz, int reverse, //轴线 
												double angle, int saveOri);
	void GEOMETRYCOMMANDAPI CreateMoveFeature(char * bodys, char* method,double startpt0, double startpt1, double startpt2, double endpt0, double endpt1, double endpt2, char* save,  char*reverse,
												double length, double dir0, double dir1, double dir2);
	void GEOMETRYCOMMANDAPI EditMoveFeature(int id, char * bodys, char* method, double startpt0, double startpt1, double startpt2, double endpt0, double endpt1, double endpt2, char* save, char* reverse,
												double length, double dir0, double dir1, double dir2);
	void GEOMETRYCOMMANDAPI MakeMatrix(char * bodys, int optionindex, double dir10, double dir11, double dir12, int reverse1, double dis1, int count1, int showdir2,
										double dir20, double dir21, double dir22, int reverse2, double dis2, int count2, double basept0, double basept1, double basept2, double axis0, double axis1, double axis2,
										int wirereverse, int wirecount, double degree);
	void GEOMETRYCOMMANDAPI EditMatrix(int id, char * bodys, int optionindex, double dir10, double dir11, double dir12, int reverse1, double dis1, int count1, int showdir2,
										double dir20, double dir21, double dir22, int reverse2, double dis2, int count2, double basept0, double basept1, double basept2, double axis0, double axis1, double axis2,
										int wirereverse, int wirecount, double degree);
	void GEOMETRYCOMMANDAPI CreateExtrusion(int id,char* name,char *edges,double dis,double pt0,double pt1,double pt2,char* reverse,char* solid);
	void GEOMETRYCOMMANDAPI CreateRevol(int id, char* name, char *edges, double basept0, double basept1, double basept2, double degree, char* optionindex, int axissetid, int edgeindex, double coor0, double coor1, double coor2, char* reverse, char* solid);
	void GEOMETRYCOMMANDAPI CreateLoft(int id, char* name, char* solid, char* sec);
	void GEOMETRYCOMMANDAPI CreateSweep(int id, char*edges, char* solid, int pathset,int pathedge);
	void GEOMETRYCOMMANDAPI MakeGeoSplitter(char* bodystr, char* method, int facebody, int faceid, char* planemethod, double random0, double random1, double random2, double base0, double base1, double base2);
	void GEOMETRYCOMMANDAPI EditGeoSplitter(int editid, char* bodystr, char* method, int facebody, int faceid, char* planemethod, double random0, double random1, double random2, double base0, double base1, double base2);
	void GEOMETRYCOMMANDAPI MakeFillHole(char* faces, int editID);
	void GEOMETRYCOMMANDAPI MakeRemoveSurface(char* faces, int editID);
	void GEOMETRYCOMMANDAPI CreateFillGap(char* type, int set1, int body1, int set2, int body2);
	void GEOMETRYCOMMANDAPI EditFillGap(int id, char*type, int set1, int body1, int set2, int body2);
}
#endif