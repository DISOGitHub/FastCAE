#ifndef INPFWRITER_H
#define INPFWRITER_H

#include <QFile>
#include <QTextStream>

class vtkDataSet;

namespace MeshData
{
	class MeshSet;
}


namespace ThermalFluid{
	struct DataCut
	{
		int start{ -1 };
		int end{ -1 };
		int step{ -1 };
	};
	
	class TFCouplingModelData;
	class ThermalData;
	class BoundaryTabTypeData;

	class INPFWriter
	{
	public:
		INPFWriter(QString path, TFCouplingModelData* data);
		~INPFWriter();

		bool write();
	private:
		void writePart1();
		void writePart2();
		void writePart3();
		void writePart4(vtkDataSet*);
		void writePart5(vtkDataSet*);
		void writePart6();
		void writePart7();
		void writePart8();
		void writePart9(int kid);
		void writePart10();


		void writeThickness();
		void writeMaterial();
		void writeThermalBoundary();
		void writeControlTemperature();
		void writeTableTypeData();
		void writeMeshGroups(int kid);
		void writeAMeshGroup(MeshData::MeshSet*,int);
		void writeTableData(BoundaryTabTypeData* tb, QString id);

		QString fillSpaceToString(QString s, int size,bool b = true);

		QList<DataCut> CutList(QList<int> & datalist);
	private: 
		QFile* _file;
		QTextStream* _writeStream;

		TFCouplingModelData* _modelData{};
		ThermalData* _thermalData{};
	};
}
#endif


