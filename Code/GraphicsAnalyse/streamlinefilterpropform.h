#ifndef STREAMLINEFILTERPROPFORM_H
#define STREAMLINEFILTERPROPFORM_H

#include <QWidget>
#include "graphicsanalyse_global.h"
#include "PipelineObject.h"
#include "pipelineobjpropwidget.h"
namespace Ui {
	class streamLineFilterPropForm;
}
class GRAPHICSANALYSESHARED_EXPORT streamLineFilterPropForm : public QWidget
{
    Q_OBJECT

public:
	explicit streamLineFilterPropForm(PipelineObjPropWidget *parent = 0);
	~streamLineFilterPropForm();
	void updateForm();
	struct streamLineFilter_pipelineObj_propData *streamLineFilter_propData;
	PipelineObject* cur_pipelineObj;
	virtual void reTranslate();
	void initSeedForm();
private slots:
	void on_vector_comboBox_currentIndexChanged(int index);
    void on_Inte_Direction_currentIndexChanged(int index);
	void on_inte_Type_currentIndexChanged(int index);
	void on_inte_StepUnit_currentIndexChanged(int index);

	void on_stream_MaxiLen_horizontalSlider_sliderMoved(int position);

    void on_seedP1_lineEdit_editingFinished();

    void on_seedP2_lineEdit_editingFinished();

    void on_seedP3_lineEdit_editingFinished();

    void on_seedPoints_spinBox_valueChanged(int arg1);

    void on_seedRadius_lineEdit_editingFinished();

    void on_seeds_Type_currentIndexChanged(int index);

    void on_stream_MaxiError_editingFinished();

    void on_stream_TerminalSpeed_editingFinished();

    void on_stream_MaxiSteps_editingFinished();

    void on_inte_MaxiStepLen_editingFinished();

    void on_inte_MiniStepLen_editingFinished();

    void on_inte_InitStepLen_editingFinished();

    void on_interpolatorType_comboBox_currentIndexChanged(int index);

    void on_surfaceLines_checkBox_clicked(bool checked);

    void on_vorticity_checkBox_clicked(bool checked);

    void on_reflection_checkBox_clicked(bool checked);

    void on_reflection_comboBox_currentIndexChanged(int index);

signals:
	void sig_enable_applyBtn();
private:
    Ui::streamLineFilterPropForm *ui;
	PipelineObjPropWidget* _parentWin{};
};

#endif // STREAMLINEFILTERPROPFORM_H
