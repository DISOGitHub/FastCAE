#include "loadfiledialog.h"
#include "ui_loadfiledialog.h"
//#include "QDir"
//#include <qmovie.h>
//#include <QKeyEvent>
//#include <QDebug>


LoadFileDialog::LoadFileDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::LoadFileDialog)
{
	ui->setupUi(this);

	//	ui->label->setPixmap(QPixmap(QString::fromUtf8(":/images/Wave angle_1.png")));

	movie = new QMovie(QString::fromUtf8(":/images/water_drop.gif"));
	//	ui->label->setAutoFillBackground(true);
}
void LoadFileDialog::DisplayMovie(QString MovieMsg)
{
	mMovieMsg = MovieMsg;
	ui->label_2->setText(MovieMsg);
	ui->label->setMovie(movie);
	movie->start();

	//	ui->label_2->setText(QStringLiteral("海洋作业平台数值试验报告生成中......"));
	//	ui->label_2->setText(glo_ship_name + QStringLiteral("数值试验报告生成中......"));
	this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
}
LoadFileDialog::~LoadFileDialog()
{
	movie->stop();
	delete movie;
	delete ui;
}

/*
void LoadFileDialog::stopMovie()
{
	ui->label_2->setText(glo_ship_name+QStringLiteral("数值试验报告生成完毕!"));
	movie->stop();
}
*/
/*
void LoadFileDialog::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Escape:
		break;
	default:
		QDialog::keyPressEvent(event);
	}
}

*/