#include "geometrySet.h"
#include "geometryModelParaBase.h"
#include "geometry/geometryModelParaBase.h"
#include <vtkDataSet.h>
#include <TopoDS_Shape.hxx>
#include <TopExp_Explorer.hxx>
#include <QDataStream>
#include <QDomDocument>
#include <QDomElement>
#include <QDomText>
#include <QDomAttr>
#include <QDebug>
#include <QString>
#include <QCoreApplication>>
#include <TopoDS_Compound.hxx>
#include <BRep_Builder.hxx>
#include <BRepTools.hxx>

namespace Geometry
{
	int GeometrySet::idOffset = 0;
	TopoDS_Shape* GeometrySet::tempShape = new TopoDS_Shape;

	int GeometrySet::getMaxID()
	{
		return idOffset;
	}
	GeometrySet::GeometrySet(GeometryType type, bool need)
	{
		_type = type;
		if (need)
		{
			idOffset++;
		    setID(idOffset);
		}
		appendProperty(QString("Visible"), _visible);
	}
	GeometrySet::~GeometrySet()
	{
		if (_shape != nullptr) delete _shape;
	
		if (_parameter != nullptr) delete _parameter;
	}
	void GeometrySet::dataToStream(QDataStream* s)
	{
		*s << _id << _name<<_visible;
	}
// 	void GeometrySet::setFilePath(const QString& filepath)
// 	{
// 		_filePath = filepath;
// 		appendProperty(QString("Path"), _filePath);
// 	}
// 	QString GeometrySet::getFilePath()
// 	{
// 		return _filePath;
// 	}
	void GeometrySet::setVisible(bool v)
	{
		_visible = v;
		appendProperty(QString("Visible"), _visible);
	}
	bool GeometrySet::isVisible()
	{
		return _visible;
	}
	void GeometrySet::setType(GeometryType t)
	{
		_type = t;
	}
	GeometryType GeometrySet::getType()
	{
		return _type;
	}
	void GeometrySet::setShape(TopoDS_Shape* shape)
	{
		_shape = shape;
		int i = 0;
		for (TopExp_Explorer faceExp(*shape, TopAbs_FACE); faceExp.More(); faceExp.Next()) ++i;
		appendProperty(QObject::tr("Surfaces"), i);
	}
	TopoDS_Shape* GeometrySet::getShape()
	{
		return _shape;
	}

	TopoDS_Shape* GeometrySet::getShape(int type, int index)
	{
		*tempShape = TopoDS_Shape();
		TopAbs_ShapeEnum shapeType;
		switch (type)
		{
		case 1: shapeType = TopAbs_VERTEX; break;
		case 2: shapeType = TopAbs_EDGE; break;
		case 3: shapeType = TopAbs_FACE; break;
		case 4: shapeType = TopAbs_SOLID; break;
		default:  return tempShape;
		}
		TopExp_Explorer ptExp(*_shape, shapeType);
		for (int i = 0; ptExp.More(); ptExp.Next(), ++i)
		{
			if (i == index)
			{
				*tempShape = ptExp.Current();
				break;
			}
				
		}
		return tempShape;
		
	}

