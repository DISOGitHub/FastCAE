#ifndef PIPELINEOBJTREEDOCKWIDGET_H
#define PIPELINEOBJTREEDOCKWIDGET_H
#include <QDockWidget>
#include <QWidget>
#include <QMenu>
#include <QAction>
#include "dropTreeWidget.h"
#include <QContextMenuEvent>
#include <QGuiApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <qlabel.h>
#include <qtreeview.h>
#include <qabstractscrollarea.h>
#include "global.h"
#include "PipelineObject.h"
#include "FilterClip.h"
#include "FilterContour.h"
#include "FilterSlice.h"
#include "FilterVector.h"
#include "graphicsanalyse_global.h"
// #include <QDomProcessingInstruction>
// #include <QDomDocument>
// #include <QDomElement>
extern bool flag_render;
extern QString openDir;
class GRAPHICSANALYSESHARED_EXPORT PipelineObjTreeDockWidget : public QDockWidget
{
	Q_OBJECT
public:
	void setDockSize(int w, int h)
	{
		wid = w;
		hei = h;
		pipelineObjTree->resize(w, h);
		//sizeHint();
	}
	QSize sizeHint() const
	{
		return QSize(wid, hei);
	}
	void writeVRXml(QString tep_filename);
	void set_viewInteraction(bool flag) { script_renderViewPara.flag_viewInteraction = flag; };
	bool get_viewInteraction() { return script_renderViewPara.flag_viewInteraction; };
	explicit PipelineObjTreeDockWidget(QWidget *parent = 0);
	~PipelineObjTreeDockWidget();
	void initWidget();
	void initPopMenu();
	void initTree();
	void AddPipelineObject(PipelineObject* pipeObj);
	void RmvPipelineObject(PipelineObject* pipeObj);
	void RmvActivePipelineObject();
	
	PipelineObject* GetActivePipelineObject() { return mActivePiplnObj; }
	//void UpdateVisibilitySymbol();
	QTreeWidget *get_pipelineObjTree(){ return pipelineObjTree; };
	
	void update_pipelineObjs(QList<PipelineObject*> tep_pipelineObjs);

	//script
	void editScript(QString tep_filename);
	void editScript_start(QString fileName);
	void editScript_dataSource(QTreeWidgetItem* data_item, QString fileName);
	void editScript_Properties_Zone(PipelineObject* piplnObj, QString fileName);
	void editScript_filter(QTreeWidgetItem* data_item, QString fileName);
	void editScript_scalarBarPosition(QTreeWidgetItem* data_item, QString fileName);
	void editScript_Properties_Plane(PipelineObject* piplnObj, QString fileName);
	void editScript_Properties_Vector(PipelineObject* piplnObj, QString fileName);
	void editScript_Properties_Contour(PipelineObject* piplnObj, QString fileName);
	void editScript_Properties_Reflection(PipelineObject* piplnObj, QString fileName);
	void editScript_Properties_Smooth(PipelineObject* piplnObj, QString fileName);
	void editScript_Properties_Calculator(PipelineObject* piplnObj, QString fileName);
	void editScript_Properties_StreamLine(PipelineObject* piplnObj, QString fileName);
	void editScript_Properties_MooringLine(PipelineObject* piplnObj, QString fileName);
	void editScript_Properties_AniDisplace(PipelineObject* piplnObj, QString fileName);
	void editScript_Properties(PipelineObject* piplnObj, QString fileName);
	void editScript_Properties_RenderView(QString fileName);
	void editScript_camera(QString fileName);
	void editScript_end(QString fileName);
	void script_set_renderViewPara(renderview_global_prop tep_para);
	void script_set_cameraPara(Camera_Para tep_para);
	void editScript_Pick(QString fileName);
	QString editScript_Properties_LookupTable(PipelineObject* piplnObj);
	QString editScript_Properties_LookupOpacity(PipelineObject* piplnObj);
	//QString script_imageName;
	//QString script_aviName;
	//bool script_autoExit;
	//int autoExit_time;
	saveScript_prop saveScript_prop_data;
	//script
	virtual void reTranslate();

	///<MG 获取三维后处理树上文件/文件组点个数
	int getNodeNum() const;

	///<MG 根据index 获取节点
	PipelineObject * getNodeByIndex(int index);

signals:
	void sig_update_cgns_pipelineObj(PipelineObject* pipeObj);
	void sig_delPipelineObj(PipelineObject* pipeObj);
	bool sig_LoadDataSource(QString, QStringList,int);
	void sig_update_buildIn_propForm();
	void sig_update_pipelineObj_propForm(PipelineObject* pipeObj);
	void sig_update_pipelineObj_toolBar(PipelineObject* pipeObj);
	void sig_update_actor_visible_plotForm(PipelineObject* pipeObj,bool flag_change);

	void sig_update_filterToolBar(bool, PipelineObject*);
	void sig_update_aniToolBar(int);

	void sig_build_planePlot_connect(PipelineObject*);

	void sig_update_scalarBar(PipelineObject *);
	void sig_update_color(PipelineObject*);
	
public slots :
	void DoSelectPipelineObject(PipelineObject* pipeObj);
    //void slot_file_updateCgns_action();
	void slot_update_scalarBar(PipelineObject* tep_pipelineObj);
    void projectTreeItemClicked(QTreeWidgetItem *item,int column);
	//void on_treeWidget_customContextMenuRequested(QTreeWidgetItem * item, int column);
	void on_treeWidget_customContextMenuRequested(QPoint pos);
	//void on_treeWidget_itemChanged(QTreeWidgetItem *item, int column);
    
    void slot_addFile();
	void OnDeletePipelineItem();
    void slot_add_dropItem(QString fileName);
	void slot_treeItem_disable(PipelineObject*);
	void slot_script_refresh_pickData(struct pick_pipelineObj_propData pick_Data);

	//script add for user click custom tree
	void script_projectTreeItemClicked(PipelineObject* tep_pipelineObj);
	//script add for user click custom tree
private:
    QMenu *file_add_popMenu;
    QMenu *file_del_popMenu;
	QAction *file_updateCgns_action;
    QAction *file_add_action;
    QAction *file_del_action;
    QTreeWidgetItem* GetTreeItemFromPipelineObj(PipelineObject* pipeObj);
	QMap<QTreeWidgetItem*, PipelineObject*> mTreeItems;
	PipelineObject* mActivePiplnObj;
	QTreeWidgetItem* mBuiltinItem;
	QTreeWidgetItem* cur_Item;
	bool flag_pipelineObj_selected;
	QTreeWidget *pipelineObjTree;
	void SetPipelineIconByPipelineType(PipelineObject* pipeObj, QTreeWidgetItem* pitem);//根据pipeline类型设置图标
	//script
	bool flag_script_ani;
	renderview_global_prop script_renderViewPara;
	Camera_Para script_cameraPara; 
	struct pick_pipelineObj_propData script_pickData;
	//script
	int wid, hei;
	
};

#endif 
