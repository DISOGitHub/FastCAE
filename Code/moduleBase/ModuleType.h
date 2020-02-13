#ifndef _MODULETYPE_H_
#define _MODULETYPE_H_

namespace ModuleBase
{
	enum ModuleType
	{
		Undefined = -1,
		ProjectWindows,
		PropertyWindows,
		MessageWindows,
		ProcessWindows,
		DesignWindow,
		GraphWindows,
		Menu,
		ToolBar,
		IO,

	};
	enum GraphWindowType
	{
		UnDefined = -1,
		PreWindows = 0,
		RealTime = 1,
		Post2D = 2, 
		Post3D = 3,
	};
	enum SelectModel
	{
		None = -1,
		GeometryWinBody,
		GeometryWinSurface,
		GeometryWinCurve,
		GeometryWinPoint,
		GeometryBody, 
		GeometrySurface,
		GeometryCurve,
		GeometryPoint,
		MeshNode,
		MeshCell,
		BoxMeshNode,
		BoxMeshCell,
		DrawSketch,
	};
	
	enum SketchType
	{
		SketchNone,
		SketchLine,
		SketchRect,
		SketchCircle,
		SketchArc,
		SketchPolyline,
		SketchSpline,
	};

}

#endif
