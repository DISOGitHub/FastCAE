#ifndef POST3DFILEMODEL_H
#define POST3DFILEMODEL_H

#include "ModelBase.h"
#include "Common.h"

namespace ConfigOption
{
	class PostConfigInfo;
}

namespace FastCAEDesigner
{

	class Post3DFileModel : public ModelBase
	{
		Q_OBJECT
	public:
		Post3DFileModel(QString nameEng, QString nameChn, QString iconName, int type, QObject *parent);
		Post3DFileModel(QString fileName, int type, QObject *parent);
		~Post3DFileModel();

		virtual int ShowEditor(QTreeWidgetItem* treeItem, QWidget* parent);
		QStringList getNodeScalarList();
		QStringList getNodeVectorList();
		QStringList getCellScalarList();
		QStringList getCellVectorList();
		void setDataList(QStringList,QStringList,QStringList,QStringList);
		void setFileName(QString);
		QString getFileName();

	private:
		QStringList _nodeScalarList{};
		QStringList _cellScalarList{};
		QStringList _nodeVectorList{};
		QStringList _cellVectorList{};
		QString _fileName;
	};
}

#endif