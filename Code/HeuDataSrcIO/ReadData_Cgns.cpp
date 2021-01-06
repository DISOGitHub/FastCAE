#include "ReadData_Cgns.h"
#include "QFile"
#include "QVector"
#include <assert.h>
#include "vtkPolyData.h"
#include "vtkUnstructuredGrid.h"
#include "vtkStructuredGrid.h"
#include "vtkDataSetMapper.h"
#include "vtkActor.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkPolyDataMapper.h"
#include "vtkInteractorStyleTrackball.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkDataSetWriter.h"
#include "vtkMergeCells.h"
#include "vtkMergeDataObjectFilter.h"
#include "vtkDataSet.h"

#include "cgnslib.h"

typedef enum tag_MG_CGNS_Cell_Type
{
	CGNS_Cell_Type_Unknown = -1,
	CGNS_Cell_Type_NODE_1,
	CGNS_Cell_Type_HEXA_8,
	CGNS_Cell_Type_QUAD_4,
	CGNS_Cell_Type_TRI_3,
	CGNS_Cell_Type_TETRA_4,
	CGNS_Cell_Type_PYRA_5,
	CGNS_Cell_Type_LINE_2,
	CGNS_Cell_Type_PENTA_6,
	CGNS_Cell_Type_MIXED,
}MG_CGNS_Cell_Type;

typedef struct tag_MG_CGNS_Common_Cell
{
	int pt_index[8];
}MG_CGNS_Common_Cell;

typedef struct tag_MG_CGNS_Cell_HEXA_8
{
	int pt_index[8];
}MG_CGNS_Cell_HEXA_8;

typedef struct tag_MG_CGNS_Cell_NODE_1
{
	int pt_index;
}MG_CGNS_Cell_Node_1;

typedef struct tag_MG_CGNS_Cell_QUAD_4
{
	int pt_index[4];
}MG_CGNS_Cell_QUAD_4;

typedef struct tag_MG_CGNS_Cell_TRI_3
{
	int pt_index[3];
}MG_CGNS_Cell_TRI_3;

typedef struct tag_MG_CGNS_Cell_TETRA_4
{
	int pt_index[4];
}MG_CGNS_Cell_TETRA_4;

typedef struct tag_MG_CGNS_Cell_PYRA_5
{
	int pt_index[5];
}MG_CGNS_Cell_PYRA_5;

typedef struct tag_MG_CGNS_Cell_LINE_2
{
	int pt_index[2];
}MG_CGNS_Cell_LINE_2;

typedef struct tag_MG_CGNS_Cell_PENTA_6				///<MG 三棱柱
{
	int pt_index[6];
}MG_CGNS_Cell_PENTA_6;

typedef struct tag_MG_CGNS_Cell
{
	MG_CGNS_Cell_Type type{ CGNS_Cell_Type_Unknown };
	MG_CGNS_Cell_HEXA_8	hex_8;
	MG_CGNS_Cell_QUAD_4 quad_4;
	MG_CGNS_Cell_TRI_3 tri_3;
	MG_CGNS_Cell_TETRA_4 tetra_4;
	MG_CGNS_Cell_LINE_2 line_2;
	MG_CGNS_Cell_PENTA_6 penta_6;
	MG_CGNS_Cell_Node_1 node_1;
	MG_CGNS_Cell_PYRA_5 pyra_5;
	int cell_id{ -1 };
}MG_CGNS_Cell;

typedef QVector<MG_CGNS_Cell > MG_CGNS_Cells;

typedef struct tag_MG_CGNS_Section
{
	MG_CGNS_Cells cells;
	QString section_name;
	MG_CGNS_Cell_Type type{ CGNS_Cell_Type_Unknown };
}MG_CGNS_Section;

typedef QVector<MG_CGNS_Section> MG_CGNS_Sections;


ReadData_Cgns::ReadData_Cgns()
{
}


ReadData_Cgns::~ReadData_Cgns()
{
}

bool ReadData_Cgns::Read(QString filePath)
{
	QFile file(filePath);
	if (!file.exists())
	{ 
		return false; 
	}

	///<MG if this file is a cgns file
	int file_type = 0;
	if (cg_is_cgns(filePath.toLocal8Bit(), &file_type))
	{
		return false;
	}

	///<MG open cgns file and get the file index to stand of file
	if (cg_open(filePath.toLocal8Bit(), CG_MODE_READ, &m_currentFileIndex))
	{
		return false;
	}

	///<MG get CGNSBase_t node number
	if (cg_nbases(m_currentFileIndex, &m_currentBaseIndex))
	{
		return false;
	}

	for (int ibase = 1; ibase <= m_currentBaseIndex; ++ibase)
	{
		///<MG 获取族的个数
		int nfamililies = 0;
		if (cg_nfamilies(m_currentFileIndex, m_currentBaseIndex, &nfamililies))
		{
			return false;
		}

		for (int iFamily = 1; iFamily <= nfamililies; ++iFamily)
		{
			int nBoco = 0, nGeos = 0;
			char name[64] = { 0 };
			///<MG 获取族名称 FamilyBC_t GeometryReference_t 节点个数
			if (cg_family_read(m_currentFileIndex, m_currentBaseIndex, iFamily, name, &nBoco, &nGeos))
			{
				return false;
			}
			MG_CGNS_FAMILY_TOTAL one_fam;
			one_fam.family_name = name;
			one_fam.nbocos = nBoco;
			one_fam.ngeos = nGeos;
			all_zones.push_back(one_fam);
		}
		
		///<MG get Zone_t node number
		int zone_node_number = 0;
		if (cg_nzones(m_currentFileIndex, ibase, &zone_node_number))
		{
			return false;
		}

		for (int izone = 1; izone <= zone_node_number; ++izone)
		{
			ReadZone(m_currentFileIndex, ibase, izone);
		}
	}
	return true;
}

QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkFloatArray>>> ReadData_Cgns::getCgnsValue(int time)
{
	QMap<vtkDataSet*, QMap<QString, vtkSmartPointer<vtkFloatArray>>> rtn;
	if ( dataSetMap.size() != m_flowSolutions.size())
	{ 
		return rtn; 
	}

	for (int iZone = 0; iZone < m_flowSolutions.size(); ++iZone)
	{
		vtkDataSet * tempData = dataSetMap.keys().at(iZone);
		QMap < QString, vtkSmartPointer<vtkFloatArray> > zoneFlowSolution;

		if (m_flowSolutions.at(iZone).flows.size() <= time)
		{
			return rtn;
		}
		MG_CGNS_FlowSolution sol = m_flowSolutions.at(iZone).flows.at(time);

		if (sol.loc == MG_CGNS_FlowSolutionLocation::MG_CGNS_CellCenter)
		{
			int iCellNum = tempData->GetNumberOfCells();
			vtkSmartPointer<vtkUnstructuredGrid> uGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
			uGrid->DeepCopy(tempData);

			int dataIndex = 0;
			for (int iCell = 0; iCell < iCellNum; ++iCell)
			{
				//int iType = uGrid->GetCell(iCell)->GetCellType();
				if (uGrid->GetCell(iCell)->GetCellType() == VTK_HEXAHEDRON || uGrid->GetCell(iCell)->GetCellType() == VTK_TETRA)
				{ ///<MG 体网格
					for (int iType = 0; iType < sol.vars.size(); ++iType)
					{
						if (iCell == 0)
						{
							vtkSmartPointer<vtkFloatArray> _array = vtkSmartPointer<vtkFloatArray>::New();
							_array->SetNumberOfComponents(1);
							QString name = sol.vars.keys().at(iType);
							zoneFlowSolution.insert(name, _array);
						}
						double data = sol.vars.values().at(iType).at(dataIndex);
						zoneFlowSolution.values().at(iType)->InsertTuple1(dataIndex, data);
					}
					dataIndex++;
				}
				else
				{///<MG 面网格，当前默认体网格中有数据，面网格中没有数据
					for (int iType = 0; iType < sol.vars.size(); ++iType)
					{
						if (iCell == 0)
						{
							vtkSmartPointer<vtkFloatArray> _array = vtkSmartPointer<vtkFloatArray>::New();
							_array->SetNumberOfComponents(1);
							QString name = sol.vars.keys().at(iType);
							zoneFlowSolution.insert(name, _array);
						}

						zoneFlowSolution.values().at(iType)->InsertTuple1(dataIndex, 0.0);
					}
					dataIndex++;
				}
			}
			rtn.insert(tempData, zoneFlowSolution);
		}
		else if (sol.loc == MG_CGNS_FlowSolutionLocation::MG_CGNS_Vertex)
		{
			for (int iVar = 0; iVar < sol.vars.size(); ++iVar)
			{
				vtkSmartPointer<vtkFloatArray> tempArray = vtkSmartPointer<vtkFloatArray>::New();
				QString name = sol.vars.keys().at(iVar);
				tempArray->SetNumberOfComponents(1);

				QVector<double> values = sol.vars.values().at(iVar);
				for (int iVal = 0; iVal < values.size(); ++iVal)
				{
					tempArray->InsertNextTuple1(values.at(iVal));
				}
				zoneFlowSolution.insert(name, tempArray);
			}
			rtn.insert(tempData, zoneFlowSolution);
		}
	}
	return rtn;
}

