#ifndef _CGNSDATAEXCHANGE_H_
#define _CGNSDATAEXCHANGE_H_

#include "meshDataExchangePlugin.h"
#include "MeshThreadBase.h"
#include <QVector>

//#ifdef Q_OS_WIN
#include <cgnslib.h>
//#endif

class vtkDataSet;

namespace MeshData
{
	class MeshData;
	class MeshKernal;

	struct Vertex
	{
		int id = 0;
		double x = 0;
		double y = 0;
		double z = 0;
	};

	class MESHDATAEXCHANGEPLUGINAPI CGNSdataExchange : public MeshThreadBase
	{		
	public:
		CGNSdataExchange(const QString &filename, MeshOperation operation, GUI::MainWindow *mw, int modelId = -1);
		~CGNSdataExchange();

		void run();
		bool read();
		bool write();

	private:
		bool readZone(int file_index, int base_index, int zone_index);
		bool readStructuredZone(int file_index, int base_index, int zone_index);
		bool readUnStructuredZone(int file_index, int base_index, int zone_index);
		bool readBC(int file_index, int base_index, int zone_index, int zonei, int zonej, int zonek, MeshKernal* k);

		void readCoordinate(int file_index, int base_index, int zone_index, int vertext_num, /*int*/cgsize_t range_from[3], /*int*/cgsize_t range_to[3], int & cooridateNum, vtkDataSet* dataset);
		void readSections(int file_index, int base_index, int zone_index, MeshKernal* kernal);
		void readCells(int start_index, int end_index, int type, cgsize_t* elements /*int* elements*/, int element_data_size, MeshKernal* kernal);

	private:
		QString _fileName{};
		MeshData* _meshData{};
		int _cellOffset{ 0 };
		QVector<Vertex> _verlist{};
		MeshOperation _operation;
		int _modelId;
	};
}
#endif