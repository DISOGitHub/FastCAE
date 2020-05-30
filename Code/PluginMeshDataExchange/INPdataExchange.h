#ifndef _INPDATAEXCHANGE_H_
#define _INPDATAEXCHANGE_H_

#include "meshDataExchangePlugin.h"
#include <QHash>
#include "MeshThreadBase.h"

class QTextStream;
class vtkUnstructuredGrid;
class vtkDataSet;

namespace MeshData
{
	class MeshData;

	class MESHDATAEXCHANGEPLUGINAPI INPdataExchange :public MeshThreadBase
	{
	public:
		INPdataExchange(const QString &filename, MeshOperation operation, GUI::MainWindow *mw ,int writeFileKid = -1);
		~INPdataExchange();

		void run();
		bool read();
		bool write();

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
		int _writeFileKid;
		MeshOperation _operation;
	};
}
#endif