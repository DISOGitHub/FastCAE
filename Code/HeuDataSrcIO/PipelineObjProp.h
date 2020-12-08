#ifndef PIPELINEOBJPROP_H
#define PIPELINEOBJPROP_H

#include "heudatasrcio_global.h"
#include "qstring.h"
#include "qstringlist.h"
#include "qmap.h"
#include "qvector.h"
#include "global_define.h"
#include "vtkScalarBarActor.h"
#include "vtkSmartPointer.h"
#include "vtkActor.h"
#include <vtkPlanes.h>

struct HEUDATASRCIOSHARED_EXPORT pipelineObj_propData
{
	int obj_id;
	bool flag_rectilinear = false;//是否是线性网格，如是可做等值面
	QStringList fileList;
	QStringList fileListBak;
	QString fileName;
	int file_xh;
	QMap<QString, bool> scalarBar_showMap;
	QMap<QString, bool> scalarBar_opacityMap;
	QMap<QString, scalarBarTitle_PropData> scalarBar_titleMap;

	//20180209 for zone
	int count_total_child;
	bool flag_show_actors;
	int count_clip;
	int count_slice;
	int count_vector;
	int count_contour;
	int count_reflection;
	int count_calculator;
	int count_smooth;
	int count_streamLine;
	double scalarColorRange[2];	
	QString mColorColumnStyle = "SolidColor";
	QString mColorColumnStyle_XYZ = "X";
	int  mPointCount = 0;
	int mCellCount = 0;
	int mPointSize;
	int mLineWidth;
	//Lighting
	//颜色相关
	scolor mSolidColor;
	double mSpecular;
	scolor mSpecularColor;
	scolor mEdgeColor;
	int mSpecularPower;
	double mAmbient;
	scolor mAmbientColor;
	double mDiffuse;
	scolor mDiffuseColor;
	struct scolor m_struct_edgecolor;
	struct Fdata_XYZ m_struct_transforming;
	struct Fdata_XYZ m_struct_scale;
	struct Fdata_XYZ m_struct_origin;
	struct Fdata_XYZ m_struct_orientation;
	EInterpolationType m_enum_interpolationtype;
	ERepresentationType m_enum_representationtype;
	double mOpacity;
	bool mScalarBarVisible; 
	QStringList m_colorList;
	QStringList scalarList;
	QStringList vectorList;
	QMap<QString, bool> block_showMap;
	int flag_grid_blocks;//grid:0,blocks:1;

	//combine .obj model
	bool flag_comModel;
	QString comModel_fileName;
	//cube Axes
	bool flag_cubeAxes;
	QString pre_componentXYZ;
};

struct HEUDATASRCIOSHARED_EXPORT aniDisplace_pipelineObj_propData
{
	QVector<FrameDisplace> m_aniDisplaceList;
	bool flag_AniDisplaceFile;//是否使用动画位移文件
	bool flag_exeDisplace;
	QString path_AniDisplaceFile;
	FrameDisplace m_pre_displace;
	FrameDisplace m_cur_displace;
	struct Fdata_XYZ m_origin;
	bool flag_inputGravity;
	int cur_pos;
	int start_pos;
	int interval;
	int dis_xh;
	double zoom_factor;
};