	const TopoDS_Shape& GeometrySet::getRealShape(int type, int index)
	{
		*tempShape = TopoDS_Shape();
		TopAbs_ShapeEnum shapeType;
		switch (type)
		{
		case 1: shapeType = TopAbs_VERTEX; break;
		case 2: shapeType = TopAbs_EDGE; break;
		case 3: shapeType = TopAbs_FACE; break;
		case 4: shapeType = TopAbs_SOLID; break;
		default: return TopoDS_Shape();
		}
		TopExp_Explorer ptExp(*_shape, shapeType);
		for (int i = 0; ptExp.More(); ptExp.Next(), ++i)
		{
			if (i == index)
				return ptExp.Current();
		}
		return TopoDS_Shape();
	}

// 	void GeometrySet::setStlDataSet(vtkSmartPointer<vtkDataSet> polyData)
// 	{
// 		_polyData = polyData;
// 		appendProperty(QObject::tr("Triangles"), (int)polyData->GetNumberOfCells());
// 	}
// 	vtkDataSet* GeometrySet::getStlDataSet()
// 	{
// 		return _polyData;
// 	}
	QDomElement& GeometrySet::writeToProjectFile(QDomDocument* doc, QDomElement* ele, bool isDiso)
	{
		QDomElement element = doc->createElement("GeoSet");  //创建子节点
		QDomAttr idattr = doc->createAttribute("ID");
		idattr.setValue(QString::number(_id));
		element.setAttributeNode(idattr);
		QDomAttr visible = doc->createAttribute("Visible");
		visible.setValue("True");
		if (!_visible) visible.setValue("False");
		element.setAttributeNode(visible);
		QDomElement nameele = doc->createElement("Name");
		QDomText nameText = doc->createTextNode(_name);
		nameele.appendChild(nameText);
		element.appendChild(nameele);
// 		QDomElement pathele = doc->createElement("Path");
// 		QDomText pathtext = doc->createTextNode(_filePath);
// 		pathele.appendChild(pathtext);
// 		element.appendChild(pathele);
		
		ele->appendChild(element);  //子节点挂载
		if (_parameter != nullptr)
			_parameter->writeToProjectFile(doc, &element);

		if (isDiso)
		{
			QString exelPath = QCoreApplication::applicationDirPath();
			const QString tempPath = exelPath + "/../tempIO/" + QString("%1.brep").arg(_id);
			bool ok = this->writeBrep(tempPath);
		}

		for (int i = 0; i < _subSetList.size(); ++i)
		{
			GeometrySet* subset = _subSetList.at(i);
			subset->writeSubSet(doc, &element, isDiso);
		}

		return element;
	}
	void GeometrySet::setID(int id)
	{
		DataBase::setID(id);
		if (id > idOffset)
			idOffset = id;
	}
	void GeometrySet::resetMaxID()
	{
		idOffset = 0;
	}
	void GeometrySet::readDataFromProjectFile(QDomElement* element, bool isDiso)
	{
		QDomNodeList subNodeList = element->elementsByTagName("SubSet");
		const int nsub = subNodeList.size();
		for (int i = 0; i < nsub; ++i)
		{
			QDomElement subele = subNodeList.at(0).toElement();
			GeometrySet* subset = new GeometrySet;
			subset->readSubSet(&subele, isDiso);
			_subSetList.append(subset);
			element->removeChild(subele);
		}

		int id = element->attribute("ID").toInt();
		this->setID(id);
		bool visible = true;
		QString vis = element->attribute("Visible");
		if (vis.toLower() == "false") visible = false;
		this->setVisible(visible);
		QDomNodeList nameNode = element->elementsByTagName("Name");
		
		if (nameNode.size() != 1) return;
		QString name = nameNode.at(0).toElement().text();
//		qDebug() << name;
		this->setName(name);

		QDomNodeList paraList = element->elementsByTagName("Parameter");
		if (paraList.size() == 1)
		{
			QDomElement paraele = paraList.at(0).toElement();
			QString type = paraele.attribute("Type");
			//qDebug() << type;
			_parameter = GeometryModelParaBase::createParaByString(type);
			if (_parameter != nullptr)
				_parameter->readDataFromProjectFile(&paraele);
		}
	
		if (isDiso)
		{
			QString exelPath = QCoreApplication::applicationDirPath();
			const QString tempPath = exelPath + "/../tempIO/" + QString("%1.brep").arg(_id);
			this->readBrep(tempPath);
		}
		

	}

	void GeometrySet::removeSubSet(GeometrySet* set)
	{
		_subSetList.removeOne(set);  
	}

	void GeometrySet::appendSubSet(GeometrySet* set)
	{
		_subSetList.append(set);
	}

	int GeometrySet::getSubSetCount()
	{
		return _subSetList.size();
	}

	GeometrySet* GeometrySet::getSubSetAt(int index)
	{
		if (index >=0 && index <_subSetList.size())
		{
			return _subSetList.at(index);
		}
		return nullptr;
	}

	bool GeometrySet::writeBrep(QString name)
	{
		QByteArray arr = name.toLatin1();
		char* ch = arr.data();

		TopoDS_Compound aRes;
		BRep_Builder aBuilder;
		aBuilder.MakeCompound(aRes);

		if (_shape == nullptr) return false;
		aBuilder.Add(aRes, *_shape);
		
	   return BRepTools::Write(aRes, ch);
	}

	void GeometrySet::setParameter(GeometryModelParaBase* p)
	{
		_parameter = p;
	}

