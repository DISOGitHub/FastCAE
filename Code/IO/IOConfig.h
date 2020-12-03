#ifndef _IOCONFIG_H_
#define _IOCONFIG_H_

#include "IOAPI.h"
#include <QString>
#include <QHash>
#include <QStringList>

namespace ModelData
{
	class ModelDataBase;
}

//д����·����ģ��
typedef bool(*WRITEINPFILE)(QString, ModelData::ModelDataBase*);
//ת��ԭ�ļ�·��
typedef bool(*TRANSFEROUTFILE)(QString);
//��������  �ļ�����
typedef bool(*IMPORTMESHFUN)(QString, int);
//��������  �ļ�����  id
typedef bool(*EXPORTMESHFUN)(QString, int);

namespace IO
{
	class IOAPI IOConfigure
	{
	public:
		IOConfigure() = default;
		~IOConfigure() = default;

		//ע��д���ĺ�׺�뷽��
		static void RegisterInputFile(QString suffix, WRITEINPFILE fun);
		//ע�����ļ�ת������
		static void RegisterOutputTransfer(QString name, TRANSFEROUTFILE fun);
		//ע�������ļ�����ķ���
		static void RegisterMeshImporter(QString suffix, IMPORTMESHFUN fun);
		//ע�����񵼳��ķ���
		static void RegisterMeshExporter(QString suffix, EXPORTMESHFUN fun);

		//�Ƴ�д���ļ��ĺ�׺ע��
		static void RemoveInputFile(QString s);
		//�Ƴ�����ļ�ת��
		static void RemoveOutputTransfer(QString name);
		//�Ƴ���������ķ���
		static void RemoveMeshImporter(QString suffix);
		//�Ƴ���������ķ���
		static void RemoveMeshExporter(QString suffix);

		//��ȡ���������ļ���ʽ
		static QStringList getInputFileFormat();
		//��ȡ��������ļ�ת����
		static QStringList getOutputFileTransfers();
		//��ȡ����ע���������뷽��
		static QStringList getMeshImporters();
		//��ȡ����ע���������뷽��
		static QStringList getMeshExporters();

		//��ȡ�ļ�д������
		static WRITEINPFILE getInputFileWriter(QString format);
		//��ȡ�ļ�ת������
		static TRANSFEROUTFILE getOutputTransfer(QString tras);
		//��ȡ�������ķ���
		static IMPORTMESHFUN getMeshImporter(QString suffix);
		//��ȡ���񵼳��ķ���
		static EXPORTMESHFUN getMeshExporter(QString suffix);

	private:
		//��׺ - ����
		static QHash<QString, WRITEINPFILE> _inpWriteFun;  
		static QHash<QString, TRANSFEROUTFILE> _outFileTransfer;
		static QHash<QString, IMPORTMESHFUN> _inputmeshFuns;
		static QHash<QString, EXPORTMESHFUN> _exportMeshFuns;
	};
}




#endif