struct HEUDATASRCIOSHARED_EXPORT sliceFilter_pipelineObj_propData
{
	QString filterName;
	bool flag_plane;
	struct Fdata_XYZ m_plane_origin;
	struct Fdata_XYZ m_plane_normal;
	bool flag_insideOut;
};
struct HEUDATASRCIOSHARED_EXPORT contourFilter_pipelineObj_propData
{
	QString filterName;
	QString contour_colName;
	QVector<contour_PropData> contour_dataList;
};
struct HEUDATASRCIOSHARED_EXPORT reflectionFilter_pipelineObj_propData
{
	QString filterName;
	enum ReflectionPlane reflection_axis;
	bool flag_copyInput;
};
struct HEUDATASRCIOSHARED_EXPORT calculatorFilter_pipelineObj_propData
{
	QString filterName;
	QString formula;
	QString pre_formula;
	QString resultName;
};
struct HEUDATASRCIOSHARED_EXPORT smoothFilter_pipelineObj_propData
{
	QString filterName;
	int smooth_coef;
	bool flag_meshSmooth;
	bool flag_cellToPoint;
};
struct  HEUDATASRCIOSHARED_EXPORT glyph_arrow
{
	int  tipRes;
	double  tipRad;
	double tipLen;
	int shaftRes;
	double shaftRad;
};
struct  HEUDATASRCIOSHARED_EXPORT glyph_cone
{
	int  Res;
	double  Rad;
	double Hei;
	struct Fdata_XYZ glyph_center;
	struct Fdata_XYZ glyph_direction;
};
////////////////////////////////////////////////////////
struct  HEUDATASRCIOSHARED_EXPORT integration_para
{
	enum IntegrationDirection direction;
	enum IntegratorType  type;
	enum IntegrationStepUnit stepUnit;
	double initStepLen;
	double miniStepLen;
	double maxiStepLen;
};
struct  HEUDATASRCIOSHARED_EXPORT stream_para
{
	int  maxiSteps;
	double  old_maxiStreamLen;
	double  maxiStreamLen;
	double terminalSpeed;
	double maxiError;
};
struct  HEUDATASRCIOSHARED_EXPORT seeds_para
{
	enum SpeedType type;
	struct Fdata_XYZ mPoint;
	int num_points;
	double radius;
};
struct  HEUDATASRCIOSHARED_EXPORT streamLineFilter_pipelineObj_propData
{
	QString filterName;
	integration_para integration_data;
	stream_para stream_data;
	bool vorticity;
	seeds_para seeds_data;
	QStringList vector_list;
	QString vectorColName;
	enum InterpolatorType interpolator_type;
	bool surface_streamLines;
};
////////////////////////////////////////////////////////
struct  HEUDATASRCIOSHARED_EXPORT glyph_cylinder
{
	int  Res;
	double  Rad;
	double Hei;
	struct Fdata_XYZ glyph_center;
};
struct  HEUDATASRCIOSHARED_EXPORT glyph
{
	int  type;//0arrow  1cone 2cylinder 3line
	double scaleFactor;
	struct  glyph_arrow arrow_data;
	struct  glyph_cone cone_data;
	struct glyph_cylinder cylinder_data;
};
struct HEUDATASRCIOSHARED_EXPORT mooringLine_pipelineObj_propData
{
	struct glyph glyph_data;
	bool flag_seaBottom;
	scolor seaBottomColor;
};
struct HEUDATASRCIOSHARED_EXPORT particles_pipelineObj_propData
{
	int type;//0 none 1 sphere
	double radius;
	int resolution;
	double max_radius;
	double min_radius;
};
struct HEUDATASRCIOSHARED_EXPORT vectorFilter_pipelineObj_propData
{
	QString filterName;
	struct glyph glyph_data;
	QStringList scalar_list;
	QStringList vector_list;
	QString scalarColName;
	QString vectorColName;
	int mask;
	int numPoints;
	QString glyphVector_colorXYZ;
	GlyphVectorXYZ_PropData	m_preGlyphVectorPara;
};
class  PipelineObjProp
{
public:
	struct pipelineObj_propData pipelineObj_base_propData;
	struct sliceFilter_pipelineObj_propData plane_propData;
	struct reflectionFilter_pipelineObj_propData reflectionFilter_propData;
	struct contourFilter_pipelineObj_propData contourFilter_propData;
	struct vectorFilter_pipelineObj_propData vectorFilter_propData;
	struct smoothFilter_pipelineObj_propData smoothFilter_propData;
	struct streamLineFilter_pipelineObj_propData streamLineFilter_propData;
	struct calculatorFilter_pipelineObj_propData calculatorFilter_propData;
	//struct pick_pipelineObj_propData pick_propData;
	struct mooringLine_pipelineObj_propData mooringLine_propData;
	struct particles_pipelineObj_propData particles_propData;
	struct aniDisplace_pipelineObj_propData aniDisplace_propData;

	PipelineObjProp();
	virtual ~PipelineObjProp();
	QString GetRepresentationType();
	void slt_set_FilterSlice_plane(double origins[3],double normals[3]);

protected:
	
};
#endif
