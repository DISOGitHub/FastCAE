#include "GenerateMesh.h"
#include "Gmsh/GmshThread.h"
#include "Gmsh/GmshModule.h"
#include "Gmsh/GmshThreadManager.h"
#include "moduleBase/processBar.h"
#include "geometry/geometryData.h"
#include "geometry/GeoComponent.h"
#include "geometry/geometrySet.h"
#include "meshData/ElementType.h"
#include <qdom.h>
#include <vtkDataSet.h>
#include <vtkCell.h>
#include <qfile.h>

namespace IO
{
	void GenerateMesh::iniGenerateMesh(GUI::MainWindow* mw, QString path)
	{
		_mw = mw;
		_filePath = path;
		iniParameter();
		connect(_thread, SIGNAL(writeToSolveFileSig(vtkDataSet*)), this, SLOT(writeToSolveFileSlot(vtkDataSet*)));
	}

	void GenerateMesh::iniParameter()
	{
		Geometry::GeometryData* geoData = Geometry::GeometryData::getInstance();
		QList<Geometry::GeoComponent*> gcs = geoData->getGeoComponentList();
		QMultiHash<int, int> Surface, Body;
		for (Geometry::GeoComponent* gc : gcs)
		{
			if (!gc)	continue;
			QMultiHash<Geometry::GeometrySet*, int> items = gc->getSelectedItems();
			QHashIterator<Geometry::GeometrySet*, int> it(items);
			while (it.hasNext())
			{
				it.next();
				int ID = it.key()->getID();
				int index = it.value();

				if (gc->getGCType() == Geometry::GeoComponentType::Surface)
					Surface.insert(ID, index);
				else if (gc->getGCType() == Geometry::GeoComponentType::Body)
					Body.insert(ID, index);
			}
		}

		Gmsh::GMshPara* para = new Gmsh::GMshPara;
		para->_solidHash = Body;
		para->_surfaceHash = Surface;
		para->_elementType = "Tet";
		para->_method = 1;
		para->_dim = 3;
		para->_elementOrder = 1;
		para->_sizeFactor = 1.00;
		para->_maxSize = 100.00;
		para->_minSize = 0.00;
		para->_isGridCoplanar = true;
		para->_geoclean = true;

		Gmsh::GmshModule* gModule = Gmsh::GmshModule::getInstance(_mw);
		_thread = gModule->iniGmshThread(para);
	}

	void GenerateMesh::startGenerateMesh()
	{
		_thread->isSaveDataToKernal(false);
		Gmsh::GmshModule* gModule = Gmsh::GmshModule::getInstance(_mw);
		Gmsh::GmshThreadManager* manager = gModule->getGmshThreadManager();
		auto processBar = new ModuleBase::ProcessBar(_mw, QObject::tr("Gmsh Working..."));
		manager->insertThread(processBar, _thread);
	}

	void GenerateMesh::writeToSolveFileSlot(vtkDataSet* data)
	{
		QFile file(_filePath + "/meshdata.xml");
		if (!file.open(QFile::ReadWrite | QFile::Text))    return;
		QDomDocument doc;
		QDomElement kernel, set;
		if (file.size() == 0)
		{
			QDomProcessingInstruction instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
			doc.appendChild(instruction);
			QDomElement root = doc.createElement("DISO_FILE_1.0");
			doc.appendChild(root);
			QDomElement mesh = doc.createElement("Mesh");
			root.appendChild(mesh);
			QDomElement kernel = doc.createElement("Kernel");
			mesh.appendChild(kernel);
			QDomElement set = doc.createElement("Set");
			mesh.appendChild(set);
		}
		else if (file.size() > 0)
		{
			if (!doc.setContent(&file))
			{
				file.close();
				return;
			}
			QDomElement mesh = doc.elementsByTagName("Mesh").at(0).toElement();
			kernel = mesh.elementsByTagName("Kernel").at(0).toElement();
			set = mesh.elementsByTagName("Set").at(0).toElement();
		}
		writeKernelToSolveFile(&doc, &kernel, data);
		writeComponentToSolveFile(&doc, &set, data);
		file.resize(0);
		QTextStream out(&file);
		doc.save(out, 4);
		file.close();
		delete this;
	}

