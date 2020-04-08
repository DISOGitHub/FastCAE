#ifndef PARAMANAGERDATA_H
#define PARAMANAGERDATA_H

#include <QList>
#include <QMap>
#include <QDomDocument>

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

		void setParameterLinkageList(QString name, ParaLinkageData* data);
		ParaLinkageData* getParameterLinkageData(QString name, int index);
		QList<ParaLinkageData*> getParameterLinkageList(QString name);

		void removeParameterLinkageDataFromList(QString name, int index);
		void removeParameterLinkageDataFromList(ParaLinkageData* data);

		void writeObserverConfig();
		void readObserverConfig();
		void writexml(QDomDocument doc, QDomElement root, QString name, QList<ParaLinkageData*> list);
		void writeActiveData(QDomDocument doc, QDomElement root, ParaLinkageData* data);
		void writeFollowData(QDomDocument doc, QDomElement root, ParaLinkageData* data);
		void writeDrivenData(QDomDocument doc, QDomElement driven, ParaLinkageData* data);
		void writeDrivenGroupData(QDomDocument doc, QDomElement root, ParaLinkageData* data);

		QString dataBoolToQString(bool l);
		bool dataQStringToBool(QString s);

		QString getDataValue(DataProperty::ParameterBase* data);
		QMap<QString, QString> insertValueToMap(QStringList var, QStringList val);

	private:
		static ParaManagerData* _instance;

	//	QList<ParaLinkageData*> _paraLinkageDataList;
		QMap<QString, QList<ParaLinkageData*>> _paraLinkageDataListDict;
	};
}
#endif // PARAMANAGERDATA_H


