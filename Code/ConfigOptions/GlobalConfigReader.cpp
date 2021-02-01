#include "GlobalConfigReader.h"
#include "ConfigOptions.h"
#include "GlobalConfig.h"
#include "GeometryConfig.h"
#include "MeshConfig.h"
#include "PostConfig.h"
#include "ConfigDataBase.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QDebug>

namespace ConfigOption
{
	GlobalConfigReader::GlobalConfigReader(QString fileName, ConfigDataBase* data)
	{
		_file.setFileName(fileName);
		_globalConfig = data->getGlobalConfig();
		_geoConfig = data->getGeometryConfig();
		_meshConfig = data->getMeshConfig();
		_postConfig = data->getPostConfig();
		// 		_globalConfig = ConfigOption::getInstance()->getGlobalConfig();
		// 		_geoConfig = ConfigOption::getInstance()->getGeometryConfig();
		// 		_meshConfig = ConfigOption::getInstance()->getMeshConfig();
		// 		_postConfig = ConfigOption::getInstance()->getPostConfig();
	}

	GlobalConfigReader::~GlobalConfigReader()
	{
		if (_doc != nullptr) delete _doc;
	}

	bool GlobalConfigReader::read()
	{
		if (!_file.open(QFile::ReadOnly | QFile::Text)) return false;
		_doc = new QDomDocument();
		if (!_doc->setContent(&_file))
		{
			_file.close();
			return false;
		}

		bool ok = false;

		QDomNodeList globallist = _doc->elementsByTagName("Global");
		if (globallist.size() != 1)  return false;
		QDomElement gele = globallist.at(0).toElement();
		ok = readGlobal(&gele);
		if (!ok) return false;

		QDomNodeList geolist = _doc->elementsByTagName("Geometry");
		if (geolist.size() != 1)  return false;
		QDomElement geoele = geolist.at(0).toElement();
		ok = readGeometry(&geoele);
		if (!ok) return false;

		QDomNodeList meshlist = _doc->elementsByTagName("Mesh");
		if (meshlist.size() != 1) return false;
		QDomElement meshele = meshlist.at(0).toElement();
		ok = readMesh(&meshele);
		if (!ok) return false;

		QDomNodeList postlist = _doc->elementsByTagName("Post");
		if (postlist.size() != 1) return false;
		QDomElement postele = postlist.at(0).toElement();
		ok = readPost(&postele);
		if (!ok) return false;

		QDomNodeList materiallist = _doc->elementsByTagName("Material");
		if (materiallist.size() != 1) return false;
		QDomElement materialele = materiallist.at(0).toElement();
		QString s = materialele.attribute("Enable");
		if (s.toLower() == "true")
			_globalConfig->enableMaterial(true);

		return true;
	}

	bool GlobalConfigReader::readGlobal(QDomElement* e)
	{
		QDomNodeList softnamelist = e->elementsByTagName("SoftName");
		if (softnamelist.size() == 1)
		{
			QDomElement nameele = softnamelist.at(0).toElement();
			QString name = nameele.attribute("name");
			QString chinese = nameele.attribute("Chinese");
			_globalConfig->setSoftName(name);
			_globalConfig->setChineseName(chinese);
		}
		QDomNodeList corlist = e->elementsByTagName("Corporation");
		if (corlist.size() == 1)
		{
			QDomElement corele = corlist.at(0).toElement();
			QString text = corele.text();
			_globalConfig->setCorporation(text);
		}
		QDomNodeList weblist = e->elementsByTagName("Website");
		if (weblist.size() == 1)
		{
			QDomElement webele = weblist.at(0).toElement();
			QString w = webele.text();
			_globalConfig->setWebsite(w);
		}
		QDomNodeList logolist = e->elementsByTagName("Logo");
		if (logolist.size() == 1)
		{
			QDomElement logoele = logolist.at(0).toElement();
			QString l = logoele.text();
			_globalConfig->setLogo(l);
		}
		QDomNodeList wellist = e->elementsByTagName("Welcome");
		if (wellist.size() == 1)
		{
			QDomElement welele = wellist.at(0).toElement();
			QString w = welele.text();
			_globalConfig->setWelcome(w);
		}
		QDomNodeList verlist = e->elementsByTagName("Version");
		if (verlist.size() == 1)
		{
			QDomElement verele = verlist.at(0).toElement();
			QString w = verele.text();
			_globalConfig->setVersion(w);
		}
		QDomNodeList maillist = e->elementsByTagName("Mail");
		if (maillist.size() == 1)
		{
			QDomElement verele = maillist.at(0).toElement();
			QString w = verele.text();
			_globalConfig->setEMail(w);
		}
		QDomNodeList helpFileList = e->elementsByTagName("HelpFile");
		if (helpFileList.size() == 1)
		{
			QDomElement verele = helpFileList.at(0).toElement();
			QString w = verele.text();
			_globalConfig->SetUserManual(w);
		}
		// 		QDomNodeList useRibbons = e->elementsByTagName("UseRibbon");
		// 		if (useRibbons.size() == 1)
		// 		{
		// 			QDomElement useRibbon = useRibbons.at(0).toElement();
		// 			QString text = useRibbon.text();
		// 			_globalConfig->setUseRibbon(text);
		// 		}
		return true;
	}
	bool GlobalConfigReader::readGeometry(QDomElement* e)
	{
		QDomNodeList importlist = e->elementsByTagName("ImportSuffix");
		QDomElement importele = importlist.at(0).toElement();
		QString importsuffix = importele.text();
		_geoConfig->setImportSuffix(importsuffix);

		QDomNodeList exportlist = e->elementsByTagName("ExportSuffix");
		QDomElement exportele = exportlist.at(0).toElement();
		QString exportsuffix = exportele.text();
		_geoConfig->setExportSuffix(exportsuffix);

		QDomNodeList sketchlist = e->elementsByTagName("CreateSketch");
		QDomElement sketchele = sketchlist.at(0).toElement();
		bool on = false;
		QString son = sketchele.attribute("Enable");
		if (son.toLower() == "true") on = true;
		_geoConfig->enableCreateSketch(on);

		QDomNodeList fmodelList = e->elementsByTagName("FeatureModeling");
		QDomElement fmodelele = fmodelList.at(0).toElement();
		on = false;
		son = fmodelele.attribute("Enable");
		if (son.toLower() == "true") on = true;
		_geoConfig->enableGeometryModeling(on);

		QDomNodeList foplList = e->elementsByTagName("FeatureOperations");
		QDomElement fopele = foplList.at(0).toElement();
		on = false;
		son = fopele.attribute("Enable");
		if (son.toLower() == "true") on = true;
		_geoConfig->enableGeometryOperations(on);

		QDomNodeList geditList = e->elementsByTagName("GeometryEdit");
		QDomElement geditele = geditList.at(0).toElement();
		on = false;
		son = geditele.attribute("Enable");
		if (son.toLower() == "true") on = true;
		_geoConfig->enableGeometryEdit(on);

		QDomNodeList csetList = e->elementsByTagName("CreateSet");
		QDomElement csetele = csetList.at(0).toElement();
		on = false;
		son = csetele.attribute("Enable");
		if (son.toLower() == "true") on = true;
		_geoConfig->enableGeometryCreateSet(on);

		QDomNodeList mdisList = e->elementsByTagName("MeasureDistance");
		QDomElement mdisele = mdisList.at(0).toElement();
		on = false;
		son = mdisele.attribute("Enable");
		if (son.toLower() == "true") on = true;
		_geoConfig->enableMeasureDsitance(on);

		return true;
	}

