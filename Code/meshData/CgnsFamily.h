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
		 
		///�����set
		void appendDataSet(vtkDataSet* set);

		void generateDisplayDataSet() override;

		virtual QDomElement& writeToProjectFile(QDomDocument* doc, QDomElement* parent) override;
		//��XML�ļ���ȡ����
		virtual void readDataFromProjectFile(QDomElement* e) override;
		//д��������ļ�
		virtual void writeBinaryFile(QDataStream* dataStream) override;
		//����������ļ�
		virtual void readBinaryFile(QDataStream* dataStream) override;

	private:
		QList<vtkDataSet*> _setList{};
	};
}

#endif