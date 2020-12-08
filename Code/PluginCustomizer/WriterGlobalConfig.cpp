/**********************************************************************************

功能：基本信息保存
建立：xvdongming
日期：2019-11
修改和维护：xvdongming

**********************************************************************************/
#include "WriterGlobalConfig.h"
#include "IOXml.h"
#include <QFile>
#include <QCoreApplication>
#include <QDebug>
#include "Common.h"
#include "ConfigOptions/GlobalConfig.h"
#include "ConfigOptions/GeometryConfig.h"
#include "ConfigOptions/MeshConfig.h"
#include <QFileInfo>

#pragma execution_character_set("utf-8")

namespace FastCAEDesigner
{
	WriterGlobalConfig::WriterGlobalConfig()
	{
		_fileName = _CONFIG_GLOBAL;
	}

	WriterGlobalConfig::~WriterGlobalConfig()
	{

	}

	/*************************************************************************************

	功能：保存GlobalConfig文件的信息
	参数：1、globalInfo：基本信息        
	返回值：xml节点
	日期：2019-11-15
	编辑：xvdongming
	备注：                 

	**************************************************************************************/
	bool WriterGlobalConfig::Write(ConfigOption::GlobalConfig* globalInfo, ConfigOption::GeometryConfig* geometryInfo, ConfigOption::MeshConfig* meshInfo)
	{
		QDomDocument doc;
		QDomProcessingInstruction instruction;
		instruction = doc.createProcessingInstruction("xml", "version=\"1.0\" encoding=\"UTF-8\"");
		doc.appendChild(instruction);
		QDomElement root = doc.createElement("GolbalConfig");
		WriteBasicPara(doc, root, globalInfo);
		WriteGeometryPara(doc, root, geometryInfo);
		WriteMeshPara(doc, root,meshInfo);
		WritePostPara(doc, root);
		doc.appendChild(root);
		SaveFile(doc);
		return true;
	}

	/*************************************************************************************

	功能：保存基本信息
	参数：1、doc：xml文档
	      2、root：xml文件根节点
		  3、basic：基本信息
	返回值：xml节点
	日期：2019-11-15
	编辑：xvdongming
	备注：                  

	**************************************************************************************/
	bool WriterGlobalConfig::WriteBasicPara(QDomDocument &doc, QDomElement &root, ConfigOption::GlobalConfig* globalConfig)
	{
		QDomElement global = IoXml::getInstance()->CreateElement(doc, "Global");
		QDomElement softInfo = IoXml::getInstance()->CreateElementAttribute(doc, "SoftName", "name", globalConfig->getSoftName(), "Chinese", globalConfig->getChineseName());
		QDomElement welcom = IoXml::getInstance()->CreateElement(doc, "Welcome", globalConfig->getWelcome());
		QDomElement version = IoXml::getInstance()->CreateElement(doc, "Version", globalConfig->getVersion());
		QDomElement mail = IoXml::getInstance()->CreateElement(doc, "Mail", globalConfig->getEMail());
		QDomElement corporation = IoXml::getInstance()->CreateElement(doc, "Corporation", globalConfig->getCorporation());
		QDomElement logo = IoXml::getInstance()->CreateElement(doc, "Logo", globalConfig->getLogo());
		QDomElement website = IoXml::getInstance()->CreateElement(doc, "Website", globalConfig->getWebsite());
		QDomElement describe = IoXml::getInstance()->CreateElement(doc, "Describe", "Memo");
		//QDomElement helpDoc = IoXml::getInstance()->CreateElement(doc, "HelpFile", globalConfig->GetUserManual());
		QDomElement helpDoc = IoXml::getInstance()->CreateElement(doc, "HelpFile", getUserManual(globalConfig->GetUserManual()));
		
		QString b = (globalConfig->isMaterialEnabled()) ? "true" : "false";
		QDomElement Material = IoXml::getInstance()->CreateElementAttribute(doc, "Material", "Enable", b);

		global.appendChild(softInfo);
		global.appendChild(logo);
		global.appendChild(welcom);
		global.appendChild(version);
		global.appendChild(mail);
		global.appendChild(corporation);
		global.appendChild(describe);
		global.appendChild(helpDoc);
		global.appendChild(website);
		
		root.appendChild(global);
		root.appendChild(Material);

		return true;
	}
	/*************************************************************************************

	功能：保存几何信息
	参数：1、doc：xml文档
	      2、root：xml文件根节点
	      3、basic：基本信息
	返回值：xml节点
	日期：2019-11-15
	编辑：xvdongming
	备注：

	**************************************************************************************/
	bool WriterGlobalConfig::WriteGeometryPara(QDomDocument &doc, QDomElement &root, ConfigOption::GeometryConfig* geometryConfig)
	{
		QDomElement geometry = IoXml::getInstance()->CreateElement(doc, "Geometry");
		//QDomElement importGeo = IoXml::getInstance()->CreateElementAttribute(doc, "ImportGeo", "Enable", "true");
		QDomElement importSuffix = IoXml::getInstance()->CreateElement(doc, "ImportSuffix", geometryConfig->getImportSuffix());
		QDomElement exportSuffix = IoXml::getInstance()->CreateElement(doc, "ExportSuffix", geometryConfig->getExportSuffix());
		//QDomElement suffix = IoXml::getInstance()->CreateElement(doc, "Suffix", "IGES(*.iges);; IGS(*.igs);; STEP(*.step);; STL(*.stl);; STP(*.stp)");
		QDomElement createGeo = IoXml::getInstance()->CreateElementAttribute(doc, "CreateGeo", "Enable", "true");
		//QDomElement geoOperation = IoXml::getInstance()->CreateElementAttribute(doc, "GeometryOperation", "Enable", "true");
		QDomElement createSketch = IoXml::getInstance()->CreateElementAttribute(doc, "CreateSketch", "Enable", geometryConfig->isCreateSketchEnabled());
		QDomElement featureModeling = IoXml::getInstance()->CreateElementAttribute(doc, "FeatureModeling", "Enable", geometryConfig->isGeometryModelingEnabled());
		QDomElement featureOperations = IoXml::getInstance()->CreateElementAttribute(doc, "FeatureOperations", "Enable", geometryConfig->isGeometryOperationsEnabled());
		geometry.appendChild(importSuffix);
		geometry.appendChild(exportSuffix);
		//importGeo.appendChild(suffix);
		//geometry.appendChild(importGeo);
		geometry.appendChild(createGeo);
		geometry.appendChild(createSketch);
		geometry.appendChild(featureModeling);
		geometry.appendChild(featureOperations);

		root.appendChild(geometry);
		return true;
	}

