#include "GeoCommandMakeHullCut.h"
#include "geometry/geometrySet.h"
#include "geometry/geometryData.h"
#include <TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS.hxx>
#include <string>
#include <list>
#include <map>
#include <IGESControl_Reader.hxx>
#include <gp_Pln.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepAlgo_Section.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <TopoDS_Edge.hxx>
#include <BRep_Tool.hxx>
#include <TopExp_Explorer.hxx>
#include <GeomLProp.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepLProp_CLProps.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <GeomConvert.hxx>
#include <ShapeFix_Wire.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <GCPnts_UniformAbscissa.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <vector>
#include <BRep_Tool.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <hash_map>
#include <BRepBndLib.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepTools.hxx>
#include <stdlib.h>
#include <QList>
#include <QMap>
#include <iomanip>
#include <QVector>
#include <TopExp.hxx>
#include <QDebug>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <IGESControl_Controller.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <IntAna_IntConicQuad.hxx>
#include <BRepIntCurveSurface_Inter.hxx>
#include "ModelPoint.h"
#include <BRep_Tool.hxx>
#include "cleaner.hpp"

namespace Command
{
	GeoCommandCreateHullCut::GeoCommandCreateHullCut()
	{
	}


	void GeoCommandCreateHullCut::setGeoIndex(int geoindex)
	{
		_geoindex = geoindex;
	}

	void GeoCommandCreateHullCut::setAxisType(QString s)
	{
		_Axistype = s;
	}

	void GeoCommandCreateHullCut::setCutLocation(double loc)
	{
		_loc = loc;
	}

	void GeoCommandCreateHullCut::setNums(int n)
	{
		_nums = n;
	}

	bool GeoCommandCreateHullCut::execute(QList<double*>& outputPntList)
	{

		//最后输出的vector
		QVector<PluginShip::ModelPoint*> result;
		//弧长
		double lengthsWire{};
		//list<double*>outputPntList{};
		//根据距离，添加点到samplelist，并且添加它在
		std::vector<PluginShip::ModelPoint*> sampPntList;
		TopoDS_Shape* plnShape = getPlnShaoe(_loc);////////////////////////////
		TopoDS_Shape* modelShape = getModelShape();
		//TopoDS_Shape* modelShape = getModelShape();
		if (NULL == plnShape || NULL == modelShape) return false;
		std::vector<PluginShip::ModelPoint*> modelPntList;
		QMap<int, PluginShip::ModelPoint*> pntMap{};

		//排序N*100个点。
		getCutShapeList(modelShape, plnShape, modelPntList, lengthsWire);
		if (modelPntList.size() < 1) return false;
		if (lengthsWire < 0) return false;
		//所有的点集添加索引，
		addIndex(modelPntList, pntMap);

		//获取jianyaoqi的点集。
		QVector<PluginShip::ModelPoint*> jyqList{};
		getJyqList(modelPntList, jyqList);
		if (modelPntList.size() < 1) return false;
		//添加jyq到result，如果点为不连续，则此点的前后都设为save==true。	
		for (QVector<PluginShip::ModelPoint*> ::iterator it = jyqList.begin(); it != jyqList.end(); ++it)
		{
			result.push_back(*it);
		}
		/*//选出不连续的线的端点。
		for (std::vector<PluginShip::ModelPoint*>::iterator it = modelPntList.begin(); it != modelPntList.end(); ++it)
		{
			if (((*it)->getFinStabilizer() == true) && it != modelPntList.begin() && it != modelPntList.end())
			{
				std::vector<PluginShip::ModelPoint*>::iterator itpre = it - 1;
				if (itpre == modelPntList.begin() || itpre == modelPntList.end()) continue;
				(*itpre)->setSave(true);
				std::vector<PluginShip::ModelPoint*>::iterator itlast = it + 1;
				if (itlast == modelPntList.begin() || itlast == modelPntList.end()) continue;
				(*itlast)->setSave(true);

			}
		}

		//获取save==true&&isf==true的点==>不连续的线段的相连的两个点。	
		getSavedPt(modelPntList, result);*/
		if (modelPntList.size()<1) return false;
		//删除相同的点。
		deleSamePt(modelPntList);
		if (modelPntList.size()<1) return false;
		//添加计算第二个map索引，剩下的点集添加索引。
		QMap<int, PluginShip::ModelPoint*> calMap{};
		addSecIndex(modelPntList, calMap);

		//计算曲率
		CalCurvature(modelPntList);

		//分成四段
		QVector<QVector<PluginShip::ModelPoint*>> splitvector{};
		sliptForParts(splitvector, modelPntList, lengthsWire);

		//根据距离分成四段，得到五个点。
		std::vector<PluginShip::ModelPoint*>temptvector;
		temptvector.push_back(splitvector[0].front());
		for (QVector<QVector<PluginShip::ModelPoint*>>::iterator it = splitvector.begin(); it != splitvector.end(); ++it)
		{
			temptvector.push_back((*it).back());
		}

		int nums = (_nums / 2 - result.size());
		if (nums < 0)
		{
			for (QVector<PluginShip::ModelPoint*>::iterator iterPnt = result.begin(); iterPnt != result.end(); ++iterPnt)
			{
				double* onePoint = new double[3];
				onePoint[0] = ((*iterPnt)->getPoint()).X();
				onePoint[1] = ((*iterPnt)->getPoint()).Y();
				onePoint[2] = ((*iterPnt)->getPoint()).Z();
				outputPntList.push_back(onePoint);
			}
			return true;
		}
		//计算四段的方差。
		bool ok = interPoints(splitvector, calMap, temptvector, nums);
		if (!ok) return false;

		//获取总的结果。
		getResult(result, temptvector, pntMap);
		sortResultAgain(result,jyqList);
		if (result.size()<1) return false;

		setMirrorPt(result);
		if (ok)
		{
			for (QVector<PluginShip::ModelPoint*>::iterator iterPnt = result.begin(); iterPnt != result.end(); ++iterPnt)
			{
				double* onePoint = new double[3];
				onePoint[0] = ((*iterPnt)->getPoint()).X();
				onePoint[1] = ((*iterPnt)->getPoint()).Y();
				onePoint[2] = ((*iterPnt)->getPoint()).Z();
				outputPntList.push_back(onePoint);
			}
		}

		if (outputPntList.size() < 1) return false;
		else return true;
		
	}

