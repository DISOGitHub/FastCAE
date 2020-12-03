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

//写出的路径与模型
typedef bool(*WRITEINPFILE)(QString, ModelData::ModelDataBase*);
//转换原文件路径
typedef bool(*TRANSFEROUTFILE)(QString);
//导入网格  文件名称
typedef bool(*IMPORTMESHFUN)(QString, int);
//导出网格  文件名称  id
typedef bool(*EXPORTMESHFUN)(QString, int);

namespace IO
{
	class IOAPI IOConfigure
	{
	public:
		IOConfigure() = default;
		~IOConfigure() = default;

		//注册写出的后缀与方法
		static void RegisterInputFile(QString suffix, WRITEINPFILE fun);
		//注册结果文件转换方法
		static void RegisterOutputTransfer(QString name, TRANSFEROUTFILE fun);
		//注册网格文件导入的方法
		static void RegisterMeshImporter(QString suffix, IMPORTMESHFUN fun);
		//注册网格导出的方法
		static void RegisterMeshExporter(QString suffix, EXPORTMESHFUN fun);

		//移除写出文件的后缀注册
		static void RemoveInputFile(QString s);
		//移除结果文件转换
		static void RemoveOutputTransfer(QString name);
		//移除导入网格的方法
		static void RemoveMeshImporter(QString suffix);
		//移除导出网格的方法
		static void RemoveMeshExporter(QString suffix);

		//获取所有输入文件格式
		static QStringList getInputFileFormat();
		//获取所有输出文件转换器
		static QStringList getOutputFileTransfers();
		//获取所有注册的网格读入方法
		static QStringList getMeshImporters();
		//获取所有注册的网格读入方法
		static QStringList getMeshExporters();

		//获取文件写出方法
		static WRITEINPFILE getInputFileWriter(QString format);
		//获取文件转换方法
		static TRANSFEROUTFILE getOutputTransfer(QString tras);
		//获取网格读入的方法
		static IMPORTMESHFUN getMeshImporter(QString suffix);
		//获取网格导出的方法
		static EXPORTMESHFUN getMeshExporter(QString suffix);

	private:
		//后缀 - 方法
		static QHash<QString, WRITEINPFILE> _inpWriteFun;  
		static QHash<QString, TRANSFEROUTFILE> _outFileTransfer;
		static QHash<QString, IMPORTMESHFUN> _inputmeshFuns;
		static QHash<QString, EXPORTMESHFUN> _exportMeshFuns;
	};
}




#endif