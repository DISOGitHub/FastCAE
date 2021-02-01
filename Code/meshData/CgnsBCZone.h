#ifndef _CGNSBCZONE_H_
#define _CGNSBCZONE_H_

#include "meshSet.h"

class vtkDataSet;

namespace MeshData
{
	class MESHDATAAPI CgnsBCZone : public MeshSet
	{
	public:
		CgnsBCZone(QString name);
		CgnsBCZone();
		~CgnsBCZone();

		void setRange(int* x, int* y, int*z);
		void setDisplayDataSet(vtkDataSet* dataset);
		void setParent(QString p);

		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		//从XML文件读取数据
		virtual void readDataFromProjectFile(QDomElement* e) override;

		//写入二进制文件
		virtual void writeBinaryFile(QDataStream* dataStream) override;
		//读入二进制文件
		virtual void readBinaryFile(QDataStream* dataStream) override;

		int getDataSetID();
		void setDataSet(vtkDataSet* d);

	private:
		QString _parent{};

		vtkDataSet* _dataset{};
		int _xRange[2];
		int _yRange[2];
		int _zRange[2];
	};
}

#endif
