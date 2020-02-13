#ifndef _CGNSREADER_H_
#define _CGNSREADER_H_

#include "meshDataAPI.h"
#include <QString>
#include <QVector>

class vtkDataSet;

namespace MeshData
{
	struct Vertex
	{
		int id = 0;
		double x = 0;
		double y = 0;
		double z = 0;
	};


	class MeshData;
	class MeshKernal;

	class MESHDATAAPI CgnsReader
	{
	public:
		CgnsReader(const QString &filename);
		~CgnsReader();

		bool read();

	private:
		bool readZone(int file_index, int base_index, int zone_index);
		bool readStructuredZone(int file_index, int base_index, int zone_index);
		bool readUnStructuredZone(int file_index, int base_index, int zone_index);
		bool readBC(int file_index, int base_index, int zone_index, int zonei, int zonej, int zonek, MeshKernal* k);

		void readCoordinate(int file_index, int base_index, int zone_index, int vertext_num, int range_from[3], int range_to[3], int & cooridateNum, vtkDataSet* dataset);
		void readSections(int file_index, int base_index, int zone_index, MeshKernal* kernal);
		void readCells(int start_index, int end_index, int type, int* elements, int element_data_size, MeshKernal* kernal);

	private:
		QString _filename{};
		MeshData* _meshData{};

		int _cellOffset{ 0 };
		QVector<Vertex> _verlist{};

	};

}



#endif