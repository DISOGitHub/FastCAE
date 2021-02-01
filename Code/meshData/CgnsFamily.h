#ifndef _CGNSFAMILY_H_
#define _CGNSFAMILY_H_

#include "meshDataAPI.h"
#include "meshSet.h"

namespace MeshData
{
	class MESHDATAAPI CgnsFamily : public MeshSet
	{
	public:
		CgnsFamily(QString name);
		CgnsFamily();
		~CgnsFamily();
		 
		///添加子set
		void appendDataSet(vtkDataSet* set);

		void generateDisplayDataSet() override;

		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		//从XML文件读取数据
		virtual void readDataFromProjectFile(QDomElement* e) override;
		//写入二进制文件
		virtual void writeBinaryFile(QDataStream* dataStream) override;
		//读入二进制文件
		virtual void readBinaryFile(QDataStream* dataStream) override;

	private:
		QList<vtkDataSet*> _setList{};
	};
}

#endif