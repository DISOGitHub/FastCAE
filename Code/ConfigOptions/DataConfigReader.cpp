#include "DataConfigReader.h"
#include "ConfigOptions.h"
#include "DataConfig.h"
#include "PostConfig.h"
#include "PostConfigInfo.h"
#include "PostCurve.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDebug>

namespace ConfigOption
{
	DataConfigReader::DataConfigReader(const QString fileName, DataConfig* dataconfig, PostConfig* postconfig)
		//: IOBase(fileName)
	{
// 		_dataConfig = ConfigOption::getInstance()->getDataConfig();
// 		_postConfig = ConfigOption::getInstance()->getPostConfig();

		_dataConfig = dataconfig;
		_postConfig = postconfig;

		_file.setFileName(fileName);
	}
	DataConfigReader::~DataConfigReader()
	{
		if (_doc != nullptr) delete _doc;
	}
	bool DataConfigReader::read()
	{
		if (!_file.open(QFile::ReadOnly | QFile::Text)) return false;
		_doc = new QDomDocument();
		if (!_doc->setContent(&_file))
		{
			_file.close();
			return false;
		}

		QDomNodeList simlist = _doc->elementsByTagName("SimlutationData");
		for (int i = 0; i < simlist.size(); ++i)
		{
			QDomElement simele = simlist.at(i).toElement();
			QString stype = simele.attribute("TreeType");
			ProjectTreeType type = getTreeTypeByString(stype);
			if (type == UnDefined) continue;

			DataProperty::DataBase* data = new DataProperty::DataBase;
			data->readParameters(&simele);
			_dataConfig->appendSimlutationData(type, data);
		}
		
		QDomNodeList solverlist = _doc->elementsByTagName("SolverSetting");
		for (int i = 0; i < solverlist.size(); ++i)
		{
			QDomElement solverele = solverlist.at(i).toElement();
			QString stype = solverele.attribute("TreeType");
			ProjectTreeType type = getTreeTypeByString(stype);
			if (type == UnDefined) continue;

			DataProperty::DataBase* data = new DataProperty::DataBase;
			data->readParameters(&solverele);
			_dataConfig->appendSolverSettingData(type, data);
		} 

		QDomNodeList datablocklist = _doc->elementsByTagName("DataBlock");
		for (int i = 0; i < datablocklist.size(); ++i)
		{
			QDomElement datablock = datablocklist.at(i).toElement();
			QString stype = datablock.attribute("TreeType");
			ProjectTreeType type = getTreeTypeByString(stype);
			if (type == UnDefined) continue;

			this->readDataBlock(type, &datablock);
		}

		QDomNodeList monitorList = _doc->elementsByTagName("Monitor");
		for (int i = 0; i < monitorList.size(); ++i)
		{
			QDomElement monitorblock = monitorList.at(i).toElement();
			QString stype = monitorblock.attribute("TreeType");
			ProjectTreeType type = getTreeTypeByString(stype);
			if (type == UnDefined) continue;
			
			QDomNodeList filenodeList = monitorblock.elementsByTagName("File");
			for (int j = 0; j < filenodeList.size(); ++j)
			{
				QDomElement fileele = filenodeList.at(j).toElement();
				QString fileName = fileele.attribute("Name");
//				_dataConfig->appendMonitorFile(type, fileName);

				QDomNodeList curvenodelist = fileele.elementsByTagName("Curve");
				for (int k = 0; k < curvenodelist.size(); ++k)
				{
					QDomElement curveele = curvenodelist.at(k).toElement();
					PostCurve* c = new PostCurve;
					QString desattr = curveele.attribute("Describe");
					QString xvariable = curveele.attribute("XVariable");
					QString yvariable = curveele.attribute("YVariable");
					c->setDescribe(desattr);
					c->setXVariable(xvariable);
					c->setYVariable(yvariable);
					c->setFile(fileName);
					_dataConfig->appendMonitorCurves(type, c);
				}
			}
		}
		
		QDomNodeList postlist = _doc->elementsByTagName("PostConfig");
		for (int i = 0; i < postlist.size(); ++i)
		{
			QDomElement postele = postlist.at(i).toElement();
			QString stype = postele.attribute("TreeType");
			ProjectTreeType type = getTreeTypeByString(stype);
			if (type == UnDefined) continue;
			this->readPostConfig(type, &postele);
		}

		_file.close();
		return true;
	}

	void DataConfigReader::readDataBlock(ProjectTreeType t, QDomElement* e)
	{
		QDomNodeList blocklist = e->elementsByTagName("Block");
		for (int i = 0; i < blocklist.size(); ++i)
		{
			QDomElement ele = blocklist.at(i).toElement();
			QString sid = ele.attribute("ID");
			QString sname = ele.attribute("TreeNode");
			const int id = sid.toInt();
			DataProperty::DataBase* data = new DataProperty::DataBase;
			data->setName(sname);
			data->readParameters(&ele);
			_dataConfig->appendConfigData(t, id, data);
		}
	}

	void DataConfigReader::readPostConfig(ProjectTreeType t, QDomElement* e)
	{
		PostConfigInfo* info = new PostConfigInfo;
		QDomNodeList d2file = e->elementsByTagName("Post2DFile");
		for (int i = 0; i < d2file.size(); ++i)
		{
			QDomElement element = d2file.at(i).toElement();
			QString fileName = element.attribute("Name");
			QDomNodeList curvelist = element.elementsByTagName("Curve");
			for (int j = 0; j < curvelist.size(); ++j)
			{
				QDomElement cele = curvelist.at(j).toElement();
				QString des = cele.attribute("Describe");
				//			qDebug() << des;
				QString xv = cele.attribute("XVariable");
				QString yv = cele.attribute("YVariable");
				PostCurve* c = new PostCurve;
				c->setDescribe(des);
				c->setXVariable(xv);
				c->setYVariable(yv);
				c->setFile(fileName);
				info->appendPostCurve(c);
			}
		}

		QDomNodeList d3file = e->elementsByTagName("Post3DFile");
		if (d3file.size() == 1)
		{
			QString sd3file = d3file.at(0).toElement().text();
			info->setPost3DFile(sd3file);
		}
		QDomNodeList d3nscalar = e->elementsByTagName("Post3DNodeScalar");
		if (d3nscalar.size() == 1)
		{
			QString sd3scalar = d3nscalar.at(0).toElement().text();
			info->setNodeScalarVariable(sd3scalar.split(","));
//			qDebug() << info->getScalarVariable();
		}
		QDomNodeList d3nvector = e->elementsByTagName("Post3DNodeVector");
		if (d3nvector.size() == 1)
		{
			QString sd3nvector = d3nvector.at(0).toElement().text();
			info->setNodeVectorVariable(sd3nvector.split(","));
//			qDebug() << info->getVectorVariable();
		}

		QDomNodeList d3cscalar = e->elementsByTagName("Post3DCellScalar");
		if (d3cscalar.size() == 1)
		{
			QString sd3scalar = d3cscalar.at(0).toElement().text();
			info->setCellScalarVariable(sd3scalar.split(","));
			//			qDebug() << info->getScalarVariable();
		}
		QDomNodeList d3cvector = e->elementsByTagName("Post3DCellVector");
		if (d3cvector.size() == 1)
		{
			QString sd3nvector = d3cvector.at(0).toElement().text();
			info->setCellVectorVariable(sd3nvector.split(","));
			//			qDebug() << info->getVectorVariable();
		}

		_postConfig->setPostConfigInfo(t, info);
	}

}