#ifndef _PARAMETERPATH_H_
#define _PARAMETERPATH_H_

#include "ParameterBase.h"
#include <QString>
#include <QStringList>

namespace DataProperty
{

	enum PathType
	{
		None = 0,
		Path,
		File,
		FileList,

	};

	class DATAPROPERTYAPI ParameterPath : public ParameterBase
	{
	public:
		ParameterPath();
		~ParameterPath() = default;

		void copy(ParameterBase* ori, bool valueOnly = false) override;
		//设置类型  Path, File, FileList
		void setType(PathType t);
		//获取类型
		PathType getType();
		//设置后缀,类型为File, FileList有效
		void setSuffix(QString s);
		//获取文件后缀
		QString getSuffix();
		//设置文件，类型为File时有效
		void setFile(QString f);
		//获取文件
		QString getFile();
		//设置文件列表，类型为FileList时有效
		void setFileList(QStringList sl);
		//获取文件列表
		QStringList getFileList();
		//设置路径，类型为Path时有效
		void setPath(QString s);
		//获取路径
		QString getPath();

		void writeParameter(QDomDocument* doc, QDomElement* parent) override;
		void readParameter(QDomElement* e) override;

		QString valueToString() override;
		void setValueFromString(QString v) override;

	private:
		PathType _type{ None };
		QString _suffix{"All Files(*)"};

		QString _path{};
		QString _file{};
		QStringList _files{};
	};

}


#endif