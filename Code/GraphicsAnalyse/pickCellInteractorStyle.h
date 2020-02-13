#ifndef PICKCELLINTERACTORSTYLE_H
#define PICKCELLINTERACTORSTYLE_H

#include <QObject>
#include "heudatasrcio_global.h"
#include <vtkCellPicker.h>
#include <vtkCommand.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSelectionNode.h>
#include <vtkSelection.h>
#include <vtkExtractSelection.h>
#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkIdTypeArray.h>
#include <vtkUnstructuredGrid.h>

class HEUDATASRCIOSHARED_EXPORT pickCellInteractorStyle : public QObject, public vtkInteractorStyleTrackballCamera
{
	Q_OBJECT
public:
	static pickCellInteractorStyle* New();

	pickCellInteractorStyle()
	{
		selectedMapper = vtkSmartPointer<vtkDataSetMapper>::New();
		selectedActor = vtkSmartPointer<vtkActor>::New();
	}
	virtual void OnLeftButtonDown();
	vtkSmartPointer<vtkDataSet> Data;//vtkSmartPointer<vtkPolyData> Data;
	vtkSmartPointer<vtkDataSetMapper> selectedMapper;
	vtkSmartPointer<vtkActor> selectedActor;
signals:
	void sig_restore_interactor();

public slots:
};
//vtkStandardNewMacro(MouseInteractorStyle);
#endif