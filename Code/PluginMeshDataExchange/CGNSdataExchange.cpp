#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include "meshData/meshSet.h"
#include "meshData/CgnsFamily.h"
#include "meshData/CgnsBCZone.h"
#include "CGNSdataExchange.h"
#include <QFile>
#include <QTextCodec>
#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
#include <vtkUnstructuredGrid.h>
#include <vtkStructuredGrid.h>
#include <vtkPoints.h>
#include <vtkIdTypeArray.h>
#include <vtkIdList.h>
#include <vtkCellType.h>

//#ifdef Q_OS_WIN
#include <cgnslib.h>
//#endif

namespace MeshData
{
	CGNSdataExchange::CGNSdataExchange(const QString &fileName, MeshOperation operation, GUI::MainWindow *mw, int writeFileKid) :
		_fileName(fileName),
		_operation(operation),
		_meshData(MeshData::getInstance()),
		MeshThreadBase(fileName, operation, mw),
		_writeFileKid(writeFileKid)
	{

	}

	CGNSdataExchange::~CGNSdataExchange()
	{
		
	}

	void CGNSdataExchange::run()
	{				
		ModuleBase::ThreadTask::run();
		bool result = false;
		switch (_operation)
		{
		case MESH_READ:
			emit showInformation(tr("Import CGNS Mesh File From \"%1\"").arg(_fileName));
			result = read();
			setReadResult(result);
			break;
		case MESH_WRITE:
			emit showInformation(tr("Export CGNS Mesh File From \"%1\"").arg(_fileName));
			result = write();
			setWriteResult(result);
			break;
		}
		defaultMeshFinished();
	}

	bool CGNSdataExchange::read()
	{
//#ifdef Q_OS_WIN		

		QFile file(_fileName);
		if (!file.exists()) return false;

		int file_type = 0;
		if (cg_is_cgns(_fileName.toStdString().c_str(), &file_type))
		{
			return false;
		}
		int m_currentFileIndex = 0;
		if (cg_open(_fileName.toStdString().c_str(), CG_MODE_READ, &m_currentFileIndex))
		{
			return false;
		}
		///<MG get CGNSBase_t node number
		int m_currentBaseIndex = 0;
		if (cg_nbases(m_currentFileIndex, &m_currentBaseIndex))
		{
			return false;
		}

		for (int ibase = 1; ibase <= m_currentBaseIndex; ++ibase)
		{
			///读取族
			int nfamily = 0;
			cg_nfamilies(m_currentFileIndex, m_currentBaseIndex, &nfamily);
			for (int i = 1; i <= nfamily; ++i)
			{
				int nBoco = 0, nGeos = 0;
				char name[64] = {0};
				cg_family_read(m_currentFileIndex, m_currentBaseIndex, i, name, &nBoco, &nGeos);
				CgnsFamily* set = new CgnsFamily(name);
				_meshData->appendMeshSet(set);
			}

			///<MG get Zone_t node number
			int zone_node_number = 0;
			if (cg_nzones(m_currentFileIndex, ibase, &zone_node_number))
			{
				return false;
			}

			for (int izone = 1; izone <= zone_node_number; ++izone)
			{
				bool ok = readZone(m_currentFileIndex, ibase, izone);
				if (!ok) return false;
			}
		}

		return true;
//#endif
//		return false;
	}

	bool CGNSdataExchange::write()
	{
		if (_writeFileKid < 1)	return false;
	}

