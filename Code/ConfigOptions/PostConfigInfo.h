#ifndef _POST2DCONFIG_H_
#define _POST2DCONFIG_H_

#include "DataProperty/DataBase.h"
#include "ConfigOptionsAPI.h"
#include <QString>
#include <QStringList>
#include <QHash>

namespace ConfigOption
{
	class PostCurve;

	class CONFIGOPTIONSAPI PostConfigInfo : public DataProperty::DataBase
	{
	public:
		PostConfigInfo() = default;
		~PostConfigInfo() = default;
	
//		void setPost2DFile(QStringList name);
		QStringList getPost2DFile();
		QStringList get2DVariables(QString f);
		void setPost3DFile(QString f);
		QString getPost3DFile();
// 		void set2DVariable(QStringList v);
// 		QStringList get2DVariable();
		void setNodeScalarVariable(QStringList sca);
		QStringList getNodeScalarVariable();
		void setNodeVectorVariable(QStringList s);
		QStringList getNodeVectorVariable();
		void setCellScalarVariable(QStringList sca);
		QStringList getCellScalarVariable();
		void setCellVectorVariable(QStringList s);
		QStringList getCellVectorVariable();
		void appendPostCurve(PostCurve* c);
		QList<PostCurve*> getCurveList();
		PostCurve* getCurveByName(QString name);


	private:
		QList<PostCurve*> _curves{};
		QString _post3DFile{};
//		QStringList _post2DVariable{};
		QStringList _postNodeScalarVariable{}; 
		QStringList _postCellScalarVariable{};
		QStringList _postNodeVectorVariable{};
		QStringList _postCellVectorVariable{};
	};
	
}


#endif
