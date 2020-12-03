#include "translator.h"
#include <QApplication>
#include <QTranslator>
#include <QStringList>
#include <QDebug>
#include <assert.h>

namespace GUI
{
	const static QStringList Lang_List = 
	{
		":/translations/MainWindow_zh_CN",
		":/translations/MainWidgets_zh_CN",
		":/translations/Setting_zh_CN",
		":/translations/ModuleBase_zh_CN",
		":/translations/ProjectTree_zh_CN",
		":/translations/PostWidgets_zh_CN",
		":/translations/Material_zh_CN",
		":/translations/IO_zh_CN",
		":/translations/GeometryWidgets_zh_CN",
		":/translations/PluginManager_zh_CN",
		":/translations/GmshModule_zh_CN",
		":/translations/SolverControl_Zh_CN",
		":/translations/UserGuidence_zh_CN",
		":/translations/SelfDefObject_zh_CN",
	};

	Translator::Translator()
	{
		_app = static_cast<QApplication*>(QCoreApplication::instance());
		_translatorList.clear();
		const int n = Lang_List.size();
		for (int i = 0; i < n; ++i)
		{
			_translatorList.append(new QTranslator);
		}
	}
	void Translator::toChinese()
	{
		for (int i = 0; i < Lang_List.count(); ++i)
		{
			auto tranlator = _translatorList.at(i);
			_app->removeTranslator(tranlator);
			const QString lang = Lang_List.at(i);
			bool ok = tranlator->load(lang);
			qDebug() << lang;
			assert(ok);
			_app->installTranslator(tranlator);
		}
	}
	void Translator::toEnglish()
	{
		assert(_translatorList.count() == Lang_List.count());
		for (int i = 0; i < Lang_List.count(); ++i)
		{
			auto tranlator = _translatorList.at(i);
			_app->removeTranslator(tranlator);
		}
	}


}