	TopoDS_Shape* GeoCommandCreateHullCut::getPlnShaoe(double coorX)
	{
		gp_Pnt plnpnt{};
		gp_Dir plndir{};

		if (_Axistype == "X_Axis")
		{
			plnpnt = gp_Pnt(coorX, 0, 0);
			plndir = gp_Dir(1, 0, 0);
		}
		else if (_Axistype == "Y_Axis")
		{
			plnpnt = gp_Pnt(0, coorX, 0);
			plndir = gp_Dir(0, 1, 0);
		}
		else if (_Axistype == "Z_Axis")
		{
			plnpnt = gp_Pnt( 0, 0, coorX);
			plndir = gp_Dir(0, 0, 1);
		}
		gp_Pln pln(plnpnt, plndir);
		TopoDS_Shape* planeShape = new TopoDS_Shape;
		*planeShape = BRepBuilderAPI_MakeFace(pln);
		if ((*planeShape).IsNull()) return nullptr;
		return planeShape;
	}

	
	void GeoCommandCreateHullCut::getCutShapeList(TopoDS_Shape*modelShape, TopoDS_Shape*plnShape, std::vector<PluginShip::ModelPoint*>& modelPntList, double&lengthsWire)
	{
		//double lengthsWire{ -1 };
		BRepAlgo_Section S(*modelShape, *plnShape, false);
		S.Build();
		if (!S.IsDone()) return;
	    const TopoDS_Shape& secShanpe = S.Shape();

		TopoDS_Shape tempshape = secShanpe;
		cleanEdge(tempshape);

		if (tempshape.IsNull()) return;
		//basicpt
		Bnd_Box cutwireBox;
		BRepBndLib::Add(tempshape, cutwireBox);
		gp_Pnt wiremax = cutwireBox.CornerMax();
		gp_Pnt wiremin = cutwireBox.CornerMin();
		//double dis = 1e66;
		gp_Pnt wireStartPnt{ wiremin.X(), wiremin.Y(), wiremax.Z() };

		//排序线和点。
		TopTools_IndexedMapOfShape edges;
		TopExp::MapShapes(tempshape, TopAbs_EDGE, edges);

		const int n = edges.Extent();
		std::list<TopoDS_Edge*> edgelist;
		std::list<TopoDS_Edge*> sortedEdge;
		for (int i = 1; i <= n; ++i)
		{
			TopoDS_Shape edge = edges.FindKey(i);
			TopoDS_Edge e = TopoDS::Edge(edge);
			TopoDS_Edge* one = new TopoDS_Edge;
			*one = e;
			edgelist.push_back(one);
		}
		int k = edgelist.size();

	/*	QList<TopoDS_Edge*> newedgelist{};
		for (std::list<TopoDS_Edge*>::iterator it = edgelist.begin(); it != edgelist.end();it++)
		{
			newedgelist.push_back(*it);
		}

		for (int i = 0; i < newedgelist.size()-1; i++)
		{
			Standard_Real first1, end1;
			Handle_Geom_Curve currencurve1 = BRep_Tool::Curve(*newedgelist[i], first1, end1);
			for (int j = i+1; j <newedgelist.size() ; j++)
			{
				Standard_Real first2,end2;
				
				Handle_Geom_Curve currencurve2 = BRep_Tool::Curve(*newedgelist[j], first2, end2);
				//currencurve2->
				gp_Pnt tpnt1{}, tpnt2{};
				gp_Vec vpnt1{}, vpnt2{};
				Standard_Real parami1{}, parami2{};
				//curve->D0(parami, tpnt);
				currencurve1->D1(parami1, tpnt1, vpnt1);
				currencurve2->D1(parami2, tpnt2, vpnt2);
				if ((tpnt1.X()==tpnt2.X()&&tpnt1.Y()==tpnt2.Y()&&tpnt1.Z()==tpnt2.Z())&&(vpnt1.X()==vpnt2.X()&&vpnt1.Y()==vpnt2.Y()&&vpnt1.Z()==vpnt2.Z()))
				{
					qDebug() << "same!" << endl;
				}
				
			}
		
		}*/

		bool ok = sortEdge(&wireStartPnt, edgelist, sortedEdge);
		//画出切出来的线。


		
	
		/*for (std::list<TopoDS_Edge*>::iterator pE = sortedEdge.begin(); pE != sortedEdge.end(); ++pE)
		{
		TopoDS_Edge* edge = *pE;
		std::string filestr = "../../../IGS/edgeone.brep";
		const char* ch = filestr.c_str();
		TopoDS_Wire r = BRepBuilderAPI_MakeWire(*edge);
		BRepTools::Write(r, ch);
		}
*/

		int z = sortedEdge.size();
		if (sortedEdge.size() < 1) return;
		if (ok)
		{
			for (std::list<TopoDS_Edge*>::iterator pE = sortedEdge.begin(); pE != sortedEdge.end(); ++pE)
			{
				Standard_Real first, last;
				Handle(Geom_Curve) curve = BRep_Tool::Curve(**pE, first, last);
				Handle(Geom_TrimmedCurve) myTrimmed = new Geom_TrimmedCurve(curve, first, last);
				Handle(Geom_BSplineCurve) NurbsCurve = GeomConvert::CurveToBSplineCurve(myTrimmed);

				GeomAdaptor_Curve GAC(NurbsCurve);
				GCPnts_UniformAbscissa UA(GAC, 200);
				if (!UA.IsDone()) continue;
				Standard_Real n = UA.NbPoints();
				std::list<PluginShip::ModelPoint*> EdgePtList;
				for (int i = 0; i < n; ++i)
				{
					Standard_Real parami = UA.Parameter(i + 1);
					gp_Pnt tpnt;
					gp_Vec vpnt;
					//curve->D0(parami, tpnt);
					curve->D1(parami, tpnt, vpnt);
					double temp[3]{};
					temp[0] = tpnt.X(); temp[1] = tpnt.Y(); temp[2] = tpnt.Z();
					PluginShip::ModelPoint* tempPnt = new PluginShip::ModelPoint;
					tempPnt->setPoint(tpnt);
					tempPnt->setVec(vpnt);
					if ((*pE)->Checked() == true)
					{
						tempPnt->setVec(vpnt);
						EdgePtList.push_back(tempPnt);
					}
					else
					{
						tempPnt->setVec(-vpnt);
						EdgePtList.push_front(tempPnt);
					}
				}
				//判断100个点的平均曲率是否<1e-7,如果是，那么不加入点集里。
				EdgePtList.front()->setSave(true);
				EdgePtList.back()->setSave(true);
				if (modelPntList.size() > 0)
				{
					double d = EdgePtList.front()->getPoint().Distance(modelPntList.back()->getPoint());

					if (d>1e-3)
					{
						EdgePtList.front()->isFinStabilizer(true);
						modelPntList.back()->isFinStabilizer(true);
					}
				}
				bool add = judeAveCur(EdgePtList);
				if (add)
				{
					Standard_Real aCurveLen = GCPnts_AbscissaPoint::Length(GeomAdaptor_Curve(curve));
					lengthsWire = lengthsWire + aCurveLen;
				}

				for (std::list<PluginShip::ModelPoint*>::iterator it = EdgePtList.begin(); it != EdgePtList.end(); ++it)
				{
					modelPntList.push_back(*it);
				}
			}
		}

	}

