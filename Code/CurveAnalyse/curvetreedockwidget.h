#ifndef CURVETREEDOCKWIDGET_H
#define CURVETREEDOCKWIDGET_H
#include <QDockWidget>
#include <QWidget>
#include <QMenu>
#include <QAction>
#include "curve_dropTreeWidget.h"
#include <QContextMenuEvent>
#include <QGuiApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <qlabel.h>
#include <qtreeview.h>
#include <qabstractscrollarea.h>
#include "curve_struct.h"
#include "readdatafileprop.h"
#include "curveanalyse_global.h"

class CurveTreeDockWidget : public QDockWidget
{
	Q_OBJECT
public:
	explicit CurveTreeDockWidget(QWidget *parent = 0);
	~CurveTreeDockWidget();
	void initWidget();
	void initPopMenu();
	void initTree();
    QTreeWidget *curveTree;
	QTreeWidgetItem *curve_project_item;
    curve_ini_data *curve_data_ptr;
    int index_file;
    int index_curve;
	bool readDataFile(QString data_filename1);
	void func_projectTreeItemClicked(int index_file);
	void addFile_from_array(QString data_filename);
	virtual void reTranslate();
signals:
    void sig_del_curve_prop(int,curve_line_data);
    void sig_del_curve_plot(int,curve_line_data);
	void sig_console_file_data_val_show(int);
	void sig_console_file_data_val_show_from_curveData(int, QVector<int>);
    void sig_update_clear_console_command();
    void sig_update_clear_console_data();
    void sig_update_clear_plotAni();

	void sig_add_curve_plot(int, int);

	void sig_del_curve_plot(int, int);
public slots :
    void projectTreeItemClicked(QTreeWidgetItem *item,int column);
	void on_treeWidget_customContextMenuRequested(QPoint pos);
	void slot_add_curve(int tep_index_file, int tep_index_col);
	void slot_del_curve(int tep_index_file, int tep_index_col);
    void slot_addFile();
    void slot_delFile();
	void slot_delAllFiles();
    void slot_update_curveTree();
    void slot_update_tree_curve_name(int tep_index_file,int tep_index_curve,QString tep_item_name);
    void slot_add_dropItem(QString fileName);
	//void projectTreeDoubleClicked(QTreeWidgetItem *item, int column);
private:
	void setAxisRange(int axis_index, int tep_file_index, int tep_column_index);
    QMenu *file_add_popMenu;
    QMenu *file_del_popMenu;
    QAction *file_add_action;
    QAction *file_del_action;
    QTreeWidgetItem *cur_item;
    curve_file_data tep_curve_file;
    curve_line_data tep_curve_line;
	bool curveExist(int tep_index_file, int tep_index_col);///////////////////////
    QStringList get_curve_filelist(QString dirname);
    
    int red0,green0,blue0,add_count;
};

#endif // CURVETREEDOCKWIDGET_H
