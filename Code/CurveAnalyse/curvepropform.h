#ifndef CURVEPROPFORM_H
#define CURVEPROPFORM_H

#include <QDialog>
#include <QTreeWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QColor>
#include <QColorDialog>
#include <QScrollArea>
#include "qfiledialog.h"
#include "qfile.h"
#include "curve_struct.h"
#include "qtextcodec.h"
#include "readdatafileprop.h"
#include "qradiobutton.h"
#include "curveanalyse_global.h"
namespace Ui {
class CurvePropForm;
}

class CurvePropForm : public QDialog
{
	Q_OBJECT
public:
    explicit CurvePropForm(QWidget *parent = 0);
    ~CurvePropForm();
	void initCurvePropTreeWidget(int tep_file_index, int tep_curve_index);
	void initGlobalPropTreeWidget();
    curve_ini_data *curve_data_ptr;
	void initForm(int index,int file_index,int curve_index);
	virtual void reTranslate();
private slots:

    void on_color_pushButton_clicked();
    void on_lineType_comboBox_currentIndexChanged(int index);
    void on_lineWidth_spinBox_valueChanged(int index);
    void on_flagPoint_checkBox_stateChanged(int state);
	void on_name_y_lineEdit_textChanged(QString);
	void on_x_indexSelected_comboBox_currentIndexChanged(int index);

	void on_backColor_pushButton_clicked();
	void on_grid_checkBox_clicked(bool);
	void on_legend_comboBox_currentIndexChanged(int);
	void on_numAxis_comboBox_currentIndexChanged(int);

	void on_aniPointType_comboBox_currentIndexChanged(int state);
	void on_color_aniPointButton_clicked();
signals:
    void sig_update_columnDataWidget_header(QStringList);
    void sig_update_tree_curve_name(int,int,QString);
	
	void sig_setBackgroundColor();
	void sig_setGrid();
	void sig_setLegend();
	void sig_setAxisNum();
	void sig_update_curve_name(int,int);//文件索引，曲线索引
	void sig_update_curve_pen(int, int);
	void sig_update_curve_linePoint(int, int);
	void sig_update_curve_axis(int, int);
private:
    Ui::CurvePropForm *ui;
	QTreeWidgetItem *headerItem;    
    QTreeWidgetItem *basePropertyItem;
    QTreeWidgetItem *y_PropertyItem;
    QTreeWidgetItem *name_baseItem;
    QTreeWidgetItem *color_baseItem;
    QTreeWidgetItem *lineType_baseItem;
    QTreeWidgetItem *lineWidth_baseItem;
    QTreeWidgetItem *flagPoint_baseItem;
    QTreeWidgetItem *name_yItem;
    QTreeWidgetItem *filename_yItem;
    QTreeWidgetItem *column_yItem;
    QTreeWidgetItem *col_name_yItem;

	QTreeWidgetItem *xAxis_index_Item;
	QTreeWidgetItem* xAxis_indexSelected_Item;

    QPushButton *color_pushButton;
    QComboBox *lineType_comboBox;
    QSpinBox *lineWidth_spinBox;
    QCheckBox *flagPoint_checkBox;

    QLineEdit *name_y_lineEdit;
    QLineEdit *column_y_lineEdit;
    QLineEdit *col_name_y_lineEdit;
	QComboBox* x_indexSelected_comboBox;

    int cur_index_file, cur_index_curve;
    curve_line_data tep_line_data;
    curve_file_data tep_file_data;

	QPushButton *backColor_pushButton;
	QCheckBox *grid_checkBox;
	QComboBox *legend_comboBox;
	QComboBox *numAxis_comboBox;

	QTreeWidgetItem *gloProp_headerItem;
	
	QTreeWidgetItem *gloProp_background_Item;
	QTreeWidgetItem *gloProp_grid_Item;
	QTreeWidgetItem *gloProp_legend_Item;
	QTreeWidgetItem *gloProp_numAxis_Item;
	QTreeWidgetItem *aniPointItem;
	QTreeWidgetItem *aniPointTypeItem;
	QTreeWidgetItem *aniPointColorItem;
	QPushButton *color_aniPointButton;
	QComboBox *aniPointType_comboBox;

	void initGlobalPropTreeWidget1();
	QList<QTreeWidgetItem*> gloItemList;
};

#endif // CURVEPROPFORM_H
