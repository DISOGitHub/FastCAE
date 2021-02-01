#ifndef SELFDEFLINEEDIT_H
#define SELFDEFLINEEDIT_H

#include <QWidget>

namespace Ui {
class SelfDefLineEdit;
}

class SelfDefLineEdit : public QWidget
{
    Q_OBJECT

public:
    explicit SelfDefLineEdit(QWidget *parent = 0);
    ~SelfDefLineEdit();

private:
    Ui::SelfDefLineEdit *ui;
};

#endif // SELFDEFLINEEDIT_H
