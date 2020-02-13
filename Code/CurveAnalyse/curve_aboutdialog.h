#ifndef CURVEABOUTDIALOG_H
#define CURVEABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class curve_aboutDialog;
}

class curve_aboutDialog : public QDialog
{
    Q_OBJECT

public:
	explicit curve_aboutDialog(QWidget *parent = 0);
	~curve_aboutDialog();

private slots:
    void on_pushButton_clicked();

private:
	Ui::curve_aboutDialog *ui;
};

#endif // ABOUTDIALOG_H
