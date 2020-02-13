#ifndef VTKREADER_H
#define VTKREADER_H

#include "meshDataAPI.h"
#include <QString>
#include <vtkSmartPointer.h>

class vtkDataSet;
class vtkDataSetReader;
class vtkSTLReader;
class vtkTecplotReader;


namespace MeshData
{
	class MeshData;

	class MESHDATAAPI VTKReader
	{
	public:
		VTKReader(const QString &filename);
		~VTKReader() = default;
		bool read();
		bool write(int kenerlID);

	private:
		vtkDataSet* readVTK();
		vtkDataSet* readSTL();
		vtkDataSet* readTecplot();

	private:
		QString _filename;
		
		MeshData* _meshData{};
		vtkSmartPointer<vtkDataSetReader> _vtkReader{};
		vtkSmartPointer<vtkSTLReader> _stlReader{};
		vtkSmartPointer<vtkTecplotReader> _tecplotReader{};
	};
}


#endif
