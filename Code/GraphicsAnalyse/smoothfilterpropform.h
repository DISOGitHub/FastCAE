#ifndef SMOOTHFILTERPROPFORM_H
#define SMOOTHFILTERPROPFORM_H

#include <QWidget>
#include "graphicsanalyse_global.h"
#include "PipelineObject.h"
#include "pipelineobjpropwidget.h"
namespace Ui {
class smoothFilterPropForm;
}
class smoothFilterPropForm : public QWidget
{
    Q_OBJECT

public:
	explicit smoothFilterPropForm(PipelineObjPropWidget *parent = 0);
    ~smoothFilterPropForm();
	void updateForm();
	struct smoothFilter_pipelineObj_propData *smoothFilter_propData;
	PipelineObject* cur_pipelineObj;
	virtual void reTranslate();
private slots:
    void on_coef_spinBox_valueChanged(int arg1);
    void on_cellToPoint_radioButton_clicked(bool checked);

    void on_meshSmooth_radioButton_clicked(bool checked);

signals:
	void sig_enable_applyBtn();
private:
    Ui::smoothFilterPropForm *ui;
	PipelineObjPropWidget* _parentWin{};
};

#endif // SMOOTHFILTERPROPFORM_H
