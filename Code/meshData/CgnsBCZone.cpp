#include "CgnsBCZone.h"
#include "meshSingleton.h"
#include "meshKernal.h"
#include <vtkDataSet.h>
#include <vtkStructuredGrid.h>
#include <QDomElement>
#include <QDomAttr>
#include <QDataStream>
#include <QDebug>

namespace MeshData
{
	CgnsBCZone::CgnsBCZone(QString name) : MeshSet(name,BCZone)
	{
	}
	CgnsBCZone::CgnsBCZone()
	{
		this->setType(BCZone);
	}
	CgnsBCZone::~CgnsBCZone()
	{

	}
	void CgnsBCZone::setRange(int* x, int* y, int* z)
	{
		_xRange[0] = x[0]; _xRange[1] = x[1];
		_yRange[0] = y[0]; _yRange[1] = y[1];
		_zRange[0] = z[0]; _zRange[1] = z[1];
		this->appendProperty("XRange", QString("[%1,%2]").arg(_xRange[0]).arg(_xRange[1]));
		this->appendProperty("YRange", QString("[%1,%2]").arg(_yRange[0]).arg(_yRange[1]));
		this->appendProperty("ZRange", QString("[%1,%2]").arg(_zRange[0]).arg(_zRange[1]));
	}

	void CgnsBCZone::setDisplayDataSet(vtkDataSet* dataset)
	{
		_displayDataSet = dataset;
	}

	void CgnsBCZone::setParent(QString p)
	{
		_parent = p;
		this->appendProperty("Parent", p);
	}

	QDomElement& CgnsBCZone::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement setEle = doc->createElement("MeshSet");

		QDomAttr idAttr = doc->createAttribute("ID");
		idAttr.setValue(QString::number(_id));
		setEle.setAttributeNode(idAttr);
		QDomAttr name = doc->createAttribute("Name");
		name.setValue(_name);
		setEle.setAttributeNode(name);
		QDomAttr typeAttr = doc->createAttribute("Type");
		typeAttr.setValue("CgnsBCZone");
		setEle.setAttributeNode(typeAttr);

		QDomElement kid = doc->createElement("KernalID");
		QDomText ktext = doc->createTextNode(QString("%1").arg(getDataSetID()));
		kid.appendChild(ktext);
		setEle.appendChild(kid);

		QDomElement dparent = doc->createElement("Parent");
		QDomText tparent = doc->createTextNode(_parent);
		dparent.appendChild(tparent);
		setEle.appendChild(dparent);

		QDomElement drange = doc->createElement("Range");
		QDomText trange = doc->createTextNode(QString("%1,%2,%3,%4,%5,%6").arg(_xRange[0]).arg(_xRange[1]).arg(_yRange[0]).arg(_yRange[1]).arg(_zRange[0]).arg(_zRange[1]));
		drange.appendChild(trange);
		setEle.appendChild(drange);

		QDomElement desMeshEle = doc->createElement("DisplayMesh");
		const int nnode = _displayDataSet->GetNumberOfPoints();
		for (int i = 0; i < nnode; ++i)
		{
			QDomElement disnode = doc->createElement("DisNode");
			double* c = _displayDataSet->GetPoint(i);
			QStringList coor;
			for (int j = 0; j < 3; ++j)
				coor.append(QString::number(c[j]));
			QDomText domtext = doc->createTextNode(coor.join(","));
			disnode.appendChild(domtext);
			desMeshEle.appendChild(disnode);
		}
		setEle.appendChild(desMeshEle);

