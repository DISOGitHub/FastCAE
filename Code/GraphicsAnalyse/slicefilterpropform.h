#ifndef SLICEFILTERPROPFORM_H
#define SLICEFILTERPROPFORM_H

#include <QWidget>
#include "PipelineObjProp.h"
#include "PipelineObject.h"
#include "graphicsanalyse_global.h"
#include "pipelineobjpropwidget.h"
namespace Ui {
class sliceFilterPropForm;
}
class GRAPHICSANALYSESHARED_EXPORT sliceFilterPropForm : public QWidget
{
    Q_OBJECT

public:
	explicit sliceFilterPropForm(PipelineObjPropWidget *parent = 0);
    ~sliceFilterPropForm();
	void updateForm();
	struct sliceFilter_pipelineObj_propData *plane_propData;
	PipelineObject* cur_pipelineObj;
	virtual void reTranslate();
public slots:
	void slot_update_planeProp_widget(double* origins_val, double* normals_val);
private slots:
    void on_m_planeOrigin_lineEdit1_editingFinished();
    void on_m_planeOrigin_lineEdit2_editingFinished();
    void on_m_planeOrigin_lineEdit3_editingFinished();
    void on_m_planeNormal_lineEdit1_editingFinished();
    void on_m_planeNormal_lineEdit2_editingFinished();
    void on_m_planeNormal_lineEdit3_editingFinished();
    void on_checkBox_clicked(bool checked);

    void on_xNormal_btn_clicked();
    void on_yNormal_btn_clicked();
    void on_zNormal_btn_clicked();
    void on_cameraNormal_btn_clicked();

    void on_insideOut_checkBox_clicked(bool checked);

private:
    Ui::sliceFilterPropForm *ui;
	PipelineObjPropWidget* _parentWin{};
	double normals[3];
	double origins[3];
signals:
	void sig_update_planeOrigin(PipelineObject *);
	void sig_update_planeNormal(PipelineObject *);
	void sig_update_planeVis(PipelineObject *);
	void sig_enable_applyBtn();
	void sig_update_planeCameraNormal(PipelineObject *);

};

#endif // SLICEFILTERPROPFORM_H
