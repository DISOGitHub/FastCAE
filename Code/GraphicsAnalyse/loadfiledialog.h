#ifndef LOADFILEDIALOG_H
#define LOADFILEDIALOG_H

#include <QDialog>
#include <QMovie>
#include "qstring.h"

namespace Ui {
class LoadFileDialog;
}

class LoadFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoadFileDialog(QWidget *parent = 0);
    ~LoadFileDialog();
	QString mMovieMsg;//�Ի����м���ʾ��Ϣ
	void DisplayMovie(QString MovieMsg);
private:
    Ui::LoadFileDialog *ui;
	QMovie *movie;
	
private slots:
//	void stopMovie();
//	void keyPressEvent(QKeyEvent *event);
};

#endif // LOADFILEDIALOG_H
