#include "StartPage.h"
#include "ui_StartPage.h"
#include "DownloadManager.h"
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/GlobalConfig.h"
#include <QCoreApplication>
#include <QNetworkReply>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>
#include <QString>
#include <QtWebView/QtWebView>
#include <QWebEngineView>

namespace GUI
{

	StartPage::StartPage()
	{
#ifdef Q_OS_WIN32
		QtWebView::initialize();
		_ui = new Ui::StartPage;
		_ui->setupUi(this);
//		_ui->webView->page()->setForwardUnsupportedContent(true);
//		_ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);

		//connect(_ui->webView->page(), SIGNAL(linkClicked(QUrl)), this, SLOT(linkClicked(QUrl)));

		QString html = QCoreApplication::applicationDirPath() + "/FastCAE.html";
		QFile f(html);
		
		if (f.exists())
		{
			_ui->webView->load(QUrl::fromLocalFile(html));
		}
		else
		{
			QString web = "http://www.diso.cn/";
			ConfigOption::GlobalConfig* g = ConfigOption::ConfigOption::getInstance()->getGlobalConfig();
			QString w = g->getWebsite();
			if (!w.isEmpty())
				web = w;
			this->load(web);
		}
		 //  _ui->webView->load(QUrl("http://www.fastcae.com/")); 
#endif
	}
	StartPage::~StartPage()
	{
		if (_ui != nullptr) delete _ui;
		emit closePage();
	}

	/*//void StartPage::download(const QNetworkRequest& aaa)
	{
		/ *qDebug() << "2018 11 22";
		/ *QNetworkRequest request;
		request.setUrl(aaa.url());* /
		QNetworkAccessManager m_pManager(this);
		QNetworkReply* m_pReply = m_pManager.get(aaa);
		QString fileName = qApp->applicationDirPath() + "/../Download/123.exe";
		QFile m_pFile(fileName);
		if (!m_pFile.open(QIODevice::WriteOnly))
		{
			return;
		}
		m_pFile.write(m_pReply->readAll());
		m_pFile.close();* /
	}

	void StartPage::test(QUrl qurl)
	{
	}

	void StartPage::lick(const QUrl &qurl)
	{
		QString q = qurl.toString();
		qDebug() << q;
		//qDebug()<<
	}*/

	void StartPage::reTranslate()
	{
		_ui->retranslateUi(this);
	}


	void StartPage::linkClicked(QUrl url)
	{
#ifdef Q_OS_WIN32
		qDebug() << url;
		QString path = url.path();
		if (path.contains(".qq.")) return;
		QFileInfo file(path);
		QString suffix = file.suffix();
		if (!suffix.contains("zip"))
		{
			_ui->webView->load(url);
			return;
		}

		QString f= QFileDialog::getSaveFileName(nullptr, "Save As", "", QString("%1(*.%1)").arg(suffix));
		if (f.isEmpty())
		{
			_ui->webView->load(url);
			return;
		}
		DownloadManager *ma = new DownloadManager;
		ma->download(url, f);
#endif
	}

	void StartPage::load(QString web)
	{
#ifdef Q_OS_WIN32
		if (web.isEmpty())
			web = "www.fastcae.com";
		if (!web.toLower().startsWith("http"))
			web = "http://" + web;
		_ui->webView->load(QUrl(web));
#endif
	}

} 
