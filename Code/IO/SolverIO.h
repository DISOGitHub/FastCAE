#ifndef _SOLVERIO_H_
#define _SOLVERIO_H_

#include "IOAPI.h"

namespace ModelData
{
	class ModelDataBase;
}

namespace DataProperty
{
	class DataBase;
}

namespace IO
{
	class GenerateMesh;

	class IOAPI SolverIO
	{
	public:
		SolverIO() = default;
		~SolverIO() = default;
		//д���ļ�
		static bool writeInpFile(QString suffix, ModelData::ModelDataBase* data);
		//��������ļ�ģ���滻
		static bool replaceTemplate(QString templa, QString path, DataProperty::DataBase* data);
		//����ļ�ת��
		static bool transformFile(QString trans, QString path);

	private:
		static bool writeXml(ModelData::ModelDataBase* m);
		
	private:
		static GenerateMesh gm;
	};

}


#endif