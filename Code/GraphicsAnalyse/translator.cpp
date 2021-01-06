#include "translator.h"
#include <QApplication>
#include <QTranslator>
#include <QStringList>
#include <QDebug>
#include <assert.h>

const static QStringList Lang_List = 
{
	":/translations/GraphicsAnalyse_zh_CN",
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
		//assert(ok);
		if (ok)
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