#ifndef _NEUDATAEXCHANGE_H_
#define _NEUDATAEXCHANGE_H_

#include "meshDataExchangePlugin.h"
#include "MeshThreadBase.h"

class QTextStream;
class vtkUnstructuredGrid;
class vtkDataSet;

namespace MeshData
{
	class MeshData;

	class MESHDATAEXCHANGEPLUGINAPI NEUdataExchange :public MeshThreadBase
	{
	public:
		NEUdataExchange(const QString &fileName, MeshOperation operation, GUI::MainWindow *mw, int modelId = -1);
		~NEUdataExchange();

		void run();
		bool read();
		bool write();

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
		int _modelId;
		MeshOperation _operation;
	};
}

#endif
