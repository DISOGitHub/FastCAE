/********************************************************************************
** Form generated from reading UI file 'selfdeflineedit.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SELFDEFLINEEDIT_H
#define UI_SELFDEFLINEEDIT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SelfDefLineEdit
{
public:
    QGridLayout *gridLayout;
    QLineEdit *lineEdit;

    void setupUi(QWidget *SelfDefLineEdit)
    {
        if (SelfDefLineEdit->objectName().isEmpty())
            SelfDefLineEdit->setObjectName(QStringLiteral("SelfDefLineEdit"));
        SelfDefLineEdit->resize(400, 21);
        gridLayout = new QGridLayout(SelfDefLineEdit);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        lineEdit = new QLineEdit(SelfDefLineEdit);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        gridLayout->addWidget(lineEdit, 0, 0, 1, 1);


        retranslateUi(SelfDefLineEdit);

        QMetaObject::connectSlotsByName(SelfDefLineEdit);
    } // setupUi

    void retranslateUi(QWidget *SelfDefLineEdit)
    {
        SelfDefLineEdit->setWindowTitle(QApplication::translate("SelfDefLineEdit", "Form", 0));
    } // retranslateUi

};

namespace Ui {
    class SelfDefLineEdit: public Ui_SelfDefLineEdit {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SELFDEFLINEEDIT_H
