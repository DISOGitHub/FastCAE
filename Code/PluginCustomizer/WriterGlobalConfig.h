/**********************************************************************************

���ܣ�������Ϣ����
������xvdongming
���ڣ�2019-11
�޸ĺ�ά����xvdongming

**********************************************************************************/
#ifndef WRITERGLOBALCONFIG_H
#define WRITERGLOBALCONFIG_H

#include "WriterBase.h"
#include <QDomDocument>

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
		WriterGlobalConfig();
		~WriterGlobalConfig();

	public:

		//����GlobalConfigģ���������Ϣ
		bool Write(ConfigOption::GlobalConfig* globalInfo, ConfigOption::GeometryConfig* geometryInfo, ConfigOption::MeshConfig* meshInfo);

	private:
		//���������Ϣ
		bool WriteBasicPara(QDomDocument &doc, QDomElement &root, ConfigOption::GlobalConfig* globalConfig);

		//���漸����Ϣ
		bool WriteGeometryPara(QDomDocument &doc, QDomElement &root, ConfigOption::GeometryConfig* geometryConfig);
		//����������Ϣ
		bool WriteMeshPara(QDomDocument &doc, QDomElement &root, ConfigOption::MeshConfig* meshConfig);
		//���������Ϣ
		bool WritePostPara(QDomDocument &doc, QDomElement &root);

		//�����û��ֲ��� xuxinwei 20200306 
		QString getUserManual(QString surName);
	};

}

#endif