bool ReadData_Cgns::ReadZone(int file_index, int base_index, int zone_index)
{
	///<MG get Zone_t node zone 's type
	CGNS_ENUMT(ZoneType_t) zonetype;
	if (cg_zone_type(file_index, base_index, zone_index, &zonetype))
	{
		return false;
	}
	if (zonetype == CGNS_ENUMV(Structured))
		return ReadStructedZone(file_index, base_index, zone_index);
	else if (zonetype == CGNS_ENUMV(Unstructured))
		return ReadUnStructedZone(file_index, base_index, zone_index);
	else
		return false;
}

bool ReadData_Cgns::ReadStructedZone(int file_index, int base_index, int zone_index)
{
	char zone_name[33] = { 0 };
	int zone_ijk[3][3] = { 0 };
	///<MG get Zone_t node 's name and every direction vertex cell number
	if (cg_zone_read(file_index, base_index, zone_index, zone_name, zone_ijk[0]))
	{
		return false;
	}

	cg_goto(file_index, base_index, "Zone_t", zone_index, "end");
	char cName[64] = { 0 }; 
	cg_famname_read(cName);

	MG_CGNS_FAMILY fam;
	fam.zone_name = zone_name;
	fam.type = 0;
	fam.family_name = cName;
	zone_families.insert(fam.zone_name, fam);

	///<MG get grid vertex number and set read whole grid data
	int vertex_number = zone_ijk[0][0] * zone_ijk[0][1] * zone_ijk[0][2];
	int read_range[2][3] = { 0 };
	for (int n = 0; n < 3; n++)
	{
		read_range[0][n] = 1;
		read_range[1][n] = zone_ijk[0][n];
	}

	int ncoords = 0;
	QVector<MG_CGNS_Point> pts;
	ReadGridCooridate(file_index, base_index, zone_index, vertex_number, read_range[0], read_range[1], ncoords, pts);

	vtkUnstructuredGrid* whole_Grid = vtkUnstructuredGrid::New();

	///<MG structured grid 
	vtkStructuredGrid* Grid = vtkStructuredGrid::New(); 
	Grid->SetDimensions(zone_ijk[0][0], zone_ijk[0][1], zone_ijk[0][2]);
	vtkSmartPointer<vtkPoints> vtk_pts = vtkSmartPointer<vtkPoints>::New();
	for (int iCnt = 0; iCnt < pts.size(); ++iCnt)
	{
		vtk_pts->InsertNextPoint(pts[iCnt].x, pts[iCnt].y, pts[iCnt].z);
	}
	Grid->SetPoints(vtk_pts);

	///<MG copy structured grid to unstructured grid
	int grid_type;
	vtkSmartPointer<vtkIdTypeArray> idTypeArray = vtkSmartPointer<vtkIdTypeArray>::New();
	idTypeArray->SetName("_cell000");
	int iCellIndex = 0;
	for (int iCell = 0; iCell < Grid->GetNumberOfCells(); ++iCell)
	{
		grid_type = Grid->GetCell(iCell)->GetCellType();
		whole_Grid->InsertNextCell(grid_type, Grid->GetCell(iCell)->GetPointIds());
		idTypeArray->InsertNextTuple1(iCellIndex);
		iCellIndex++;
	}

	whole_Grid->SetPoints(Grid->GetPoints());
	QMap<QString, vtkSmartPointer<vtkIdTypeArray>> baseNeed;
	baseNeed.insert("Body", idTypeArray);
	int needPoints = whole_Grid->GetNumberOfPoints();
	
	///<MG　ZoneBC_t --- BC_t
	int nbocos = 0;
	if (cg_nbocos(file_index, base_index, zone_index, &nbocos)){
		return false;
	}

	for (int bc_index = 1; bc_index <= nbocos; ++bc_index)
	{
		CGNS_ENUMT(BCType_t) bctype;
		CGNS_ENUMT(PointSetType_t) ptype;
		CGNS_ENUMT(GridLocation_t) location;
		CGNS_ENUMT(DataType_t) datatype;
		char bc_name[33] = { 0 };
		int np = 0;
		int nrmlindex[3] = { 0 };
		int is = 0, ib = 0;

		if (cg_boco_info(file_index, base_index, zone_index, bc_index, bc_name, &bctype, &ptype, &np, nrmlindex, &is,
			&datatype, &ib))
		{
			return false;
		}

		cg_goto(file_index, base_index, "Zone_t", zone_index, "ZoneBC_t", 1, "BC_t", bc_index, "end");
		char cName[64] = { 0 };
		cg_famname_read(cName);

		MG_CGNS_FAMILY fam_bc;
		fam_bc.family_name = cName;
		fam_bc.type = 1;
		fam_bc.zone_name = bc_name;
		zone_families.insert(bc_name, fam_bc);

		int indexDeimension = 0;
		cg_index_dim(file_index, base_index, zone_index, &indexDeimension);

		int npnts = np * indexDeimension;///<MG indexDemension
		assert(6 == npnts);
		cgsize_t * pnpnts = new cgsize_t[npnts];
		cgsize_t * plist = NULL;
		if (is)
		{
			plist = new cgsize_t[is];
		}

		if (cg_boco_read(file_index, base_index, zone_index, bc_index, pnpnts, plist))
		{
			return false;
		}
		int rang[6];
		for (int iCnt = 0; iCnt < 6; ++iCnt)
		{
			rang[iCnt] = pnpnts[iCnt];
		}

		int i_range[2] = { 0 }, j_range[2] = { 0 }, k_range[2] = { 0 };
		i_range[0] = pnpnts[0] - 1, i_range[1] = pnpnts[3] - 1, j_range[0] = pnpnts[1] - 1;
		j_range[1] = pnpnts[4] - 1, k_range[0] = pnpnts[2] - 1, k_range[1] = pnpnts[5] - 1;
		delete[]pnpnts;

		vtkStructuredGrid* bc_temp = vtkStructuredGrid::New();
		
		vtkSmartPointer<vtkPoints> bc_pts = vtkSmartPointer<vtkPoints>::New();
		for (int k = k_range[0]; k <= k_range[1]; ++k)
		{
			for (int j = j_range[0]; j <= j_range[1]; ++j)
			{
				for (int i = i_range[0]; i <= i_range[1]; ++i)
				{
					int point_index = GetPtIndexInStructedGrid(i , j, k, zone_ijk[0][0], zone_ijk[0][1], zone_ijk[0][2]);
					double bc_point[3];
					bc_pts->InsertNextPoint(vtk_pts->GetPoint(point_index));
				}
			}
		}
		bc_temp->SetPoints(bc_pts);
		bc_temp->SetExtent(i_range[0], i_range[1], j_range[0], j_range[1], k_range[0], k_range[1]);

		int bc_type;
		vtkSmartPointer<vtkIdTypeArray> idTypeArray = vtkSmartPointer<vtkIdTypeArray>::New();
		idTypeArray->SetName("_cell000");
		for (int iCell = 0; iCell < bc_temp->GetNumberOfCells(); ++iCell)
		{
			bc_type = bc_temp->GetCell(iCell)->GetCellType();
			vtkSmartPointer<vtkIdList> myList = vtkSmartPointer<vtkIdList>::New();
			for (int i = 0; i < bc_temp->GetCell(iCell)->GetNumberOfPoints(); i++)
			{
				whole_Grid->GetPoints()->InsertNextPoint(bc_temp->GetCell(iCell)->GetPoints()->GetPoint(i));
				myList->InsertNextId((whole_Grid->GetPoints()->GetNumberOfPoints()) - 1);
			}
			whole_Grid->InsertNextCell(bc_type, myList);
			idTypeArray->InsertNextTuple1(iCellIndex);
			iCellIndex++;
		}
		baseNeed.insert(bc_name, idTypeArray);
	}
	vtkUnstructuredGrid* uGridForMerge = vtkUnstructuredGrid::New();//20180326 modified by cxl
	//vtkSmartPointer<vtkUnstructuredGrid> uGridForMerge = vtkSmartPointer<vtkUnstructuredGrid>::New();
	vtkSmartPointer<vtkMergeCells> mergeCells = vtkSmartPointer<vtkMergeCells>::New();
	mergeCells->SetUnstructuredGrid(uGridForMerge);
	mergeCells->SetTotalNumberOfCells(whole_Grid->GetNumberOfCells());
	mergeCells->SetTotalNumberOfDataSets(1);
	mergeCells->SetTotalNumberOfPoints(needPoints);
	mergeCells->MergeDataSet(whole_Grid);

	dataSetMap.insert(uGridForMerge,baseNeed);
	dataSetNameMap.insert(uGridForMerge, zone_name);

	int dimension[3];
	dimension[0] = zone_ijk[0][0], dimension[1] = zone_ijk[0][1], dimension[2] = zone_ijk[0][2];
	MG_CGNS_FlowSolutions sols;
	if (ReadFlowSolution(file_index, base_index, zone_index, MG_CGNS_GridType::MG_CGNS_GridUnStructured, dimension, sols))
	{
		if (sols.isEmpty())
		{ 
			return true; 
		}
		MG_CGNS_FlowSolutions val_sols = GetWithValFlowSolutions(sols);
		numberOfCgnsTimes = val_sols.size();
		if (val_sols.isEmpty())
		{ 
			return true; 
		}
		AddFlowSolutionToGrid(val_sols.at(0), uGridForMerge);
		MG_CGNS_Zone_FlowSolution zone_sol;
		zone_sol.base_index = base_index;
		zone_sol.file_index = file_index;
		zone_sol.flows = val_sols;
		zone_sol.zone_index = zone_index;
		m_flowSolutions.push_back(zone_sol);
	}
	else
		numberOfCgnsTimes = 0;
	return true;
}