	void GeoCommandCreateHullCut::addIndex(std::vector<PluginShip::ModelPoint*>&modelPntList, QMap<int, PluginShip::ModelPoint*>&pntMap)
	{
		//添加索引map
		int index = 0;
		for (std::vector<PluginShip::ModelPoint*>::iterator iterstart = modelPntList.begin(); iterstart != modelPntList.end(); ++iterstart)
		{
			(*iterstart)->setIndex(index);
			pntMap.insert(index, *iterstart);
			index++;

		}
	}

	void GeoCommandCreateHullCut::getJyqList(std::vector<PluginShip::ModelPoint*>&modelPntList, QVector<PluginShip::ModelPoint*>& jyqList)
	{
		for (std::vector<PluginShip::ModelPoint*>::iterator it = modelPntList.begin(); it != modelPntList.end();)
		{
			if ((*it)->getIsJyq() == true)
			{

				jyqList.push_back(*it);
				it = modelPntList.erase(it);
			}
			else
			{
				++it;
			}
		}
		PluginShip::ModelPoint* pt0 = new PluginShip::ModelPoint;

		for (QVector<PluginShip::ModelPoint*>::iterator iterPnt = jyqList.begin(); iterPnt != jyqList.end();)
		{
			if (iterPnt == jyqList.begin())
			{
				pt0 = *iterPnt;
				++iterPnt;
			}
			PluginShip::ModelPoint* temp2 = *iterPnt;
			gp_Pnt pstart = pt0->getPoint();
			gp_Pnt ptend = temp2->getPoint();
			double distance = pstart.Distance(ptend);
			pt0 = temp2;
			if (distance < 1e-3)
			{
				iterPnt = jyqList.erase(iterPnt);
			}
			else
			{
				++iterPnt;
			}
		}
	}

