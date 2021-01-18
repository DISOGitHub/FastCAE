#ifndef XRANDOMWIDGET_H
#define XRANDOMWIDGET_H
#include "ui_RandomWidget.h"
#include <QDialog>
#include <QList>


#define _MAX_TRY_TIME_		10000												
class XRandomWidget : public QDialog
{
	Q_OBJECT

public:
	explicit XRandomWidget(QDialog *parent = 0);
	~XRandomWidget();

	QList<double> getRandomValues() const;

	void setMode(int mode);

	void setRange(int min , int max);

	void setInitSize(QSize nSize);

protected:
	virtual QSize sizeHint() const override;

private:
	Ui::Dialog ui;
	QList<QWidget *> m_gaussWidgets;
	QList<QWidget *> m_exponentialWidgets;
	QList<QWidget *> m_randomParaWidgets;
	QList<double> m_randomValues;
	QList<QLineEdit *> m_lineEdits;
	int m_min{ 0 }, m_max{0};
	QSize m_initSize{ QSize() };

	private slots:
	void slot_clicked_btn_random();
	void slot_clicked_radio_btn(bool bCheck);

	void slot_check_value();

	
private:
	void init();

	void accept();
};

#endif // XRANDOMWIDGET_H