bool ReadData_Cgns::ReadUnStructedZone(int file_index, int base_index, int zone_index)
{
	char zone_name[33] = { 0 };
	int zone_ijk[3][3] = { 0 };

	///<MG get Zone_t node 's name and every direction vertex cell number
	if (cg_zone_read(file_index, base_index, zone_index, zone_name, zone_ijk[0]))
	{
		return false;
	}

	///<MG get grid vertex number and set read whole grid data
	int vertex_number = zone_ijk[0][0];
	int read_range[2][3] = { 0 };
	for (int ijk = 0; ijk < 3; ++ijk)
	{
		read_range[0][ijk] = 1;
		read_range[1][ijk] = vertex_number;
	}

	///<MG get GridCoordinate_t node number
	int coordinate_number = 0;
	if (cg_ncoords(file_index, base_index, zone_index, &coordinate_number))
	{
		return false;
	}

	///<MG get x,y,z point value
	QVector<MG_CGNS_Point> pts;
	if (!ReadGridCooridate(file_index, base_index, zone_index, vertex_number, read_range[0], read_range[1], coordinate_number, pts))
	{
		return false;
	}

	int nsections = 0;
	cg_nsections(file_index, base_index, zone_index, &nsections);

	MG_CGNS_Sections sections;
	///<MG range the cell index
	int cell_index = 0;

	for (int section_index = 1; section_index <= nsections; section_index++)
	{
		char section_name[33] = { 0 };
		int istart = 0, iend = 0, nbndry = 0, iparent_flag = 0;
		CGNS_ENUMT(ElementType_t) itype;

		if (cg_section_read(file_index, base_index, zone_index, section_index, section_name, &itype, &istart, &iend, &nbndry, &iparent_flag))
		{
			return false;
		}

		int element_data_size = 0;
		if (cg_ElementDataSize(file_index, base_index, zone_index, section_index, &element_data_size))
		{
			return false;
		}

		cgsize_t * elements = new cgsize_t[element_data_size];
		
		cgsize_t * element_parent = NULL;
		if (iparent_flag)
		{
			int size = 4 * (iend - istart + 1);
			element_parent = new cgsize_t[size];
		}

		if (cg_elements_read(file_index, base_index, zone_index, section_index, elements, element_parent))
		{
			return false;
		}
		///<MG
		if (CGNS_ENUMT(HEXA_8) == itype)
		{///<MG  六面体 只包含 顶点
			assert(0 == element_data_size % 8);
			MG_CGNS_Section section;
			MG_CGNS_Cells cells;
			int cell_number = element_data_size / 8;
			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_HEXA_8;
				cell.hex_8.pt_index[0] = elements[iCnt * 8] - 1;
				cell.hex_8.pt_index[1] = elements[iCnt * 8 + 1] - 1;
				cell.hex_8.pt_index[2] = elements[iCnt * 8 + 2] - 1;
				cell.hex_8.pt_index[3] = elements[iCnt * 8 + 3] - 1;
				cell.hex_8.pt_index[4] = elements[iCnt * 8 + 4] - 1;
				cell.hex_8.pt_index[5] = elements[iCnt * 8 + 5] - 1;
				cell.hex_8.pt_index[6] = elements[iCnt * 8 + 6] - 1;
				cell.hex_8.pt_index[7] = elements[iCnt * 8 + 7] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}
			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_HEXA_8;
			sections.push_back(section);
		}
		else if (CGNS_ENUMV(NODE) == itype)
		{	///<MG 点类型
			MG_CGNS_Section section;
			MG_CGNS_Cells cells;
			int cell_number = element_data_size;
			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_NODE_1;
				cell.node_1.pt_index = elements[iCnt] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}
			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_NODE_1;
			sections.push_back(section);
		}
		else if (CGNS_ENUMT(QUAD_4) == itype)
		{///<MG 四边形，只包含 顶点
			assert(0 == element_data_size % 4);
			MG_CGNS_Section section;
			MG_CGNS_Cells cells;
			int cell_number = element_data_size / 4;
			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_QUAD_4;
				cell.quad_4.pt_index[0] = elements[iCnt * 4] - 1;
				cell.quad_4.pt_index[1] = elements[iCnt * 4 + 1] - 1;
				cell.quad_4.pt_index[2] = elements[iCnt * 4 + 2] - 1;
				cell.quad_4.pt_index[3] = elements[iCnt * 4 + 3] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}
			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_QUAD_4;
			sections.push_back(section);
		}
		else if (CGNS_ENUMT(QUAD_8) == itype)
		{
			assert(0 == element_data_size % 8);
			MG_CGNS_Section section;
			MG_CGNS_Cells cells;
			int cell_number = element_data_size / 8;
			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_QUAD_4;
				cell.quad_4.pt_index[0] = elements[iCnt * 8] - 1;
				cell.quad_4.pt_index[1] = elements[iCnt * 8 + 1] - 1;
				cell.quad_4.pt_index[2] = elements[iCnt * 8 + 2] - 1;
				cell.quad_4.pt_index[3] = elements[iCnt * 8 + 3] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}
			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_QUAD_4;
			sections.push_back(section);
		}
		else if (CGNS_ENUMT(QUAD_9) == itype)
		{
			assert(0 == element_data_size % 9);
			MG_CGNS_Section section;
			MG_CGNS_Cells cells;
			int cell_number = element_data_size / 9;
			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_QUAD_4;
				cell.quad_4.pt_index[0] = elements[iCnt * 9] - 1;
				cell.quad_4.pt_index[1] = elements[iCnt * 9 + 1] - 1;
				cell.quad_4.pt_index[2] = elements[iCnt * 9 + 2] - 1;
				cell.quad_4.pt_index[3] = elements[iCnt * 9 + 3] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}
			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_QUAD_4;
			sections.push_back(section);
		}
		else if (CGNS_ENUMT(PENTA_6) == itype)
		{	///<MG 三棱柱
			assert(0 == element_data_size % 6);
			MG_CGNS_Section section;
			MG_CGNS_Cells cells;
			int cell_number = element_data_size / 6;
			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_PENTA_6;
				cell.penta_6.pt_index[0] = elements[iCnt * 6] - 1;
				cell.penta_6.pt_index[1] = elements[iCnt * 6 + 1] - 1;
				cell.penta_6.pt_index[2] = elements[iCnt * 6 + 2] - 1;
				cell.penta_6.pt_index[3] = elements[iCnt * 6 + 3] - 1;
				cell.penta_6.pt_index[4] = elements[iCnt * 6 + 4] - 1;
				cell.penta_6.pt_index[5] = elements[iCnt * 6 + 5] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}
			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_PENTA_6;
			sections.push_back(section);
		}
		else if (CGNS_ENUMT(PENTA_15) == itype)
		{	///<MG 三棱柱
			assert(0 == element_data_size % 15);
			MG_CGNS_Section section;
			MG_CGNS_Cells cells;
			int cell_number = element_data_size / 15;
			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_PENTA_6;
				cell.penta_6.pt_index[0] = elements[iCnt * 15] - 1;
				cell.penta_6.pt_index[1] = elements[iCnt * 15 + 1] - 1;
				cell.penta_6.pt_index[2] = elements[iCnt * 15 + 2] - 1;
				cell.penta_6.pt_index[3] = elements[iCnt * 15 + 3] - 1;
				cell.penta_6.pt_index[4] = elements[iCnt * 15 + 4] - 1;
				cell.penta_6.pt_index[5] = elements[iCnt * 15 + 5] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}
			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_PENTA_6;
			sections.push_back(section);
		}
		else if (CGNS_ENUMT(PENTA_18) == itype)
		{	///<MG 三棱柱
			assert(0 == element_data_size % 18);
			MG_CGNS_Section section;
			MG_CGNS_Cells cells;
			int cell_number = element_data_size / 18;
			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_PENTA_6;
				cell.penta_6.pt_index[0] = elements[iCnt * 18] - 1;
				cell.penta_6.pt_index[1] = elements[iCnt * 18 + 1] - 1;
				cell.penta_6.pt_index[2] = elements[iCnt * 18 + 2] - 1;
				cell.penta_6.pt_index[3] = elements[iCnt * 18 + 3] - 1;
				cell.penta_6.pt_index[4] = elements[iCnt * 18 + 4] - 1;
				cell.penta_6.pt_index[5] = elements[iCnt * 18 + 5] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}
			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_PENTA_6;
			sections.push_back(section);
		}
		else if (CGNS_ENUMT(MIXED) == itype)
		{
			int index = 0;
			MG_CGNS_Section section;
			MG_CGNS_Cells cells;

			while (true)
			{
				if (index >= element_data_size)
				{ 
					break; 
				}
				int itype = elements[index];
				
				if (itype == CGNS_ENUMT(PENTA_6))
				{	///<MG 三棱柱
					MG_CGNS_Cell cell;
					cell.type = CGNS_Cell_Type_PENTA_6;
					cell.penta_6.pt_index[0] = elements[index + 1] - 1;
					cell.penta_6.pt_index[1] = elements[index + 2] - 1;
					cell.penta_6.pt_index[2] = elements[index + 3] - 1;
					cell.penta_6.pt_index[3] = elements[index + 4] - 1;
					cell.penta_6.pt_index[4] = elements[index + 5] - 1;
					cell.penta_6.pt_index[5] = elements[index + 6] - 1;
					cell.cell_id = cell_index;
					cells.push_back(cell);
					cell_index++;

					index += 7;
				}
				else if (itype == CGNS_ENUMT(TETRA_4))
				{
					MG_CGNS_Cell cell;
					cell.type = CGNS_Cell_Type_TETRA_4;
					cell.tetra_4.pt_index[0] = elements[index + 1] - 1;
					cell.tetra_4.pt_index[1] = elements[index + 2] - 1;
					cell.tetra_4.pt_index[2] = elements[index + 3] - 1;
					cell.tetra_4.pt_index[3] = elements[index + 4] - 1;
					cell.cell_id = cell_index;
					cells.push_back(cell);
					cell_index++;

					index += 5;
				}
				else if (itype == CGNS_ENUMT(PYRA_5))
				{
					MG_CGNS_Cell cell;
					cell.type = CGNS_Cell_Type_TETRA_4;
					cell.pyra_5.pt_index[0] = elements[index + 1] - 1;
					cell.pyra_5.pt_index[1] = elements[index + 2] - 1;
					cell.pyra_5.pt_index[2] = elements[index + 3] - 1;
					cell.pyra_5.pt_index[3] = elements[index + 4] - 1;
					cell.pyra_5.pt_index[4] = elements[index + 5] - 1;
					cell.cell_id = cell_index;
					cells.push_back(cell);
					cell_index++;

					index += 6;
				}
				else if (itype == CGNS_ENUMT(HEXA_8))
				{
					MG_CGNS_Cell cell;
					cell.type = CGNS_Cell_Type_HEXA_8;
					cell.hex_8.pt_index[0] = elements[index + 1] - 1;
					cell.hex_8.pt_index[1] = elements[index + 2] - 1;
					cell.hex_8.pt_index[2] = elements[index + 3] - 1;
					cell.hex_8.pt_index[3] = elements[index + 4] - 1;
					cell.hex_8.pt_index[4] = elements[index + 5] - 1;
					cell.hex_8.pt_index[5] = elements[index + 6] - 1;
					cell.hex_8.pt_index[6] = elements[index + 7] - 1;
					cell.hex_8.pt_index[7] = elements[index + 8] - 1;
					cell.cell_id = cell_index;
					cells.push_back(cell);
					cell_index++;

					index += 9;
				}
				else if (itype == CGNS_ENUMT(TRI_3))
				{
					MG_CGNS_Cell cell;
					cell.type = CGNS_Cell_Type_TRI_3;
					cell.tri_3.pt_index[0] = elements[index + 1] - 1;
					cell.tri_3.pt_index[1] = elements[index + 2] - 1;
					cell.tri_3.pt_index[2] = elements[index + 3] - 1;

					cell.cell_id = cell_index;
					cells.push_back(cell);
					cell_index++;

					index += 4;
				}
				else if (itype == CGNS_ENUMT(QUAD_4))
				{
					MG_CGNS_Cell cell;
					cell.type = CGNS_Cell_Type_QUAD_4;
					cell.quad_4.pt_index[0] = elements[index + 1] - 1;
					cell.quad_4.pt_index[1] = elements[index + 2] - 1;
					cell.quad_4.pt_index[2] = elements[index + 3] - 1;
					cell.quad_4.pt_index[3] = elements[index + 4] - 1;

					cell.cell_id = cell_index;
					cells.push_back(cell);
					cell_index++;

					index += 5;
				}
				else if (itype == CGNS_ENUMV(NODE))
				{
					MG_CGNS_Cell cell;
					cell.type = CGNS_Cell_Type_NODE_1;
					cell.node_1.pt_index = elements[index + 1] - 1;

					cell.cell_id = cell_index;
					cells.push_back(cell);
					cell_index++;

					index += 2;
				}
				else
					assert(0);
			}

			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_MIXED;
			sections.push_back(section);
		}
		else if (CGNS_ENUMT(TRI_3) == itype)
		{	///<MG 三角形
			assert(0 == element_data_size % 3);
			MG_CGNS_Section section;
			MG_CGNS_Cells cells;
			int cell_number = element_data_size / 3;
			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_TRI_3;
				cell.tri_3.pt_index[0] = elements[iCnt * 3] - 1;
				cell.tri_3.pt_index[1] = elements[iCnt * 3 + 1] - 1;
				cell.tri_3.pt_index[2] = elements[iCnt * 3 + 2] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}

			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_TRI_3;
			sections.push_back(section);
		}
		else if (CGNS_ENUMT(TRI_6) == itype)
		{
			assert(0 == element_data_size % 6);
			MG_CGNS_Section section;
			MG_CGNS_Cells cells;
			int cell_number = element_data_size / 6;
			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_TRI_3;
				cell.tri_3.pt_index[0] = elements[iCnt * 6] - 1;
				cell.tri_3.pt_index[1] = elements[iCnt * 6 + 1] - 1;
				cell.tri_3.pt_index[2] = elements[iCnt * 6 + 2] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}

			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_TRI_3;
			sections.push_back(section);
		}
		else if (CGNS_ENUMT(TETRA_4) == itype)
		{	///<MG 四边形
			assert(0 == element_data_size % 4);

			MG_CGNS_Section section;
			MG_CGNS_Cells cells;
			int cell_number = element_data_size / 4;

			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_TETRA_4;
				cell.tetra_4.pt_index[0] = elements[iCnt * 4] - 1;
				cell.tetra_4.pt_index[1] = elements[iCnt * 4 + 1] - 1;
				cell.tetra_4.pt_index[2] = elements[iCnt * 4 + 2] - 1;
				cell.tetra_4.pt_index[3] = elements[iCnt * 4 + 3] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}
			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_TETRA_4;
			sections.push_back(section);
		}
		else if (CGNS_ENUMT(TETRA_10) == itype)
		{
			assert(0 == element_data_size % 10);

			MG_CGNS_Section section;
			MG_CGNS_Cells cells;
			int cell_number = element_data_size / 10;

			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_TETRA_4;
				cell.tetra_4.pt_index[0] = elements[iCnt * 10] - 1;
				cell.tetra_4.pt_index[1] = elements[iCnt * 10 + 1] - 1;
				cell.tetra_4.pt_index[2] = elements[iCnt * 10 + 2] - 1;
				cell.tetra_4.pt_index[3] = elements[iCnt * 10 + 3] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}
			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_TETRA_4;
			sections.push_back(section);
		}
		else if (CGNS_ENUMT(PYRA_5) == itype)
		{
			assert(0 == element_data_size % 5);

			MG_CGNS_Section section;
			MG_CGNS_Cells cells;
			int cell_number = element_data_size / 5;

			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_PYRA_5;
				cell.pyra_5.pt_index[0] = elements[iCnt * 5] - 1;
				cell.pyra_5.pt_index[1] = elements[iCnt * 5 + 1] - 1;
				cell.pyra_5.pt_index[2] = elements[iCnt * 5 + 2] - 1;
				cell.pyra_5.pt_index[3] = elements[iCnt * 5 + 3] - 1;
				cell.pyra_5.pt_index[4] = elements[iCnt * 5 + 4] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}
			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_PYRA_5;
			sections.push_back(section);
		}
		else if (CGNS_ENUMT(PYRA_14) == itype)
		{
			assert(0 == element_data_size % 14);

			MG_CGNS_Section section;
			MG_CGNS_Cells cells;
			int cell_number = element_data_size / 14;

			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_PYRA_5;
				cell.pyra_5.pt_index[0] = elements[iCnt * 14] - 1;
				cell.pyra_5.pt_index[1] = elements[iCnt * 14 + 1] - 1;
				cell.pyra_5.pt_index[2] = elements[iCnt * 14 + 2] - 1;
				cell.pyra_5.pt_index[3] = elements[iCnt * 14 + 3] - 1;
				cell.pyra_5.pt_index[4] = elements[iCnt * 14 + 4] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}
			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_PYRA_5;
			sections.push_back(section);
		}
		else if (CGNS_ENUMT(BAR_2) == itype)
		{	///<MG 直线
			MG_CGNS_Cells cells;
			MG_CGNS_Section section;
			assert(0 == element_data_size % 2);
			int cell_number = element_data_size / 2;

			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_LINE_2;
				cell.line_2.pt_index[0] = elements[iCnt * 2] - 1;
				cell.line_2.pt_index[1] = elements[iCnt * 2 + 1] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}
			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_LINE_2;
			sections.push_back(section);
		}
		else if (CGNS_ENUMT(BAR_3) == itype)
		{	///<MG 直线  带有中间值
			MG_CGNS_Cells cells;
			MG_CGNS_Section section;
			assert(0 == element_data_size % 3);
			int cell_number = element_data_size / 3;

			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				MG_CGNS_Cell cell;
				cell.type = CGNS_Cell_Type_LINE_2;
				cell.line_2.pt_index[0] = elements[iCnt * 3] - 1;
				cell.line_2.pt_index[1] = elements[iCnt * 3 + 1] - 1;
				cell.cell_id = cell_index;
				cells.push_back(cell);
				cell_index++;
			}
			section.cells = cells;
			section.section_name = section_name;
			section.type = CGNS_Cell_Type_LINE_2;
			sections.push_back(section);
		}
		else
		{
			assert(0);
		}
		if (elements)
		{
			delete elements;
			elements = nullptr;
		}
		if (element_parent)
		{
			delete element_parent;
			element_parent = nullptr;
		}
	}

	vtkUnstructuredGrid* uGrid = vtkUnstructuredGrid::New();
	vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();

	/*points convert*/
	vtkSmartPointer<vtkPoints> vtk_pts = vtkSmartPointer<vtkPoints>::New();
	for (int iCnt = 0; iCnt < pts.size(); ++iCnt)
	{
		vtk_pts->InsertNextPoint(pts[iCnt].x, pts[iCnt].y, pts[iCnt].z);
	}
	uGrid->SetPoints(vtk_pts);

	/*sections */
	QMap<QString, vtkSmartPointer<vtkIdTypeArray>> baseNeed;

	for (int iCnt = 0; iCnt < sections.size(); ++iCnt)
	{
		vtkSmartPointer<vtkIdTypeArray> boundaryPointsIdArray = vtkSmartPointer<vtkIdTypeArray>::New();
		boundaryPointsIdArray->SetName("_cell000");
		MG_CGNS_Section temp = sections[iCnt];
		if (temp.type == CGNS_Cell_Type_HEXA_8)
		{
			for (int iCell = 0; iCell < temp.cells.size(); ++iCell)
			{
				vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();

				MG_CGNS_Cell cell = temp.cells[iCell];
				idList->InsertNextId(cell.hex_8.pt_index[0]);
				idList->InsertNextId(cell.hex_8.pt_index[1]);
				idList->InsertNextId(cell.hex_8.pt_index[2]);
				idList->InsertNextId(cell.hex_8.pt_index[3]);
				idList->InsertNextId(cell.hex_8.pt_index[4]);
				idList->InsertNextId(cell.hex_8.pt_index[5]);
				idList->InsertNextId(cell.hex_8.pt_index[6]);
				idList->InsertNextId(cell.hex_8.pt_index[7]);

				boundaryPointsIdArray->InsertNextTuple1(cell.cell_id);
				uGrid->InsertNextCell(/*12*/ VTK_HEXAHEDRON, idList);
			}
		}
		else if (temp.type == CGNS_Cell_Type_PENTA_6)
		{
			for (int iCell = 0; iCell < temp.cells.size(); ++iCell)
			{
				vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();

				MG_CGNS_Cell cell = temp.cells[iCell];
				idList->InsertNextId(cell.penta_6.pt_index[0]);
				idList->InsertNextId(cell.penta_6.pt_index[1]);
				idList->InsertNextId(cell.penta_6.pt_index[2]);
				idList->InsertNextId(cell.penta_6.pt_index[3]);
				idList->InsertNextId(cell.penta_6.pt_index[4]);
				idList->InsertNextId(cell.penta_6.pt_index[5]);

				boundaryPointsIdArray->InsertNextTuple1(cell.cell_id);
				uGrid->InsertNextCell(/*13*/ VTK_WEDGE, idList);
			}
		}
		else if (temp.type == CGNS_Cell_Type_QUAD_4)
		{
			for (int iCell = 0; iCell < temp.cells.size(); ++iCell)
			{
				vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();

				MG_CGNS_Cell cell = temp.cells[iCell];
				idList->InsertNextId(cell.quad_4.pt_index[0]);
				idList->InsertNextId(cell.quad_4.pt_index[1]);
				idList->InsertNextId(cell.quad_4.pt_index[2]);
				idList->InsertNextId(cell.quad_4.pt_index[3]);

				boundaryPointsIdArray->InsertNextTuple1(cell.cell_id);
				uGrid->InsertNextCell(/*9*/VTK_QUAD, idList);
			}
		}
		else if (temp.type == CGNS_Cell_Type_TRI_3)
		{
			for (int iCell = 0; iCell < temp.cells.size(); ++iCell)
			{
				vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();

				MG_CGNS_Cell cell = temp.cells[iCell];
				idList->InsertNextId(cell.tri_3.pt_index[0]);
				idList->InsertNextId(cell.tri_3.pt_index[1]);
				idList->InsertNextId(cell.tri_3.pt_index[2]);

				boundaryPointsIdArray->InsertNextTuple1(cell.cell_id);
				uGrid->InsertNextCell(/*5*/VTK_TRIANGLE, idList);
			}
		}
		else if (temp.type == CGNS_Cell_Type_TETRA_4)
		{
			for (int iCell = 0; iCell < temp.cells.size(); ++iCell)
			{
				vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();

				MG_CGNS_Cell cell = temp.cells[iCell];
				idList->InsertNextId(cell.tetra_4.pt_index[0]);
				idList->InsertNextId(cell.tetra_4.pt_index[1]);
				idList->InsertNextId(cell.tetra_4.pt_index[2]);
				idList->InsertNextId(cell.tetra_4.pt_index[3]);

				boundaryPointsIdArray->InsertNextTuple1(cell.cell_id);
				uGrid->InsertNextCell(/*10*/VTK_TETRA, idList);
			}
		}
		else if (temp.type == CGNS_Cell_Type_LINE_2)
		{
			for (int iCell = 0; iCell < temp.cells.size(); ++iCell)
			{
				vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();

				MG_CGNS_Cell cell = temp.cells[iCell];
				idList->InsertNextId(cell.line_2.pt_index[0]);
				idList->InsertNextId(cell.line_2.pt_index[1]);

				boundaryPointsIdArray->InsertNextTuple1(cell.cell_id);
				uGrid->InsertNextCell(/*3*/VTK_LINE, idList);
			}
		}
		else if (temp.type == CGNS_Cell_Type_NODE_1)
		{
			for (int iCell = 0; iCell < temp.cells.size(); ++iCell)
			{
				vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();

				MG_CGNS_Cell cell = temp.cells[iCell];
				idList->InsertNextId(cell.node_1.pt_index);

				boundaryPointsIdArray->InsertNextTuple1(cell.cell_id);
				uGrid->InsertNextCell(/*3*/VTK_VERTEX, idList);
			}
		}
		else if (temp.type == CGNS_Cell_Type_PYRA_5)
		{
			for (int iCell = 0; iCell < temp.cells.size(); ++iCell)
			{
				vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();

				MG_CGNS_Cell cell = temp.cells[iCell];
				idList->InsertNextId(cell.pyra_5.pt_index[0]);
				idList->InsertNextId(cell.pyra_5.pt_index[1]);
				idList->InsertNextId(cell.pyra_5.pt_index[2]);
				idList->InsertNextId(cell.pyra_5.pt_index[3]);
				idList->InsertNextId(cell.pyra_5.pt_index[4]);

				boundaryPointsIdArray->InsertNextTuple1(cell.cell_id);
				uGrid->InsertNextCell(/*14*/VTK_PYRAMID, idList);
			}
		}
		else if (temp.type == CGNS_Cell_Type_MIXED)
		{
			for (int iCell = 0; iCell < temp.cells.size(); ++iCell)
			{
				vtkSmartPointer<vtkIdList> idList = vtkSmartPointer<vtkIdList>::New();

				MG_CGNS_Cell cell = temp.cells[iCell];

				if (cell.type == CGNS_Cell_Type_PENTA_6)
				{
					idList->InsertNextId(cell.penta_6.pt_index[0]);
					idList->InsertNextId(cell.penta_6.pt_index[1]);
					idList->InsertNextId(cell.penta_6.pt_index[2]);
					idList->InsertNextId(cell.penta_6.pt_index[3]);
					idList->InsertNextId(cell.penta_6.pt_index[4]);
					idList->InsertNextId(cell.penta_6.pt_index[5]);

					boundaryPointsIdArray->InsertNextTuple1(cell.cell_id);
					uGrid->InsertNextCell(/*13*/ VTK_WEDGE, idList);
				}
				else if (cell.type == CGNS_Cell_Type_HEXA_8)
				{
					idList->InsertNextId(cell.hex_8.pt_index[0]);
					idList->InsertNextId(cell.hex_8.pt_index[1]);
					idList->InsertNextId(cell.hex_8.pt_index[2]);
					idList->InsertNextId(cell.hex_8.pt_index[3]);
					idList->InsertNextId(cell.hex_8.pt_index[4]);
					idList->InsertNextId(cell.hex_8.pt_index[5]);
					idList->InsertNextId(cell.hex_8.pt_index[6]);
					idList->InsertNextId(cell.hex_8.pt_index[7]);

					boundaryPointsIdArray->InsertNextTuple1(cell.cell_id);
					uGrid->InsertNextCell(/*12*/ VTK_HEXAHEDRON, idList);
				}
				else if (cell.type == CGNS_Cell_Type_TRI_3)
				{
					idList->InsertNextId(cell.tri_3.pt_index[0]);
					idList->InsertNextId(cell.tri_3.pt_index[1]);
					idList->InsertNextId(cell.tri_3.pt_index[2]);

					boundaryPointsIdArray->InsertNextTuple1(cell.cell_id);
					uGrid->InsertNextCell(/*5*/ VTK_TRIANGLE, idList);
				}
				else if (cell.type == CGNS_Cell_Type_QUAD_4)
				{
					idList->InsertNextId(cell.quad_4.pt_index[0]);
					idList->InsertNextId(cell.quad_4.pt_index[1]);
					idList->InsertNextId(cell.quad_4.pt_index[2]);
					idList->InsertNextId(cell.quad_4.pt_index[3]);

					boundaryPointsIdArray->InsertNextTuple1(cell.cell_id);
					uGrid->InsertNextCell(/*5*/ VTK_QUAD, idList);
				}
				else if (cell.type == CGNS_Cell_Type_TETRA_4)
				{
					idList->InsertNextId(cell.tetra_4.pt_index[0]);
					idList->InsertNextId(cell.tetra_4.pt_index[1]);
					idList->InsertNextId(cell.tetra_4.pt_index[2]);
					idList->InsertNextId(cell.tetra_4.pt_index[3]);

					boundaryPointsIdArray->InsertNextTuple1(cell.cell_id);
					uGrid->InsertNextCell(/*5*/ VTK_TETRA, idList);
				}
				else if (cell.type == CGNS_Cell_Type_NODE_1)
				{
					idList->InsertNextId(cell.node_1.pt_index);

					boundaryPointsIdArray->InsertNextTuple1(cell.cell_id);
					uGrid->InsertNextCell(/*5*/ VTK_VERTEX, idList);
				}
				else if (cell.type == CGNS_Cell_Type_PYRA_5)
				{
					idList->InsertNextId(cell.pyra_5.pt_index[0]);
					idList->InsertNextId(cell.pyra_5.pt_index[1]);
					idList->InsertNextId(cell.pyra_5.pt_index[2]);
					idList->InsertNextId(cell.pyra_5.pt_index[3]);
					idList->InsertNextId(cell.pyra_5.pt_index[4]);

					boundaryPointsIdArray->InsertNextTuple1(cell.cell_id);
					uGrid->InsertNextCell(/*5*/ VTK_PYRAMID, idList);
				}
				else
					assert(0);
			}
		}

		int iSize = boundaryPointsIdArray->GetNumberOfTuples();
		baseNeed.insert(temp.section_name, boundaryPointsIdArray);
	}
	
	dataSetMap.insert(vtkDataSet::SafeDownCast(uGrid), baseNeed);
	dataSetNameMap.insert(uGrid, zone_name);

	///<MG Read FlowSolutions
	int dimension[3] = { 0 };
	dimension[0] = pts.size();
	int cell_num = 0;
	for (int iCnt = 0; iCnt < sections.size(); ++iCnt)
	{
		if (sections.at(iCnt).type == CGNS_Cell_Type_HEXA_8 || sections.at(iCnt).type == CGNS_Cell_Type_TETRA_4)
		{
			cell_num += sections.at(iCnt).cells.size();
		}
	}
	dimension[1] = cell_num;

	MG_CGNS_FlowSolutions sols;
	if (ReadFlowSolution(file_index, base_index, zone_index, MG_CGNS_GridType::MG_CGNS_GridUnStructured, dimension, sols))
	{
		MG_CGNS_FlowSolutions val_sols = GetWithValFlowSolutions(sols);
		if (val_sols.size() == 0)
		{ 
			return true; 
		}
		numberOfCgnsTimes = val_sols.size();
		AddFlowSolutionToGrid(val_sols.at(0), uGrid);
		MG_CGNS_Zone_FlowSolution zone_sol;
		zone_sol.base_index = base_index;
		zone_sol.file_index = file_index;
		zone_sol.flows = val_sols;
		zone_sol.zone_index = zone_index;
		m_flowSolutions.push_back(zone_sol);

	}
	else
		numberOfCgnsTimes = 0;
	return true;
}

