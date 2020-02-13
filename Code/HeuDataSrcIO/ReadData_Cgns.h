#ifndef READDATACGNS_H
#define READDATACGNS_H

#include "ReadData_Base.h"
#include "vtkUnstructuredGrid.h"
#include "vtkSmartPointer.h"
#include "vtkCellType.h"

typedef enum tag_MG_CGNS_GridType
{
	MG_CGNS_GridStructured = 0,
	MG_CGNS_GridUnStructured,
}MG_CGNS_GridType;

typedef struct tag_MG_CGNS_Point
{
	double x;
	double y;
	double z;
	int pt_index;
}MG_CGNS_Point;
typedef QVector<MG_CGNS_Point> MG_CGNS_Points;

typedef enum tag_MG_CGNS_FlowSolutionLocation
{
	MG_CGNS_Vertex = 0,
	MG_CGNS_CellCenter,
}MG_CGNS_FlowSolutionLocation;

typedef struct tag_MG_CGNS_FlowSolution
{
	char cName[33];
	MG_CGNS_FlowSolutionLocation loc;
	QMap<QString, QVector<double>> vars;
}MG_CGNS_FlowSolution;

typedef QVector<MG_CGNS_FlowSolution> MG_CGNS_FlowSolutions;

///<MG 单一区域不同时刻流场解
typedef struct tag_MG_CGNS_Zone_FlowSolution
{
	MG_CGNS_FlowSolutions flows;
	int file_index;
	int base_index;
	int zone_index;
}MG_CGNS_Zone_FlowSolution;

typedef struct tag_MG_CGNS_FAMILY
{
	QString zone_name;///<MG Zone_t FamilyName_t  Or  ZoneBC_t BC_t FamilyName_t  's Name
	int type;/// 0 Zont_t FamilyName_t  1 ZoneBC_t BC_t FamilyName_t 
	QString family_name;	///<MG FamilyName_t 's Data
}MG_CGNS_FAMILY;

typedef struct tag_MG_CGNS_FAMILY_TOTAL
{
	QString family_name;				///<MG FamilyName_t  's Name
	int nbocos;							///<MG relation BC_t or Zone_t Number
	int ngeos;							///<MG GeometryReference_t Number
}MG_CGNS_FAMILY_TOTAL;

typedef QVector<MG_CGNS_FAMILY_TOTAL> MG_CGNS_FAMILY_TOTALS;

///<MG 所有区域不同时刻流场解
typedef QVector<MG_CGNS_Zone_FlowSolution> tag_MG_CGNS_Zone_FlowSolutions;

class HEUDATASRCIOSHARED_EXPORT ReadData_Cgns : public ReadData_Base
{
public:
	ReadData_Cgns();
	~ReadData_Cgns();

	virtual bool Read(QString filePath) override;

	virtual QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkFloatArray>>> getCgnsValue(int time) override;

private:
	int m_currentFileIndex{ -1 };
	int m_currentBaseIndex{ -1 };
	tag_MG_CGNS_Zone_FlowSolutions  m_flowSolutions;

	///<MG Key当前区域、子区域名称、Value 区域 类型、区域的族名称  用于记录 区域 属于哪个族
	QMap<QString, MG_CGNS_FAMILY> zone_families;
	MG_CGNS_FAMILY_TOTALS all_zones;
	///<MG 用于记录项目中的族信息

private:
	bool ReadZone(int file_index, int base_index, int zone_index);

	bool ReadStructedZone(int file_index, int base_index, int zone_index);

	bool ReadUnStructedZone(int file_index, int base_index, int zone_index);

	void ShowGrid(vtkDataSet * dataSet);

	int GetPtIndexInStructedGrid(int iPos, int jPos, int kPos, int iMax, int jMax, int kMax);

	bool ReadGridCooridate(int file_index, int base_index, int zone_index,int vertext_num, int range_from[3], int range_to[3], int & cooridateNum, MG_CGNS_Points & pts);

	bool ReadFlowSolution(int file_index, int base_index, int zone_index, MG_CGNS_GridType gridType, int dimension[3], MG_CGNS_FlowSolutions & sols);

	MG_CGNS_FlowSolutions GetWithValFlowSolutions(MG_CGNS_FlowSolutions sols);

	bool AddFlowSolutionToGrid(MG_CGNS_FlowSolution flowSolution, vtkSmartPointer<vtkUnstructuredGrid> uGrid);

	VTKCellType getWithResultCellType(vtkSmartPointer<vtkUnstructuredGrid> uGrid,int resultNum);
};
#endif

