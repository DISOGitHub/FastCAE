#ifndef SCALARRANGEDIALOG_H
#define SCALARRANGEDIALOG_H

#include <QDialog>
namespace Ui {
class scalarRangeDialog;
}

class scalarRangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit scalarRangeDialog(QWidget *parent = 0);
    ~scalarRangeDialog();
	void updateForm(double*);
private slots:
    void on_rescale_pushButton_clicked();

    void on_close_pushButton_clicked();

private:
    Ui::scalarRangeDialog *ui;
signals:
	void sig_update_scalarRange(double*);
};

#endif // SCALARRANGEDIALOG_H