void ReadData_Cgns::ShowGrid(vtkDataSet * dataSet)
{
	vtkSmartPointer<vtkDataSetMapper> mapper =
		vtkSmartPointer<vtkDataSetMapper>::New();

	mapper->SetInputData(dataSet);

	vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkRenderer> renderer =	vtkSmartPointer<vtkRenderer>::New();
	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->AddRenderer(renderer);
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	// Add the actor to the scene
	renderer->AddActor(actor);
	renderer->SetBackground(.3, .6, .3); // Background color green

	// Render and interact
	renderWindow->Render();
	renderWindowInteractor->Start();
}

int ReadData_Cgns::GetPtIndexInStructedGrid(int iPos, int jPos, int kPos, int iMax, int jMax, int kMax)
{
	return iPos + (jPos * iMax) + (kPos * iMax * jMax);
}

bool ReadData_Cgns::ReadGridCooridate(int file_index, int base_index, int zone_index, int vertext_num, int range_from[3], int range_to[3], int & cooridateNum, MG_CGNS_Points & pts)
{
	if (vertext_num != pts.size())
	{
		pts.clear();
		MG_CGNS_Point pt = {0};
		pts.fill(pt, vertext_num);
	}

	if (cg_ncoords(file_index, base_index, zone_index, &cooridateNum))
	{
		return false;
	}
	for (int coor_index = 1; coor_index <= cooridateNum; ++coor_index)
	{
		CGNS_ENUMV(DataType_t) datatype;
		char coordinate_name[33] = { 0 };
		if (cg_coord_info(file_index, base_index, zone_index, coor_index, &datatype, coordinate_name))
		{
			return false;
		}

		double * xyz = new double[vertext_num];
		if (cg_coord_read(file_index, base_index, zone_index, coordinate_name, CGNS_ENUMV(RealDouble), range_from, range_to, xyz))
		{
			return false;
		}

		if (!strcmp(coordinate_name, "CoordinateX"))
		{
			for (int ipt = 0; ipt < vertext_num; ++ipt)
			{
				pts[ipt].x = xyz[ipt];
			}
		}
		else if (!strcmp(coordinate_name, "CoordinateY"))
		{
			for (int ipt = 0; ipt < vertext_num; ++ipt)
			{
				pts[ipt].y = xyz[ipt];
			}
		}
		else if (!strcmp(coordinate_name, "CoordinateZ"))
		{
			for (int ipt = 0; ipt < vertext_num; ++ipt)
			{
				pts[ipt].z = xyz[ipt];
			}
		}
		delete[]xyz;
	}
	return true;
}

