#ifndef VECTORFILTERPROPFORM_H
#define VECTORFILTERPROPFORM_H

#include <QWidget>
#include "graphicsanalyse_global.h"
#include "PipelineObject.h"
#include "pipelineobjpropwidget.h"
namespace Ui {
class vectorFilterPropForm;
}
class GRAPHICSANALYSESHARED_EXPORT vectorFilterPropForm : public QWidget
{
    Q_OBJECT

public:
	explicit vectorFilterPropForm(PipelineObjPropWidget *parent = 0);
    ~vectorFilterPropForm();
	void updateForm();
	struct vectorFilter_pipelineObj_propData *vectorFilter_propData;
	PipelineObject* cur_pipelineObj;
	virtual void reTranslate();
private slots:
    void on_glyphType_comboBox_currentIndexChanged(int index);

    void on_scalar_comboBox_currentIndexChanged(int index);

    void on_vector_comboBox_currentIndexChanged(int index);
    //void on_mask_horizontalSlider_sliderMoved(int position);

    void on_numPoints_spinBox_valueChanged(int arg1);

	void on_tipRes_spinBox_valueChanged(int arg1);
    void on_tipRad_doubleSpinBox_valueChanged(double arg1);
    void on_tipLen_doubleSpinBox_valueChanged(double arg1);
	void on_shaftRes_spinBox_valueChanged(int arg1);
    void on_shaftRad_doubleSpinBox_valueChanged(double arg1);

signals:
	void sig_enable_applyBtn();
private:
    Ui::vectorFilterPropForm *ui;
	PipelineObjPropWidget* _parentWin{};
};

#endif // VECTORFILTERPROPFORM_H
