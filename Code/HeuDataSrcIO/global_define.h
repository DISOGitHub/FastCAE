#ifndef GLOBALDEFINE_H
#define GLOBALDEFINE_H
#include "qstringlist.h"
#define scalarBar_FontSize 12
#define scalarBar_NumLables 5
enum  EInterpolationType
{
	kFlat = 0,
	kGouraud = 1
};
enum  ERepresentationType
{
	kkPoints = 0,
	kkSurface = 1,
	kkSurfaceWithEdges = 2,
	kWIreframe = 3,
	k3DGlyphs = 4,
	kOutLine = 5,
	kPointGaussian = 6,
	kVolume = 7

};
struct  scolor
{
	int  r = 255;
	int  g = 255;
	int  b = 255;
};

struct  Fdata_XYZ
{
	double x = 0;
	double y = 0;
	double z = 0;
};
enum Def_GridType
{
	dSTRUCTURED_GRID = 0,
	dRECTILINEAR_GRID = 1,
	dUNSTRUCTURED_GRID = 2,
	dPolyData = 3,
	dDefault = 4,
};
enum  Def_CellType
{
	dUNSTRUCTURED_CELL = 0,
	dPOLYGON_CELL = 1,
};
enum Def_SURFACE_TYPE { dGlyph_Arrow = 0, dGlyph_Cone, dGlyph_Box };
enum Def_ColorMode {
	dCOLOR_BY_SCALAR,
	dCOLOR_BY_VECTOR_MAGNITUDE,
	dCOLOR_BY_VECTOR_X,
	dCOLOR_BY_VECTOR_Y,
	dCOLOR_BY_VECTOR_Z,
	dCOLOR_CONSTANT,
};
enum Def_PipelineObjType
{
	dNumeric_DataSource = 0,
	dClip_DataSource = 1,
	dSlice_DataSource = 2,
	dContour_DataSource = 3,	
	dVector_DataSource = 4,
	dReflection_DataSource = 5,
	dSmooth_DataSource=6,
	dStreamLine_DataSource = 7,
	dUnknowPO_Type = 8,
	dBox_DataSource = 9,
	dCalculator_DataSource = 10,
};
enum  Def_DataAttributeType
{
	dPOINT_DATA = 0,
	dCELL_DATA,
	dFIELD_DATA,
};
enum Def_ElemType
{
	dElem2D_Tri3 = 0,
	dElem2D_Rect4,
	dElem3D_Tet4,
	dElem3D_Box4,
	dElemPOLYGON,
	dElem_Unknown

};
enum ReflectionPlane
{
	USE_X_MIN = 0,
	USE_Y_MIN = 1,
	USE_Z_MIN = 2,
	USE_X_MAX = 3,
	USE_Y_MAX = 4,
	USE_Z_MAX = 5,
	USE_X = 6,
	USE_Y = 7,
	USE_Z = 8
};
enum IntegrationDirection
{
	FORWARD = 0,
	BACKWARD = 1,
	BOTH = 2
};
enum IntegratorType
{
	Runge_Kutta_2 = 0,
	Runge_Kutta_4 = 1,
	Runge_Kutta_4_5 = 2
};
enum InterpolatorType
{
	PointLocator = 0,
	CellLoactor = 1
};
enum IntegrationStepUnit
{
	Length = 1,
	Cell_Length= 2
};
enum SpeedType
{
	Point_Source = 0,
	High_Resolution_Line_Source = 1
};
struct  ScalarColor_Data
{
	double val;
	double r;
	double g;
	double b;
};
struct  ScalarOpacity_Data
{
	double val;
	double opacity;
};
struct  Scalar_Vector_NameListData
{
	QStringList scalarList_Point;
	QStringList scalarList_Cell;
	QStringList vectorList_Point;
	QStringList vectorList_Cell;
};
typedef struct FrameDisplace_struct
{
	float time;
	float Surge_GX;
	float Sway_GY;
	float Heave_GZ;
	float Roll_Gxb;
	float Pitch_Gyb;
	float Yaw_Gzb;
} FrameDisplace;

typedef struct contourProp_struct
{
	double val;
	double min;
	double max;
	QString colName;
} contour_PropData;

typedef struct ScalarBarPosition_Para1
{
	double position[4];
	int orietation;
} ScalarBarPosition_Para;

typedef struct scalarBarTitle_struct
{
	int size;
	int num_lable;
	QString title;
} scalarBarTitle_PropData;

typedef struct lookupTableTransfer_struct
{
	double val;
	double r;
	double g;
	double b;
} lookupTableTransfer_PropData;
typedef struct lookupTableOpacity_struct
{
	double val;
	double opa;
} lookupTableOpacity_PropData;
typedef struct GlyphVectorXYZ_struct
{
	QString vecName;
	QString XYZName;
	QString colName;
	int numLables;
	int fontSize;
	QString title;
} GlyphVectorXYZ_PropData;
typedef struct reflection_struct
{
	bool flag_refleciton;
	ReflectionPlane type;
} reflection_PropData;

#endif;