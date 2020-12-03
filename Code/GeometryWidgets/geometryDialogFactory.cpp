#include "geometryDialogFactory.h"
#include "geometry/geometryModelParaBase.h"
#include "geometry/geometrySet.h"
#include "dialogCreateBox.h"
#include "dialogCreateCylinder.h"
#include "dialogCreateCone.h"
#include "dialogMakeFillet.h"
#include "dialogCreateSphere.h"
#include "dialogCreatePoint.h"
#include "dialogCreateLine.h"
#include "dialogCreateFace.h"
#include "dialogMakeChamfer.h"
#include "dialogVariableFillet.h"
#include "dialogBoolOperation.h"
#include "dialogMirrorFeature.h"
#include "dialogRotateFeature.h"
#include "dialogMakeMatrix.h"
#include "dialogMakeExtrusion.h"
#include "dialogMakeLoft.h"
#include "dialogMakeSweep.h"
#include "dialogMoveFeature.h"
#include "dialogMakeRevol.h"
#include "dialogGeoSplitter.h"
#include "dialogMakeFillHole.h"
#include "dialogMakeFillGap.h"
#include "dialogMakeRemoveSurface.h"
namespace GeometryWidget
{
	QDialog* GeometryDialogFactory::editGeometryDialog(GUI::MainWindow* m, MainWidget::PreWindow* p, Geometry::GeometrySet* set)
	{
		Geometry::GeometryModelParaBase* para = set->getParameter();
		Geometry::GeometryParaType type = para->getParaType();
		QDialog* dlg = nullptr;

		switch (type)
		{
		case Geometry::GeometryParaCreateBox:
			dlg = new CreateBoxDialog(m, p, set); break;
		case Geometry::GeometryParaCreateCylinder:
			dlg = new CreateCylinderDialog(m, p, set); break;
		case Geometry::GeometryParaCreateFillet:
			dlg = new CreateFiletDialog(m, p, set); break;
		case Geometry::GeometryParaCreateCone:
			dlg = new CreateConeDialog(m, p, set); break;
		case Geometry::GeometryParaCreateSphere:
			dlg = new CreateSphereDialog(m, p, set); break;
		case Geometry::GeometryParaCreatePoint:
			dlg = new CreatePointDialog(m, p, set); break;
		case Geometry::GeometryParaCreateLine :
			dlg = new CreateLineDialog(m, p, set); break;
		case Geometry::GeometryParaCreateFace:
			dlg = new CreateFaceDialog(m, p, set); break;
		case Geometry::GeometryParaMakeChamfer:
			dlg = new CreateChamferDialog(m, p, set); break;
		case Geometry::GeometryParaMakeVariableFillet:
			dlg = new VariableFilletDialog(m, p, set); break;
		case Geometry::GeometryParaMakeBoolOperation:
			dlg = new BoolOpertionDialog(m, p, set); break;
		case Geometry::GeometryParaMakeMirrorFeature:
			dlg = new MirorFeatureDialog(m, p, set); break;
		case  Geometry::GeometryParaMakeRotateFeature:
			dlg = new RotateFeatureDialog(m, p, set); break;
		case Geometry::GeometryParaMakeMoveFeature:
			dlg = new MoveFeatureDialog(m, p, set); break;
		case Geometry::GeometryParaMakeMatrix:
			dlg = new MakeMatrixDialog(m, p, set); break;
		case Geometry::GeometryParaMakeExtrusion:
			dlg = new CreateExtrusionDialog(m, p, set); break;
		case Geometry::GeometryParaMakeLoft:
			dlg = new CreateLoftDialog(m, p, set); break;
		case Geometry::GeometryParaMakeSweep:
			dlg = new SweepDialog(m, p, set); break;
		case  Geometry::GeometryParaMakeRevol:
			dlg = new CreateRevolDialog(m, p, set); break; 
		case  Geometry::GeometryParaSplitter:
			dlg = new GeoSplitterDialog(m, p, set); break;
		case  Geometry::GeometryParaFillHole:
			dlg = new MakeFillHoleDialog(m, p, set); break;
		case  Geometry::GeometryParaRemoveSurface:
			dlg = new MakeRemoveSurfaceDialog(m, p, set); break;
		case  Geometry::GeometryParaFillGap:
			dlg = new MakeFillGapDialog(m, p, set); break;
		default:
			break;
		}
		return dlg;
	}

}