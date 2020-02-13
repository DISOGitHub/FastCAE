#ifndef SCALARBAREDITFORM_H
#define SCALARBAREDITFORM_H
#include <QWidget>
#include "PipelineObject.h"
#include "qcolordialog.h"
#include "vtkColorTransferFunction.h"
#include "vtkScalarsToColors.h"
#include "vtkCompositeControlPointsItem.h"
#include "vtkCompositeTransferFunctionItem.h"
#include "vtkPiecewiseFunction.h"
#include "vtkEventQtSlotConnect.h"
#include "vtkVolume.h"
#include "vtkVolumeMapper.h"
#include "vtkVolumeProperty.h"
namespace Ui {
class ScalarBarEditForm;
}

class ScalarBarEditForm : public QWidget
{
    Q_OBJECT

public:
    explicit ScalarBarEditForm(QWidget *parent = 0);
    ~ScalarBarEditForm();
	void updateForm(PipelineObject* tep_PipelineObj);
	void clearAll();
	virtual void reTranslate();
public slots:
	void slot_update_scalarRange(double *);
private slots:
    void on_scalarRange_dataBtn_clicked();

    void on_scalarRange_customBtn_clicked();

    void on_scalarRange_tableBtn_clicked();
	void computeRange(double* range);
	void updateRange();
	void onAxesModified();

	void moveAllPoints(double xOffset, double yOffset, bool dontMoveFirstAndLast);
	void spreadAllPoints(double factor, bool dontSpreadFirstAndLast);
	void slot_update_scalarBarTable();
    void on_enableOpacity_checkBox_clicked(bool checked);
	void slot_update_zoneTable();
	void OnZoneColorBtnClicked();
    void on_spinBox_numLabels_valueChanged(int arg1);

    void on_colName_lineEdit_editingFinished();

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::ScalarBarEditForm *ui;
	PipelineObject* cur_PipelineObj;
	vtkActor* cur_actor;
	double *dRange;
	void chartsBounds(double bounds[4])const;
	void chartsExtent(double extent[4])const;
	vtkColorTransferFunction* colorTransferFunction;
	vtkPiecewiseFunction* opacityFunction; 
	void modify_scalarBarTable(double val[2]);
	bool flag_init;
	void enable_components(bool flag);
signals:
	void chartsExtentChanged();
	void sig_update_plot_pipelineObj(PipelineObject*, vtkColorTransferFunction*, vtkPiecewiseFunction*);
	void sig_volume_scalarBarEdit(PipelineObject*, vtkSmartPointer<vtkLookupTable>);
	void sig_update_scalarBar_title(PipelineObject*, QString, QString);
	void sig_update_scalarBar_fontSize(PipelineObject*, QString,int);
	void sig_update_scalarBar_numLables(PipelineObject*, QString,int);
	void sig_update_scalarBar(PipelineObject*);
};

#endif // SCALARBAREDITFORM_H
