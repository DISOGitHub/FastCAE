#ifndef DATASOURCEPROPFORM_H
#define DATASOURCEPROPFORM_H

#include <QWidget>
#include "qlistwidget.h"
#include "PipelineObjProp.h"
#include "PipelineObject.h"
extern QList<PipelineObject*> g_ani_objList;
extern QString openDisDir;
namespace Ui {
class DataSourcePropForm;
}

class DataSourcePropForm : public QWidget
{
    Q_OBJECT

public:
    explicit DataSourcePropForm(QWidget *parent = 0);
    ~DataSourcePropForm();
	QStringList m_colorList;
	QMap<QString, bool> block_showMap;
	QString suffix_filename;
	void updateForm();
	struct mooringLine_pipelineObj_propData *mooringLine_propData;
	struct particles_pipelineObj_propData *particles_propData;
	//struct dataSource_pipelineObj_propData *dataSource_propData;
	PipelineObject* cur_pipelineObj;
	virtual void reTranslate();
	//QList<PipelineObject*> mPipelnObjs;
private slots:
	void on_particlesRadius_doubleSpinBox_valueChanged(double arg1);
	void on_particlesRes_spinBox_valueChanged(int arg1);
    //void on_listWidget_itemChanged(QListWidgetItem *item);

    void on_lineType_comboBox_currentIndexChanged(int index);


	void on_Res_cone_horizontalSlider_sliderMoved(int position);

    void on_RadCone_doubleSpinBox_valueChanged(double arg1);

    void on_ConeHei_doubleSpinBox_valueChanged(double arg1);

    void on_centerX_Cone_lineEdit_editingFinished();

    void on_centerY_Cone_lineEdit_editingFinished();

    void on_centerZ_Cone_lineEdit_editingFinished();

    void on_DirX_lineEdit_editingFinished();

    void on_DirY_lineEdit_editingFinished();

    void on_DirZ_lineEdit_editingFinished();

    void on_RadCylinder_doubleSpinBox_valueChanged(double arg1);

    void on_HeiCylinder_doubleSpinBox_valueChanged(double arg1);

    void on_CenterX_Cylinder_lineEdit_editingFinished();

    void on_CenterY_Cylinder_lineEdit_editingFinished();

    void on_CenterZ_Cylinder_lineEdit_editingFinished();

    void on_Res_Cylinder_horizontalSlider_sliderMoved(int position);

    void on_tipRes_spinBox_valueChanged(int arg1);

    void on_tipRad_doubleSpinBox_valueChanged(double arg1);

    void on_tipLen_doubleSpinBox_valueChanged(double arg1);

    void on_shaftRes_spinBox_valueChanged(int arg1);

    void on_shaftRad_doubleSpinBox_valueChanged(double arg1);

    void on_scaleFactor_doubleSpinBox_valueChanged(double arg1);

    void on_block_listWidget_clicked(const QModelIndex &index);

    void on_grid_blocks_comboBox_currentIndexChanged(int index);

    void on_comModel_checkBox_clicked(bool checked);

    void on_comModel_comboBox_currentIndexChanged(int index);

    void on_seaBottom_checkBox_clicked(bool checked);

    void on_seaBotColor_pushButton_clicked();

    void on_displace_checkBox_clicked(bool checked);

    void on_displace_pushButton_clicked();

    void on_pos_spinBox_valueChanged(int arg1);

    void on_interval_spinBox_valueChanged(int arg1);

    void on_zoomFactor_doubleSpinBox_valueChanged(double arg1);

    void on_gravityCenter_checkBox_clicked(bool checked);

    void on_gravityX_lineEdit_editingFinished();
    void on_gravityY_lineEdit_editingFinished();
    void on_gravityZ_lineEdit_editingFinished();
    void on_particlesType_comboBox_currentIndexChanged(int index);

private:
    Ui::DataSourcePropForm *ui;
signals:
    void sig_update_pipelineObj_colorPropWidget(QString,bool);
	void sig_enable_applyBtn();
};

#endif // DATASOURCEPROPFORM_H