bool ReadData_Cgns::ReadFlowSolution(int file_index, int base_index, int zone_index, MG_CGNS_GridType gridType, int dimension[3], MG_CGNS_FlowSolutions & sols)
{
	int nsol = 0;
	if (cg_nsols(file_index, base_index, zone_index, &nsol))
	{
		return false;
	}

	for (int sol_index = 1; sol_index <= nsol; ++sol_index)
	{
		///<MG 
		MG_CGNS_FlowSolution sol = {0};
		CGNS_ENUMT(GridLocation_t) varloc;
		char sol_name[33] = { 0 };
		if (cg_sol_info(file_index, base_index, zone_index, sol_index, sol_name, &varloc))
		{
			return false;
		}
		memcpy(sol.cName, sol_name, sizeof(char) * 33);

		if (varloc == CGNS_ENUMT(Vertex))
		{
			sol.loc = MG_CGNS_Vertex;
		}
		else if (varloc == CGNS_ENUMT(CellCenter))
		{
			sol.loc = MG_CGNS_CellCenter;
		}
		else
			return false;

		int var_num = 0;
		if (cg_nfields(file_index, base_index, zone_index, sol_index, &var_num))
		{
			return false;
		}

		///<MG 无数据流场解
		if (var_num == 0)
		{
			sols.push_back(sol);
			continue;
		}

		int var_val_num = 0;
		int min[3] = { 1 }, max[3] = { 0 };
		if (gridType == MG_CGNS_GridStructured)
		{
			if (sol.loc == MG_CGNS_Vertex)
			{
				max[0] = dimension[0], max[1] = dimension[1], max[2] = dimension[2];
			}
			else if (sol.loc == MG_CGNS_CellCenter)
			{
				int rind[3][2] = {0};
				cg_rind_read((int*)rind);
				max[0] = dimension[0] - 1 + rind[0][0] + rind[0][1];
				max[1] = dimension[1] - 1 + rind[1][0] + rind[1][1];
				max[2] = dimension[2] - 1 + rind[2][0] + rind[2][1];
			}
			var_val_num = max[0] * max[1] * max[2];
		}
		else if (gridType == MG_CGNS_GridUnStructured)
		{
			if (sol.loc == MG_CGNS_Vertex)
			{
				var_val_num = dimension[0];
				max[0] = max[1] = max[2] = var_val_num;
			}
			else if (sol.loc == MG_CGNS_CellCenter)
			{
				var_val_num = dimension[1];
				max[0] = max[1] = max[2] = var_val_num;
			}
		}
		else
			assert(0);

		double * var_val = new double[var_val_num];
		for (int var_index = 1; var_index <= var_num; ++var_index)
		{
			CGNS_ENUMT(DataType_t) datatype;
			char var_name[33] = {0};
			if (cg_field_info(file_index, base_index, zone_index, sol_index, var_index, &datatype, var_name))
			{
				return false;
			}

			if (cg_field_read(file_index, base_index, zone_index, sol_index, var_name, CGNS_ENUMV(RealDouble), min, max, var_val))
			{
				return false;
			}
			QVector<double> vals;
			for (int iCnt = 0; iCnt < var_val_num; ++iCnt)
			{
				vals.push_back(var_val[iCnt]);
			}
			sol.vars.insert(var_name, vals);
		}
		sols.push_back(sol);
		if (var_val)
		{
			delete []var_val;
			var_val = nullptr;
		}
	}
	return true;
}

