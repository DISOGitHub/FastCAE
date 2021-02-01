#ifndef _VTKDATAEXCHANGE_H_
#define _VTKDATAEXCHANGE_H_

#include "meshDataExchangePlugin.h"
#include <vtkSmartPointer.h>
#include "MeshThreadBase.h"

class vtkDataSet;
class vtkDataSetReader;
class vtkSTLReader;
class vtkTecplotReader;

namespace MeshData
{
	class MeshData;

	class MESHDATAEXCHANGEPLUGINAPI VTKdataExchange :public MeshThreadBase
	{
	public:
		VTKdataExchange(const QString &fileName, MeshOperation operation, GUI::MainWindow *mw, int modelId = -1);
		~VTKdataExchange() = default;				

		void run();
		bool read();
		bool write();

	private:
		vtkDataSet* readVTK();
		vtkDataSet* readSTL();
		vtkDataSet* readTecplot();

	private:
		QString _fileName;	
		MeshData* _meshData{};
		vtkSmartPointer<vtkDataSetReader> _vtkReader{};
		vtkSmartPointer<vtkSTLReader> _stlReader{};
		vtkSmartPointer<vtkTecplotReader> _tecplotReader{};
		MeshOperation _operation;
		int _modelId;
	};
}
#endif