	void GeoCommandCreateHullCut::addSecIndex(std::vector<PluginShip::ModelPoint*>&modelPntList, QMap<int, PluginShip::ModelPoint*>&pntMap)
	{
		//添加索引map
		int index = 0;
		for (std::vector<PluginShip::ModelPoint*>::iterator iterstart = modelPntList.begin(); iterstart != modelPntList.end(); ++iterstart)
		{
			//(*iterstart)->setIndex(index);
			pntMap.insert(index, *iterstart);
			index++;
			
		}
	}

	bool GeoCommandCreateHullCut::judeAveCur(std::list<PluginShip::ModelPoint*>& modelPntList)
	{
		bool save;
		gp_Pnt pnt0;
		gp_Vec vec0;
		for (std::list<PluginShip::ModelPoint*>::iterator iterPnt = modelPntList.begin(); iterPnt != modelPntList.end(); ++iterPnt)
		{

			if (iterPnt == modelPntList.begin())
			{
				PluginShip::ModelPoint* temp0 = *iterPnt;
				pnt0 = temp0->getPoint(); 
				vec0 = temp0->getVec();
				++iterPnt;
				PluginShip::ModelPoint* temp2 = *iterPnt;
				gp_Pnt ptend = temp2->getPoint();
				gp_Vec vecEnd = temp2->getVec();

				double angle = vec0.Angle(vecEnd);
				double distance = pnt0.Distance(ptend);
				double curvature = fabs(angle) / fabs(distance);
				temp0->setCurvature(curvature);
			}
			
			PluginShip::ModelPoint* temp = *iterPnt;
			gp_Pnt ptend = temp->getPoint();
			gp_Vec vecEnd = temp->getVec();
			double angle = vec0.Angle(vecEnd);
			double distance = pnt0.Distance(ptend);
			double curvature = fabs(angle) / fabs(distance);
			temp->setCurvature(curvature);
			pnt0 = ptend;
			vec0 = vecEnd;
		}
		double sumcur{};
		for (std::list<PluginShip::ModelPoint*>::iterator iter = modelPntList.begin(); iter != modelPntList.end(); ++iter)
		{
			sumcur = sumcur + ((*iter)->getCurvature());
		}
		double ave = sumcur / modelPntList.size();
		if (ave < 1e-7)
		{
			save = false;
			PluginShip::ModelPoint* start = modelPntList.front();
			start->setSave(true);
			start->isFinStabilizer(true);
			start->setIsJyq(true);
			PluginShip::ModelPoint* end = modelPntList.back();
			end->setSave(true);
			end->isFinStabilizer(true);
			end->setIsJyq(true);
			modelPntList.clear();
			modelPntList.push_back(start);
			modelPntList.push_back(end); 
		}
		else  save = true;
		return save;
	}

	void GeoCommandCreateHullCut::getSavedPt(std::vector<PluginShip::ModelPoint*>& modelPntList, QVector<PluginShip::ModelPoint*>& result)
	{
		int i = 1;
		for (std::vector<PluginShip::ModelPoint*>::iterator iter = modelPntList.begin(); iter != modelPntList.end();)
		{
			if (((*iter)->getSave() == true) && ((*iter)->getFinStabilizer() == true))
			{
				result.push_back(*iter);
				iter = modelPntList.erase(iter);
			}
			else ++iter;
		}
		PluginShip::ModelPoint* pt0 = new PluginShip::ModelPoint;

		for (QVector<PluginShip::ModelPoint*>::iterator iterPnt = result.begin(); iterPnt != result.end();)
		{
			if (iterPnt == result.begin())
			{
				pt0 = *iterPnt;
				++iterPnt;
			}
			PluginShip::ModelPoint* temp2 = *iterPnt;
			gp_Pnt pstart = pt0->getPoint();
			gp_Pnt ptend = temp2->getPoint();
			double distance = pstart.Distance(ptend);
			pt0 = temp2;
			if (distance < 1e-7)
			{
				iterPnt = result.erase(iterPnt);
			}
			else
			{
				++iterPnt;
			}
		}
	
	}

