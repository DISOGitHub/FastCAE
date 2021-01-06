#include "ReadData_Igs.h"
#include <IVtkOCC_Shape.hxx>
#include <IVtkTools_ShapeDataSource.hxx>
#include <IVtkTools_DisplayModeFilter.hxx>
#include <TopoDS_Shape.hxx>
#include <vtkPolyDataNormals.h>

ReadData_Igs::ReadData_Igs()
{
    
}
ReadData_Igs::~ReadData_Igs()
{
    
}
/*void ReadData_Igs::BuildMesh(vtkMultiBlockDataSet* blockDataSet,const TopoDS_Face& face, double deflection = 0.1)//double deflection = 0.1
{
     TopLoc_Location location;
 	BRepMesh_IncrementalMesh::BRepMesh_IncrementalMesh(face, deflection);
     //BRepMesh::Mesh(face, deflection);
     Handle_Poly_Triangulation triFace = BRep_Tool::Triangulation(face, location);
     Standard_Integer nTriangles = triFace->NbTriangles();
     gp_Pnt vertex1;
     gp_Pnt vertex2;
     gp_Pnt vertex3;
     Standard_Integer nVertexIndex1 = 0;
     Standard_Integer nVertexIndex2 = 0;
     Standard_Integer nVertexIndex3 = 0;
     TColgp_Array1OfPnt nodes(1, triFace->NbNodes());
     Poly_Array1OfTriangle triangles(1, triFace->NbTriangles());
     nodes = triFace->Nodes();
     triangles = triFace->Triangles();
     vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
     vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
     vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
     points->Allocate(nTriangles * 3);
     cells->Allocate(nTriangles);
     int id = 0;
     for (Standard_Integer i = 1; i <= nTriangles; i++)
     {
         Poly_Triangle aTriangle = triangles.Value(i);
         aTriangle.Get(nVertexIndex1, nVertexIndex2, nVertexIndex3);
         vertex1 = nodes.Value(nVertexIndex1).Transformed(location.Transformation());
         vertex2 = nodes.Value(nVertexIndex2).Transformed(location.Transformation());
         vertex3 = nodes.Value(nVertexIndex3).Transformed(location.Transformation());
         points->InsertNextPoint(vertex1.X(), vertex1.Y(), vertex1.Z());
         points->InsertNextPoint(vertex2.X(), vertex2.Y(), vertex2.Z());
         points->InsertNextPoint(vertex3.X(), vertex3.Y(), vertex3.Z());
         vtkSmartPointer<vtkTriangle> triangle = vtkSmartPointer<vtkTriangle>::New();
         triangle->GetPointIds()->SetId(0,id * 3);
         triangle->GetPointIds()->SetId(1,id * 3 + 1);
         triangle->GetPointIds()->SetId(2,id *3 + 2);
         // Add the triangle to a cell array
         cells->InsertNextCell(triangle);
         id++;
     }
     polyData->SetPoints(points);
     polyData->SetPolys(cells);
 	blockDataSet->SetBlock(blockDataSet->GetNumberOfBlocks(), polyData);
}*/

bool ReadData_Igs::Read(QString tep_filename)
{
	fileName = tep_filename;
	//qDebug() << "read fileName=  " << tep_filename << endl;
	char * fileNamechar;
	QByteArray ba = QStringToChar(fileName);
	fileNamechar = ba.data();
	IGESControl_Reader Reader;
	Standard_Integer status = Reader.ReadFile(fileNamechar);
	if (status != IFSelect_RetDone)
	{
		return false;
	}
	Reader.TransferRoots();
	TopoDS_Shape aShape = Reader.OneShape();

	IVtkTools_DisplayModeFilter* DF = IVtkTools_DisplayModeFilter::New();
	IVtkTools_ShapeDataSource* DS = IVtkTools_ShapeDataSource::New();
	IVtkOCC_Shape::Handle aShapeImpl = nullptr;
	aShapeImpl = new IVtkOCC_Shape(aShape);

	aShapeImpl->DecrementRefCounter();
	DS->SetShape(aShapeImpl);
	DF->AddInputConnection(DS->GetOutputPort());
	DF->SetDisplayMode(DM_Shading);
	DF->SetDisplaySharedVertices(false);

	vtkPolyDataNormals* normals = vtkPolyDataNormals::New();
	normals->SetInputConnection(DF->GetOutputPort());
	normals->FlipNormalsOn();
	normals->Update();

// 	TopTools_HSequenceOfShape* sequanceOfShape = new TopTools_HSequenceOfShape;
// 	sequanceOfShape->Append(aShape);
// 	Standard_Integer index = sequanceOfShape->Length();
// 	TopoDS_Shape theCompSolid = sequanceOfShape->ChangeValue(index);
// 	vtkMultiBlockDataSet* blockDataSet = vtkMultiBlockDataSet::New();
// 	for (TopExp_Explorer faceExp(theCompSolid, TopAbs_FACE); faceExp.More(); faceExp.Next())
// 	{
// 		// The 3d-mesh of the FACE is assembled to form the
// 		// boundary of the SOLID.
// 		const TopoDS_Face& theFace = TopoDS::Face(faceExp.Current());
// 		BuildMesh(blockDataSet,theFace, 0.1);
// 	}
// 	if (blockDataSet->GetNumberOfBlocks() < 1)
// 		return false;
// 	vtkAppendFilter* appendFilter = vtkAppendFilter::New();
// 	for (int i = 0; i < blockDataSet->GetNumberOfBlocks(); i++)
// 	{
// 		vtkDataSet* tep_dataset = vtkDataSet::SafeDownCast(blockDataSet->GetBlock(i));
// 		if (tep_dataset == NULL)
// 			continue;
// 		appendFilter->AddInputData(tep_dataset);
// 	}
// 	appendFilter->Update();
	set_GridType(dPolyData);
// 	vtkUnstructuredGrid* tep_grid = vtkUnstructuredGrid::New();
// 	tep_grid->DeepCopy(appendFilter->GetOutput());
	dataSet = normals->GetOutput();

	mNodeCount = dataSet->GetNumberOfPoints();
	mCellCount = dataSet->GetNumberOfCells();
	if ((mNodeCount > 0) && (mCellCount > 0))
	{
		//ReadVarsList(dataSet);
		return true;
	}
	return false;
}
