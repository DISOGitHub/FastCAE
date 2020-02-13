#include "FileDialogDirectory.h"
#include "qdir.h"
#include <QDebug>
#include <QDesktopServices>
#include <QStandardItemModel>
FileDialogDirectory::FileDialogDirectory(QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);

	QString filter = tr("Data Files (*.vtk *.vtu *.vtr *.vts *.vtp *.dat *.moor *.cgns *.msh *.obj *.sol *.obj *.igs *.iges *.neu *.stl *.cntm)");
	mCurrentFilePath = ui.FilePath->text();

	connect(ui.cmbFilePath, SIGNAL(clicked()), this, SLOT(OnFilePath()));
	connect(ui.OKChoice, SIGNAL(clicked()), this, SLOT(on_OK_clicked()));
	connect(ui.Cancel, SIGNAL(clicked()), this, SLOT(onCancelClicked()));
	QObject::connect(ui.FileType, SIGNAL(currentIndexChanged(QString)), this, SLOT(onFilterChange(QString)));
	mPipeTree = ui.mPipelineTree;

	mPipeTree->setSelectionMode(QAbstractItemView::ExtendedSelection);
	//connect(mPipeTree, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(OnPipeTreeViewDoubleClick(const QModelIndex)));
	connect(mPipeTree, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(OnDoubleClickedObject(QTreeWidgetItem*, int)));
	connect(mPipeTree, SIGNAL(itemSelectionChanged()), this, SLOT(OnItemSelectionChanged()));

	QStringList names;
	names << mFatherNodeText;

	mBuiltinItem = new QTreeWidgetItem(mPipeTree, names);
	mPipeTree->setSelectionBehavior(QAbstractItemView::SelectRows);

	//�����̷�
	ui.splitter->setStretchFactor(0, 3);
	ui.splitter->setStretchFactor(1, 7);

	connect(ui.mTreeView, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(OnlineTreeViewDoubleClick(const QModelIndex)));
	docPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	deskTopPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	userName="";
	rootName="";
#ifdef Q_OS_UNIX
	{
		userPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        rootPath="/";
		userName = userPath.section("/", -1, -1);
		rootName = "Computer";
	}
#endif
    SetDirectoryForDrives(/*drives*/);
	ui.cmbFilePath->setVisible(false);
}

FileDialogDirectory::~FileDialogDirectory()
{
	mPipeTree->clear();
}

void FileDialogDirectory::BuidFilePathTree()
{
	mPipeTree->clear();
	QStringList names;
	names << mFatherNodeText;
	mBuiltinItem = new QTreeWidgetItem(mPipeTree, names);
	QTreeWidgetItem* parentItem = mBuiltinItem;
	QMap<QString, QVector<QString>>::iterator it;
	for (it = mFileGroup.begin(); it != mFileGroup.end(); it++)
	{
		QString FilePath = it.key();
		QVector<QString> FileNames = it.value();
		QTreeWidgetItem *itm = new QTreeWidgetItem(mPipeTree);
		itm->setText(0, FilePath);
		itm->setData(0, Qt::UserRole + 1, FatherGroupFlag);
		if (FileNames.count() > 1)
		{
			parentItem->addChild(itm);
			for (int j = 0; j < FileNames.count(); j++)
			{
				QTreeWidgetItem *itmchild = new QTreeWidgetItem(itm);
				itmchild->setText(0, FileNames[j]);
				itmchild->setData(0, Qt::UserRole + 1, ChildGroupFlag);
				itm->addChild(itmchild);
			}
		}
		else
		{

		}
	}
}