		parent->appendChild(setEle);
		return setEle;
	}

	void CgnsBCZone::readDataFromProjectFile(QDomElement* setEle)
	{
		QString name = setEle->attribute("Name");
		QString sID = setEle->attribute("ID");
		this->setID(sID.toInt());
		this->setName(name);
		MeshData* meshdata = MeshData::getInstance();

		QDomNodeList domkidlist = setEle->elementsByTagName("KernalID");
		if (domkidlist.size() == 1)
		{
			QDomElement ele = domkidlist.at(0).toElement();
			QString text = ele.text();
			int id = text.toInt();
			MeshKernal* k = meshdata->getKernalByID(id);
			if (k != nullptr)
			{
				vtkDataSet* dataset = k->getMeshData();
				this->setDataSet(dataset);
			}
		}

		QDomNodeList dompidlist = setEle->elementsByTagName("Parent");
		if (dompidlist.size() == 1)
		{
			QDomElement ele = dompidlist.at(0).toElement();
			QString text = ele.text();
			this->setParent(text);
		}

		QDomNodeList domrangelist = setEle->elementsByTagName("Range");
		if (domrangelist.size() == 1)
		{
			QDomElement ele = domrangelist.at(0).toElement();
			QString text = ele.text();
			QStringList range = text.split(",");
			const int n = range.size();
			if (n != 6) return;
			_xRange[0] = range[0].toInt();
			_xRange[1] = range[1].toInt();
			_yRange[0] = range[2].toInt();
			_yRange[1] = range[3].toInt();
			_zRange[0] = range[4].toInt();
			_zRange[1] = range[5].toInt();
			this->setRange(_xRange, _yRange, _zRange);
		}

		QDomNodeList meshList = setEle->elementsByTagName("DisplayMesh");
		if (meshList.size() != 1) return;
		QDomElement disele = meshList.at(0).toElement();
		vtkStructuredGrid* sg = vtkStructuredGrid::New();
		_displayDataSet = sg;
		sg->SetDimensions(_xRange[1] - _xRange[0] + 1, _yRange[1] - _yRange[0] + 1, _zRange[1] - _zRange[0] + 1);
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		QDomNodeList ptlist = disele.elementsByTagName("DisNode");
		for (int i = 0; i < ptlist.size(); ++i)
		{
			QDomElement ne = ptlist.at(i).toElement();
			QStringList t = ne.text().split(",");
			double c[3];
			for (int j = 0; j < 3; ++j)
				c[j] = t[j].toDouble();
			points->InsertNextPoint(c);
		}
		sg->SetPoints(points);
	}
	
	void CgnsBCZone::writeBinaryFile(QDataStream* dataStream)
	{
		*dataStream << int(_type) << _id << _name << getDataSetID() << _parent;
		*dataStream << _xRange[0] << _xRange[1] << _yRange[0] << _yRange[1] << _zRange[0] << _zRange[1];
		const int nnode = _displayDataSet->GetNumberOfPoints();
		*dataStream << nnode;
		for (int i = 0; i < nnode; ++i)
		{
			double* coor = _displayDataSet->GetPoint(i);
			for (int j = 0; j < 3; ++j)
				*dataStream << coor[j];
		}
	}
	
	void CgnsBCZone::readBinaryFile(QDataStream* dataStream)
	{
		int CgnsBCZoneID = 0, KernalID = 0;
		QString CgnsBCZoneName, parent;
		*dataStream >> CgnsBCZoneID >> CgnsBCZoneName >> KernalID >> parent;
		this->setID(CgnsBCZoneID);
		this->setName(CgnsBCZoneName);

		MeshData* meshdata = MeshData::getInstance();
		MeshKernal* k = meshdata->getKernalByID(KernalID);
		if (k != nullptr)
		{
			vtkDataSet* dataset = k->getMeshData();
			this->setDataSet(dataset);
		}

		this->setParent(parent);

		*dataStream >> _xRange[0] >> _xRange[1] >> _yRange[0] >> _yRange[1] >> _zRange[0] >> _zRange[1];
		this->setRange(_xRange, _yRange, _zRange);

		vtkStructuredGrid* sg = vtkStructuredGrid::New();
		_displayDataSet = sg;
		sg->SetDimensions(_xRange[1] - _xRange[0] + 1, _yRange[1] - _yRange[0] + 1, _zRange[1] - _zRange[0] + 1);
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		int  nptlist = 0;
		*dataStream >> nptlist;
		for (int i = 0; i < nptlist; ++i)
		{
			double c[3] = {0};
			for (int j = 0; j < 3; ++j)
				*dataStream >> c[j];
			points->InsertNextPoint(c);
		}
		sg->SetPoints(points);
	}

	int CgnsBCZone::getDataSetID()
	{
		if (_dataset == nullptr) return -1;
		MeshData* m = MeshData::getInstance();
		return m->getIDByDataSet(_dataset);
	}

	void CgnsBCZone::setDataSet(vtkDataSet* d)
	{
		_dataset = d;
	}

}