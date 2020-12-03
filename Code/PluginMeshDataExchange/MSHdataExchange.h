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
	//面的信息
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
	}MshFaces;//13，面列表

	class MESHDATAEXCHANGEPLUGINAPI MSHdataExchange : public MeshThreadBase
	{
	public:
		MSHdataExchange(const QString& fileName, MeshOperation operation, GUI::MainWindow *mw, int modelId = -1);
		~MSHdataExchange();

		bool read();
		QString readLine();
		//读取文件头判断是GAMBIT to Flent文件还是Flent生成文件
		bool readHeader();
		bool write();
		void run() ;

	private:
		meshType mMeshType{ typeNone };
		int _totalNumber;
		//读取16进制点
		void readPoints10(vtkUnstructuredGrid* dataset, QString info);		
		void readPoints130(vtkUnstructuredGrid* dataset, QString info);
		void readCells120(vtkUnstructuredGrid* dataset, QString info);
		void readFluentCells12(vtkUnstructuredGrid* dataset, QString info);
		void readZone45(QString zone);
		//wzy添加用于读取面
		bool readFace13(vtkUnstructuredGrid* dataset, QString info);
		//去掉头标志下一处中的括号等信息
		QString getNextLineInfo(QString line);
		//判断头信息中是否有16进制字符串
		bool isHex(QString line);
		//判断输入字符串，是16进制还是10进制，调用不同转换方法
		int  strToInt(QString line,bool isHex=false);
		//解析Gambit生成fluent文件
		bool readGambitFile(vtkUnstructuredGrid* dataset);
		//解析Flent文件
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