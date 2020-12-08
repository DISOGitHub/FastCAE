#include "exportcurvedialog.h"
#include "ui_exportcurvedialog.h"

ExportCurveDialog::ExportCurveDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::ExportCurveDialog)
{
	ui->setupUi(this);
	movie = NULL;
	exportThreadPtr = NULL;
	//setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);//ÎÞ±ß¿ò 
	//setAttribute(Qt::WA_TranslucentBackground);//±³¾°Í¸Ã÷
}
void ExportCurveDialog::DisplayMovie(QString MovieMsg)
{
	if (movie == NULL)
	{
		movie = new QMovie(QString::fromUtf8(":/images/exportfile.gif"));
		movie->setSpeed(100);
	}
	ui->label_text->setText(MovieMsg);
	ui->label_gif->setMovie(movie);
	movie->start();
}
void ExportCurveDialog::slot_changeExportText(QString MovieMsg)
{
	ui->label_text->setText(MovieMsg);
	movie->stop();
	if (MovieMsg == tr("file exported succeed!"))
		ui->pushButton->setText(tr("close"));
	//delete movie;
	//movie = NULL;
}
ExportCurveDialog::~ExportCurveDialog()
{		
	delete ui;
}
void ExportCurveDialog::on_pushButton_clicked()
{
	ui->label_text->setText(tr("file export cancle!"));
	emit sig_stopFileExport();
	if (exportThreadPtr != NULL)
	{
		exportThreadPtr->wait();
	}
	movie->stop();
	delete movie;
	movie = NULL;
	close();
}