#ifndef _CNTMDATAEXCHANGE_H_
#define _CNTMDATAEXCHANGE_H_

#include "MeshThreadBase.h"
#include "meshDataExchangePlugin.h"
#include <QHash>

class QTextStream;
class vtkUnstructuredGrid;
class vtkDataSet;

namespace MeshData
{	
	class MeshData;

	class MESHDATAEXCHANGEPLUGINAPI CNTMdataExchange : public MeshThreadBase
	{
	public:
		CNTMdataExchange(const QString &filename, MeshOperation operation, GUI::MainWindow *mw, int modelId = -1);
		~CNTMdataExchange();

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
		vtkUnstructuredGrid* _gird{};
		int _modelId;
		MeshOperation _operation;
	};
}


#endif