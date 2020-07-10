#ifndef MESHREADER_H
#define MESHREADER_H

#include <QString>
#include <QTextStream>
#include <vtkCellType.h>

class vtkUnstructuredGrid;
class vtkDataSet;

namespace MeshData{
	class MeshData;
	class MeshKernal;
}

namespace Gmsh{
	class MeshReader
	{
	public:
		MeshReader(QString file);
		~MeshReader();

		bool read();

	private:
		void readLine(QString &line, bool b = false);
		bool readNodes(vtkUnstructuredGrid* g, QString &line);
		bool readElements(vtkUnstructuredGrid* g, QString &line);
		bool readNSet(QString &line, MeshData::MeshKernal* k);
		bool readElSet(QString &line, MeshData::MeshKernal* k);
		VTKCellType getVTKCellType(QString s);

		/*bool judgetIndex(int d);*/

	private:
		QString _fileName{};
		QTextStream* _stream{};
		MeshData::MeshData* _meshData{};
		QHash<int, int> _nodeIDIndex{};
		QHash<int, int> _elemIDIndex{};

		int _nIndex{ 0 };
		int _eIndex{ 0 };

		/*QList<int> _indexList{};*/
	};
}

#endif // MESHREADER_H


