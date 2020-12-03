#ifndef PICKCURVEDOCKWIDGET_H
#define PICKCURVEDOCKWIDGET_H
#include <QDockWidget>
#include "CurveAnalyse/curve_mainwindow.h"
class pickCurveDockWidget : public QDockWidget
{
    Q_OBJECT
public:
	explicit pickCurveDockWidget(QWidget *parent = 0);
	~pickCurveDockWidget();
	void setDiaplayCurveName(QString tep_name);
	virtual void reTranslate();
public slots:
	void slot_del_3d_curve();
signals:
private slots :
	void slot_draw_3d_curve(QString tep_curve_name, QVector<double> data);
	void slot_update_3d_curve(int tep_file_index,int tep_column_index,double val_x,double val_y);

private:
	curve_MainWindow* curve_widget;
	QString curve_name;
};

#endif // PICKCURVEDOCKWIDGET_H
