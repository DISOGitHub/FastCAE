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
		//��������  Path, File, FileList
		void setType(PathType t);
		//��ȡ����
		PathType getType();
		//���ú�׺,����ΪFile, FileList��Ч
		void setSuffix(QString s);
		//��ȡ�ļ���׺
		QString getSuffix();
		//�����ļ�������ΪFileʱ��Ч
		void setFile(QString f);
		//��ȡ�ļ�
		QString getFile();
		//�����ļ��б�����ΪFileListʱ��Ч
		void setFileList(QStringList sl);
		//��ȡ�ļ��б�
		QStringList getFileList();
		//����·��������ΪPathʱ��Ч
		void setPath(QString s);
		//��ȡ·��
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