void FileDialogDirectory::OnShowDirDialog()
{
	dirDlg = new FileDialogDirectory(this);
	dirDlg->exec();
}
void FileDialogDirectory::initWin(QString dirStr)
{
	if (dirStr == "")
		return;
	QDir dir(dirStr);
	if (!dir.exists())
		return;
	ui.FilePath->setText(dirStr);
	on_OKChoice_clicked();
	m_currentIndex = m_dirModel->index(dirStr);
	mTreeWidgetFilename = dirStr;
}
void FileDialogDirectory::SetLastIndex(QString curPath)
{
#ifdef Q_OS_WIN
	int index = mCurrentFilePath.lastIndexOf("/", mCurrentFilePath.size());
	if ((index <= 0) && (mCurrentFilePath.size()>3))
	{
		mCurrentFilePath = mCurrentFilePath + "/";
	}
#endif
}

void FileDialogDirectory::OnFilePath()
{
	if (mFlag_fordouble_clicked)
	{
		mCurrentFilePath = mTreeWidgetFilename;
		SetLastIndex(mCurrentFilePath);
		ui.FilePath->setText("");
		ui.FilePath->setText(mCurrentFilePath);
	}
	else{

		QFileDialog dialog;
		dialog.setFileMode(QFileDialog::DirectoryOnly);
		if (dialog.exec())
		{
			QStringList list = dialog.selectedFiles();
			if (list.count() > 0)
			{
				mCurrentFilePath = list.at(0);
				SetLastIndex(mCurrentFilePath);
				ui.FilePath->setText(mCurrentFilePath);
			}
		}
	}
	if (mCurrentFilePath != NULL)
	{
		ui.FileName->setText("");
		onFilterChange(ui.FileType->currentText());
		on_OKChoice_clicked();
	}
}

