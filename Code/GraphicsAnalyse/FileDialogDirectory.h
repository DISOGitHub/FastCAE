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
	QString mCurrentFilePath;//��ǰѡ��·��
	//���ڱ���һ�������ļ����࣬��;�ָ�
	QString mSelectionText;
	QMap<QString, QVector<QString>> mFileGroup; //keyΪ12 ...dat��valueΪ�����¶�Ӧ���ļ��б�
	QStringList mSelectedFiles;//��ȡѡ���ļ��б�
	FileDialogDirectory(QWidget *parent = 0);
	~FileDialogDirectory();
	void initWin(QString dirStr);
private slots:
	void onCancelClicked();
	void on_OK_clicked();
	//������ǰ���ַ����γ��ļ������б�������������ʾ
	void on_OKChoice_clicked();
	//ѡ��һ���򿪵�·��
	void OnFilePath();
	//�����ͱ�����������ʾ
	void BuidFilePathTree();
	//�ļ���չ�������仯�����·���
	void onFilterChange(QString filter);
	//����ѡ��һ�������µ��ļ��γ��б�
	void OnClickedObject(QTreeWidgetItem* pitem, int column);
	//ִ��ȷ����ť����
	void OnDoubleClickedObject(QTreeWidgetItem* pitem, int column);
	//��ʱû��
	void onDoubleClickFile(QModelIndex&);
	//����ϵͳ�ļ��Ի���
	void OnShowDirDialog();
	//����·���е��̷�
	void OnlineTreeViewDoubleClick(const QModelIndex&);
	void func_OnlineTreeViewDoubleClick(const QModelIndex& index);
    void unix_OnlineTreeViewDoubleClick(QString itemName);
	//ѡ������仯��������ۣ������ļ������ѡ����
	void OnItemSelectionChanged();
	void OnPipeTreeViewDoubleClick(const QModelIndex&);
	void on_up_pushButton_clicked();
private:
	Ui::FileDialogDirectory ui;
	QDirModel *m_dirModel;
	QString docPath,deskTopPath,userPath,rootPath,userName,rootName;
	FileDialogDirectory *dirDlg;
	QString mFatherNodeText = "FileName";//���ڵ�����
	QStringList mFilterList;//���ڱ����ļ������б�
	QString mFilterType;//�ļ�����
	QTreeWidget* mPipeTree;//treeView
	QTreeWidgetItem* mBuiltinItem; //���ڵ�

	QHash<QString, int> mHeadFile; //keyΪ12@data,valueΪ��չ��dat,����������Ϊ��ͬ�ļ�������չ�����ܻ᲻ͬ�����ڲ�ͬ�ķ���

	QVector<QString> mFileList;//���������ļ��б�
	QString mTreeWidgetFilename;  //����QTreeView˫������ļ�·��
	bool mFlag_fordouble_clicked=false;
	void GetFileList(QString dirName);//����Ŀ¼����ȡĿ¼�µ��ļ�
	//	QStringList mHeaderList;
	//	QVector<SHeadData*> mHeaderVectorList;
	QAction* filePathAct;
	QStringList GetFilterFromString(const QString& filter);//�����ļ������е��ַ�����ȡÿһ���ļ���������
	//��ȡ�ļ���չ��
	QString  GetFileExt(QString fileName);
	QString  GetFileHead(QString fileName);
	QString SplitExt = "@";//�����ļ�ͷ���ͷָ������12@dat
	QString FatherGroupFlag = "Group";//���ڵ��־
	QString ChildGroupFlag = "Child";//�ӽڵ��־
	void GetHeadFileExt(QString HeadName, QString& HeadExt, QString& HeadFront);//��ȡĿ¼����@�����չ��
	QStringList GetAllSelectedFiles();//����ѡ�нڵ㣬�����Ŀ¼�ڵ㣬��ȡĿ¼�ڵ��µĶ���ļ�����ͨ�ڵ��ȡ�����ļ���
	//	QString GetHeadFileFront(QString HeadName);
	//�ж�ѡ����ļ��Ƿ�Ϊ��
	bool AcceptFile(QStringList selectedFiles);
	
	void SetLastIndex(QString curPath);//����·�������Ϊ/��β
	void SetDirectoryForDrives(/*const QFileInfoList &tepTrives*/);  //���ô����б�Ŀ¼
	//��ȡ�ļ�����ȫ·��
	QString FindFullFilename(const QModelIndex& index);
	int isHasSpace(QString fileName);

	//<MG
	void refreshRightView();
	QModelIndex m_currentIndex;
};

#endif // FILEDIALOGDIRECTORY_H
