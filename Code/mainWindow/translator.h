#ifndef _TRANSLATER_H_
#define _TRANSLATER_H_

#include <QObject>
#include <QList>

class QApplication;
class QTranslator;

namespace GUI
{
	class Translator : public QObject
	{
		Q_OBJECT
	public:
		Translator();
		~Translator() = default;
	public slots:
		void toEnglish();
		void toChinese();

	private:
		QApplication* _app;
		QList<QTranslator*> _translatorList;



	};
}


#endif
