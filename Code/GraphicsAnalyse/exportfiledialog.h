#ifndef EXPORTFILEDIALOG_H
#define REPORTFILEDIALOG_H

#include <QDialog>
#include <QMovie>
#include "qstring.h"
#include "exportFileThread.h"
namespace Ui {
class ExportFileDialog;
}

class ExportFileDialog : public QDialog
{
    Q_OBJECT

public:
	explicit ExportFileDialog(QWidget *parent = 0);
	~ExportFileDialog();
	void DisplayMovie(QString MovieMsg);
	ExportFileThread* exportThreadPtr;
private:
	Ui::ExportFileDialog *ui;
	QMovie *movie;
	
private slots:
	void slot_changeExportText(QString MovieMsg);
//	void stopMovie();
//	void keyPressEvent(QKeyEvent *event);
    void on_pushButton_clicked();

signals:
	void sig_stopFileExport();
};

#endif // REPORTFILEDIALOG_H
