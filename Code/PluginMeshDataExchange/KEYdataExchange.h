#ifndef _KEYDATAEXCHANGE_H_
#define _KEYDATAEXCHANGE_H_

#include "MeshThreadBase.h"
#include "meshDataExchangePlugin.h"
#include <vtkSmartPointer.h>
#include <vtkUnstructuredGrid.h>

class QTextStream;

namespace MeshData
{
	class MeshData;
	class MESHDATAEXCHANGEPLUGINAPI KEYdataExchange : public MeshThreadBase
	{
		public:		
			KEYdataExchange(const QString &fileName, MeshOperation operation, GUI::MainWindow *mw, int modelId = -1);
			~KEYdataExchange();

			void run();
			bool read();
			bool write();

		private:			
			void readLine(QString&);
			bool readNodes(vtkSmartPointer<vtkUnstructuredGrid>, QString&);
			bool readElements(vtkSmartPointer<vtkUnstructuredGrid>, QString&);
			bool readMeshGroups(const int, QString&);
			void setGridCells(vtkSmartPointer<vtkUnstructuredGrid>, QString&);

			bool writePointPart(vtkDataSet*);
			bool writeCellPart(vtkDataSet*);
			bool writeMeshPart();

		private:
			MeshOperation _operation;
			const QString _fileName;
			MeshData* _meshData{};
			QTextStream* _stream{};
			QString _preLine{};
			int _modelId;
	};
}

#endif