	bool CGNSdataExchange::readZone(int file_index, int base_index, int zone_index)
	{
//#ifdef Q_OS_WIN
		CGNS_ENUMT(ZoneType_t) zonetype;
		if (cg_zone_type(file_index, base_index, zone_index, &zonetype))
			return false;
		if (zonetype == CGNS_ENUMV(Structured))
			return readStructuredZone(file_index, base_index, zone_index);
		else if (zonetype == CGNS_ENUMV(Unstructured))
			return readUnStructuredZone(file_index, base_index, zone_index);		
		else    return false;
// #endif
// 		return false;
	}
	bool CGNSdataExchange::readStructuredZone(int file_index, int base_index, int zone_index)
	{
//#ifdef Q_OS_WIN
		char zone_name[33] = { 0 };
		int zone_ijk[3][3] = { 0 };
		///<MG get Zone_t node 's name and every direction vertex cell number
		if (cg_zone_read(file_index, base_index, zone_index, zone_name, zone_ijk[0])) return false;

		int vertex_number = zone_ijk[0][0] * zone_ijk[0][1] * zone_ijk[0][2];
		int read_range[2][3] = { 0 };
		for (int n = 0; n < 3; n++)
		{
			read_range[0][n] = 1;
			read_range[1][n] = zone_ijk[0][n];
		}

		int coordinate_number = 0;
		if (cg_ncoords(file_index, base_index, zone_index, &coordinate_number)) return false;
		vtkSmartPointer<vtkStructuredGrid> gird = vtkSmartPointer<vtkStructuredGrid>::New();
		gird->SetDimensions(zone_ijk[0][0], zone_ijk[0][1], zone_ijk[0][2]);

		this->readCoordinate(file_index, base_index, zone_index, vertex_number, read_range[0], read_range[1], coordinate_number, gird);
		if (gird->GetNumberOfPoints() != vertex_number) return false;
		MeshKernal* kernal = new MeshKernal;
		kernal->setMeshData(gird);
		kernal->setPath(_fileName);
		kernal->setName(zone_name);
		kernal->setDimension(coordinate_number);
		_meshData->appendMeshKernal(kernal);

		cg_goto(file_index, base_index, "Zone_t", zone_index, "end");
		char cname[64] = { 0 };
		cg_famname_read(cname);
		MeshSet* fset = _meshData->getMeshSetByName(cname);
		CgnsFamily* fam = dynamic_cast<CgnsFamily*>(fset);
		if (fam != nullptr)
			fam->appendDataSet(gird);

// 		const int ncell = gird->GetNumberOfCells();
// 		vtkSmartPointer<vtkIdTypeArray> idlist = vtkSmartPointer<vtkIdTypeArray>::New();
// 		for (int i = 0; i <= ncell; ++i)
// 		{
// 			idlist->InsertNextValue(i); 
// 		}
// 		MeshSet* set = new MeshSet(zone_name, Element);
// 		set->setDataSet(gird);
// 		set->setIDList(idlist);
// 		_meshData->appendMeshSet(set);
// 		kernal->setMeshData(gird);

		readBC(file_index, base_index, zone_index, zone_ijk[0][0], zone_ijk[0][1], zone_ijk[0][2], kernal);

		return true;
// #endif
// 		return false;
	}