	void GeoCommandCreateHullCut::deleSamePt(std::vector<PluginShip::ModelPoint*>&modelPntList)
	{
		PluginShip::ModelPoint* pt0 = new PluginShip::ModelPoint;

		for (std::vector<PluginShip::ModelPoint*>::iterator iterPnt = modelPntList.begin(); iterPnt != modelPntList.end();)
		{
			if (iterPnt == modelPntList.begin())
			{
				pt0 = *iterPnt;
				++iterPnt;
			}
			PluginShip::ModelPoint* temp2 = *iterPnt;
			gp_Pnt pstart = pt0->getPoint();
			gp_Pnt ptend = temp2->getPoint();
			double distance = pstart.Distance(ptend);
			pt0 = temp2;
			if (distance < 1e-3)
			{
				iterPnt = modelPntList.erase(iterPnt);
			}
			else
			{
				++iterPnt;
			}
		}
	}

	void GeoCommandCreateHullCut::CalCurvature(std::vector<PluginShip::ModelPoint*>&modelPntList)
	{
		gp_Pnt pnt0;
		gp_Vec vec0;
		for (std::vector<PluginShip::ModelPoint*>::iterator iterPnt = modelPntList.begin(); iterPnt != modelPntList.end(); ++iterPnt)
		{

			if (iterPnt == modelPntList.begin())
			{
				PluginShip::ModelPoint* temp0 = *iterPnt;
				pnt0 = temp0->getPoint();
				vec0 = temp0->getVec();
				++iterPnt;
				PluginShip::ModelPoint* temp2 = *iterPnt;
				gp_Pnt ptend = temp2->getPoint();
				gp_Vec vecEnd = temp2->getVec();

				double angle = vec0.Angle(vecEnd);
				double distance = pnt0.Distance(ptend);
				double curvature = fabs(angle) / fabs(distance);
				temp0->setCurvature(curvature);
			}

			PluginShip::ModelPoint* temp = *iterPnt;
			gp_Pnt ptend = temp->getPoint();
			gp_Vec vecEnd = temp->getVec();

			double angle = vec0.Angle(vecEnd);
			double distance = pnt0.Distance(ptend);
			double curvature = fabs(angle) / fabs(distance);
			temp->setCurvature(curvature);
			pnt0 = ptend;
			vec0 = vecEnd;
		}

	}

	void GeoCommandCreateHullCut::sliptForParts(QVector<QVector<PluginShip::ModelPoint*>>&splitvector, std::vector<PluginShip::ModelPoint*>&modelPntList, double lengthsWire)
	{
		double minlen = lengthsWire / 4;
		QVector<PluginShip::ModelPoint*>* newone = new QVector<PluginShip::ModelPoint *>;
		QVector<PluginShip::ModelPoint*> lastone;
		int N = 1;
		double dis{};
		PluginShip::ModelPoint* temp = modelPntList.front();
		for (std::vector<PluginShip::ModelPoint*>::iterator iter = modelPntList.begin() + 1; iter != modelPntList.end(); ++iter)
		{
			if (N < 4)
			{
				if (iter == modelPntList.begin() + 1) newone->push_back(modelPntList.front());
				if ((temp->getSave() == true) && ((*iter)->getSave() == true) && (temp->getPoint().Distance((*iter)->getPoint()))>1e-3)
				{
					temp = *iter;
					iter = iter + 1;
				}
				double d = (*iter)->getPoint().Distance(temp->getPoint());
				dis = dis + d;
				if (dis < N*minlen)
				{
					newone->push_back(*iter);
				}
				else
				{
					newone->push_back(*iter);
					splitvector.push_back(*newone);
					QVector<PluginShip::ModelPoint*>* one = new QVector<PluginShip::ModelPoint *>;
					newone = one;
					N++;
				}
				temp = *iter;
			}
			else
			{
				lastone.push_back(*iter);
				if (iter == modelPntList.end() - 1)  splitvector.push_back(lastone);
			}

		}
	}