MG_CGNS_FlowSolutions ReadData_Cgns::GetWithValFlowSolutions(MG_CGNS_FlowSolutions sols)
{
	MG_CGNS_FlowSolutions result;
	for (int iCnt = 0; iCnt < sols.size(); ++iCnt)
	{
		if (!sols.at(iCnt).vars.isEmpty())
		{
			result.push_back( sols.at(iCnt) );
		}
	}
	return result;
}

bool ReadData_Cgns::AddFlowSolutionToGrid(MG_CGNS_FlowSolution flowSolution, vtkSmartPointer<vtkUnstructuredGrid> uGrid)
{
	///<MG 查找矢量符合矢量名称规则的量
	QVector<QString> vecNames;		///<MG 矢量分量名称
	for (int iCnt = 0; iCnt < flowSolution.vars.keys().size(); ++iCnt)
	{
		QString varName = flowSolution.vars.keys().at(iCnt);
		QString varNameTail = varName.right(1);
		if (varNameTail.toLower() == "x" || varNameTail.toLower() == "y" || varNameTail.toLower() == "z")
			vecNames.push_back(varName);
	}

	///<MG 判断是否为矢量
	QSet<QString> vec;
	for (int iCnt = 0; iCnt < vecNames.size(); ++iCnt)
	{
		QString name = vecNames.at(iCnt).left(vecNames.at(iCnt).size() - 1);
		if ((vecNames.contains(name + "X") && vecNames.contains(name + "Y") && vecNames.contains(name + "Z")) || (vecNames.contains(name + "x") && vecNames.contains(name + "y") && vecNames.contains(name + "z")))
		{
			vec.insert(name);
		}
	}
	///<MG 添加矢量分量
	for (int iCnt = 0; iCnt < vec.size(); ++iCnt)
	{
		QString vecName = vec.toList().at(iCnt);
		QVector<double> x_values, y_values, z_values;
		///<MG 获取x,y.z分量的数据
		for (int iVar = 0; iVar < flowSolution.vars.keys().size(); ++iVar)
		{
			QString varName = flowSolution.vars.keys().at(iVar);
			if (varName.left(varName.size() - 1) == vecName)
			{
				if ((varName.right(1) == "X" || varName.right(1) == "x"))
					x_values = flowSolution.vars.values().at(iVar);
				if ((varName.right(1) == "Y" || varName.right(1) == "y"))
					y_values = flowSolution.vars.values().at(iVar);
				if ((varName.right(1) == "Z" || varName.right(1) == "z"))
					z_values = flowSolution.vars.values().at(iVar);
			}
		}
		if (x_values.size() == y_values.size()&& x_values.size() == z_values.size() && z_values.size() != 0)
		{
			vtkSmartPointer<vtkFloatArray> gridData = vtkSmartPointer<vtkFloatArray>::New();
			gridData->SetName(vecName.toStdString().c_str());
			gridData->SetNumberOfComponents(3);

			if (flowSolution.loc == MG_CGNS_CellCenter)
			{	///<MG 体单元中心
				int cellNum = uGrid->GetNumberOfCells();
				int dataIndex = 0;
				if (cellNum < x_values.size())
				{ 
					continue; 
				}

				///<MG 判断数据落在那种类型Cell 中
				VTKCellType type = getWithResultCellType(uGrid, x_values.size());
				int index = 0;
				for (int iCell = 0; iCell < cellNum; ++iCell)
				{
					if (uGrid->GetCell(iCell)->GetCellType() == type)
					{
						gridData->InsertNextTuple3(x_values.at(index), y_values.at(index), z_values.at(index));
						index++;
					}
					else
					{
						gridData->InsertNextTuple3(0, 0, 0);
					}
				}
				uGrid->GetCellData()->AddArray(gridData);
			}
			else if (flowSolution.loc == MG_CGNS_Vertex)
			{	///<MG 数据位于点上

				int ptNum = uGrid->GetNumberOfPoints();
				if (ptNum != x_values.size()){ continue; }

				for (int iPt = 0; iPt < ptNum; ++iPt)
				{
					gridData->InsertNextTuple3(x_values.at(iPt), y_values.at(iPt), z_values.at(iPt));
				}
				uGrid->GetCellData()->AddArray(gridData);
			}
		}
	}

	///<MG 添加标量
	for (int iCnt = 0; iCnt < flowSolution.vars.size(); ++iCnt)
	{
		QString varName = flowSolution.vars.keys().at(iCnt);
		bool isVec = false;
		for (int iVec = 0; iVec < vec.toList().size(); ++iVec)
		{
			QString vecName = vec.toList().at(iVec); 
			if (varName == vecName + "X" || varName == vecName + "Y" || varName == vecName + "Z" || 
				varName == vecName + "x" || varName == vecName + "y" || varName == vecName + "z")
			{
				isVec = true;
				break;
			}
		}
		if (isVec)
		{ 
			continue; 
		}

		vtkSmartPointer<vtkFloatArray> gridData = vtkSmartPointer<vtkFloatArray>::New();
		gridData->SetName(varName.toStdString().c_str());
		gridData->SetNumberOfComponents(1);
		QVector<double> values = flowSolution.vars.values().at(iCnt);

		if (flowSolution.loc == MG_CGNS_CellCenter)
		{
			int cellNum = uGrid->GetNumberOfCells();
			if (cellNum < values.size())
			{ 
				continue; 
			}
			VTKCellType type = getWithResultCellType(uGrid, values.size());

			int index = 0;
			for (int iCell = 0; iCell < cellNum; ++iCell)
			{
				if (type == uGrid->GetCell(iCell)->GetCellType())
				{
					gridData->InsertNextTuple1(values.at(index));
					index++;
				}
				else
					gridData->InsertNextTuple1(0);
				

			}
			
			uGrid->GetCellData()->AddArray(gridData);
		}
		else if (flowSolution.loc == MG_CGNS_Vertex)
		{
			int ptNum = uGrid->GetNumberOfPoints();
			if (ptNum != values.size())
			{ 
				continue; 
			}

			for (int iPt = 0; iPt < ptNum; ++iPt)
			{
				gridData->InsertNextTuple1(values.at(iPt));
			}
			uGrid->GetPointData()->AddArray(gridData);
		}
	}
	return true;
}

VTKCellType ReadData_Cgns::getWithResultCellType(vtkSmartPointer<vtkUnstructuredGrid> uGrid, int resultNum)
{
	int iCellNum = uGrid->GetNumberOfCells();
	QMap<VTKCellType, int> _typeCellNum;
	for (int iCnt = 0; iCnt < iCellNum; ++iCnt)
	{
		VTKCellType type = (VTKCellType)uGrid->GetCell(iCnt)->GetCellType();
		if (_typeCellNum.keys().contains(type))
		{
			int iNum = _typeCellNum.value(type);
			iNum += 1;
			_typeCellNum.insert(type, iNum);
		}
		else
		{
			_typeCellNum.insert(type, 1);
		}
	}
	for (int iCnt = 0; iCnt < _typeCellNum.size(); ++iCnt)
	{
		int iNum = _typeCellNum.values().at(iCnt);
		if (iNum == resultNum)
		{
			return _typeCellNum.keys().at(iCnt);
		}
	}
	return VTK_EMPTY_CELL;
}