	void GenerateMesh::writeKernelToSolveFile(QDomDocument* doc, QDomElement* kernel, vtkDataSet* data)
	{
		int pointsCount = data->GetNumberOfPoints();
		int cellsCount = data->GetNumberOfCells();
		QDomElement generatedMesh = doc->createElement("MeshKernel");
		kernel->appendChild(generatedMesh);
		QDomElement nodelist = doc->createElement("NodeList");
		generatedMesh.appendChild(nodelist);
		for (int i = 0; i < pointsCount; i++)
		{
			double* array = data->GetPoint(i);
			QStringList list;
			list << QString::number(i) << QString::number(array[0]) << QString::number(array[1]) << QString::number(array[2]);
			QString str = list.join(',');

			QDomText text = doc->createTextNode(str);
			QDomElement node = doc->createElement("Node");
			nodelist.appendChild(node);
			node.appendChild(text);
		}

		QDomElement elementlist = doc->createElement("ElementList");
		generatedMesh.appendChild(elementlist);
		for (int i = 0; i < cellsCount; i++)
		{
			vtkCell* cell = data->GetCell(i);
			QString stype = MeshData::vtkCellTYpeToString((VTKCellType)cell->GetCellType());
			QString text = QString("%1,%2").arg(i).arg(stype);
			vtkIdList* ids = cell->GetPointIds();
			int count = ids->GetNumberOfIds();
			QDomElement element = doc->createElement("Element");
			for (int j = 0; j < count; j++)
			{
				int id = ids->GetId(j);
				text.append(",");
				text.append(QString("%1").arg(id));
			}
			QDomText eletext = doc->createTextNode(text);
			elementlist.appendChild(element);
			element.appendChild(eletext);
		}
	}

	void GenerateMesh::writeComponentToSolveFile(QDomDocument* doc, QDomElement* set, vtkDataSet* data)
	{
		Geometry::GeometryData* geoData = Geometry::GeometryData::getInstance();
		QList<Geometry::GeoComponent*> gcs = geoData->getGeoComponentList();
		QList<Gmsh::itemInfo> infos = _thread->generateGeoIds(data);
		for (Geometry::GeoComponent* gc : gcs)
		{
			if (!gc)	continue;
			QDomElement geoComponent = doc->createElement("GeoComponent");
			geoComponent.setAttribute("ID", gc->getID());
			geoComponent.setAttribute("Name", gc->getName());
			geoComponent.setAttribute("Type", "Element");
			set->appendChild(geoComponent);

			QDomElement member = doc->createElement("Member");
			geoComponent.appendChild(member);
			QString cellIndexs = getCellIndexs(infos, gc);
			QDomText text = doc->createTextNode(cellIndexs);
			member.appendChild(text);
		}
	}

	QString GenerateMesh::getCellIndexs(QList<Gmsh::itemInfo>& infos, Geometry::GeoComponent*gc/*int geoSetID, int itemIndex*/)
	{
		QString cellIndexs;
		QMultiHash<Geometry::GeometrySet*, int> items = gc->getSelectedItems();
		QHashIterator<Geometry::GeometrySet*, int> it(items);
		while (it.hasNext())
		{
			it.next();
			int geoSetID = it.key()->getID();
			int itemIndex = it.value();
			for (Gmsh::itemInfo info : infos)
			{
				if (info.geoSetID == geoSetID && info.itemIndex == itemIndex)
				{
					for (int index : info.cellIndexs)
						cellIndexs.append(QString::number(index)).append(',');
					break;
				}
			}
		}
		return cellIndexs.left(cellIndexs.size() - 1);
	}
}