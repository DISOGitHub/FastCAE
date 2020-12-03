#include "pickCellInteractorStyle.h"
#include <vtkRenderWindowInteractor.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRendererCollection.h>

void pickCellInteractorStyle::OnLeftButtonDown()
{
	// Get the location of the click (in window coordinates)
	int* pos = this->GetInteractor()->GetEventPosition();

	vtkSmartPointer<vtkCellPicker> picker =
		vtkSmartPointer<vtkCellPicker>::New();
	picker->SetTolerance(0.0005);

	// Pick from this location.
	picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

	double* worldPosition = picker->GetPickPosition();
	std::cout << "Cell id is: " << picker->GetCellId() << std::endl;

	//on Actor£¿
	vtkActor* tep_actor = picker->GetActor();
	if (tep_actor == NULL)
		return;
	Data = tep_actor->GetMapper()->GetInput();
	if (picker->GetCellId() != -1)
	{

		std::cout << "Pick position is: " << worldPosition[0] << " " << worldPosition[1]
			<< " " << worldPosition[2] << endl;

		vtkSmartPointer<vtkIdTypeArray> ids =
			vtkSmartPointer<vtkIdTypeArray>::New();
		ids->SetNumberOfComponents(1);
		ids->InsertNextValue(picker->GetCellId());

		vtkSmartPointer<vtkSelectionNode> selectionNode =
			vtkSmartPointer<vtkSelectionNode>::New();
		selectionNode->SetFieldType(vtkSelectionNode::CELL);
		selectionNode->SetContentType(vtkSelectionNode::INDICES);
		selectionNode->SetSelectionList(ids);

		vtkSmartPointer<vtkSelection> selection =
			vtkSmartPointer<vtkSelection>::New();
		selection->AddNode(selectionNode);

		vtkSmartPointer<vtkExtractSelection> extractSelection =
			vtkSmartPointer<vtkExtractSelection>::New();
#if VTK_MAJOR_VERSION <= 5
		extractSelection->SetInput(0, this->Data);
		extractSelection->SetInput(1, selection);
#else
		extractSelection->SetInputData(0, this->Data);
		extractSelection->SetInputData(1, selection);
#endif
		extractSelection->Update();

		// In selection
		vtkSmartPointer<vtkUnstructuredGrid> selected =
			vtkSmartPointer<vtkUnstructuredGrid>::New();
		selected->ShallowCopy(extractSelection->GetOutput());

		std::cout << "There are " << selected->GetNumberOfPoints()
			<< " points in the selection." << std::endl;
		std::cout << "There are " << selected->GetNumberOfCells()
			<< " cells in the selection." << std::endl;


#if VTK_MAJOR_VERSION <= 5
		selectedMapper->SetInputConnection(
			selected->GetProducerPort());
#else
		selectedMapper->SetInputData(selected);
#endif

		selectedActor->SetMapper(selectedMapper);
		selectedActor->GetProperty()->EdgeVisibilityOn();
		selectedActor->GetProperty()->SetEdgeColor(1, 0, 0);
		selectedActor->GetProperty()->SetLineWidth(3);

		this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer()->AddActor(selectedActor);

	}
	// Forward events
	vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
}