	bool GeoCommandCreateHullCut::interPoints(QVector<QVector<PluginShip::ModelPoint*>>& splitvector, QMap<int, PluginShip::ModelPoint*> calMap, std::vector<PluginShip::ModelPoint*>& temptvector, int nums)
	{

		QMap<double, QVector<PluginShip::ModelPoint*>> varToVec;
		for (int i = 0; i < splitvector.size(); i++)
		{
			double sumVar{};
			for (int j = 0; j < splitvector[i].size(); j++)
			{
				sumVar = sumVar + (splitvector[i][j])->getCurvature();
			}
			double ave = sumVar / (splitvector[i]).size();

			double  varN{};
			QVector<PluginShip::ModelPoint*> s = splitvector[i];
			for (int j = 0; j < s.size(); j++)
			{
				varN = varN + pow((((s[j])->getCurvature()) - ave), 2);

			}
			double var = varN / (splitvector[i]).size();
			varToVec.insert(var, splitvector[i]);
		}
		//求最大方差及其vector
		double maxvar{};
		for (int i = 0; i < varToVec.size(); i++)
		{
			QList<double> varlist = varToVec.keys();
			for (int i = 0; i < varlist.size(); ++i) maxvar = maxvar>varlist[i] ? maxvar : varlist[i];
		}
		QVector<PluginShip::ModelPoint*> maxVarVec = varToVec.value(maxvar);
		PluginShip::ModelPoint* ptstart = maxVarVec.front();
		int start = calMap.key(ptstart);
		PluginShip::ModelPoint* ptend = maxVarVec.back();
		int end = calMap.key(ptend);
		//寻找中间点。
		int mid = ((start + end) / 2);
		PluginShip::ModelPoint* minpt = calMap.value(mid);
		//本来是五个，再加入一个mid。
		temptvector.push_back(minpt);
		//将一分为二。
		QVector<QVector<PluginShip::ModelPoint*>> separatevec{};
		QVector<PluginShip::ModelPoint*> firstvec{};
		QVector<PluginShip::ModelPoint*> secondvec{};
		int indexmid = maxVarVec.indexOf(minpt);
		for (int i = 0; i < maxVarVec.size(); i++)
		{
			if (i < indexmid + 1) firstvec.push_back(maxVarVec[i]);
			else secondvec.push_back(maxVarVec[i]);
		}
		separatevec.push_back(firstvec);
		separatevec.push_back(secondvec);

		int maxVarVecIndex = splitvector.indexOf(maxVarVec);
		splitvector.replace(maxVarVecIndex, firstvec);
		splitvector.insert(maxVarVecIndex + 1, secondvec);

		if (temptvector.size() < nums) interPoints(splitvector, calMap, temptvector, nums);
		else return true;
	}

	void GeoCommandCreateHullCut::setMirrorPt(QVector<PluginShip::ModelPoint*>& result)
	{
		gp_Pnt p(0, 0, 0);
		gp_Dir dir(0, 1, 0);
		gp_Ax2 plane(p, dir);
		std::list<PluginShip::ModelPoint*> temp;
		for (PluginShip::ModelPoint* var : result)
		{
			if (fabs(var->getPoint().Y()) < 1e-7) continue;
			TopoDS_Vertex vertex = BRepBuilderAPI_MakeVertex(var->getPoint());
			gp_Trsf aTrsf;
			aTrsf.SetMirror(plane);
			BRepBuilderAPI_Transform aBRespTrsf(vertex, aTrsf);
			aBRespTrsf.Build();
			const TopoDS_Shape& resShape = aBRespTrsf.Shape();
			if (resShape.IsNull()) continue;
			TopoDS_Vertex vt = TopoDS::Vertex(resShape);
			gp_Pnt*pt = new gp_Pnt;
			*pt = BRep_Tool::Pnt(vt);
			PluginShip::ModelPoint* md = new PluginShip::ModelPoint;
			md->setPoint(*pt);
			temp.push_back(md);

		}
		temp.reverse();
		for(PluginShip::ModelPoint* var : temp)
		{
			result.push_back(var);
		}
	}



	void GeoCommandCreateHullCut::getResult(QVector<PluginShip::ModelPoint*>&result, std::vector<PluginShip::ModelPoint*> temptvector, QMap<int, PluginShip::ModelPoint*>&pntMap)
	{
		
		QList<int> indexlist;
		for (QVector<PluginShip::ModelPoint*>::iterator iter = result.begin(); iter != result.end(); ++iter)
		{
			int k = (*iter)->getIndex();
			indexlist.push_back((*iter)->getIndex());
			
		}
		for (std::vector<PluginShip::ModelPoint*>::iterator iter = temptvector.begin(); iter != temptvector.end(); ++iter)
		{
			int k = (*iter)->getIndex();
			indexlist.push_back((*iter)->getIndex());
		}
		qSort(indexlist.begin(), indexlist.end());
		result.clear();
		for (int i = 0; i < indexlist.size(); i++)
		{
			int k = indexlist[i];
			PluginShip::ModelPoint* temp = pntMap.value(indexlist[i]);
			result.push_back(temp);
		}
	}


