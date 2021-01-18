#include "BCConfig.h"
#include "BCBase/BCUserDef.h"
#include <QDebug>

namespace ConfigOption
{
	BCConfig::~BCConfig()
	{
		clearData();
	}
	void BCConfig::clearData()
	{
		QList<ProjectTreeType> types = _bcList.keys();
		for (int j = 0; j < types.size(); ++j)
		{
			QList<BCBase::BCUserDef*> bcList = _bcList.value(types.at(j));
			const int n = bcList.size();
			for (int i = 0; i < n; ++i)
			{
				BCBase::BCUserDef* b = bcList.at(i);
				delete b;
			}
		}
		_bcList.clear();
		_enableType.clear();

	}

	BCBase::BCUserDef* BCConfig::getBCByName(QString name, ProjectTreeType t)
	{

		BCBase::BCUserDef* bc = nullptr;
		if (!_bcList.contains(t)) return bc;

		QList<BCBase::BCUserDef*> bcList = _bcList.value(t);
		const int n = bcList.size();

		for (int i = 0; i < n; ++i)
		{
			BCBase::BCUserDef* b = bcList.at(i);
			if (b->getName() == name)
			{
				bc = b;
				break;
			}
		}
		return bc;
	}

	void BCConfig::appendBC(BCBase::BCUserDef* bc, ProjectTreeType t)
	{
		if (bc != nullptr)
			_bcList[t].append(bc);
	}
	int BCConfig::getBCCount(ProjectTreeType t)
	{
		if (!_bcList.contains(t)) return 0;
		QList<BCBase::BCUserDef*> bcList = _bcList.value(t);
		return bcList.size();
	}
	BCBase::BCUserDef* BCConfig::getBCAt(const int index, ProjectTreeType t)
	{
		if (!_bcList.contains(t)) return nullptr;
		QList<BCBase::BCUserDef*> bcList = _bcList.value(t);
		if (index < 0 || index >= bcList.size()) return nullptr;
		return bcList.at(index);

	}
	bool BCConfig::isEnable(QString type, ProjectTreeType t)
	{
		bool s = false;
		if (!_enableType.contains(t)) return false;
		QStringList enableType = _enableType.value(t);
		if (enableType.contains(type.toLower()))
			s = true;
		return s;
	}
	void BCConfig::appendEnableType(QString s, ProjectTreeType t)
	{
		if (this->isEnable(s, t)) return;
		_enableType[t].append(s.toLower());
		//		qDebug() << t << " , " << s;
	}

	QStringList BCConfig::getEnabledType(ProjectTreeType t)
	{
		if (_enableType.contains(t))
			return _enableType.value(t);
		return QStringList();
	}

}