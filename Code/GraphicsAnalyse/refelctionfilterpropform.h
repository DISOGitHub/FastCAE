#ifndef REFELCTIONFILTERPROPFORM_H
#define REFELCTIONFILTERPROPFORM_H

#include <QWidget>
#include "graphicsanalyse_global.h"
#include "PipelineObject.h"
#include "pipelineobjpropwidget.h"
namespace Ui {
class refelctionFilterPropForm;
}
class refelctionFilterPropForm : public QWidget
{
    Q_OBJECT

public:
	explicit refelctionFilterPropForm(PipelineObjPropWidget *parent = 0);
    ~refelctionFilterPropForm();
	void updateForm();
	struct reflectionFilter_pipelineObj_propData *reflectionFilter_propData;
	PipelineObject* cur_pipelineObj;
	virtual void reTranslate();
private slots:
    void on_comboBox_currentIndexChanged(int index);
    void on_copyInput_checkBox_clicked(bool checked);

signals:
	void sig_enable_applyBtn();
private:
    Ui::refelctionFilterPropForm *ui;
	PipelineObjPropWidget* _parentWin{};
};

#endif // REFELCTIONFILTERPROPFORM_H