	GeometryModelParaBase* GeometrySet::getParameter()
	{
		return _parameter;
	}

	bool GeometrySet::isEditable()
	{
		return _parameter != nullptr;
	}

	bool GeometrySet::readBrep(QString name)
	{
		QByteArray arr = name.toLatin1();
		char* ch = arr.data();
		TopoDS_Shape* ashape = new TopoDS_Shape;
		BRep_Builder builder;
		BRepTools::Read(*ashape, (const Standard_CString)ch, builder);
		if (ashape->IsNull())
		{
			delete ashape;
			return false;
		}
		_shape = ashape;
	}

	void GeometrySet::writeSubSet(QDomDocument* doc, QDomElement* parent, bool isdiso)
	{
		QDomElement element = doc->createElement("SubSet");  //创建子节点
		QDomAttr idattr = doc->createAttribute("ID");
		idattr.setValue(QString::number(_id));
		element.setAttributeNode(idattr);
		QDomAttr visible = doc->createAttribute("Visible");
		visible.setValue("True");
		if (!_visible) visible.setValue("False");
		element.setAttributeNode(visible);
		QDomElement nameele = doc->createElement("Name");
		QDomText nameText = doc->createTextNode(_name);
		nameele.appendChild(nameText);
		element.appendChild(nameele);

		parent->appendChild(element);  //子节点挂载
		if (_parameter != nullptr)
			_parameter->writeToProjectFile(doc, &element);

		if (isdiso)
		{
			QString exelPath = QCoreApplication::applicationDirPath();
			const QString tempPath = exelPath + "/../tempIO/" + QString("%1.brep").arg(_id);
			bool ok = this->writeBrep(tempPath);
		}

	}

	void GeometrySet::readSubSet(QDomElement* element, bool isDiso /*= false*/)
	{
		int id = element->attribute("ID").toInt();
		this->setID(id);
		bool visible = true;
		QString vis = element->attribute("Visible");
		if (vis.toLower() == "false") visible = false;
		this->setVisible(visible);
		QDomNodeList nameNode = element->elementsByTagName("Name");
		if (nameNode.size() != 1) return;
		QString name = nameNode.at(0).toElement().text();
		//qDebug() << name;
		this->setName(name);

		QDomNodeList paraList = element->elementsByTagName("Parameter");
		if (paraList.size() == 1)
		{
			QDomElement paraele = paraList.at(0).toElement();
			QString type = paraele.attribute("Type");
			_parameter = GeometryModelParaBase::createParaByString(type);
			if (_parameter != nullptr)
				_parameter->readDataFromProjectFile(&paraele);
		}

		if (isDiso)
		{
			QString exelPath = QCoreApplication::applicationDirPath();
			const QString tempPath = exelPath + "/../tempIO/" + QString("%1.brep").arg(_id);
			this->readBrep(tempPath);
		}
	}

	GeometrySet* GeometrySet::getSetByID(int id)
	{
		GeometrySet* s = nullptr;
		if (id == _id) 
			s = this;
		else
		{
			const int n = _subSetList.size();
			for (int i = 0; i < n; ++i)
			{
				GeometrySet* temp = _subSetList.at(i);
				if (id == temp->getID())
				{
					s = temp;
					break;
				}

			}
		}
		return s;
			
	}

	int GeometrySet::getGeoMemberCount(int type)
	{
		TopAbs_ShapeEnum shapeType;
		switch (type)
		{
		case 1: shapeType = TopAbs_VERTEX; break;
		case 2: shapeType = TopAbs_EDGE; break;
		case 3: shapeType = TopAbs_FACE; break;
		case 4: shapeType = TopAbs_SOLID; break;
		default:  return -1;
		}
		TopExp_Explorer ptExp(*_shape, shapeType);
		QList<Handle(TopoDS_TShape)> tshapelist;
		for (int index = 0; ptExp.More(); ptExp.Next(), ++index)
		{
			TopoDS_Shape s = ptExp.Current();
			Handle(TopoDS_TShape) ts = s.TShape();
			if (tshapelist.contains(ts)) continue;
			tshapelist.append(ts);
		}
		return tshapelist.size();
	}

	void GeometrySet::releaseSubSet()
	{
		for (int i = 0; i < _subSetList.size(); ++i)
		{
			delete _subSetList.at(i);
		}
		_subSetList.clear();
	}





}