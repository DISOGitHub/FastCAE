/********************************************************************************
** Form generated from reading UI file 'vtkQtRichTextView.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_VTKQTRICHTEXTVIEW_H
#define UI_VTKQTRICHTEXTVIEW_H

#include <QtCore/QVariant>
#include <QtWebKitWidgets/QWebView>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_vtkQtRichTextView
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *Title;
    QSpacerItem *horizontalSpacer;
    QToolButton *BackButton;
    QToolButton *ForwardButton;
    QToolButton *ZoomIn;
    QToolButton *ZoomReset;
    QToolButton *ZoomOut;
    QWebView *WebView;

    void setupUi(QWidget *vtkQtRichTextView)
    {
        if (vtkQtRichTextView->objectName().isEmpty())
            vtkQtRichTextView->setObjectName(QStringLiteral("vtkQtRichTextView"));
        vtkQtRichTextView->resize(400, 300);
        verticalLayout = new QVBoxLayout(vtkQtRichTextView);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        Title = new QLabel(vtkQtRichTextView);
        Title->setObjectName(QStringLiteral("Title"));

        horizontalLayout->addWidget(Title);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        BackButton = new QToolButton(vtkQtRichTextView);
        BackButton->setObjectName(QStringLiteral("BackButton"));
        BackButton->setArrowType(Qt::LeftArrow);

        horizontalLayout->addWidget(BackButton);

        ForwardButton = new QToolButton(vtkQtRichTextView);
        ForwardButton->setObjectName(QStringLiteral("ForwardButton"));
        ForwardButton->setArrowType(Qt::RightArrow);

        horizontalLayout->addWidget(ForwardButton);

        ZoomIn = new QToolButton(vtkQtRichTextView);
        ZoomIn->setObjectName(QStringLiteral("ZoomIn"));

        horizontalLayout->addWidget(ZoomIn);

        ZoomReset = new QToolButton(vtkQtRichTextView);
        ZoomReset->setObjectName(QStringLiteral("ZoomReset"));

        horizontalLayout->addWidget(ZoomReset);

        ZoomOut = new QToolButton(vtkQtRichTextView);
        ZoomOut->setObjectName(QStringLiteral("ZoomOut"));

        horizontalLayout->addWidget(ZoomOut);


        verticalLayout->addLayout(horizontalLayout);

        WebView = new QWebView(vtkQtRichTextView);
        WebView->setObjectName(QStringLiteral("WebView"));
        WebView->setUrl(QUrl(QStringLiteral("about:blank")));

        verticalLayout->addWidget(WebView);


        retranslateUi(vtkQtRichTextView);

        QMetaObject::connectSlotsByName(vtkQtRichTextView);
    } // setupUi

    void retranslateUi(QWidget *vtkQtRichTextView)
    {
        vtkQtRichTextView->setWindowTitle(QApplication::translate("vtkQtRichTextView", "Form", 0));
        Title->setText(QString());
#ifndef QT_NO_TOOLTIP
        BackButton->setToolTip(QApplication::translate("vtkQtRichTextView", "Go back one page", 0));
#endif // QT_NO_TOOLTIP
        BackButton->setText(QApplication::translate("vtkQtRichTextView", "Back", 0));
#ifndef QT_NO_TOOLTIP
        ForwardButton->setToolTip(QApplication::translate("vtkQtRichTextView", " Go forward one page", 0));
#endif // QT_NO_TOOLTIP
        ForwardButton->setText(QApplication::translate("vtkQtRichTextView", "Forward", 0));
        ZoomIn->setText(QApplication::translate("vtkQtRichTextView", "+", 0));
        ZoomReset->setText(QApplication::translate("vtkQtRichTextView", "100%", 0));
        ZoomOut->setText(QApplication::translate("vtkQtRichTextView", "-", 0));
    } // retranslateUi

};

namespace Ui {
    class vtkQtRichTextView: public Ui_vtkQtRichTextView {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VTKQTRICHTEXTVIEW_H
