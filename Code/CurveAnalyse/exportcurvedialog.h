#ifndef EXPORTCURVEDIALOG_H
#define REPORTCURVEDIALOG_H

#include <QDialog>
#include <QMovie>
#include "qstring.h"
#include "exportCurveThread.h"
namespace Ui {
class ExportCurveDialog;
}

class ExportCurveDialog : public QDialog
{
    Q_OBJECT

public:
	explicit ExportCurveDialog(QWidget *parent = 0);
	~ExportCurveDialog();
	void DisplayMovie(QString MovieMsg);
	ExportCurveThread* exportThreadPtr;
private:
	Ui::ExportCurveDialog *ui;
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
