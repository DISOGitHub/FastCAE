#ifndef _SCRIPTHANDLER_H_
#define _SCRIPTHANDLER_H_
#include <QString>
#include "NumericSrcObject.h"
#include <QObject>
class graph_MainWindow;
class ScriptHandler : public QObject
{
	Q_OBJECT
public:
	ScriptHandler(graph_MainWindow* mainwindow, QString fileName);
	~ScriptHandler();
	void Handle();
	bool flag_commandLine;
private:
	void splitCode(QString code);
	void execCode(QString name, QString args);

	bool openfile(QString args);
	void Filter_Create(QString name);
	void Filter_Clip();
	void Filter_Slice();
	void Filter_Contour();
	void Filter_Vector();
	void Filter_Reflection();
	void Filter_Smooth();
	void Filter_StreamLine();
	void Filter_Calculator();
	/*void Filter_Clip(QString args);
	void Filter_Slice(QString args);
	void Filter_Contour(QString args);
	void Filter_Vector(QString args);*/

	void Camera_FocalPoint(QString args);
	void Camera_ClippingRange(QString args);
	void Camera_Viewup(QString args);
	void Camera_Position(QString args);
	void Camera_ViewAngle(QString args);
	void Camera_ParalellScale(QString args);
	void Camera_Zoom(QString args);
	void Camera_SelfAdaptation();

	//setViewDireciton
	void Properties_viewDirection(QString args);
	void Properties_resetView();
	//properties view
	void Properties_view_backgroundType(QString args);
	void Properties_view_backgroundColor(QString args);
	void Properties_view_background2Color(QString args);
	void Properties_view_axesVisible(QString args);
	void Properties_view_remarkVisible(QString args);
	void Properties_view_remarkFontColor(QString args);
	void Properties_view_remarkText(QString args);
	void Properties_view_remarkPosition1(QString pos);
	void Properties_view_remarkPosition2(QString pos);
	void Properties_view_cameraParallel(QString args);
	void Properties_view_interaction(QString args);
	void Properties_renderView();
	//properties datasource zone
	void Properties_DataSource_DisplayGridOrZone(QString args);
	void Properties_DataSourceZone(QString args);
	//properties plane
	void Properties_planeOrigin(QString args);
	void Properties_planeNormal(QString args);
	void Properties_planeVisible(QString args);
	void Properties_insideOut(QString args);
	//properties vector
	void Properties_vector_GlyphVector(QString args);
	void Properties_vector_scalar(QString args);
	void Properties_vector_normal(QString args);
	void Properties_vector_numPoints(QString args); 
	void Properties_vector_glyph_type(QString args);
	void Properties_vector_glyph_tipRes(QString args);
	void Properties_vector_glyph_tipRad(QString args);
	void Properties_vector_glyph_tipLen(QString args);
	void Properties_vector_glyph_shaftRes(QString args);
	void Properties_vector_glyph_shaftRad(QString args);
	//streamline
	void Properties_streamline_vector(QString args);
	void Properties_streamline_integration_direction(QString args);
	void Properties_streamline_integration_type(QString args);
	void Properties_streamline_integration_stepUnit(QString args);
	void Properties_streamline_integration_initStepLen(QString args);
	void Properties_streamline_integration_miniStepLen(QString args);
	void Properties_streamline_integration_maxiStepLen(QString args);
	void Properties_streamline_stream_maxiSteps(QString args);
	void Properties_streamline_stream_maxiStreamLen(QString args);
	void Properties_streamline_stream_terminalSpeed(QString args);
	void Properties_streamline_stream_maxiError(QString args);
	void Properties_streamline_seeds_type(QString args);
	void Properties_streamline_seeds_mPoint(QString args);
	void Properties_streamline_seeds_num_points(QString args);
	void Properties_streamline_seeds_radius(QString args);
	void Properties_streamline_vorticity(QString args);
	void Properties_streamline_interpolatorType(QString args);
	void Properties_streamline_surface_streamLines(QString args);
	void Properties_streamline_reflection(QString args);
	void Properties_streamline_reflectionAxes(QString args);
	//streamline
	//contour properties
	void Properties_contourColumn(QString args); 
	void Properties_contourValue(QString args); 
	void Properties_contour_reflection(QString args);
	void Properties_contour_reflectionAxes(QString args);
	//properties reflection
	void Properties_reflectionAxes(QString args);
	void Properties_reflectionCopyInput(QString args);
	//properties smooth
	void Properties_smooth(QString args);
	//properties_calculator
	void Properties_calculatorFormula(QString args);
	void Properties_calculatorResultName(QString args);
	//properties mooringLine
	void Properties_mooringLine_type(QString args);
	void Properties_mooringLine_tipRes(QString args);
	void Properties_mooringLine_tipRad(QString args);
	void Properties_mooringLine_tipLen(QString args);
	void Properties_mooringLine_shaftRes(QString args);
	void Properties_mooringLine_shaftRad(QString args);
	void Properties_mooringLine_displaySeaBottom(QString args);
	void Properties_mooringLine_colorSeaBottom(QString args);
	//properties aniDisplace
	void Properties_aniDisplace_fileName(QString args);
	void Properties_aniDisplace_startPosition(QString args);
	void Properties_aniDisplace_interval(QString args);
	void Properties_aniDisplace_zoomFactor(QString args);
	void Properties_aniDisplace_gravityCenter(QString args);
	//properties display
	void Properties_Opacity(QString args);
	void Properties_colorColumn(QString args);
	void Properties_colorColumn_component(QString args);
	void Properties_scalarBarTitle(QString args);
	void Properties_scalarBarFontSize(QString args);
	void Properties_scalarBarNumLables(QString args);
	void Properties_lineWidth(QString args);
	void Properties_pointSize(QString args);
	void Properties_translate(QString args);
	void Properties_origin(QString args);
	void Properties_scale(QString args);
	void Properties_orientation(QString args);
	void Properties_representation(QString args);
	void Properties_specular(QString args);
	void Properties_diffuse(QString args);
	void Properties_ambient(QString args);
	void Properties_specularPower(QString args);
	void Properties_specularColor(QString args);
	void Properties_solidColor(QString args);
	void Properties_edgeColor(QString args);
	void Properties_interpolation(QString args);
	void Properties_visible(QString args); 
	void Properties_Flag_scalarBars(QString args);
	void Properties_EnableOpacityMap(QString args);
	void Properties_LookupTableMap(QString val);
	void Properties_LookupOpacityMap(QString val);
	void Properties_show_scalarBars();//dataSource
	void Properties_show_cubeAxes();

	void Properties_scalarBarPosition(QString args);

	//pick
	void Pick_Point(QString args);
	void Pick_Cell(QString args);
	void Pick_PointCurve(QString args);
	void Pick_CellCurve(QString args);

	void Save_Image(QString args);
	void Save_Animate(QString args);
	void Exit_SoftWare(QString args);
	void Set_PlotSize(QString args);
	void ReturnToOldSize();

	void clearAllPipelineObjs(QString args);
	QString _filename;
	graph_MainWindow* _mainWindow{};
	bool flag_openFile;
	PipelineObject* script_dataObj;
	PipelineObject* script_curObj;
	sliceFilter_pipelineObj_propData plane_propData;
	contourFilter_pipelineObj_propData contour_propData;
	vectorFilter_pipelineObj_propData vector_propData;
	
signals:
	void sig_script_clearAllPipelineObjs(int);
	void sig_script_setPlotSize(int*);
};

#endif
