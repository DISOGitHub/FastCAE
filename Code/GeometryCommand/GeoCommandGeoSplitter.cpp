#include "GeoCommandGeoSplitter.h"
#include "geometry/geometryData.h"
#include "geometry/geometrySet.h"
#include <BOPAlgo_Splitter.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepTools.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <QDebug>
#include "geometry/geometryParaGeoSplitter.h"
#include <TopoDS_Face.hxx>
#include <gp_Pln.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <BRepBuilderAPI.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include "GeoCommandCommon.h"
#include <BRepBuilderAPI_Copy.hxx>
namespace Command
{
	CommandGeoSplitter::CommandGeoSplitter(GUI::MainWindow* m, MainWidget::PreWindow* p)
		: GeoCommandBase(m, p)
	{
	}

	bool CommandGeoSplitter::execute()
	{
		this->getDirection();
		gp_Pnt p(_planeLocation[0], _planeLocation[1], _planeLocation[2]);
		gp_Dir dir(_planeDirection[0], _planeDirection[1], _planeDirection[2]);

		if (_solidhash.size() < 1 )
			return false;
		if (_isEdit) //编辑模式下将原来的模型压进列表
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaGeoSplitter* p = dynamic_cast<Geometry::GeometryParaGeoSplitter*>(pm);
			if (p == nullptr) return false;

			Geometry::GeometrySet* ori = p->getOriSet();
			if (ori == nullptr) return false;
		
			_geoData->appendGeometrySet(ori);
			_editSet->removeSubSet(ori);

		}
		bool success = false;

