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

		//设置linkageData列表
		void setParameterLinkageList(int type, ParaLinkageData* data);
		//获取指定列表中的确定linkagData
		ParaLinkageData* getParameterLinkageData(int type, int index);
		//获取linkageData的列表
		QList<ParaLinkageData*> getParameterLinkageList(int type);
		//将一个linkageData的数据从指定列表中移除
		void removeParameterLinkageDataFromList(int type, int index);
		void removeParameterLinkageDataFromList(ParaLinkageData* data);

		
		void writeObserverConfig();
		void readObserverConfig();

		void setReadFlag(bool b);

	private:
		//写observerconfig文件
		void writexml(QDomDocument doc, QDomElement root, int type, QList<ParaLinkageData*> list);
		void writeActiveData(QDomDocument doc, QDomElement root, ParaLinkageData* data);
		void writeFollowData(QDomDocument doc, QDomElement root, ParaLinkageData* data);
		void writeDrivenData(QDomDocument doc, QDomElement driven, ParaLinkageData* data);
		void writeDrivenGroupData(QDomDocument doc, QDomElement root, ParaLinkageData* data);
		//调用主函数中的写xml
		void writeActieParamters(QDomDocument doc, QDomElement root, ParaLinkageData* data);
		void writeFollowParamters(QDomDocument doc, QDomElement root, ParaLinkageData* data);

		//读取observerconfig并设置数据
		QList<ParaLinkageData*> readObserverList(QDomElement* ele, ProjectTreeType type);
		void readObserver(QDomElement* ele, ProjectTreeType type, ParaLinkageData* data);

		void setActiveData(QDomElement* ele, DataProperty::ParameterBase* p);
		void setDrivenData(QDomElement* ele, DataProperty::ParameterBase* p);

		//bool类型和QString类型转化
		QString dataBoolToQString(bool l);
		bool dataQStringToBool(QString s);

		//获取参数的参数值
		QString getDataValue(DataProperty::ParameterBase* data);
		//将相关信息插入map中
		QMap<QString, QString> insertValueToMap(QStringList var, QStringList val);

		//获取observerconfig的地址
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


