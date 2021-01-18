#include "DialogAbout.h"
#include "ui_DialogAbout.h"
#include <QCoreApplication>
#include "ConfigOptions/ConfigOptions.h"
#include "ConfigOptions/GlobalConfig.h"

namespace GUI
{
	AboutDialog::AboutDialog() : _ui(new Ui::AboutDialog)
	{
		_ui->setupUi(this);
		Qt::WindowFlags flags = Qt::Dialog;
		flags |= Qt::WindowCloseButtonHint;
		setWindowFlags(flags);
		init();
	}
	AboutDialog::~AboutDialog()
	{
		if (_ui != nullptr) delete _ui;
	}
	void AboutDialog::init()
	{
		ConfigOption::GlobalConfig* g = ConfigOption::ConfigOption::getInstance()->getGlobalConfig();

		QString logo = g->getLogo();
		QString ver = g->getVersion();
		QString softname = g->getSoftName();
		QString cor = g->getCorporation();
		QString web = g->getWebsite();
		QString e = g->getEMail();
		QString wel = g->getWelcome();

		_ui->groupBox->setTitle(softname + " " + ver);
		_ui->corporation->setText(cor);
		_ui->website->setOpenExternalLinks(true);
		QString text = QString::fromLocal8Bit("<a style='color: blue;' href = %1>%1</a>").arg(web);
		_ui->website->setText(text);

		logo = qApp->applicationDirPath() + "/../ConfigFiles/Icon/" + logo;
		_ui->pic->setPixmap(logo);
// 		wel = qApp->applicationDirPath() + "/../ConfigFiles/Icon/" + logo;
// 		_ui->pic->setPixmap(wel);

		_ui->email->setText(e);
	}

}