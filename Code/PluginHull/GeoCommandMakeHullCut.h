#ifndef GEOCOMMANDCREATEBOXHULLCUT_H_
#define GEOCOMMANDCREATEBOXHULLCUT_H_

#include "GeometryCommand/GeoCommandBase.h"
#include <QString>
#include "HullPluginAPI.h"
namespace PluginShip
{
	class ModelPoint;
}
class gp_Pnt;
class  TopoDS_Edge;
class TopoDS_Shape;
namespace Command
{
	class HULLPLUGINAPI GeoCommandCreateHullCut : public  QObject
	{
	public:
		GeoCommandCreateHullCut();
		~GeoCommandCreateHullCut() = default;
		
		void setGeoIndex(int geoindex);
		void setAxisType(QString s); 		
		void setCutLocation(double loc);
		void setNums(int n);
		bool execute(QList<double*>& outputPntList);

		TopoDS_Shape* getModelShape();
		TopoDS_Shape* getPlnShaoe(double coorX);
		void getCutShapeList(TopoDS_Shape*modelShape, TopoDS_Shape*plnShape, std::vector<PluginShip::ModelPoint*>& modelPntList, double&lengthsWire);
		void addIndex(std::vector<PluginShip::ModelPoint*>&modelPntList, QMap<int, PluginShip::ModelPoint*>&pntMap);
		void getJyqList(std::vector<PluginShip::ModelPoint*>&modelPntList, QVector<PluginShip::ModelPoint*>& jyqList);
		void addSecIndex(std::vector<PluginShip::ModelPoint*>&modelPntList, QMap<int, PluginShip::ModelPoint*>&pntMap);
		bool judeAveCur(std::list<PluginShip::ModelPoint*>& modelPntList);
		void getSavedPt(std::vector<PluginShip::ModelPoint*>& modelPntList, QVector<PluginShip::ModelPoint*>& result);
		void deleSamePt(std::vector<PluginShip::ModelPoint*>&modelPntList);
		void CalCurvature(std::vector<PluginShip::ModelPoint*>&modelPntList);
		void sliptForParts(QVector<QVector<PluginShip::ModelPoint*>>&splitvector, std::vector<PluginShip::ModelPoint*>&modelPntList, double lengthsWire);
		bool interPoints(QVector<QVector<PluginShip::ModelPoint*>>& splitvector, QMap<int, PluginShip::ModelPoint*> calMap, std::vector<PluginShip::ModelPoint*>& temptvector, int nums);
		void setMirrorPt(QVector<PluginShip::ModelPoint*>& result);
		//void sortResult(QVector<PluginShip::ModelPoint*>&result);
		void getResult(QVector<PluginShip::ModelPoint*>&result, std::vector<PluginShip::ModelPoint*> temptvector, QMap<int, PluginShip::ModelPoint*>&pntMap);
		void sortResultAgain(QVector<PluginShip::ModelPoint*>&result, QVector<PluginShip::ModelPoint*> jyqList);
		bool sortEdge(gp_Pnt* wireStartPnt, std::list<TopoDS_Edge*> &edgelist, std::list<TopoDS_Edge*> &sortedEdge);
	private:
		int _geoindex{};
		QString _Axistype{"X_Axis"};
		double _loc{};
		int _nums{};

		//
		double _coorX{};
		QList<double*> _outputList{};

		double _begin{};
		double _div{};
		int _numzhan{};
		double _minx{};
		double _maxx{};
		double _boatlength{};
		double _minZ{};
		double _heigh{};
		int _panel{};

		PluginShip::ModelPoint* _seapt;


	};


}


#endif 