#ifndef _MSHDATAEXCHANGE_H_
#define _MSHDATAEXCHANGE_H_

#include "MeshThreadBase.h"
#include "meshDataExchangePlugin.h"
#include <QHash>

class QTextStream;
class vtkUnstructuredGrid;
class QFile;

namespace MeshData
{ 
	class MeshData;
	class MeshSet;

	enum meshType
	{
		typeNone = 0,
		typeGambit, typeFluent,
	};
	//�����Ϣ
	typedef struct
	{
		int *vertics;
		int cl;
		int cr;
		int type;
		QList<int> pointList;
	}FaceData;

	typedef struct
	{
		int first_index;
		int last_index;
		int zone_id;
		int nd;
		int type;
		int element_type;
		QList<FaceData> facedatas;
	}MshFaces;//13�����б�

	class MESHDATAEXCHANGEPLUGINAPI MSHdataExchange : public MeshThreadBase
	{
	public:
		MSHdataExchange(const QString& fileName, MeshOperation operation, GUI::MainWindow *mw, int modelId = -1);
		~MSHdataExchange();

		bool read();
		QString readLine();
		//��ȡ�ļ�ͷ�ж���GAMBIT to Flent�ļ�����Flent�����ļ�
		bool readHeader();
		bool write();
		void run() ;

	private:
		meshType mMeshType{ typeNone };
		int _totalNumber;
		//��ȡ16���Ƶ�
		void readPoints10(vtkUnstructuredGrid* dataset, QString info);		
		void readPoints130(vtkUnstructuredGrid* dataset, QString info);
		void readCells120(vtkUnstructuredGrid* dataset, QString info);
		void readFluentCells12(vtkUnstructuredGrid* dataset, QString info);
		void readZone45(QString zone);
		//wzy������ڶ�ȡ��
		bool readFace13(vtkUnstructuredGrid* dataset, QString info);
		//ȥ��ͷ��־��һ���е����ŵ���Ϣ
		QString getNextLineInfo(QString line);
		//�ж�ͷ��Ϣ���Ƿ���16�����ַ���
		bool isHex(QString line);
		//�ж������ַ�������16���ƻ���10���ƣ����ò�ͬת������
		int  strToInt(QString line,bool isHex=false);
		//����Gambit����fluent�ļ�
		bool readGambitFile(vtkUnstructuredGrid* dataset);
		//����Flent�ļ�
		bool readFluentFile(vtkUnstructuredGrid* dataset);

	private:
		QFile *_file;
		QString _baseFileName{};
		QString _filePath{};
		QString _fileName{};
		MeshData* _meshData{};
		QTextStream* _stream{};
		QHash<int, MeshSet*> _idsetList{};

		QString _describe{};
		int _ND{ 3 };
		int _staticid{ 0 };
		int _modelId;
		MeshOperation _operation;	
	};
}
#endif 