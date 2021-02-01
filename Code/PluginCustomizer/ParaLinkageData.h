#ifndef PARALINKAGEDATA_H
#define PARALINKAGEDATA_H

#include <QString>
#include <QList>

namespace DataProperty
{
	class ParameterGroup;
	class ParameterBase;
}

namespace FastCAEDesigner{
	class ParaLinkageData
	{
	public:
		ParaLinkageData();
		~ParaLinkageData();

		void setParameterName(QString name);
		void setParameterType(int type);
		void setActiveList(QList<DataProperty::ParameterBase*> list);
		void setDrivenList(QList<DataProperty::ParameterBase*> list);
		void setDrivenGroupList(QList<DataProperty::ParameterGroup*> list);

		QString getParameterName();
		int getParameterType();
		QList<DataProperty::ParameterBase*> getActiveList();
		QList<DataProperty::ParameterBase*> getDrivenList();
		QList<DataProperty::ParameterGroup*> getDrivenGroupList();


	private:
		QString _name;
		int _type;
		QList<DataProperty::ParameterBase*> _activeList;
		QList<DataProperty::ParameterBase*> _drivenList;
		QList<DataProperty::ParameterGroup*> _drivenGroupList;
	};
}

#endif // PARALINKAGEDATA_H