bool FileDialogDirectory::AcceptFile(QStringList selectedFiles)
{
	if (selectedFiles.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}

QStringList FileDialogDirectory::GetAllSelectedFiles()
{
	return QStringList();
}

void FileDialogDirectory::GetHeadFileExt(QString HeadName, QString& HeadExt, QString& HeadFront)
{
	QString f;
	int start, end;
	start = HeadName.indexOf(SplitExt);
	end = HeadName.length();
	if (start != -1 && end != -1)
	{
		HeadExt = HeadName.mid(start + 1, end - start - 1);
		HeadFront = HeadName.mid(0, start);
	}
	else if (start != -1 || end != -1)
	{
		f = QString();  // hmm...  I'm confused
	}
}

QString  FileDialogDirectory::GetFileExt(QString fileName)
{
	QFileInfo fileInfo;
	fileInfo = QFileInfo(fileName);
	return fileInfo.suffix();
}

QStringList FileDialogDirectory::GetFilterFromString(const QString& filter)
{
	QString f = filter;
	int start, end;
	start = filter.indexOf('(');
	end = filter.lastIndexOf(')');
	if (start != -1 && end != -1)
	{
		f = f.mid(start + 1, end - start - 1);
	}
	else if (start != -1 || end != -1)
	{
		f = QString(); 
	}
	QStringList fs = f.split(QRegExp("[\\s+;]"), QString::SkipEmptyParts);

	return fs;
}

void FileDialogDirectory::onDoubleClickFile(QModelIndex& index)
{
	QModelIndex actual_index = index;
	QStringList selected_files;

}


void FileDialogDirectory::OnPipeTreeViewDoubleClick(const QModelIndex& index)
{
	QString name = index.data().toString();
	m_currentIndex = index;

	int rowNum = 0;
	while (true)
	{
		QModelIndex index = m_currentIndex.child(rowNum, 0);
		if (!index.isValid())
		{
			break;
		}
		if (index.data() == name)
		{
			if (name.indexOf(".vtk") != -1 ||
				name.indexOf(".dat") != -1 ||
				name.indexOf(".moor") != -1 ||
				name.indexOf(".msh") != -1 ||
				name.indexOf(".obj") != -1 ||
				name.indexOf(".cgns") != -1)
			{
				break;
			}
			func_OnlineTreeViewDoubleClick(index);
			return;
		}
		rowNum++;
	}


	ui.OKChoice->click();
}
void FileDialogDirectory::OnDoubleClickedObject(QTreeWidgetItem* pitem, int column)
{
	QString name = pitem->text(column);

	int rowNum = 0;
#ifdef Q_OS_WIN
	while ( true )
	{
		QModelIndex index = m_currentIndex.child(rowNum, 0);
		if ( !index.isValid() )
		{
			break;
		}
		if ( index.data() == name )
		{
			if ( name.indexOf(".vtk") != -1 ||
				name.indexOf(".dat")!= -1 ||
				name.indexOf(".moor") != -1 ||
				name.indexOf(".msh")!=-1 ||
				name.indexOf(".obj") != -1 ||
				name.indexOf(".cgns")!=-1)
			{
				break;
			}
			func_OnlineTreeViewDoubleClick(index);
			return;
		}
		rowNum++;
	}
#endif
#ifdef Q_OS_UNIX
    while ( true )
    {
        if ( name.indexOf(".vtk") != -1 ||
                name.indexOf(".dat")!= -1 ||
                name.indexOf(".moor") != -1 ||
                name.indexOf(".msh")!=-1 ||
                name.indexOf(".obj") != -1 ||
                name.indexOf(".cgns")!=-1)
        {
            break;
        }
        unix_OnlineTreeViewDoubleClick(name);
        return;
        rowNum++;
    }
#endif
	ui.OKChoice->click();
}

void FileDialogDirectory::OnClickedObject(QTreeWidgetItem* pitem, int column)
{
	if (pitem->text(column) == mFatherNodeText)
	{
		return;
	}

	QVector<QString> GroupFileList;
	QString fileString;
	const QModelIndexList indices = ui.mPipelineTree->selectionModel()->selectedIndexes();
	const QModelIndexList rows = ui.mPipelineTree->selectionModel()->selectedRows();
	if (pitem->data(0, Qt::UserRole + 1) == FatherGroupFlag)
	{
		mSelectedFiles.clear();
		fileString = pitem->text(column);
		QMap<QString, QVector<QString>>::iterator mi;
		mi = mFileGroup.find(fileString);
		if (mi != mFileGroup.end())
		{
			GroupFileList = mFileGroup[fileString];
			for (int i = 0; i < GroupFileList.count(); i++)
			{
				mSelectedFiles.push_back(mCurrentFilePath + GroupFileList.at(i));
			}
		}
	}
	else
	{
		mSelectedFiles.clear();
		fileString = pitem->text(column);
		mSelectedFiles.push_back(mCurrentFilePath + fileString);
	}
	ui.FileName->setText(pitem->text(column));

}

void FileDialogDirectory::onFilterChange(QString filter)
{
	mFilterType = filter;
	mFilterList.clear();
	mFilterList = GetFilterFromString(filter);
	on_OKChoice_clicked();
}

void FileDialogDirectory::GetFileList(QString dirName)
{
	QFileInfo fileInfo;
	mFileList.clear();
	QString filePath;

	fileInfo = QFileInfo(dirName);
	filePath = fileInfo.absolutePath();
	QDir dir(dirName);
	if (!dir.exists())
		return;
	dir.setFilter(QDir::Files);
	dir.setSorting(QDir::Name);//
	
	dir.setNameFilters(mFilterList);
	QFileInfoList list = dir.entryInfoList();
	if (list.count()>0)
	{
		for (int i = 0; i<list.count(); i++)
		{
			mFileList.push_back(list.at(i).fileName());
		}
	}
}


void FileDialogDirectory::onCancelClicked()
{
	close();
}


void FileDialogDirectory::on_OK_clicked()
{
	if (mSelectionText == "")
	{
		return;
	}
	accept();
}


void FileDialogDirectory::on_OKChoice_clicked()
{
	QString tempFileHeader;
	mCurrentFilePath = ui.FilePath->text().trimmed();
#ifdef Q_OS_WIN
	if (mCurrentFilePath.right(1) != "/")
		mCurrentFilePath += "/";
#endif
	openDir = mCurrentFilePath;
	GetFileList(mCurrentFilePath);

	mFileGroup.clear();
	mHeadFile.reserve(mFileList.count() * 2);
	mHeadFile.clear();
	for (int i = 0; i < mFileList.count(); i++)
	{
		QString fileName = mFileList.at(i);
		QString fileExt = GetFileExt(fileName);
		QString fileHead, fileHeadExt;
		int endPos = fileName.length() - fileExt.length() - 2, beginPos;
		for (beginPos = endPos; beginPos >= 0; --beginPos) {
			if (fileName[beginPos] > '9' || fileName[beginPos] < '0') break;
		}
		fileHead = fileName.mid(0, beginPos + 1);
		fileHeadExt = fileHead + SplitExt + fileExt;
		if (mHeadFile.contains(fileHeadExt) == false)
		{
			mHeadFile.insert(fileHeadExt, 1);
		}
		else 
		{
			mHeadFile[fileHeadExt] += 1;
		}
	}
	mHeadFile.squeeze();
	QString headExt, headFront;
	QMap<QString, int>::iterator it;
	for (int i = 0; i < mFileList.count(); i++)
	{
		QString fileName = mFileList.at(i);
		QString fileExt = GetFileExt(fileName);
		QString fileHead, fileHeadExt, NodeText;
		int endPos = fileName.length() - fileExt.length() - 2, beginPos;
		for (beginPos = endPos; beginPos >= 0; --beginPos) {
			if (fileName[beginPos] > '9' || fileName[beginPos] < '0') break;
		}
		fileHead = fileName.mid(0, beginPos + 1);
		fileHeadExt = fileHead + SplitExt + fileExt;
		if (mHeadFile[fileHeadExt] > 1)
		{
			NodeText = fileHead + " ..." + fileExt;
		}
		else
		{
			NodeText = fileName;
		}
		if (mFileGroup.contains(NodeText) == false)
			mFileGroup.insert(NodeText, QVector<QString>());
		mFileGroup[NodeText].push_back(fileName);
	}
	BuidFilePathTree();
}

#include <QDesktopServices>
#include <QStandardItemModel>
void FileDialogDirectory::SetDirectoryForDrives(/*const QFileInfoList &tepTrives*/)
{
	QStringList pathList;
	
	qDebug() << QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
	pathList.push_back(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation));

	//QFileSystemModel *model = new QFileSystemModel;
	//model->setRootPath("/");
	//model->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
	////model->sort(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);
	//ui.mTreeView->setModel(model);
	QStandardItemModel* tep_model = new QStandardItemModel;
#ifdef Q_OS_WIN
	{
		m_dirModel = new QDirModel;
		//temp->setSorting(QDir::NoSort);
		m_dirModel->setSorting(QDir::DirsFirst | QDir::IgnoreCase | QDir::Name);
		m_dirModel->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
		//temp->setReadOnly(true);
		//ui.mTreeView->setModel(temp);
		int num_row = m_dirModel->rowCount(QModelIndex());
		//QStandardItemModel* tep_model = new QStandardItemModel;
		//tep_model->sort(0,Qt::AscendingOrder);
		tep_model->appendRow(new QStandardItem(tr("DeskTop")));
		tep_model->appendRow(new QStandardItem(tr("My Documents")));
		for (int i = 0; i < num_row; i++)
		{
			tep_model->appendRow(new QStandardItem(m_dirModel->index(i, 0).data().toString()));
		}
	}
#endif
#ifdef Q_OS_UNIX
	{
		QString userPath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
		QString userName = userPath.section("/", -1, -1);
		tep_model->appendRow(new QStandardItem(rootName));
		tep_model->appendRow(new QStandardItem(userName));
	}
#endif	
	ui.mTreeView->setModel(tep_model);
	ui.mTreeView->setAnimated(true);
	ui.mTreeView->setIndentation(20);
	ui.mTreeView->setSortingEnabled(false);
	ui.mTreeView->header()->hide();
	ui.mTreeView->hideColumn(1);
	ui.mTreeView->hideColumn(2);
	ui.mTreeView->hideColumn(3);
	ui.mTreeView->setItemsExpandable(false);
	ui.mTreeView->resizeColumnToContents(0);
	ui.mTreeView->setEditTriggers(QAbstractItemView::EditTrigger::NoEditTriggers);
	ui.mTreeView->show(); 
}