	/*************************************************************************************

	功能：保存网格信息
	参数：1、doc：xml文档
	      2、root：xml文件根节点
	      3、basic：基本信息
	返回值：xml节点
	日期：2019-11-15
	编辑：xvdongming
	备注：

	**************************************************************************************/
	bool WriterGlobalConfig::WriteMeshPara(QDomDocument &doc, QDomElement &root, ConfigOption::MeshConfig* meshConfig)
	{
		QDomElement Mesh = IoXml::getInstance()->CreateElement(doc, "Mesh");
		//QDomElement ImportMesh = IoXml::getInstance()->CreateElementAttribute(doc, "ImportMesh", "Enable", "true");
		QDomElement ImportSuffix = IoXml::getInstance()->CreateElement(doc, "ImportSuffix", meshConfig->getImportSuffix());
		//QDomElement ExportMesh = IoXml::getInstance()->CreateElementAttribute(doc, "ExportMesh", "Enable", "true");
		QDomElement ExportSuffix = IoXml::getInstance()->CreateElement(doc, "ExportSuffix", meshConfig->getExportSuffix());
		//QDomElement CreateGeo = IoXml::getInstance()->CreateElementAttribute(doc, "CreateGeo", "Enable", "true");
		QDomElement MeshGeneration = IoXml::getInstance()->CreateElementAttribute(doc, "MeshGeneration", "Enable", "true");
		QDomElement SurfaceMesh = IoXml::getInstance()->CreateElementAttribute(doc, "SurfaceMesh", "Enable", meshConfig->isSurfaceMeshEnabled());
		//QDomElement FluentMesh = IoXml::getInstance()->CreateElementAttribute(doc, "FluentMesh", "Enable", meshConfig->isFluidMeshEnabled());
		QDomElement SolidMesh = IoXml::getInstance()->CreateElementAttribute(doc, "SolidMesh", "Enable", meshConfig->isSolidMeshEnabled());
		QDomElement Component = IoXml::getInstance()->CreateElementAttribute(doc, "Component", "Enable", meshConfig->isComponentEnabled());
		QDomElement CheckMesh = IoXml::getInstance()->CreateElementAttribute(doc, "CheckMesh", "Enable", meshConfig->getCheckMeshEnabled());

		//Mesh.appendChild(ImportMesh);
		//Mesh.appendChild(ExportMesh);
		Mesh.appendChild(ImportSuffix);
		Mesh.appendChild(ExportSuffix);
		Mesh.appendChild(MeshGeneration);
		Mesh.appendChild(Component);
		Mesh.appendChild(CheckMesh);
		MeshGeneration.appendChild(SurfaceMesh);
		//MeshGeneration.appendChild(FluentMesh);
		MeshGeneration.appendChild(SolidMesh);

		root.appendChild(Mesh);
		return true;
	}

	/*************************************************************************************

	功能：保存后处理信息
	参数：1、doc：xml文档
	      2、root：xml文件根节点
	      3、basic：基本信息
	返回值：xml节点
	日期：2019-11-15
	编辑：xvdongming
	备注：

	**************************************************************************************/
	bool WriterGlobalConfig::WritePostPara(QDomDocument &doc, QDomElement &root)
	{
		QDomElement Post = IoXml::getInstance()->CreateElementAttribute(doc, "Post", "Enable", "true");
		QDomElement Post2D = IoXml::getInstance()->CreateElementAttribute(doc, "Post2D", "Enable", "true");
		QDomElement Post3D = IoXml::getInstance()->CreateElementAttribute(doc, "Post3D", "Enable", "true");
		Post.appendChild(Post2D);
		Post.appendChild(Post3D);
		root.appendChild(Post);
		return true;
	}

	//xuxinwei  20200306
	QString WriterGlobalConfig::getUserManual(QString surUserManual)
	{
		QString userManual{};
		if (surUserManual.isEmpty())
			return userManual;
		QFileInfo surNameFile(surUserManual);
		QStringList surNameList = surNameFile.fileName().split(".");
		QString surNameSuffix = surNameList.last();
		surUserManual.remove(surNameFile.fileName());

		QStringList desNamelist{};
		desNamelist.append(QString("UserManual"));
		desNamelist.append(surNameSuffix);

		QString desName = desNamelist.join(".");
		userManual = surUserManual + desName;

		return userManual;
	}

}