#include "exportfiledialog.h"
#include "ui_exportfiledialog.h"

ExportFileDialog::ExportFileDialog(QWidget *parent) :
QDialog(parent),
ui(new Ui::ExportFileDialog)
{
	ui->setupUi(this);
	exportThreadPtr = NULL;
	movie = NULL;
	//setAttribute(Qt::WA_DeleteOnClose);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	//setAttribute(Qt::WA_TranslucentBackground);
}
void ExportFileDialog::DisplayMovie(QString MovieMsg)
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
void ExportFileDialog::slot_changeExportText(QString MovieMsg)
{
	ui->label_text->setText(MovieMsg);
	movie->stop();
	ui->pushButton->setText(tr("close"));
}
ExportFileDialog::~ExportFileDialog()
{	
	delete ui;
}
void ExportFileDialog::on_pushButton_clicked()
{
	ui->label_text->setText(tr("file export cancle!"));
	emit sig_stopFileExport();
	if (exportThreadPtr != NULL)
	{
		exportThreadPtr->stop();
		//exportThreadPtr->wait();
	}
	movie->stop();
	delete movie;
	movie = NULL;
	close();
}
