/**********************************************************************************

功能：基本信息保存
建立：xvdongming
日期：2019-11
修改和维护：xvdongming

**********************************************************************************/
#ifndef WRITERGLOBALCONFIG_H
#define WRITERGLOBALCONFIG_H

#include "WriterBase.h"
#include <QDomDocument>

namespace GUI
{
	class MainWindow;
}

namespace ConfigOption
{
	class GlobalConfig;
	class GeometryConfig;
	class MeshConfig;
}

namespace FastCAEDesigner
{
	class WriterGlobalConfig : public WriterBase
	{
	public:
		WriterGlobalConfig(GUI::MainWindow* mainWindow = nullptr);
		~WriterGlobalConfig();

	public:

		//保存GlobalConfig模块的所有信息
		bool Write(ConfigOption::GlobalConfig* globalInfo, ConfigOption::GeometryConfig* geometryInfo, ConfigOption::MeshConfig* meshInfo);

	private:
		//保存基本信息
		bool WriteBasicPara(QDomDocument &doc, QDomElement &root, ConfigOption::GlobalConfig* globalConfig);

		//保存几何信息
		bool WriteGeometryPara(QDomDocument &doc, QDomElement &root, ConfigOption::GeometryConfig* geometryConfig);
		//保存网格信息
		bool WriteMeshPara(QDomDocument &doc, QDomElement &root, ConfigOption::MeshConfig* meshConfig);
		//保存后处理信息
		bool WritePostPara(QDomDocument &doc, QDomElement &root);

		//更改用户手册名 xuxinwei 20200306 
		QString getUserManual(QString surName);
		GUI::MainWindow* _mainWindow{};
	};

}

#endif