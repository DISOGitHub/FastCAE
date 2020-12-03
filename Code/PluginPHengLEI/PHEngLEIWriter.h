#ifndef _PHENGLEIPLUGINWRITER_H__
#define _PHENGLEIPLUGINWRITER_H__

#include "PHEngLEIPluginAPI.h"
#include <QObject>
class QTextStream;
namespace ModelData
{
	class ModelDataBase;
}
namespace Plugins
{
	class PHENGLEIPLUGINAPI PHengLEIWriter: public QObject
	{
		Q_OBJECT
	public:
		PHengLEIWriter() = default;
		~PHengLEIWriter() = default;
		
		void writeHyparaFile(ModelData::ModelDataBase* model, QString filename);
		void modifyKeyFile(ModelData::ModelDataBase* model, QString filename);
		void startSolver(ModelData::ModelDataBase* model,QString process, bool Mpi);

		/*基本参数*/
		void addBasicPara(ModelData::ModelDataBase* model, QTextStream& stream);
		/*来流参数*/
		void addFlowPara(ModelData::ModelDataBase* model, QTextStream& stream);
		/*网格相关*/
		void addGridPara(ModelData::ModelDataBase* model, QTextStream& stream);
		/*物理模型*/
		void addPhysicalModelPara(ModelData::ModelDataBase* model, QTextStream& stream);
		/*空间离散化*/
		void SpatialStruct(ModelData::ModelDataBase* model, QTextStream& stream, bool istr);
		/*时间离散化*/
		void addTimeDiscretePara(ModelData::ModelDataBase* model, QTextStream& stream);
		/*路径设置及其他*/
		void addOtherPara(ModelData::ModelDataBase* model, QTextStream& stream);
		QString getMeshName();
		/*清空文件夹*/
		void clearTempFiles(const QString& temp_path);

	public slots:
		void fileUpdated(QString filepath);

	private:
		void writeKeyFile(ModelData::ModelDataBase* model);
		void writeGridPara(ModelData::ModelDataBase* model);
		void writepartition(ModelData::ModelDataBase* model);
		void writeParaSubsonic(ModelData::ModelDataBase* model);
		QString readPhengLeiFile(ModelData::ModelDataBase* model, QString filename);
	
		

	};
}


#endif