void FileDialogDirectory::OnItemSelectionChanged()
{
	QList<QTreeWidgetItem*> selectItems;
	selectItems=mPipeTree->selectedItems();

	if (selectItems.count() > 0)
	{
		mSelectionText = "";
	}
	for (int i = 0; i < selectItems.count(); i++)
	{
		QTreeWidgetItem *curitem = selectItems.at(i);
		if (curitem->text(0).contains("FileName"))
		{
			continue;
		}
		
		if (i == selectItems.size() - 1) 
		{
			mSelectionText +=curitem->text(0);
		}
		else 
		{
			mSelectionText += curitem->text(0);
			mSelectionText += ";";
		}

	}
	ui.FileName->setText(mSelectionText);
}


void FileDialogDirectory::func_OnlineTreeViewDoubleClick(const QModelIndex& index)
{
	m_currentIndex = index;
	qDebug() << index.parent().data().toString() << endl;

	if ((index.data().toString() == ".") | (index.data().toString() == ".."))
		return;

	
	mTreeWidgetFilename = FindFullFilename(index);
	qDebug() << mTreeWidgetFilename << endl;

    if (mTreeWidgetFilename.indexOf("C:") != -1)
    {
        if (-1 == mTreeWidgetFilename.indexOf("/", 1))
        {
            mTreeWidgetFilename = "C:";
        }
        else
            mTreeWidgetFilename.replace(0, mTreeWidgetFilename.indexOf("/", 1), "C:");
    }
    else if (mTreeWidgetFilename.indexOf("D:") != -1)
    {
        if (-1 == mTreeWidgetFilename.indexOf("/", 1))
        {
            mTreeWidgetFilename = "D:";
        }
        else
            mTreeWidgetFilename.replace(0, mTreeWidgetFilename.indexOf("/", 1), "D:");
    }
    else if (mTreeWidgetFilename.indexOf("E:") != -1)
    {
        if (-1 == mTreeWidgetFilename.indexOf("/", 1))
        {
            mTreeWidgetFilename = "E:";
        }
        else
            mTreeWidgetFilename.replace(0, mTreeWidgetFilename.indexOf("/", 1), "E:");
    }
    else if (mTreeWidgetFilename.indexOf("F:") != -1)
    {
        if (-1 == mTreeWidgetFilename.indexOf("/", 1))
        {
            mTreeWidgetFilename = "F:";
        }
        else
            mTreeWidgetFilename.replace(0, mTreeWidgetFilename.indexOf("/", 1), "F:");
    }
    else if (mTreeWidgetFilename.indexOf("G:") != -1)
    {
        if (-1 == mTreeWidgetFilename.indexOf("/", 1))
        {
            mTreeWidgetFilename = "G:";
        }
        else
            mTreeWidgetFilename.replace(0, mTreeWidgetFilename.indexOf("/", 1), "G:");
    }
    else if (mTreeWidgetFilename.indexOf("H:") != -1)
    {
        if (-1 == mTreeWidgetFilename.indexOf("/", 1))
        {
            mTreeWidgetFilename = "H:";
        }
        else
            mTreeWidgetFilename.replace(0, mTreeWidgetFilename.indexOf("/", 1), "H:");
    }
    else if (mTreeWidgetFilename.indexOf("I:") != -1)
    {
        if (-1 == mTreeWidgetFilename.indexOf("/", 1))
        {
            mTreeWidgetFilename = "I:";
        }
        else
            mTreeWidgetFilename.replace(0, mTreeWidgetFilename.indexOf("/", 1), "I:");
    }
    else if (mTreeWidgetFilename.indexOf(tr("DeskTop")) >= 0)
    {
        mTreeWidgetFilename = deskTopPath;
    }
    else if (mTreeWidgetFilename.indexOf(tr("My Documents")) >= 0)
    {
        mTreeWidgetFilename = docPath;
    }
    else return;

	qDebug() << mTreeWidgetFilename << endl;
	mFlag_fordouble_clicked = true;
	OnFilePath();
	mFlag_fordouble_clicked = false;

	refreshRightView();
}
void FileDialogDirectory::unix_OnlineTreeViewDoubleClick(QString itemName)
{
    mTreeWidgetFilename =ui.FilePath->text();
    if(mTreeWidgetFilename!="/")
        mTreeWidgetFilename+="/";
    mTreeWidgetFilename +=itemName;
    qDebug() << mTreeWidgetFilename << endl;
    mFlag_fordouble_clicked = true;
    OnFilePath();
    mFlag_fordouble_clicked = false;

    refreshRightView();
}
void FileDialogDirectory::OnlineTreeViewDoubleClick(const QModelIndex& index)
{
#ifdef Q_OS_WIN
	{
		QString cur_name = index.data().toString();
		if ((cur_name == tr("DeskTop")) || (cur_name == tr("My Documents")))
		{
			mTreeWidgetFilename = cur_name;
			if (cur_name == tr("DeskTop"))
				m_currentIndex = m_dirModel->index(deskTopPath);
			else
				m_currentIndex = m_dirModel->index(docPath);
		}
		else
		{
			int i;
			for (i = 0; i < m_dirModel->rowCount(QModelIndex()); i++)
			{
				if (m_dirModel->index(i, 0).data().toString() == index.data().toString())
				{
					m_currentIndex = m_dirModel->index(i, 0);
					break;
				}
			}
			if (i == m_dirModel->rowCount(QModelIndex()))
				return;
			//m_currentIndex = index;

			/*QModelIndex temp = index;
			while (true)
			{
			if ( temp.isValid() )
			{
			ui.mTreeView->expand(temp);
			}
			else
			break;
			temp = temp.parent();
			}*/

			qDebug() << index.parent().data().toString() << endl;

			if ((index.data().toString() == ".") | (index.data().toString() == ".."))
				return;


			mTreeWidgetFilename = FindFullFilename(index);
		}
		qDebug() << mTreeWidgetFilename << endl;


		if (mTreeWidgetFilename.indexOf("C:") != -1)
		{
			if (-1 == mTreeWidgetFilename.indexOf("/", 1))
			{
				mTreeWidgetFilename = "C:";
			}
			else
				mTreeWidgetFilename.replace(0, mTreeWidgetFilename.indexOf("/", 1), "C:");
		}
		else if (mTreeWidgetFilename.indexOf("D:") != -1)
		{
			if (-1 == mTreeWidgetFilename.indexOf("/", 1))
			{
				mTreeWidgetFilename = "D:";
			}
			else
				mTreeWidgetFilename.replace(0, mTreeWidgetFilename.indexOf("/", 1), "D:");
		}
		else if (mTreeWidgetFilename.indexOf("E:") != -1)
		{
			if (-1 == mTreeWidgetFilename.indexOf("/", 1))
			{
				mTreeWidgetFilename = "E:";
			}
			else
				mTreeWidgetFilename.replace(0, mTreeWidgetFilename.indexOf("/", 1), "E:");
		}
		else if (mTreeWidgetFilename.indexOf("F:") != -1)
		{
			if (-1 == mTreeWidgetFilename.indexOf("/", 1))
			{
				mTreeWidgetFilename = "F:";
			}
			else
				mTreeWidgetFilename.replace(0, mTreeWidgetFilename.indexOf("/", 1), "F:");
		}
		else if (mTreeWidgetFilename.indexOf("G:") != -1)
		{
			if (-1 == mTreeWidgetFilename.indexOf("/", 1))
			{
				mTreeWidgetFilename = "G:";
			}
			else
				mTreeWidgetFilename.replace(0, mTreeWidgetFilename.indexOf("/", 1), "G:");
		}
		else if (mTreeWidgetFilename.indexOf("H:") != -1)
		{
			if (-1 == mTreeWidgetFilename.indexOf("/", 1))
			{
				mTreeWidgetFilename = "H:";
			}
			else
				mTreeWidgetFilename.replace(0, mTreeWidgetFilename.indexOf("/", 1), "H:");
		}
		else if (mTreeWidgetFilename.indexOf("I:") != -1)
		{
			if (-1 == mTreeWidgetFilename.indexOf("/", 1))
			{
				mTreeWidgetFilename = "I:";
			}
			else
				mTreeWidgetFilename.replace(0, mTreeWidgetFilename.indexOf("/", 1), "I:");
		}
		else if (mTreeWidgetFilename.indexOf(tr("DeskTop")) >= 0)
		{
			mTreeWidgetFilename = deskTopPath;
		}
		else if (mTreeWidgetFilename.indexOf(tr("My Documents")) >= 0)
		{
			mTreeWidgetFilename = docPath;
		}
		else return;
		qDebug() << mTreeWidgetFilename << endl;
		mFlag_fordouble_clicked = true;
		OnFilePath();
		mFlag_fordouble_clicked = false;

	}
#endif
#ifdef Q_OS_UNIX
	{
		QString cur_name = index.data().toString();
		if ((cur_name == rootName) || (cur_name == userName))
		{
            mTreeWidgetFilename = cur_name;
		}
		qDebug() << mTreeWidgetFilename << endl;


        if (mTreeWidgetFilename==rootName)
		{
			mTreeWidgetFilename = rootPath;
		}
        else if (mTreeWidgetFilename==userName)
		{
			mTreeWidgetFilename = userPath;
		}
		else return;
		qDebug() << mTreeWidgetFilename << endl;
		mFlag_fordouble_clicked = true;
		OnFilePath();
		mFlag_fordouble_clicked = false;
	}
#endif
	refreshRightView();
}

