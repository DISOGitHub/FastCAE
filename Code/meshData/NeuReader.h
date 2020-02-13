#ifndef _NEUREADER_H_
#define _NEUREADER_H_

#include "meshDataAPI.h"
#include <QString>


class QTextStream;
class vtkUnstructuredGrid;
class vtkDataSet;

namespace MeshData
{
	class MeshData;

	class MESHDATAAPI NeuReader
	{
	public:
		NeuReader(const QString &filename);
		~NeuReader();

		bool read();
		bool write(int kenerlID);

	private:
		bool readBasicInfo();
		bool readPoints(vtkUnstructuredGrid* dateset);
		bool readElements(vtkUnstructuredGrid* dateset);
		QString readLine();

		void writeBasicInfo();
		void writepoints(vtkDataSet* dateset);
		void writeElements(vtkDataSet* dateset);

	private:
		QString _fileName{};
		MeshData* _meshData{};
		QTextStream *_stream{};
		int _pointNum{ -1 };
		int _eleNum{ -1 };
	};
}

#endif
