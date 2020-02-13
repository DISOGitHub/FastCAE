#ifndef CONTOURFILTERPROPFORM_H
#define CONTOURFILTERPROPFORM_H

#include <QWidget>
#include "graphicsanalyse_global.h"
#include "PipelineObject.h"
#include "pipelineobjpropwidget.h"
namespace Ui {
class contourFilterPropForm;
}

class GRAPHICSANALYSESHARED_EXPORT contourFilterPropForm : public QWidget
{
    Q_OBJECT

public:
	explicit contourFilterPropForm(PipelineObjPropWidget *parent = 0);
    ~contourFilterPropForm();
	void updateForm();
	struct contourFilter_pipelineObj_propData *contourFilter_propData;
	PipelineObject* cur_pipelineObj;
	virtual void reTranslate();
signals:
	void sig_enable_applyBtn();
private slots:

    void on_val_spinbox_valueChanged(double arg1);

    void on_comboBox_currentIndexChanged(int index);

    void on_reflection_checkBox_clicked(bool checked);

    void on_reflection_comboBox_currentIndexChanged(int index);

private:
    Ui::contourFilterPropForm *ui;
	PipelineObjPropWidget* _parentWin{};
};

#endif // CONTOURFILTERPROPFORM_H
