#ifndef READDATAIGS_H
#define READDATAIGS_H
#define WNT 

#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Sphere.hxx>

#include <Poly_Polygon3D.hxx>
#include <Poly_Triangulation.hxx>

#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <IGESControl_Controller.hxx>
#include <IGESControl_Writer.hxx>
#include <IGESControl_Reader.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>

#include <BRep_Tool.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
//#include <BRepMesh.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>

#include <BRepAdaptor_Curve.hxx>
#include <GCPnts_TangentialDeflection.hxx>
#include <TopExp_Explorer.hxx>
#include <Standard_TypeDef.hxx>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkTriangle.h>
#include "global_define.h"
#include "qstring.h"
#include "qtimer.h"
#include "ReadData_Base.h"
#include "vtkDataSetReader.h"
#include "vtkUnstructuredGrid.h"
#include "vtkPointData.h"
#include "vtkStructuredGrid.h"
#include "vtkDataSet.h"
#include "vtkDataArray.h"
#include "vtkCellArray.h"
#include "vtkRectilinearGrid.h"
#include "vtkUnstructuredGrid.h"
#include "vtkAppendFilter.h"
class ReadData_Igs :public ReadData_Base
{
public:

	ReadData_Igs();
	~ReadData_Igs();
	//void BuildMesh(vtkMultiBlockDataSet* blockDataSet,const TopoDS_Face& face, double deflection);
	bool Read(QString tep_filename) override;
private:
	vtkDataSetReader* mVtkDataSetReader;
};

#endif // READIGSMODEL_H