#ifndef _PARAMETEROBSERVER_H_
#define _PARAMETEROBSERVER_H_

#include "ConfigOptionsAPI.h"
#include <QHash>
#include <QString>
#include <QStringList>
#include <QObject>

namespace DataProperty
{
	class ParameterBase;
	class ParameterGroup;
}

namespace ConfigOption
{

	class CONFIGOPTIONSAPI ParameterObserver : public QObject
	{
		Q_OBJECT

	public:
		ParameterObserver() = default;
		~ParameterObserver();

		ParameterObserver* copy();

		void appendCofigActive(QString name, DataProperty::ParameterBase* p);
		void appendConfigFollow(QString name, DataProperty::ParameterBase* p);
		void appendConfigFollowGroup(QString name, DataProperty::ParameterGroup* p);

		void removeConfigActive(QString name);
		void removeConfigFollow(QString name);
		void removeConfigFollowGroup(QString name);

		QStringList getActiveParameterNames();
		QStringList getFollowParameterNames();
		QStringList getFollowGroupNames();

		void appendDataActive(QString name, DataProperty::ParameterBase*p);
		void appendDataFollow(QString name, DataProperty::ParameterBase*p);
		void appendDataFollowGroup(QString name, DataProperty::ParameterGroup* p);

	signals:
		void updateDisplay();

	public slots:
		void observe();

	private:
		bool check();
		void changeStates();

	private:
		QHash<QString, DataProperty::ParameterBase*> _configActive{};
		QHash<QString, DataProperty::ParameterBase*> _configfollow{};
		QHash<QString, DataProperty::ParameterGroup*> _configfollowGroup{};

		QHash<QString, DataProperty::ParameterBase*> _dataActive{};
		QHash<QString, DataProperty::ParameterBase*> _datafollow{};
		QHash<QString, DataProperty::ParameterGroup*> _datafollowGroup{};
	};


}


#endif