	void GeoCommandCreateHullCut::sortResultAgain(QVector<PluginShip::ModelPoint*>&result, QVector<PluginShip::ModelPoint*> jyqList)
	{
		PluginShip::ModelPoint* connectpt = new PluginShip::ModelPoint;
		for (int i = 0; i < result.size();)
		{
			if (result[i]->getIsJyq()==true)
			{
				result.removeAt(i);
				
				if (result[i - 1]->getIsJyq() == false) connectpt = result[i - 1];
			}
			else i++;
		}
		QMap<double,PluginShip::ModelPoint*> mapdistance;
		PluginShip::ModelPoint* firstpt = result.front();
		for (int i = 1; i < result.size(); i++)
		{
			double dis = firstpt->getPoint().Distance(result[i]->getPoint());
			mapdistance.insert(dis, result[i]);
		}
		QList<double> dislist = mapdistance.keys();
		qSort(dislist.begin(), dislist.end());
		result.clear();
		result.push_back(firstpt);
		for (size_t i = 0; i < dislist.size(); i++)
		{
			result.push_back(mapdistance.value(dislist[i]));
		}
		QVector<PluginShip::ModelPoint*> cutlist{};

		for (int i = 0; i < result.size(); i++)
		{
			if (result[i] == connectpt)
			{
				cutlist = result.mid(i, result.size() - i);
			}

		}
		//
		/*ofstream ofile;//①
		ofile.open("D:\\testcut.py", ios::out);//②
		for (int i = 0; i < cutlist.size(); i++)
		{
			ofile << "point = CAD.Point()" << endl;
			QString str = QString("point.setName('Point_%1')").arg(i + 1);
			ofile << str.toStdString() << endl;
			ofile << "point.setLocation(0,0,0)" << endl;
			QString sttt = QString("point.setOffset(0, %1, %2)").arg(cutlist[i]->getPoint().Y()).arg(cutlist[i]->getPoint().Z());
			ofile << sttt.toStdString() << endl;
			ofile << "point.create()" << endl;
		}
		ofile.close();*/

		PluginShip::ModelPoint* insertpt = new PluginShip::ModelPoint;
		for (int  i = 1; i < cutlist.size(); i++)
		{
			double d = jyqList.front()->getPoint().Distance(cutlist.front()->getPoint());
			double dis = cutlist[i]->getPoint().Distance(cutlist.front()->getPoint());
			if (dis<d)
			{
				insertpt = cutlist[i];
				insertpt->setIndex(i);
			}
		}
		if (insertpt->getIndex()>-1)
		{
			for (int i = 0; i < result.size(); i++)
			{
				if (insertpt == result[i])
				{
					for (int j = 0; j < jyqList.size(); j++)
					{
						result.insert(i + j, jyqList[j]);
					}

				}

			}
		}
		else
		{
			for (int  i = 0; i < result.size(); i++)
			{
				if (result[i] == connectpt)
				{
					for (int j = 0; j < jyqList.size(); j++)
					{
						result.insert(i + 1 + j, jyqList[j]);
					}
				}
			}
		}
		
	}

	bool GeoCommandCreateHullCut::sortEdge(gp_Pnt* wireStartPnt, std::list<TopoDS_Edge*> &edgelist, std::list<TopoDS_Edge*> &sortedEdge)
	{
		double dist;
		std::map<double, TopoDS_Edge*> disEdgeMap;
		int i = 0;
		bool ok{ false };
		for (std::list<TopoDS_Edge*>::iterator pE = edgelist.begin(); pE != edgelist.end(); ++pE)
		{

			TopoDS_Edge* edge = *pE;
			/*
			string filestr = "../../IGS/edgeone.brep";
			const char* ch = filestr.c_str();
			TopoDS_Wire r = BRepBuilderAPI_MakeWire(*edge);
			BRepTools::Write(r, ch);
			*/

			Standard_Real first, last;
			Handle(Geom_Curve) curve = BRep_Tool::Curve(*edge, first, last);
			gp_Pnt startpt, endpt;
			curve->D0(first, startpt);
			curve->D0(last, endpt);

			double startdis = (*wireStartPnt).Distance(startpt);
			double enddis = (*wireStartPnt).Distance(endpt);
			double temp = startdis < enddis ? startdis : enddis;
			disEdgeMap.insert(std::pair<double, TopoDS_Edge*>(temp, edge));
			//设置dist初值。
			if (pE == edgelist.begin())
			{
				dist = startdis < enddis ? startdis : enddis;
			}
			else
			{
				//比较第一个dist和每个边的first和end点与basicpt的距离。
				dist = dist < temp ? dist : temp;
			}
			//遍历结束，找到最小的dist及对应的边，剔除边，加入结果list，替换wirept。回调.
			if (i == edgelist.size() - 1)
			{
				TopoDS_Edge* cancelEdge = new TopoDS_Edge;
				std::map<double, TopoDS_Edge*>::iterator it_find;
				it_find = disEdgeMap.find(dist);
				if (it_find != disEdgeMap.end()){
					cancelEdge = it_find->second;
				}
				edgelist.remove(cancelEdge);
				sortedEdge.push_back(cancelEdge);
				Standard_Real first, last;
				Handle(Geom_Curve) curve = BRep_Tool::Curve(*cancelEdge, first, last);
				gp_Pnt startpt, endpt;
				curve->D0(first, startpt);
				curve->D0(last, endpt);

				gp_Pnt* ptstart = new gp_Pnt;
				gp_Pnt* ptend = new gp_Pnt;
				ptstart = &startpt;
				ptend = &endpt;
				double startdis = (*wireStartPnt).Distance(startpt);
				double enddis = (*wireStartPnt).Distance(endpt);
				if (startdis > enddis)
				{
					wireStartPnt = ptstart;
					cancelEdge->Checked(false);

				}
				else
				{
					wireStartPnt = ptend;
					cancelEdge->Checked(true);
				}
				if (edgelist.size() < 1) return ok = true;
				if (!ok)
				{
					ok = sortEdge(wireStartPnt, edgelist, sortedEdge);
					if (ok) return ok;
				}
			}
			++i;
		}
	}

