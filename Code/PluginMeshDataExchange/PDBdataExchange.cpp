#include "PDBdataExchange.h"
#include "meshData/meshSingleton.h"
#include "meshData/meshKernal.h"
#include <QTextCodec>
#include <vtkSmartPointer.h>
#include <cmath>
#include <vtkGlyph3D.h>
#include <vtkLODActor.h>
#include <vtkLODActor.h>
#include <vtkPDBReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
#include <vtkTubeFilter.h>
#include <QDebug>

namespace MeshData{
	PDBdataExchange::PDBdataExchange(const QString &fileName, MeshOperation operation, GUI::MainWindow *mw, int modelId) :
		MeshThreadBase(fileName, operation, mw), 
		_fileName(fileName), 
		_operation(operation), 
		_modelId(modelId),
		_meshData(MeshData::getInstance())
	{
	}

	PDBdataExchange::~PDBdataExchange()
	{
		//if (_stream != nullptr) delete _stream;
		//qDebug() << "PDB Delete!" << endl;
	}

	void PDBdataExchange::run()
	{
		ModuleBase::ThreadTask::run();
		bool result = false;
		switch (_operation)
		{
		case MESH_READ:
			emit showInformation(tr("Import PDB Mesh File From \"%1\"").arg(_fileName));
			result = read();
			setReadResult(result);
			break;
		case MESH_WRITE:
			emit showInformation(tr("Export PDB Mesh File From \"%1\"").arg(_fileName));
			//			result = write();
			//			setWriteResult(result);
			break;
		}
		defaultMeshFinished();
	}

	bool PDBdataExchange::read()
	{
		/*VTKÏÔÊ¾×¢ÊÍ*/
//  		vtkSmartPointer<vtkRenderer> renderer =
//  			vtkSmartPointer<vtkRenderer>::New();
//  		renderer->SetBackground(.1, .2, .3);
//  		vtkSmartPointer<vtkRenderWindow> renderWindow =
//  			vtkSmartPointer<vtkRenderWindow>::New();
//  		renderWindow->AddRenderer(renderer);
//  		renderWindow->SetSize(300, 300);
//  		vtkSmartPointer<vtkRenderWindowInteractor> interactor =
//  			vtkSmartPointer<vtkRenderWindowInteractor>::New();
//  		interactor->SetRenderWindow(renderWindow);

		vtkSmartPointer<vtkPDBReader> pdb =
			vtkSmartPointer<vtkPDBReader>::New();
		//char * fileName = "caffeine.pdb";
// 		std::string file = _fileName.toStdString();
// 		const char * fileName = file.c_str();
		QTextCodec *codec = QTextCodec::codecForName("GB18030");
		QByteArray fileName = codec->fromUnicode(_fileName);
		pdb->SetFileName(fileName);
// 		pdb->SetHBScale(1.0);
// 		pdb->SetBScale(1.0);
		pdb->Update();
	//	std::cout << "# of atoms is: " << pdb->GetNumberOfAtoms() << std::endl;

// 		double resolution = std::sqrt(300000.0 / pdb->GetNumberOfAtoms());
// 		if (resolution > 20)
// 		{
// 			resolution = 20;
// 		}
// 		if (resolution < 4)
// 		{
// 			resolution = 4;
// 		}
// 		std::cout << "Resolution is: " << resolution << std::endl;
// 		vtkSmartPointer<vtkSphereSource> sphere =
// 			vtkSmartPointer<vtkSphereSource>::New();
// 		sphere->SetCenter(0, 0, 0);
// 		sphere->SetRadius(1);
// 		sphere->SetThetaResolution(static_cast<int>(resolution));
// 		sphere->SetPhiResolution(static_cast<int>(resolution));
// 
// 		vtkSmartPointer<vtkGlyph3D> glyph =
// 			vtkSmartPointer<vtkGlyph3D>::New();
// 		glyph->SetInputConnection(pdb->GetOutputPort());
// 		glyph->SetOrient(1);
// 		glyph->SetColorMode(1);
// 		glyph->SetScaleMode(2);
// 		glyph->SetScaleFactor(.25);
// 		glyph->SetSourceConnection(sphere->GetOutputPort());
// 
// 		vtkSmartPointer<vtkPolyDataMapper> atomMapper =
// 			vtkSmartPointer<vtkPolyDataMapper>::New();
// 		atomMapper->SetInputConnection(glyph->GetOutputPort());
// 		atomMapper->ImmediateModeRenderingOn();
// 		atomMapper->UseLookupTableScalarRangeOff();
// 		atomMapper->ScalarVisibilityOn();
// 		atomMapper->SetScalarModeToDefault();
// 
// 		vtkSmartPointer<vtkLODActor> atom =
// 			vtkSmartPointer<vtkLODActor>::New();
// 		atom->SetMapper(atomMapper);
// 		atom->GetProperty()->SetRepresentationToSurface();
// 		atom->GetProperty()->SetInterpolationToGouraud();
// 		atom->GetProperty()->SetAmbient(0.15);
// 		atom->GetProperty()->SetDiffuse(0.85);
// 		atom->GetProperty()->SetSpecular(0.1);
// 		atom->GetProperty()->SetSpecularPower(30);
// 		atom->GetProperty()->SetSpecularColor(1, 1, 1);
// 		atom->SetNumberOfCloudPoints(30000);
// 
// 		renderer->AddActor(atom);
// 
// 		vtkSmartPointer<vtkTubeFilter> tube =
// 			vtkSmartPointer<vtkTubeFilter>::New();
// 		tube->SetInputConnection(pdb->GetOutputPort());
// 		tube->SetNumberOfSides(static_cast<int>(resolution));
// 		tube->CappingOff();
// 		tube->SetRadius(0.2);
// 		tube->SetVaryRadius(0);
// 		tube->SetRadiusFactor(10);
// 
// 		vtkSmartPointer<vtkPolyDataMapper> bondMapper =
// 			vtkSmartPointer<vtkPolyDataMapper>::New();
// 		bondMapper->SetInputConnection(tube->GetOutputPort());
// 		bondMapper->ImmediateModeRenderingOn();
// 		bondMapper->UseLookupTableScalarRangeOff();
// 		bondMapper->ScalarVisibilityOff();
// 		bondMapper->SetScalarModeToDefault();
// 
// 		vtkSmartPointer<vtkLODActor> bond =
// 			vtkSmartPointer<vtkLODActor>::New();
// 		bond->SetMapper(bondMapper);
// 		bond->GetProperty()->SetRepresentationToSurface();
// 		bond->GetProperty()->SetInterpolationToGouraud();
// 		bond->GetProperty()->SetAmbient(0.15);
// 		bond->GetProperty()->SetDiffuse(0.85);
// 		bond->GetProperty()->SetSpecular(0.1);
// 		bond->GetProperty()->SetSpecularPower(30);
// 		bond->GetProperty()->SetSpecularColor(1, 1, 1);
// 		bond->GetProperty()->SetDiffuseColor(1.0000, 0.8941, 0.70981);
// 		renderer->AddActor(bond);
// 		interactor->Initialize();
// 		interactor->Start();

		vtkDataSet* dataset = pdb->GetOutput();
		if (dataset == nullptr) return false;

		auto k = new MeshKernal();
		k->setName(QString("Mesh_%1").arg(k->getID()));
		k->setMeshData(dataset);

		_meshData->appendMeshKernal(k);

		return true;
	}

	bool PDBdataExchange::write()
	{
		return false;
	}

}