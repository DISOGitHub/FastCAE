#ifndef FILEDIALOGDIRECTORY_H
#define FILEDIALOGDIRECTORY_H

#include <QDialog>
//#include "graphicsanalyse_global.h"
#include "ui_FileDialogDirectory.h"
#include "qfiledialog.h"
#include "qtreewidget.h"
//#include "QFileSystemModel.h"
//#include "QDirModel.h"
#include <QDirModel>
extern QString openDir;
class FileDialogDirectory : public QDialog
{
	Q_OBJECT
public:
	QString mCurrentFilePath;//当前选择路径
	//用于保存一个或多个文件分类，用;分割
	QString mSelectionText;
	QMap<QString, QVector<QString>> mFileGroup; //key为12 ...dat，value为分类下对应的文件列表
	QStringList mSelectedFiles;//获取选中文件列表
	FileDialogDirectory(QWidget *parent = 0);
	~FileDialogDirectory();
	void initWin(QString dirStr);
private slots:
	void onCancelClicked();
	void on_OK_clicked();
	//按数字前的字符串形成文件分类列表，并在树型中显示
	void on_OKChoice_clicked();
	//选择一个打开的路径
	void OnFilePath();
	//分类型表在树型中显示
	void BuidFilePathTree();
	//文件扩展名发生变化，更新分类
	void onFilterChange(QString filter);
	//单击选择一个分类下的文件形成列表
	void OnClickedObject(QTreeWidgetItem* pitem, int column);
	//执行确定按钮功能
	void OnDoubleClickedObject(QTreeWidgetItem* pitem, int column);
	//暂时没用
	void onDoubleClickFile(QModelIndex&);
	//调用系统文件对话框
	void OnShowDirDialog();
	//处理路径中的盘符
	void OnlineTreeViewDoubleClick(const QModelIndex&);
	void func_OnlineTreeViewDoubleClick(const QModelIndex& index);
    void unix_OnlineTreeViewDoubleClick(QString itemName);
	//选择项发生变化触发这个槽，用于文件分类多选保存
	void OnItemSelectionChanged();
	void OnPipeTreeViewDoubleClick(const QModelIndex&);
	void on_up_pushButton_clicked();
private:
	Ui::FileDialogDirectory ui;
	QDirModel *m_dirModel;
	QString docPath,deskTopPath,userPath,rootPath,userName,rootName;
	FileDialogDirectory *dirDlg;
	QString mFatherNodeText = "FileName";//根节点名称
	QStringList mFilterList;//用于保存文件类型列表
	QString mFilterType;//文件类型
	QTreeWidget* mPipeTree;//treeView
	QTreeWidgetItem* mBuiltinItem; //根节点

	QHash<QString, int> mHeadFile; //key为12@data,value为扩展名dat,这样用是因为相同文件名，扩展名可能会不同，属于不同的分类

	QVector<QString> mFileList;//保存所有文件列表
	QString mTreeWidgetFilename;  //保存QTreeView双击后的文件路径
	bool mFlag_fordouble_clicked=false;
	void GetFileList(QString dirName);//根据目录名获取目录下的文件
	//	QStringList mHeaderList;
	//	QVector<SHeadData*> mHeaderVectorList;
	QAction* filePathAct;
	QStringList GetFilterFromString(const QString& filter);//根据文件类型中的字符串获取每一个文件过滤类型
	//获取文件扩展名
	QString  GetFileExt(QString fileName);
	QString  GetFileHead(QString fileName);
	QString SplitExt = "@";//用于文件头类型分割符，如12@dat
	QString FatherGroupFlag = "Group";//父节点标志
	QString ChildGroupFlag = "Child";//子节点标志
	void GetHeadFileExt(QString HeadName, QString& HeadExt, QString& HeadFront);//获取目录名中@后的扩展名
	QStringList GetAllSelectedFiles();//根据选中节点，如果是目录节点，获取目录节点下的多个文件，普通节点获取单个文件名
	//	QString GetHeadFileFront(QString HeadName);
	//判断选择的文件是否为空
	bool AcceptFile(QStringList selectedFiles);
	
	void SetLastIndex(QString curPath);//设置路径名最后为/结尾
	void SetDirectoryForDrives(/*const QFileInfoList &tepTrives*/);  //设置磁盘列表目录
	//获取文件名的全路径
	QString FindFullFilename(const QModelIndex& index);
	int isHasSpace(QString fileName);

	//<MG
	void refreshRightView();
	QModelIndex m_currentIndex;
};

#endif // FILEDIALOGDIRECTORY_H
