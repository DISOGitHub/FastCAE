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

		/*��������*/
		void addBasicPara(ModelData::ModelDataBase* model, QTextStream& stream);
		/*��������*/
		void addFlowPara(ModelData::ModelDataBase* model, QTextStream& stream);
		/*�������*/
		void addGridPara(ModelData::ModelDataBase* model, QTextStream& stream);
		/*����ģ��*/
		void addPhysicalModelPara(ModelData::ModelDataBase* model, QTextStream& stream);
		/*�ռ���ɢ��*/
		void SpatialStruct(ModelData::ModelDataBase* model, QTextStream& stream, bool istr);
		/*ʱ����ɢ��*/
		void addTimeDiscretePara(ModelData::ModelDataBase* model, QTextStream& stream);
		/*·�����ü�����*/
		void addOtherPara(ModelData::ModelDataBase* model, QTextStream& stream);
		QString getMeshName();
		/*����ļ���*/
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