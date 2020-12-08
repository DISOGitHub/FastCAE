#ifndef SOLUTIONDATAIO_H
#define SOLUTIONDATAIO_H
#include "solutiondataio_global.h"
#include "qstring.h"
#include "vtkFloatArray.h"
#include "vtkCell.h"
#include "vtkCellArray.h"
#include "vtkCellType.h"
#include "vtkDataSet.h"
#include "vtkCellData.h"
#include "vtkPointData.h"
#include "qmap.h"
#include "vtkIdTypeArray.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPoints.h"
#include "vtkMultiBlockDataSet.h"
#include <vtkAutoInit.h>
#include "qfile.h"
#include "qtextstream.h"
#include "vtkIdList.h"
#include "qdebug.h"
#define Data_Interval "   "
#define Num_ZoneId_Per_Row 10
#define pointsID "POINTS"
#define cellsID "CELLS"
#define pScalarValID "SCALAR_POINTS_VALUE"
#define cScalarValID "SCALAR_CELLS_VALUE"
#define pNormalValID "NORMAL_POINTS_VALUE"
#define cNormalValID "NORMAL_CELLS_VALUE"
#define zoneCellIDList "ZONE_CELL_IDS"
#define zoneID "ZONE"
#define zoneStartID "zoneStart!"
#define zoneEndID "zoneEnd!"
#define zonePointsID "ZONE_POINTS"
#define zoneCellsID "ZONE_CELLS"
#define pZoneScalarValID "ZONE_SCALAR_POINTS_VALUE"
#define cZoneScalarValID "ZONE_SCALAR_CELLS_VALUE"
#define pZoneNormalValID "ZONE_NORMAL_POINTS_VALUE"
#define cZoneNormalValID "ZONE_NORMAL_CELLS_VALUE"
#define floatType "FLOAT"
class SOLUTIONDATAIO_EXPORT SolutionDataIO 
{
public:
	SolutionDataIO();
	~SolutionDataIO();
	bool writeSolutionData(QString fileName, vtkDataSet* dataSet);
	bool write_Points(QString fileName, vtkDataSet* dataSet, QString strID);
	bool write_Cells(QString fileName, vtkDataSet* dataSet, QString strID);
	bool write_Points_Value(QString fileName, vtkDataSet* dataSet, QString strID);
	bool write_Cells_Value(QString fileName, vtkDataSet* dataSet, QString strID);
	bool write_Zones(QString fileName, QMap<QString, vtkSmartPointer<vtkIdTypeArray>> tep_zoneMap);
	bool write_Zones(QString fileName, vtkMultiBlockDataSet* blockDataSet);
	bool write_ZonesString(QString fileName, QString strID);

	vtkDataSet* readSolutionData(QString fileName);
	bool read_Points(int num_points, vtkUnstructuredGrid* tep_grid1);
	bool read_Cells(int num_cells, vtkUnstructuredGrid* tep_grid1);
	bool read_scalarPValue(int num_scalarPVal, QString name, vtkUnstructuredGrid* tep_grid1);
	bool read_normalPValue(int num_scalarPVal, QString name, vtkUnstructuredGrid* tep_grid1);
	bool read_scalarCValue(int num_scalarCVal, QString name, vtkUnstructuredGrid* tep_grid1);
	bool read_normalCValue(int num_scalarCVal, QString name, vtkUnstructuredGrid* tep_grid1);
	bool read_ZonesCellID(int num_Ids, QString name);
	
	QMap<QString, vtkSmartPointer<vtkIdTypeArray>> zoneCell_map;
	vtkMultiBlockDataSet* blockDataSet;
private:
	int cur_num_points, cur_num_cells;
	vtkUnstructuredGrid* tep_grid;
	vtkUnstructuredGrid* tep_zoneGrid;
	QFile rFile;
	
};

#endif // SOLUTIONDATAIO_H