	bool CGNSdataExchange::readBC(int file_index, int base_index, int zone_index, int zonei, int zonej, int zonek, MeshKernal* kernal)
	{
//#ifdef Q_OS_WIN
		int nbocos = 0;
		if (cg_nbocos(file_index, base_index, zone_index, &nbocos)) return false;
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

			if (cg_boco_info(file_index, base_index, zone_index, bc_index, bc_name, &bctype, &ptype, &np, nrmlindex, &is, &datatype, &ib)) return false;
			cg_goto(file_index, base_index, "Zone_t", zone_index, "ZoneBC_t", 1, "BC_t", bc_index, "end");
			char cName[64] = { 0 };
			cg_famname_read(cName);
			int indexDeimension = 0;
			cg_index_dim(file_index, base_index, zone_index, &indexDeimension);
			int npnts = np * indexDeimension;
			assert(6 == npnts);
			cgsize_t * pnpnts = new cgsize_t[npnts];
			cgsize_t * plist = NULL;
			if (is)
				plist = new cgsize_t[is];

			if (cg_boco_read(file_index, base_index, zone_index, bc_index, pnpnts, plist)) return false;
			int rang[6];
			for (int iCnt = 0; iCnt < 6; ++iCnt)
				rang[iCnt] = pnpnts[iCnt];

			int i_range[2] = { 0 }, j_range[2] = { 0 }, k_range[2] = { 0 };
			i_range[0] = pnpnts[0] - 1, i_range[1] = pnpnts[3] - 1, j_range[0] = pnpnts[1] - 1;
			j_range[1] = pnpnts[4] - 1, k_range[0] = pnpnts[2] - 1, k_range[1] = pnpnts[5] - 1;
			delete[]pnpnts;
			CgnsBCZone* bcset = new CgnsBCZone(bc_name);
			bcset->setDataSet(kernal->getMeshData());
			bcset->setParent(cName);
			bcset->setRange(i_range, j_range, k_range);
			
			_meshData->appendMeshSet(bcset);
			int xd = i_range[1] - i_range[0] + 1;
			int yd = j_range[1] - j_range[0] + 1;
			int zd = k_range[1] - k_range[0] + 1;
			vtkStructuredGrid* sgrid = vtkStructuredGrid::New();
			sgrid->SetDimensions(xd, yd, zd);
			vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New(); 
			for (int k = k_range[0]; k <= k_range[1]; ++k)
			{
				for (int j = j_range[0]; j <= j_range[1]; ++j)
				{
					for (int i = i_range[0]; i <= i_range[1]; ++i)
					{
						int index = i + (j * zonei) + (k * zonei * zonej);
						Vertex v = _verlist.at(index);
						points->InsertNextPoint(v.x, v.y, v.z);
					}
				}
			}
			sgrid->SetPoints(points);
			bcset->setDisplayDataSet(sgrid);
			MeshSet* fset = _meshData->getMeshSetByName(cName);
			CgnsFamily* fam = dynamic_cast<CgnsFamily*>(fset);
			if (fam == nullptr)
			{
				fam = new CgnsFamily(cName);
				_meshData->appendMeshSet(fam);
			}
			fam->appendDataSet(sgrid);
		}
		return true;
// #endif
// 		return false;
	}

	bool CGNSdataExchange::readUnStructuredZone(int file_index, int base_index, int zone_index)
	{
//#ifdef Q_OS_WIN
		char zone_name[33] = { 0 };
		int zone_ijk[3][3] = { 0 };

		///<MG get Zone_t node 's name and every direction vertex cell number
		if (cg_zone_read(file_index, base_index, zone_index, zone_name, zone_ijk[0]))  return false;

		int vertex_number = zone_ijk[0][0];
		int read_range[2][3] = { 0 };
		for (int ijk = 0; ijk < 3; ++ijk)
		{
			read_range[0][ijk] = 1;
			read_range[1][ijk] = vertex_number;
		}

		///<MG get GridCoordinate_t node number
		int coordinate_number = 0;
		if (cg_ncoords(file_index, base_index, zone_index, &coordinate_number)) return false;
		
		vtkSmartPointer<vtkUnstructuredGrid> gird = vtkSmartPointer<vtkUnstructuredGrid>::New();
		this->readCoordinate(file_index, base_index, zone_index, vertex_number, read_range[0], read_range[1], coordinate_number, gird);

		if (gird->GetNumberOfPoints()!= vertex_number) return false;

		MeshKernal* kernal = new MeshKernal;
		kernal->setMeshData(gird);
		kernal->setPath(_fileName);
		kernal->setName(zone_name);
		kernal->setDimension(coordinate_number);
		_meshData->appendMeshKernal(kernal);

		cg_goto(file_index, base_index, "Zone_t", zone_index, "end");
		char cname[64] = { 0 };
		cg_famname_read(cname);
		MeshSet* fset = _meshData->getMeshSetByName(cname);
		CgnsFamily* fam = dynamic_cast<CgnsFamily*>(fset);
		if (fam != nullptr)
			fam->appendDataSet(gird);

		this->readSections(file_index, base_index, zone_index, kernal);
		kernal->setMeshData(gird);

		return true;
// #endif
// 		return false;
	}

	void CGNSdataExchange::readCoordinate(int file_index, int base_index, int zone_index, int vertext_num,
		int range_from[3], int range_to[3], int & cooridateNum, vtkDataSet* dataset)
	{
//#ifdef Q_OS_WIN
		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();

		_verlist.clear();
		_verlist.fill(Vertex(), vertext_num);

		char coordinate_name[33] = { 0 };
		CGNS_ENUMV(DataType_t) datatype;

		for (int coor_index = 1; coor_index <= cooridateNum; ++coor_index)
		{

			if (cg_coord_info(file_index, base_index, zone_index, coor_index, &datatype, coordinate_name)) return;
			double * xyz = new double[vertext_num];
			if (cg_coord_read(file_index, base_index, zone_index, coordinate_name, CGNS_ENUMV(RealDouble), range_from, range_to, xyz))	return;
			
			if (!strcmp(coordinate_name, "CoordinateX"))
			{
				for (int ipt = 0; ipt < vertext_num; ++ipt)
				{
					_verlist[ipt].x = xyz[ipt];
				}
			}
			else if (!strcmp(coordinate_name, "CoordinateY"))
			{
				for (int ipt = 0; ipt < vertext_num; ++ipt)
				{
					_verlist[ipt].y = xyz[ipt];
				}
			}
			else if (!strcmp(coordinate_name, "CoordinateZ"))
			{
				for (int ipt = 0; ipt < vertext_num; ++ipt)
				{
					_verlist[ipt].z = xyz[ipt];
				}
			}
			delete xyz;
		}
		for (int i = 0; i < vertext_num; ++i)
		{
			Vertex v = _verlist[i];
			points->InsertNextPoint(v.x, v.y, v.z);
		}


		CGNS_ENUMT(ZoneType_t) zonetype;
		if (cg_zone_type(file_index, base_index, zone_index, &zonetype)) return;
		if (zonetype == CGNS_ENUMV(Structured))
		{
			vtkStructuredGrid* gird = dynamic_cast<vtkStructuredGrid*>(dataset);
			gird->SetPoints(points);
		}
		else if (zonetype == CGNS_ENUMV(Unstructured))
		{
			vtkUnstructuredGrid* ungird = dynamic_cast<vtkUnstructuredGrid*>(dataset);
			ungird->SetPoints(points);
		}
//#endif
	}

	void CGNSdataExchange::readSections(int file_index, int base_index, int zone_index, MeshKernal* kernal)
	{
//#ifdef Q_OS_WIN
		vtkDataSet* dataset = kernal->getMeshData();
		vtkUnstructuredGrid* ungird = dynamic_cast<vtkUnstructuredGrid*>(dataset);

		int nsections = 0;
		cg_nsections(file_index, base_index, zone_index, &nsections);
		int offset = _cellOffset;
		for (int section_index = 1; section_index <= nsections; section_index++)
		{
			char section_name[33] = { 0 };
			int istart = 0, iend = 0, nbndry = 0, iparent_flag = 0;
			CGNS_ENUMT(ElementType_t) itype;
			if (cg_section_read(file_index, base_index, zone_index, section_index, section_name, &itype, &istart, &iend, &nbndry, &iparent_flag)) return;

			int element_data_size = 0;
			if (cg_ElementDataSize(file_index, base_index, zone_index, section_index, &element_data_size)) return;

			cgsize_t * elements = new cgsize_t[element_data_size];
			cgsize_t * element_parent = NULL;
			if (iparent_flag)
			{
				int size = 4 * (iend - istart + 1);
				element_parent = new cgsize_t[size];
			}
			if (cg_elements_read(file_index, base_index, zone_index, section_index, elements, element_parent)) return;

			MeshSet* set = new MeshSet(section_name, Element);
			MeshData* m = MeshData::getInstance();
			int kid = m->getIDByDataSet(dataset);
			if (kid <0) continue;
//			vtkSmartPointer<vtkIdTypeArray> idlist = vtkSmartPointer<vtkIdTypeArray>::New();
			for (int i = istart; i <= iend; ++i)
			{
//				idlist->InsertNextValue(i-_cellOffset);
				set->appendMember(kid, i - _cellOffset);
			}
			
// 			set->setDataSet(dataset);
// 			set->setIDList(idlist);
			_meshData->appendMeshSet(set);
			this->readCells(istart, iend, itype, elements, element_data_size, kernal);
			if (iend > offset) offset = iend;
		}
		_cellOffset = offset + 1;
//#endif
	}
	void CGNSdataExchange::readCells(int start_index, int end_index, int type, int* elements, int element_data_size, MeshKernal* kernal)
	{
//#ifdef Q_OS_WIN
		vtkDataSet* dataset = kernal->getMeshData();
		vtkUnstructuredGrid* ungird = dynamic_cast<vtkUnstructuredGrid*>(dataset);

		if (CGNS_ENUMT(TRI_3) == type)  //三角形
		{
			int cell_number = element_data_size / 3;
			assert(0 == element_data_size % 3);
			assert(end_index - start_index + 1 == cell_number);

			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				vtkSmartPointer<vtkIdList> idlist = vtkSmartPointer<vtkIdList>::New();
				idlist->InsertNextId(elements[iCnt * 3 + 0] - 1);
				idlist->InsertNextId(elements[iCnt * 3 + 1] - 1);
				idlist->InsertNextId(elements[iCnt * 3 + 2] - 1);
				ungird->InsertNextCell(VTK_TRIANGLE, idlist);
			}

		}
		else if (CGNS_ENUMT(BAR_2) == type) //二节点梁单元
		{
			int cell_number = element_data_size / 2;
			assert(0 == element_data_size % 2);
			assert(end_index - start_index + 1 == cell_number);
			
			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				vtkSmartPointer<vtkIdList> idlist = vtkSmartPointer<vtkIdList>::New();
				idlist->InsertNextId(elements[iCnt * 2 + 0] - 1);
				idlist->InsertNextId(elements[iCnt * 2 + 1] - 1);
				ungird->InsertNextCell(VTK_LINE, idlist);
			}
		}
		else if (CGNS_ENUMT(TETRA_4) == type) //四节点四面体
		{
			int cell_number = element_data_size / 4;
			assert(0 == element_data_size % 4);
			assert(end_index - start_index + 1 == cell_number);


			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				vtkSmartPointer<vtkIdList> idlist = vtkSmartPointer<vtkIdList>::New();
				idlist->InsertNextId(elements[iCnt * 4 + 0] - 1);
				idlist->InsertNextId(elements[iCnt * 4 + 1] - 1);
				idlist->InsertNextId(elements[iCnt * 4 + 2] - 1);
				idlist->InsertNextId(elements[iCnt * 4 + 3] - 1);
				ungird->InsertNextCell(VTK_TETRA, idlist);
			}
		}
		else if (CGNS_ENUMT(QUAD_4) == type)  //四节点四边形
		{
			int cell_number = element_data_size / 4;
			assert(0 == element_data_size % 4);
			assert(end_index - start_index + 1 == cell_number);


			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				vtkSmartPointer<vtkIdList> idlist = vtkSmartPointer<vtkIdList>::New();
				idlist->InsertNextId(elements[iCnt * 4 + 0] - 1);
				idlist->InsertNextId(elements[iCnt * 4 + 1] - 1);
				idlist->InsertNextId(elements[iCnt * 4 + 2] - 1);
				idlist->InsertNextId(elements[iCnt * 4 + 3] - 1);
				ungird->InsertNextCell(VTK_QUAD, idlist);
			}
		}
		else if (CGNS_ENUMT(HEXA_8) == type) //八节点六面体
		{
			int cell_number = element_data_size / 8;
			assert(0 == element_data_size % 8);
			assert(end_index - start_index + 1 == cell_number);

			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				vtkSmartPointer<vtkIdList> idlist = vtkSmartPointer<vtkIdList>::New();
				idlist->InsertNextId(elements[iCnt * 8 + 0] - 1);
				idlist->InsertNextId(elements[iCnt * 8 + 1] - 1);
				idlist->InsertNextId(elements[iCnt * 8 + 2] - 1);
				idlist->InsertNextId(elements[iCnt * 8 + 3] - 1);
				idlist->InsertNextId(elements[iCnt * 8 + 4] - 1);
				idlist->InsertNextId(elements[iCnt * 8 + 5] - 1);
				idlist->InsertNextId(elements[iCnt * 8 + 6] - 1);
				idlist->InsertNextId(elements[iCnt * 8 + 7] - 1);
				ungird->InsertNextCell(VTK_HEXAHEDRON, idlist);
			}
		}
		else if (CGNS_ENUMT(PENTA_6)== type)  //六节点三棱柱
		{
			int cell_number = element_data_size / 6;
			assert(0 == element_data_size % 6);
			assert(end_index - start_index + 1 == cell_number);

			for (int iCnt = 0; iCnt < cell_number; ++iCnt)
			{
				vtkSmartPointer<vtkIdList> idlist = vtkSmartPointer<vtkIdList>::New();
				idlist->InsertNextId(elements[iCnt * 6 + 0] - 1);
				idlist->InsertNextId(elements[iCnt * 6 + 1] - 1);
				idlist->InsertNextId(elements[iCnt * 6 + 2] - 1);
				idlist->InsertNextId(elements[iCnt * 6 + 3] - 1);
				idlist->InsertNextId(elements[iCnt * 6 + 4] - 1);
				idlist->InsertNextId(elements[iCnt * 6 + 5] - 1);
				ungird->InsertNextCell(VTK_WEDGE, idlist);
			}
		}
		else if (CGNS_ENUMV(MIXED) == type)
		{
//			qDebug() << "mix";
			int index = 0;
			while (index < element_data_size)
			{
				int eleType = elements[index];
				if (eleType == CGNS_ENUMT(PENTA_6))
				{
					vtkSmartPointer<vtkIdList> idlist = vtkSmartPointer<vtkIdList>::New();
					idlist->InsertNextId(elements[index + 1] - 1);
					idlist->InsertNextId(elements[index + 2] - 1);
					idlist->InsertNextId(elements[index + 3] - 1);
					idlist->InsertNextId(elements[index + 4] - 1);
					idlist->InsertNextId(elements[index + 5] - 1);
					idlist->InsertNextId(elements[index + 6] - 1);
					ungird->InsertNextCell(VTK_WEDGE, idlist);
					index += 7;
				}
				else if (eleType == CGNS_ENUMT(HEXA_8))
				{
					vtkSmartPointer<vtkIdList> idlist = vtkSmartPointer<vtkIdList>::New();
					idlist->InsertNextId(elements[index + 1] - 1);
					idlist->InsertNextId(elements[index + 2] - 1);
					idlist->InsertNextId(elements[index + 3] - 1);
					idlist->InsertNextId(elements[index + 4] - 1);
					idlist->InsertNextId(elements[index + 5] - 1);
					idlist->InsertNextId(elements[index + 6] - 1);
					idlist->InsertNextId(elements[index + 7] - 1);
					idlist->InsertNextId(elements[index + 8] - 1);
					ungird->InsertNextCell(VTK_HEXAHEDRON, idlist);
					index += 9;
				}
				else if (eleType == CGNS_ENUMT(TRI_3))
				{
					vtkSmartPointer<vtkIdList> idlist = vtkSmartPointer<vtkIdList>::New();
					idlist->InsertNextId(elements[index + 1] - 1);
					idlist->InsertNextId(elements[index + 2] - 1);
					idlist->InsertNextId(elements[index + 3] - 1);
					ungird->InsertNextCell(VTK_TRIANGLE, idlist);
					index += 4;
				}
				else if (eleType == CGNS_ENUMT(QUAD_4))
				{
					vtkSmartPointer<vtkIdList> idlist = vtkSmartPointer<vtkIdList>::New();
					idlist->InsertNextId(elements[index + 1] - 1);
					idlist->InsertNextId(elements[index + 2] - 1);
					idlist->InsertNextId(elements[index + 3] - 1);
					idlist->InsertNextId(elements[index + 4] - 1);
					ungird->InsertNextCell(VTK_QUAD, idlist);
					index += 5;
				}
				else if (eleType == CGNS_ENUMT(TETRA_4))
				{
					vtkSmartPointer<vtkIdList> idlist = vtkSmartPointer<vtkIdList>::New();
					idlist->InsertNextId(elements[index + 1] - 1);
					idlist->InsertNextId(elements[index + 2] - 1);
					idlist->InsertNextId(elements[index + 3] - 1);
					idlist->InsertNextId(elements[index + 4] - 1);
					ungird->InsertNextCell(VTK_TETRA, idlist);
					index += 5;
				}
				else if (eleType == CGNS_ENUMT(BAR_2)) //二节点梁单元
				{
					vtkSmartPointer<vtkIdList> idlist = vtkSmartPointer<vtkIdList>::New();
					idlist->InsertNextId(elements[index + 1] - 1);
					idlist->InsertNextId(elements[index + 2] - 1);
					ungird->InsertNextCell(VTK_LINE, idlist);
					index += 3;
				}
				else
					index += element_data_size;
			}
		}
//#endif
	}
}