	TopoDS_Shape* GeoCommandCreateHullCut::getModelShape()
	{
		Geometry::GeometryData* data = Geometry::GeometryData::getInstance();
		Geometry::GeometrySet * geoset = data->getGeometrySetAt(_geoindex);
		TopoDS_Shape* aShape = geoset->getShape();

		if ((*aShape).IsNull()) return nullptr;
		//shape-box
		Bnd_Box bndBox;
		BRepBndLib::Add(*aShape, bndBox);
		bndBox.SetGap(0.0000);
		gp_Pnt cornerMin = bndBox.CornerMin();
		gp_Pnt cornerMax = bndBox.CornerMax();
		//船体底部的Z最小值，根据H求得面的Z。
		_minZ = cornerMin.Z();
		gp_Pnt cornerNewMax(cornerMax.X(), (cornerMin.Y() + cornerMax.Y()) / 2, cornerMax.Z());
		TopoDS_Shape halfBoatBox = BRepPrimAPI_MakeBox(cornerMin, cornerNewMax).Shape();

		//半船盒子与船shape做相交.
		TopoDS_Shape* halfBoat = new TopoDS_Shape;
		*halfBoat = BRepAlgoAPI_Common(*aShape, halfBoatBox);
		if (halfBoat->IsNull()) return nullptr;

		//剔除船甲板平面
		TopExp_Explorer anExp(*halfBoat, TopAbs_FACE);
		for (; anExp.More(); anExp.Next())
		{
			Bnd_Box faceBox;
			BRepBndLib::Add(anExp.Current(), faceBox);
			gp_Pnt gpmax = faceBox.CornerMax();
			gp_Pnt gpmin = faceBox.CornerMin();

			double dis = gpmax.Distance(gpmin);
			double tol = dis*0.001;
			if (fabs(faceBox.CornerMax().Z() - faceBox.CornerMin().Z())<tol)
			{
				TopoDS_Face currentVt = TopoDS::Face(anExp.Current());
				Bnd_Box cutBox;
				BRepBndLib::Add(currentVt, cutBox);
				bndBox.SetGap(0.0000);
				gp_Pnt cutMin = cutBox.CornerMin();
				gp_Pnt cutMax = cutBox.CornerMax();

				*halfBoat = BRepAlgoAPI_Cut(*halfBoat, currentVt);

			}
		}
		QList<gp_Pnt*> pnlist{};
		TopExp_Explorer aExp(*halfBoat, TopAbs_VERTEX);
		for (; aExp.More(); aExp.Next())
		{
			gp_Pnt* gp = new gp_Pnt;
			TopoDS_Vertex vt = TopoDS::Vertex(aExp.Current());
			*gp = BRep_Tool::Pnt(vt);
			pnlist.append(gp);

		}

		for (int i = 0; i < pnlist.size() - 1; i++)
		{
			gp_Pnt *aa = pnlist[i];
			gp_Pnt*bb = pnlist[i + 1];
			double a = pnlist[i]->X();
			double b = pnlist[i + 1]->X();
			double c = a > b ? a : b;
			_maxx = _maxx > c ? _maxx : c;
			double d = a < b ? a : b;
			_minx = _minx < d ? _minx : d;

		}

		return halfBoat;
	}
}