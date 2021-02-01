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
		//写出文件
		static bool writeInpFile(QString suffix, ModelData::ModelDataBase* data);
		//求解输入文件模板替换
		static bool replaceTemplate(QString templa, QString path, DataProperty::DataBase* data);
		//结果文件转换
		static bool transformFile(QString trans, QString path);

	private:
		static bool writeXml(ModelData::ModelDataBase* m);
	};
}
#endif