	bool GlobalConfigReader::readMesh(QDomElement* e)
	{
		QDomNodeList nodeList{};
		QDomElement ele{};
		QString text{};

		nodeList = e->elementsByTagName("ImportSuffix");
		ele = nodeList.at(0).toElement();
		text = ele.text();
		_meshConfig->setImportSuffix(text);

		nodeList = e->elementsByTagName("ExportSuffix");
		ele = nodeList.at(0).toElement();
		text = ele.text();
		_meshConfig->setExportSuffix(text);

		nodeList = e->elementsByTagName("SurfaceMesh");
		ele = nodeList.at(0).toElement();
		text = ele.attribute("Enable");
		if (text.toLower() != "true")
			_meshConfig->enableSurfaceMesh(false);

		nodeList = e->elementsByTagName("SolidMesh");
		ele = nodeList.at(0).toElement();
		text = ele.attribute("Enable");
		if (text.toLower() != "true")
			_meshConfig->enableSolidMesh(false);

		nodeList = e->elementsByTagName("Component");
		ele = nodeList.at(0).toElement();
		text = ele.attribute("Enable");
		if (text.toLower() != "true")
			_meshConfig->enableComponent(false);

		nodeList = e->elementsByTagName("CheckMesh");
		ele = nodeList.at(0).toElement();
		text = ele.attribute("Enable");
		if (text.toLower() != "true")
			_meshConfig->setCheckMesh(false);

		nodeList = e->elementsByTagName("FluidMesh");
		ele = nodeList.at(0).toElement();
		text = ele.attribute("Enable");
		if (text.toLower() != "true")
			_meshConfig->enableFluidMesh(false);

		nodeList = e->elementsByTagName("FilterMesh");
		ele = nodeList.at(0).toElement();
		text = ele.attribute("Enable");
		if (text.toLower() != "true")
			_meshConfig->enableFilterMesh(false);

		nodeList = e->elementsByTagName("MeshModeling");
		ele = nodeList.at(0).toElement();
		text = ele.attribute("Enable");
		if (text.toLower() != "true")
			_meshConfig->enableMeshModeling(false);

		return true;
	}

	bool GlobalConfigReader::readPost(QDomElement* e)
	{
		QString postattr = e->attribute("Enable");
		if (postattr.toLower() == "true")
			_postConfig->enablePost(true);
		QDomNodeList d2list = e->elementsByTagName("Post2D");
		QDomElement d2ele = d2list.at(0).toElement();
		QString post2dattr = d2ele.attribute("Enable");
		if (post2dattr.toLower() == "true")
			_postConfig->enablePost2D(true);
		QDomNodeList d3list = e->elementsByTagName("Post3D");
		QDomElement d3ele = d3list.at(0).toElement();
		QString post3dattr = d3ele.attribute("Enable");
		if (post3dattr.toLower() == "true")
			_postConfig->enablePost3D(true);

		return true;
	}
}