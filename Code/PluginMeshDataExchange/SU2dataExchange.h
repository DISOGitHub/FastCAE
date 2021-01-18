#ifndef _SU2DATAEXCHANGE_H_
#define _SU2DATAEXCHANGE_H_

#include "meshDataExchangePlugin.h"
#include <QHash>
#include "MeshThreadBase.h"

class QTextStream;
class vtkUnstructuredGrid;
class vtkDataSet;

namespace MeshData
{
	class MeshData;

	class MESHDATAEXCHANGEPLUGINAPI SU2dataExchange :public MeshThreadBase
	{
	public:
		SU2dataExchange(const QString &fileName, MeshOperation operation, GUI::MainWindow *mw, int modelId = -1);
		~SU2dataExchange();
	
		void run();
		bool read();
		bool write();

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
		//vtkUnstructuredGrid* _gird{};
		int _modelId;
		MeshOperation _operation;
		QList<vtkUnstructuredGrid*> _gridList{};
		int _gridCount{ 0 };
	};
}


#endif