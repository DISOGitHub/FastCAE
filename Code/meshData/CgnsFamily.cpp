#include "CgnsFamily.h"
#include "meshKernal.h"
#include "meshSingleton.h"
#include <vtkSmartPointer.h>
#include <vtkAppendFilter.h>
#include <vtkStructuredGrid.h>
#include <vtkUnstructuredGrid.h>
#include "meshSingleton.h"
#include <QDomElement>
#include <QDataStream>
#include <QDomAttr>
#include <QDomText>
#include <QDebug>

namespace MeshData
{
	CgnsFamily::CgnsFamily(QString name)
	{
		this->setName(name);
		this->setType(Family);
	}

	CgnsFamily::CgnsFamily()
	{
		this->setType(Family);
	}

	CgnsFamily::~CgnsFamily()
	{
	}

	void CgnsFamily::generateDisplayDataSet()
	{
		if (_displayDataSet != nullptr) return;
		if (_setList.size() <= 0) return;

		vtkSmartPointer<vtkAppendFilter> appfilter = vtkSmartPointer<vtkAppendFilter>::New();
		const int n = _setList.size();
		for (int i = 0; i < n; ++i)
		{
			vtkDataSet* kdataset = _setList.at(i);
			if (kdataset == nullptr) continue;;
			appfilter->AddInputData(kdataset);
		}
		appfilter->Update();
		_displayDataSet = vtkUnstructuredGrid::New();
		_displayDataSet->DeepCopy(appfilter->GetOutput());
	}

	void CgnsFamily::appendDataSet(vtkDataSet* set)
	{
		_setList.append(set);
	}

	QDomElement& CgnsFamily::writeToProjectFile(QDomDocument* doc, QDomElement* parent)
	{
		QDomElement setEle = doc->createElement("MeshSet");

		QDomAttr idAttr = doc->createAttribute("ID");
		idAttr.setValue(QString::number(_id));
		setEle.setAttributeNode(idAttr);
		QDomAttr name = doc->createAttribute("Name");
		name.setValue(_name);
		setEle.setAttributeNode(name);
		QDomAttr typeAttr = doc->createAttribute("Type");
		typeAttr.setValue("CgnsFamily");
		setEle.setAttributeNode(typeAttr);

// 		QStringList ids;
// 		MeshData* meshdata = MeshData::getInstance();
// 		for (int i = 0; i < _setList.size(); ++i)
// 		{
// 			vtkDataSet* dataset = _setList.at(i);
// 			int id = meshdata->getIDByDataSet(dataset);
// 			ids.append(QString::number(id));
// 		}
// 
// 		QDomElement kid = doc->createElement("DataSetID");
// 		QDomText ktext = doc->createTextNode(ids.join(","));
// 		kid.appendChild(ktext);
// 		setEle.appendChild(kid);

		QDomElement MeshListEle = doc->createElement("DataSetList");
		for (int d = 0; d < _setList.size(); ++d)
		{
			QDomElement datasetele = doc->createElement("DataSet");

			vtkStructuredGrid* sg = dynamic_cast<vtkStructuredGrid*>(_setList.at(d));
			if (sg == nullptr) continue;
			int* dim = sg->GetDimensions();
			QStringList sdim;
			for (int i = 0; i < 3; ++i)
				sdim.append(QString::number(dim[i]));
			datasetele.setAttribute("Dim", sdim.join(","));

			const int nnode = sg->GetNumberOfPoints();
			for (int i = 0; i < nnode; ++i)
			{
				QDomElement disnode = doc->createElement("DisNode");
				double* c = sg->GetPoint(i);
				QStringList coor;
				for (int j = 0; j < 3; ++j)
					coor.append(QString::number(c[j]));
				QDomText domtext = doc->createTextNode(coor.join(","));
				disnode.appendChild(domtext);
				datasetele.appendChild(disnode);
			}
			MeshListEle.appendChild(datasetele);
		}
		setEle.appendChild(MeshListEle);
		
		parent->appendChild(setEle);
		return setEle;
	}

	void CgnsFamily::readDataFromProjectFile(QDomElement* setEle)
	{
		QString name = setEle->attribute("Name");
		QString sID = setEle->attribute("ID");
		this->setID(sID.toInt());
		this->setName(name);

		QDomNodeList meshList = setEle->elementsByTagName("DataSetList");
		if (meshList.size() != 1) return;
		QDomElement meshele = meshList.at(0).toElement();

		QDomNodeList datasetlist =  meshele.elementsByTagName("DataSet");

		for (int d = 0; d < datasetlist.size(); ++d)
		{
			QDomElement ele = datasetlist.at(d).toElement();
			QString sdim = ele.attribute("Dim");
			QStringList dimlist = sdim.split(",");
			if (dimlist.size() != 3) continue;
			vtkStructuredGrid* sg = vtkStructuredGrid::New();
			sg->SetDimensions(dimlist[0].toInt(), dimlist[1].toInt(), dimlist[2].toInt());
			vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
			QDomNodeList ptlist = ele.elementsByTagName("DisNode");
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

			appendDataSet(sg);
		}
	}
	void CgnsFamily::writeBinaryFile(QDataStream* dataStream)
	{
		const int n = _setList.size();
		*dataStream << int(_type) << _id << _name << n;
		for (int d = 0; d < n; ++d)
		{
			vtkStructuredGrid* sg = dynamic_cast<vtkStructuredGrid*>(_setList.at(d));
			if (sg == nullptr) continue;
			int* dim = sg->GetDimensions();
			for (int i = 0; i < 3; ++i)
				*dataStream << dim[i];
			const int nnode = sg->GetNumberOfPoints();
			*dataStream << nnode;
			for (int i = 0; i < nnode; ++i)
			{
				double* coor = sg->GetPoint(i);				
				for (int j = 0; j < 3; ++j)
					*dataStream << coor[j];
			}
			
		}
		
	}


	void CgnsFamily::readBinaryFile(QDataStream* dataStream)
	{
		int CgnsFamilyID = 0;
		int n = 0;
		QString CgnsFamilyName;
		*dataStream >> CgnsFamilyID >> CgnsFamilyName >> n;
		this->setID(CgnsFamilyID);
		this->setName(CgnsFamilyName);

		for (int d = 0; d < n; ++d)
		{
			int dimlist[3] = {0};
			*dataStream >> dimlist[0] >> dimlist[1] >> dimlist[2];
			vtkStructuredGrid* sg = vtkStructuredGrid::New();
			sg->SetDimensions(dimlist[0], dimlist[1], dimlist[2]);

			vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
			int nptlist = 0;
			*dataStream >> nptlist;

			double c[3] = { 0 };
			for (int i = 0; i < nptlist; ++i)
			{
				for (int j = 0; j < 3; ++j)
					*dataStream >> c[j];
				points->InsertNextPoint(c);
			}
			sg->SetPoints(points);

			appendDataSet(sg);
		}
	}
}