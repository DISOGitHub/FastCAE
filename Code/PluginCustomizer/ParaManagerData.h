#ifndef PARAMANAGERDATA_H
#define PARAMANAGERDATA_H

#include <QList>
#include <QMap>
#include <QDomDocument>
#include "DataProperty/modelTreeItemType.h"

namespace DataProperty{
	class ParameterBase;
	class parametrGroup;
}

namespace FastCAEDesigner{
	class ParaLinkageData;

	class ParaManagerData
	{
	private:
		ParaManagerData();
		~ParaManagerData();

	public:
		static ParaManagerData* getInstance();

		//����linkageData�б�
		void setParameterLinkageList(int type, ParaLinkageData* data);
		//��ȡָ���б��е�ȷ��linkagData
		ParaLinkageData* getParameterLinkageData(int type, int index);
		//��ȡlinkageData���б�
		QList<ParaLinkageData*> getParameterLinkageList(int type);
		//��һ��linkageData�����ݴ�ָ���б����Ƴ�
		void removeParameterLinkageDataFromList(int type, int index);
		void removeParameterLinkageDataFromList(ParaLinkageData* data);

		
		void writeObserverConfig();
		void readObserverConfig();

		void setReadFlag(bool b);

	private:
		//дobserverconfig�ļ�
		void writexml(QDomDocument doc, QDomElement root, int type, QList<ParaLinkageData*> list);
		void writeActiveData(QDomDocument doc, QDomElement root, ParaLinkageData* data);
		void writeFollowData(QDomDocument doc, QDomElement root, ParaLinkageData* data);
		void writeDrivenData(QDomDocument doc, QDomElement driven, ParaLinkageData* data);
		void writeDrivenGroupData(QDomDocument doc, QDomElement root, ParaLinkageData* data);
		//�����������е�дxml
		void writeActieParamters(QDomDocument doc, QDomElement root, ParaLinkageData* data);
		void writeFollowParamters(QDomDocument doc, QDomElement root, ParaLinkageData* data);

		//��ȡobserverconfig����������
		QList<ParaLinkageData*> readObserverList(QDomElement* ele, ProjectTreeType type);
		void readObserver(QDomElement* ele, ProjectTreeType type, ParaLinkageData* data);

		void setActiveData(QDomElement* ele, DataProperty::ParameterBase* p);
		void setDrivenData(QDomElement* ele, DataProperty::ParameterBase* p);

		//bool���ͺ�QString����ת��
		QString dataBoolToQString(bool l);
		bool dataQStringToBool(QString s);

		//��ȡ�����Ĳ���ֵ
		QString getDataValue(DataProperty::ParameterBase* data);
		//�������Ϣ����map��
		QMap<QString, QString> insertValueToMap(QStringList var, QStringList val);

		//��ȡobserverconfig�ĵ�ַ
		QString getFileName();

	private:
		void clearQMap();
		
	private:
		static ParaManagerData* _instance;
		QMap<int, QList<ParaLinkageData*>> _paraLinkageDataListDict;
		bool _writeConfig{ false };
		bool _readConfig{ true };
	};
}
#endif // PARAMANAGERDATA_H


