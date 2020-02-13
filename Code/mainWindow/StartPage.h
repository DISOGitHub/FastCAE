#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QWidget>
#include <QUrl>
#include <QNetworkRequest>;

class QNetworkReply;

namespace Ui
{
	class StartPage;
}
namespace GUI
{
	class StartPage : public QWidget
	{
		Q_OBJECT
	public:
		StartPage();
		~StartPage();
		void load(QString web);
		void reTranslate();
	signals:
		void closePage();
	
	private slots:
	   void linkClicked(QUrl);

	private:
		Ui::StartPage* _ui{};
	};
}


#endif