		QList<Geometry::GeometrySet*> setlist = _solidhash.uniqueKeys();
		for (Geometry::GeometrySet* set : setlist)
		{

			//保存下某个set下所选中的所有solid.
			QMultiHash<Geometry::GeometrySet*, int> setToIndex{};
			//aRes=new shape+无关shape.
			TopoDS_Compound aRes;
			BRep_Builder aBuilder;
			aBuilder.MakeCompound(aRes);
			QList<int> indexList = _solidhash.values(set);
			for (int i = 0; i < indexList.size(); i++)
			{
				setToIndex.insert(set, indexList[i]);
				TopoDS_Shape shape = *set->getShape(4, indexList[i]);
				if (shape.IsNull()) return false;


				/*TopoDS_Shape* faceshape = _faceBody->getShape();
				TopExp_Explorer faceExp(*faceshape, TopAbs_FACE);
				for (int index = 0; index < _faceIndex && faceExp.More(); faceExp.Next(), ++index);
				const TopoDS_Shape& faceLimiteShape = faceExp.Current();
				if (faceLimiteShape.IsNull()) return false;

				//判断是否为平面。
				const TopoDS_Face &face = TopoDS::Face(faceLimiteShape);
				if (face.IsNull()) return false;
				BRepAdaptor_Surface adapt(face);
				if (adapt.GetType() != GeomAbs_Plane) return false;*/
				gp_Pln plane(p,dir);

				BRepBuilderAPI_MakeFace mkFace(plane);
				const TopoDS_Face& faceShape = mkFace.Face();
				if (faceShape.IsNull()) return false;
				BOPAlgo_Splitter splitter;
				splitter.AddArgument(shape);
				splitter.AddTool(faceShape);
				splitter.Perform();

				/*TopoDS_Compound comp;
				BRep_Builder builder;
				builder.MakeCompound(comp);*/
				TopExp_Explorer shapeExp(splitter.Shape(), TopAbs_SOLID);
				for (; shapeExp.More(); shapeExp.Next())
				{
					const TopoDS_Solid& anSolid = TopoDS::Solid(shapeExp.Current());
					aBuilder.Add(aRes, anSolid);
				}
			}
			//将无关的solid存在compound中。
			TopoDS_Shape* setShape = set->getShape();
			TopoDS_Shape* setCopyShape = new TopoDS_Shape;
			TopoDS_Shape* setOriShape = new TopoDS_Shape;
			*setOriShape = BRepBuilderAPI_Copy(*setShape);
			for (int i = 0; i < indexList.size(); i++)
			{
				TopoDS_Shape tempShape = *set->getShape(4, indexList[i]);
				*setCopyShape = GeoCommandCommon::removeShape(setShape, &tempShape);

			}
			if (!GeoCommandCommon::isEmpty(*setShape))
			{
				aBuilder.Add(aRes, *setShape);

			}
			success = true;
			TopoDS_Shape* mshape = new TopoDS_Shape;
			*mshape = aRes;
			QString name = QString("Splitter-%1").arg(set->getName());
			Geometry::GeometrySet* newset = new Geometry::GeometrySet;
			newset->setName(name);
			newset->setShape(mshape);

			_geoData->appendGeometrySet(newset);
			_geoData->removeTopGeometrySet(set);
			newset->appendSubSet(set);
			_inputOutputHash.insert(newset, set);
			emit showSet(newset);
			emit removeDisplayActor(set);

			Geometry::GeometryParaGeoSplitter* para = new Geometry::GeometryParaGeoSplitter;
			set->setShape(setOriShape);
			para->setOriSet(set);
			for (int k = 0; k < indexList.size(); k++)
			{
				para->appendBody(set, indexList[k]);
			}
			para->setCurrentIndex(_typeindex);
			para->setFaceIndex(_faceIndex);
			para->setFaceBody(_faceBody);
			para->setPlaneIndex(_planeindex);
			para->setDirection(_randomdir);
			para->setbasepoint(_basepoint);

			newset->setParameter(para);
			GeoCommandBase::execute();

			if (_isEdit)
			{
				_geoData->removeTopGeometrySet(_editSet);
				_releaseEdit = true;     //标记释放状态
				_releasenew = false;
			}

		}
		emit updateGeoTree();
		GeoCommandBase::execute();
		return success;
	}

	void CommandGeoSplitter::undo()
	{
		if (_isEdit)
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaGeoSplitter* p = dynamic_cast<Geometry::GeometryParaGeoSplitter*>(pm);
			if (p == nullptr) return;
			Geometry::GeometrySet* ori = p->getOriSet();
			if (ori == nullptr) return;
			QList<Geometry::GeometrySet*> setliist = _inputOutputHash.keys(ori);
			int sinz = setliist.size();
			auto newset = _inputOutputHash.key(ori);
			if (newset == nullptr) return;
			_geoData->replaceSet(_editSet, newset);
	
			newset->removeSubSet(ori);
			_editSet->appendSubSet(ori);
			_geoData->removeTopGeometrySet(ori);

			emit removeDisplayActor(ori);
			emit showSet(_editSet);
			emit removeDisplayActor(newset);

			emit updateGeoTree();
			_releasenew = true;
			_releaseEdit = false;
			return;
		}

		QList<Geometry::GeometrySet*> geoList = _inputOutputHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
	
			auto ori = _inputOutputHash.value(set);
			set->removeSubSet(ori);
			_geoData->appendGeometrySet(ori);
			emit showSet(ori);
		
			_geoData->removeTopGeometrySet(set);
			emit removeDisplayActor(set);
		}
		emit updateGeoTree();
	}

	void CommandGeoSplitter::redo()
	{
		if (_isEdit)
		{
			Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
			Geometry::GeometryParaGeoSplitter* p = dynamic_cast<Geometry::GeometryParaGeoSplitter*>(pm);
			if (p == nullptr) return;
			Geometry::GeometrySet* ori = p->getOriSet();
			if (ori == nullptr) return;
			auto newset = _inputOutputHash.key(ori);
			if (newset == nullptr) return;
			_geoData->replaceSet(newset, _editSet);
			
			_editSet->removeSubSet(ori);
			newset->appendSubSet(ori);
			_geoData->removeTopGeometrySet(ori);

			emit removeDisplayActor(ori);
			emit showSet(newset);
			emit removeDisplayActor(_editSet);

			emit updateGeoTree();
			_releasenew = false;
			_releaseEdit = true;
			return;
		}
		QList<Geometry::GeometrySet*> geoList = _inputOutputHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
		
			auto ori = _inputOutputHash.value(set);
			set->appendSubSet(ori);
			_geoData->removeTopGeometrySet(ori);
			emit removeDisplayActor(ori);
			
			_geoData->appendGeometrySet(set);
			emit showSet(set);
		}
		emit updateGeoTree();
	}

	void CommandGeoSplitter::releaseResult()
	{
		if (_isEdit)
		{
			if (_releasenew)
			{
				Geometry::GeometryModelParaBase* pm = _editSet->getParameter();
				Geometry::GeometryParaGeoSplitter* p = dynamic_cast<Geometry::GeometryParaGeoSplitter*>(pm);
				if (p == nullptr) return;
				Geometry::GeometrySet* ori = p->getOriSet();
				if (ori == nullptr) return;
				auto newset = _inputOutputHash.key(ori);
				if (newset == nullptr) return;
				delete newset;
			}
			if (_releaseEdit)
			{
				delete _editSet;
			}

			return;
		}

		QList<Geometry::GeometrySet*> geoList = _inputOutputHash.keys();
		for (int i = 0; i < geoList.size(); ++i)
		{
			Geometry::GeometrySet* set = geoList.at(i);
			delete set;
		}
	}
	  
	void CommandGeoSplitter::setBodys(QMultiHash<Geometry::GeometrySet*, int> setidList)
	{
		_solidhash= setidList;
	}

	void CommandGeoSplitter::setFaceIndex(int faceIndex)
	{
		_faceIndex = faceIndex;
	}

	void CommandGeoSplitter::setFaceBody(Geometry::GeometrySet* faceBody)
	{
		_faceBody = faceBody;
	}

	void CommandGeoSplitter::setTypeIndex(int i)
	{
		_typeindex = i;
	}


	void CommandGeoSplitter::setPlaneIndex(int planeindex)
	{
		_planeindex = planeindex;
	}

	void CommandGeoSplitter::setreverse(bool reverse)
	{
		_reverse = reverse;
	}

	void CommandGeoSplitter::setRandomDir(double* randomdir)
	{
		for (int i = 0; i < 3; i++)
		{
			_randomdir[i] = randomdir[i];
		}
	}

	void CommandGeoSplitter::setBasePt(double* basepoint)
	{
		for (int i = 0; i < 3; i++)
		{
			_basepoint[i] = basepoint[i];
		}
	}
	void CommandGeoSplitter::getDirection()
	{
		if (_typeindex == 0)
		{
			if (_faceBody == nullptr) return;
			TopoDS_Shape* shape = _faceBody->getShape();
			TopExp_Explorer faceExp(*shape, TopAbs_FACE);
			for (int index = 0; index < _faceIndex && faceExp.More(); faceExp.Next(), ++index);

			const TopoDS_Shape& faceShape = faceExp.Current();
			if (faceShape.IsNull()) return;

			/*	std::string filestr = "D:/IGS/edgeone.brep";
			const char* ch = filestr.c_str();
			BRepTools::Write(faceShape, ch);*/

			const TopoDS_Face &face = TopoDS::Face(faceShape);
			if (face.IsNull()) return;

			BRepAdaptor_Surface adapt(face);
			if (adapt.GetType() != GeomAbs_Plane) return;
			gp_Pln plane = adapt.Plane();
			gp_Ax1 normal = plane.Axis();

			gp_Pnt loca = normal.Location();
			gp_Dir dirt = normal.Direction();

			_planeLocation[0] = loca.X(); _planeLocation[1] = loca.Y(); _planeLocation[2] = loca.Z();
			_planeDirection[0] = dirt.X(); _planeDirection[1] = dirt.Y(); _planeDirection[2] = dirt.Z();

		}
		else if (_typeindex == 1)
		{
			switch (_planeindex)
			{
			case 0: _planeDirection[2] = 1.0; break;
			case 1: _planeDirection[1] = 1.0; break;
			case 2: _planeDirection[0] = 1.0; break;
			default: break;
			}
		}
		else if (_typeindex == 2)
		{
			for (int i = 0; i < 3; ++i)
			{
				_planeLocation[i] = _basepoint[i];
				_planeDirection[i] = _randomdir[i];
			}
		}
		if (_planeDirection[0] * _planeDirection[0] + _planeDirection[1] * _planeDirection[1] + _planeDirection[2] * _planeDirection[2] < 1e-6) return;
	}
}