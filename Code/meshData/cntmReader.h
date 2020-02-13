#ifndef CNTMREADER_H_
#define CNTMREADER_H_

#include "meshDataAPI.h"
#include <QString>
#include <QHash>

class QTextStream;
class vtkUnstructuredGrid;
class vtkDataSet;

namespace MeshData
{
	class MeshData;

	class MESHDATAAPI CntmReader
	{
	public:
		CntmReader(const QString &filename);
		~CntmReader();

		bool read();

	private:
		QString readLine();
		bool readNodes();
		bool readElements();

	private:
		QString _fileName{};
		QTextStream* _stream{};

		int _nodeNum{ 0 };
		int _cellNum{ 0 };

		QHash<int, int> _nodeIdIndex{};

		MeshData* _meshData{};
		vtkUnstructuredGrid* _gird{};

	};


}


#endif