QString FileDialogDirectory::FindFullFilename(const QModelIndex& index)
{
	if (index.data().toString() == NULL) return NULL;
	return FindFullFilename(index.parent()) + "/" + index.data().toString();
}

void FileDialogDirectory::refreshRightView()
{
	QDir dir(mCurrentFilePath);
	dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

	QFileInfoList list = dir.entryInfoList();

	QStyle* style = QApplication::style();
	QIcon icon = style->standardIcon(QStyle::SP_DirIcon);
	for (int iCnt = 0; iCnt < list.size();++iCnt)
	{
		QTreeWidgetItem * item = new QTreeWidgetItem( mPipeTree );
		item->setText(0,list.at(list.size()-1-iCnt).fileName());
		item->setIcon(0,icon);
		mPipeTree->addTopLevelItem(item);
	}
}

void FileDialogDirectory::on_up_pushButton_clicked()
{
#ifdef Q_OS_WIN
	if (m_currentIndex.parent().isValid())
	{
		m_currentIndex = m_currentIndex.parent();
		QDir dir(mTreeWidgetFilename);
		if (dir.cdUp())
		{
			mTreeWidgetFilename = dir.absolutePath();
			mFlag_fordouble_clicked = true;
			OnFilePath();
			mFlag_fordouble_clicked = false;
			refreshRightView();
		}
	}
#endif
#ifdef Q_OS_UNIX
    QString str=ui.FilePath->text();
    QStringList tepList=str.split("/",QString::SkipEmptyParts);
    if(tepList.count()>=1)
    {
        QDir dir(mTreeWidgetFilename);
        if (dir.cdUp())
        {
            mTreeWidgetFilename = dir.absolutePath();
            mFlag_fordouble_clicked = true;
            OnFilePath();
            mFlag_fordouble_clicked = false;
            refreshRightView();
        }
    }
#endif
}
