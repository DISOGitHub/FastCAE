#ifndef _INPREADER_H_
#define _INPREADER_H_

#include "meshDataAPI.h"
#include <QString>
#include <QHash>

class QTextStream;
class vtkUnstructuredGrid;
class vtkDataSet;

namespace MeshData
{
	class MeshData;

	class MESHDATAAPI InpReader
	{
	public:
		InpReader(const QString &filename);
		~InpReader();

		bool read();

	private:
		void readLine(QString &line);
		bool readNodes(vtkUnstructuredGrid* g, QString &line);
		bool readElements(vtkUnstructuredGrid* g, QString &line);
		bool readNSet(QString &line);
		bool readElSet(QString &line);

	private:
		QString _fileName{};
		QTextStream* _stream{};

		MeshData* _meshData{};
		QHash<int, int> _nodeIDIndex{};
		QHash<int, int> _elemIDIndex{};

	};


}


#endif