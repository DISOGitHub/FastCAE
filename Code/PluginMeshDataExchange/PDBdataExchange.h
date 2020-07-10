#ifndef PDBDATAEXCHANGE_H
#define PDBDATAEXCHANGE_H

#include "MeshThreadBase.h"
#include <QTextStream>
#include <QHash>

class vtkUnstructuredGrid;

namespace MeshData{
	class MeshData;

	class MESHDATAEXCHANGEPLUGINAPI PDBdataExchange :public MeshThreadBase
	{
	public:
		PDBdataExchange(const QString &fileName, MeshOperation operation, GUI::MainWindow *mw, int KernalId = -1);
		~PDBdataExchange();

		void run() override;

	private: 
		bool read();
		bool write();

	private:
		QString _fileName{};
//		QTextStream* _stream{};

//		int _nodeNum{ 0 };
//		int _cellNum{ 0 };
//
	//	QHash<int, int> _nodeIdIndex{};

		MeshData* _meshData{};
		//vtkUnstructuredGrid* _gird{};
		int _writeFileKid;
		MeshOperation _operation;

	};
}

#endif


