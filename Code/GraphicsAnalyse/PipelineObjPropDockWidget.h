#ifndef PIPELINEOBJPROPDOCKWIDGET_H
#define PIPELINEOBJPROPDOCKWIDGET_H
#include <QWidget>
#include <QDockWidget>
#include "pipelineobjpropwidget.h"

class PipelineObjPropDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit PipelineObjPropDockWidget(QWidget *parent = 0);
    ~PipelineObjPropDockWidget();
	PipelineObjPropWidget* get_propWidget(){ return propWidget; };
	virtual void reTranslate();
	void setDockSize(int w,int h)
	{
		wid = w;
		hei = h;
		propWidget->resize(w, h);
		//sizeHint();
	}
	QSize sizeHint() const
	{
		return QSize(wid,hei);
	}
private:
	PipelineObjPropWidget* propWidget;
	int wid, hei;
};

#endif // PIPELINEOBJPROPDOCKWIDGET_H
