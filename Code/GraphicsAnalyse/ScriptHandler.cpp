#include "ScriptHandler.h"
#include "graph_mainwindow.h"
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QFileInfo>
#include <QList>
#include <QDebug>

ScriptHandler::ScriptHandler(graph_MainWindow* mainwindow, QString fileName) :
	_mainWindow(mainwindow), _filename(fileName)
{
	QTextCodec *codec = QTextCodec::codecForName("GBK");
	QTextCodec::setCodecForLocale(codec);
	script_dataObj=NULL;
	flag_commandLine = false;
}

ScriptHandler::~ScriptHandler()
{
	_filename.clear();
	_mainWindow = nullptr;
}

void ScriptHandler::Handle()
{
	QFile file(_filename);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		assert(0);
		return;
	}
	QTextStream stream(&file);
	QString line;
	while (!stream.atEnd())
	{
		line = stream.readLine().trimmed();
		if (line == "")
			continue;
		splitCode(line);
	}
	file.close();
}

void ScriptHandler::splitCode(QString code)
{
	if (code.startsWith("#")) 
		return;
	qDebug() << code;
	/*QStringList fa = code.split("(");
	if (fa.size() < 2)
	{
		assert(0);
		return;
	}
	QString fun = fa.at(0);
	QString args;
	args = code.right(code.length() - fun.length()-1);
	args = args.remove(")");
	*/
	int pos = code.indexOf("(", 0);
	QString fun = code.left(pos);
	QString args = code.right(code.length() - pos - 1);
	args = args.left(args.length() - 1);
	execCode(fun, args);
}

void ScriptHandler::execCode(QString name, QString args)
{
	if (name.toLower() == "openfile")
	{
		flag_openFile = false;
		flag_openFile=openfile(args);
	}
	else if ((name.toLower() == "filter_clip") || (name.toLower() == "filter_slice") || (name.toLower() == "filter_contour")
		|| (name.toLower() == "filter_vector") || (name.toLower() == "filter_reflection") || (name.toLower() == "filter_calculator")
		|| (name.toLower() == "filter_smooth") || (name.toLower() == "filter_streamline"))
	{
		Filter_Create(name);
	}
	else if (name.toLower() == "camera_position")
	{
		Camera_Position(args);
	}
	else if (name.toLower() == "properties_datasource_displaygridorzone")
	{
		Properties_DataSource_DisplayGridOrZone(args);
	}
	else if (name.toLower() == "properties_datasourcezone")
	{
		Properties_DataSourceZone(args);
	}
	else if (name.toLower() == "properties_planeorigin")
	{
		Properties_planeOrigin(args);
	}
	else if (name.toLower() == "properties_planenormal")
	{
		Properties_planeNormal(args);
	}
	else if (name.toLower() == "properties_planevisible")
	{
		Properties_planeVisible(args);
	}
	else if (name.toLower() == "properties_insideout")
	{
		Properties_insideOut(args);
	}
	else if (name.toLower() == "properties_vector_glyphvector")
	{
		Properties_vector_GlyphVector(args);
	}
	else if (name.toLower() == "properties_vector_scalar")
	{
		Properties_vector_scalar(args);
	}
	else if (name.toLower() == "properties_vector_normal")
	{
		Properties_vector_normal(args);
	}
	else if (name.toLower() == "properties_vector_numpoints")
	{
		Properties_vector_numPoints(args);
	}
	else if (name.toLower() == "properties_vector_glyph_type")
	{
		Properties_vector_glyph_type(args);
	}
	else if (name.toLower() == "properties_vector_glyph_tipres")
	{
		Properties_vector_glyph_tipRes(args);
	}
	else if (name.toLower() == "properties_vector_glyph_tiprad")
	{
		Properties_vector_glyph_tipRad(args);
	}
	else if (name.toLower() == "properties_vector_glyph_tiplen")
	{
		Properties_vector_glyph_tipLen(args);
	}
	else if (name.toLower() == "properties_vector_glyph_shaftres")
	{
		Properties_vector_glyph_shaftRes(args);
	}
	else if (name.toLower() == "properties_vector_glyph_shaftrad")
	{
		Properties_vector_glyph_shaftRad(args);
	}
	else if (name.toLower() == "properties_contourcolumn")
	{
		Properties_contourColumn(args);
	}
	else if (name.toLower() == "properties_contourvalue")
	{
		Properties_contourValue(args);
	}
	else if (name.toLower() == "properties_contour_reflection")
	{
		Properties_contour_reflection(args);
	}
	else if (name.toLower() == "properties_contour_reflectionaxes")
	{
		Properties_contour_reflectionAxes(args);
	}
	else if (name.toLower() == "properties_reflectionaxes")
	{
		Properties_reflectionAxes(args);
	}
	else if (name.toLower() == "properties_reflectioncopyinput")
	{
		Properties_reflectionCopyInput(args);
	}
	else if (name.toLower() == "properties_calculatorformula")
	{
		Properties_calculatorFormula(args);
	}
	else if (name.toLower() == "properties_calculatorresultname")
	{
		Properties_calculatorResultName(args);
	}
	else if (name.toLower() == "properties_smooth")
	{
		Properties_smooth(args);
	}
	//streamline
	else if (name.toLower() == "properties_streamline_vector")
	{
		Properties_streamline_vector(args);
	}
	else if (name.toLower() == "properties_streamline_integration_direction")
	{
		Properties_streamline_integration_direction(args);
	}
	else if (name.toLower() == "properties_streamline_integration_type")
	{
		Properties_streamline_integration_type(args);
	}
	else if (name.toLower() == "properties_streamline_integration_stepunit")
	{
		Properties_streamline_integration_stepUnit(args);
	}
	else if (name.toLower() == "properties_streamline_integration_initsteplen")
	{
		Properties_streamline_integration_initStepLen(args);
	}
	else if (name.toLower() == "properties_streamline_integration_ministeplen")
	{
		Properties_streamline_integration_miniStepLen(args);
	}
	else if (name.toLower() == "properties_streamline_integration_maxisteplen")
	{
		Properties_streamline_integration_maxiStepLen(args);
	}
	else if (name.toLower() == "properties_streamline_stream_maxisteps")
	{
		Properties_streamline_stream_maxiSteps(args);
	}
	else if (name.toLower() == "properties_streamline_stream_maxistreamlen")
	{
		Properties_streamline_stream_maxiStreamLen(args);
	}
	else if (name.toLower() == "properties_streamline_stream_terminalspeed")
	{
		Properties_streamline_stream_terminalSpeed(args);
	}
	else if (name.toLower() == "properties_streamline_reflection")
	{
		Properties_streamline_reflection(args);
	}
	else if (name.toLower() == "properties_streamline_reflectionaxes")
	{
		Properties_streamline_reflectionAxes(args);
	}
	else if (name.toLower() == "properties_streamline_stream_maxierror")
	{
		Properties_streamline_stream_maxiError(args);
	}
	else if (name.toLower() == "properties_streamline_seeds_type")
	{
		Properties_streamline_seeds_type(args);
	}
	else if (name.toLower() == "properties_streamline_seeds_mpoint")
	{
		Properties_streamline_seeds_mPoint(args);
	}
	else if (name.toLower() == "properties_streamline_seeds_num_points")
	{
		Properties_streamline_seeds_num_points(args);
	}
	else if (name.toLower() == "properties_streamline_seeds_radius")
	{
		Properties_streamline_seeds_radius(args);
	}
	else if (name.toLower() == "properties_streamline_vorticity")
	{
		Properties_streamline_vorticity(args);
	}
	else if (name.toLower() == "properties_streamline_interpolatortype")
	{
		Properties_streamline_interpolatorType(args);
	}
	else if (name.toLower() == "properties_streamline_surface_streamlines")
	{
		Properties_streamline_surface_streamLines(args);
	}
	//streamline
	else if (name.toLower() == "properties_mooringline_type")
	{
		Properties_mooringLine_type(args);
	}
	else if (name.toLower() == "properties_mooringline_tipres")
	{
		Properties_mooringLine_tipRes(args);
	}
	else if (name.toLower() == "properties_mooringline_tiprad")
	{
		Properties_mooringLine_tipRad(args);
	}
	else if (name.toLower() == "properties_mooringline_tiplen")
	{
		Properties_mooringLine_tipLen(args);
	}
	else if (name.toLower() == "properties_mooringline_shaftres")
	{
		Properties_mooringLine_shaftRes(args);
	}
	else if (name.toLower() == "properties_mooringline_shaftrad")
	{
		Properties_mooringLine_shaftRad(args);
	}
	else if (name.toLower() == "properties_mooringline_displayseabottom")
	{
		Properties_mooringLine_displaySeaBottom(args);
	}
	else if (name.toLower() == "properties_mooringline_colorseabottom")
	{
		Properties_mooringLine_colorSeaBottom(args);
	}
	//aniDisplace
	else if (name.toLower() == "properties_anidisplace_filename")
	{
		Properties_aniDisplace_fileName(args);
	}
	else if (name.toLower() == "properties_anidisplace_startposition")
	{
		Properties_aniDisplace_startPosition(args);
	}
	else if (name.toLower() == "properties_anidisplace_interval")
	{
		Properties_aniDisplace_interval(args);
	}
	else if (name.toLower() == "properties_anidisplace_zoomfactor")
	{
		Properties_aniDisplace_zoomFactor(args);
	}
	else if (name.toLower() == "properties_anidisplace_gravitycenter")
	{
		Properties_aniDisplace_gravityCenter(args);
	}
	else if (name.toLower() == "properties_opacity")
	{
		Properties_Opacity(args);
	}
	else if (name.toLower() == "properties_colorcolumn")
	{
		Properties_colorColumn(args);
	}
	else if (name.toLower() == "properties_colorcolumn_component")
	{
		Properties_colorColumn_component(args);
	}
	else if (name.toLower() == "properties_scalarbartitle")
	{
		Properties_scalarBarTitle(args);
	}
	else if (name.toLower() == "properties_scalarbarfontsize")
	{
		Properties_scalarBarFontSize(args);
	}
	else if (name.toLower() == "properties_scalarbarnumlables")
	{
		Properties_scalarBarNumLables(args);
	}
	else if (name.toLower() == "properties_linewidth")
	{
		Properties_lineWidth(args);
	}
	else if (name.toLower() == "properties_pointsize")
	{
		Properties_pointSize(args);
	}
	else if (name.toLower() == "properties_translate")
	{
		Properties_translate(args);
	}
	else if (name.toLower() == "properties_origin")
	{
		Properties_origin(args);
	}
	else if (name.toLower() == "properties_scale")
	{
		Properties_scale(args);
	}
	else if (name.toLower() == "properties_orientation")
	{
		Properties_orientation(args);
	}
	else if (name.toLower() == "properties_representation")
	{
		Properties_representation(args);
	}
	else if (name.toLower() == "properties_specular")
	{
		Properties_specular(args);
	}
	else if (name.toLower() == "properties_diffuse")
	{
		Properties_diffuse(args);
	}
	else if (name.toLower() == "properties_ambient")
	{
		Properties_ambient(args);
	}
	else if (name.toLower() == "properties_specularpower")
	{
		Properties_specularPower(args);
	}
	else if (name.toLower() == "properties_specularcolor")
	{
		Properties_specularColor(args);
	}
	else if (name.toLower() == "properties_solidcolor")
	{
		Properties_solidColor(args);
	}
	else if (name.toLower() == "properties_edgecolor")
	{
		Properties_edgeColor(args);
	}
	else if (name.toLower() == "properties_interpolation")
	{
		Properties_interpolation(args);
	}
	else if (name.toLower() == "properties_visible")
	{
		Properties_visible(args);
	}
	else if (name.toLower() == "properties_flag_scalarbars")
	{
		Properties_Flag_scalarBars(args);
	}
	else if (name.toLower() == "properties_lookuptablemap")
	{
		Properties_LookupTableMap(args);
	}
	else if (name.toLower() == "properties_lookupopacitymap")
	{
		Properties_LookupOpacityMap(args);
	}
	else if (name.toLower() == "properties_enableopacitymap")
	{
		Properties_EnableOpacityMap(args);
	}
	else if (name.toLower() == "properties_show_scalarbars")
	{
		Properties_show_scalarBars();
	}
	else if (name.toLower() == "properties_show_cubeAxes")
	{
		Properties_show_cubeAxes();
	}
	else if (name.toLower() == "properties_scalarbarposition")
	{
		Properties_scalarBarPosition(args);
	}
	else if (name.toLower() == "camera_viewup")
	{
		Camera_Viewup(args);
	}
	else if (name.toLower() == "camera_focalpoint")
	{
		Camera_FocalPoint(args);
	}
	/*else if (name.toLower() == "camera_clippingrange")
	{
		Camera_ClippingRange(args);
	}*/
	else if (name.toLower() == "camera_viewangle")
	{
		Camera_ViewAngle(args);
	}
	else if (name.toLower() == "camera_paralellscale")
	{
		Camera_ParalellScale(args);
	}
	else if (name.toLower() == "camera_selfadaptation")
	{
		Camera_SelfAdaptation();
	}
	else if (name.toLower() == "properties_view_backgroundtype")
	{
		Properties_view_backgroundType(args);
	}
	else if (name.toLower() == "properties_view_backgroundcolor")
	{
		Properties_view_backgroundColor(args);
	}
	else if (name.toLower() == "properties_view_background2color")
	{
		Properties_view_background2Color(args);
	}
	else if (name.toLower() == "properties_view_axesvisible")
	{
		Properties_view_axesVisible(args);
	}
	else if (name.toLower() == "properties_view_cameraparallel")
	{
		Properties_view_cameraParallel(args);
	}
	else if (name.toLower() == "properties_view_interaction")
	{
		Properties_view_interaction(args);
	}
	else if (name.toLower() == "properties_view_remarkvisible")
	{
		Properties_view_remarkVisible(args);
	}
	else if (name.toLower() == "properties_view_remarkfontcolor")
	{
		Properties_view_remarkFontColor(args);
	}
	else if (name.toLower() == "properties_view_remarktext")
	{
		Properties_view_remarkText(args);
	}
	else if (name.toLower() == "properties_view_remarkposition1")
	{
		Properties_view_remarkPosition1(args);
	}
	else if (name.toLower() == "properties_view_remarkposition2")
	{
		Properties_view_remarkPosition2(args);
	}
	else if (name.toLower() == "properties_renderview")
	{
		Properties_renderView();
	}
	else if (name.toLower() == "properties_viewdirection")
	{
		Properties_viewDirection(args);
	}
	else if (name.toLower() == "properties_resetView")
	{
		Properties_resetView();
	}
	else if (name.toLower() == "apply")
	{
		_mainWindow->script_applyClicked();
	}
	else if (name.toLower() == "startanimate")
	{
		_mainWindow->slot_animate();
	}
	else if (name.toLower() == "save_image")
	{
		Save_Image(args);
	}
	else if (name.toLower() == "save_animate")
	{
		Save_Animate(args);
	}
	else if (name.toLower() == "pick_point")
	{
		Pick_Point(args);
	}
	else if (name.toLower() == "pick_cell")
	{
		Pick_Cell(args);
	}
	else if (name.toLower() == "pick_pointcurve")
	{
		Pick_PointCurve(args);
	}
	else if (name.toLower() == "pick_cellcurve")
	{
		Pick_CellCurve(args);
	}
	else if (name.toLower() == "exit_software")
	{
		Exit_SoftWare(args);
	}
	else if (name.toLower() == "set_plotsize")
	{
		Set_PlotSize(args);
	}
	else if (name.toLower() == "returntooldsize")
	{
		ReturnToOldSize();
	}
	else if (name.toLower() == "clearallpipelineobjs")
	{
		clearAllPipelineObjs(args);
	}
}

bool ScriptHandler::openfile(QString args)
{
	if (_mainWindow->script_openFile(args))
	{
		script_dataObj = _mainWindow->get_cur_PipelineObj();
		script_curObj = _mainWindow->get_cur_PipelineObj();
		return true;
	}
	else
	{
		script_dataObj = NULL;
		script_curObj = NULL;
		return false;
	}
}
void ScriptHandler::Filter_Create(QString name)
{
	if (name.toLower() == "filter_slice")
	{
		Filter_Slice();
	}
	else if (name.toLower() == "filter_contour")
	{
		Filter_Contour();
	}
	else if (name.toLower() == "filter_vector")
	{
		Filter_Vector();
	}
	else if (name.toLower() == "filter_reflection")
	{
		Filter_Reflection();
	}
	else if (name.toLower() == "filter_calculator")
	{
		Filter_Calculator();
	}
	else if (name.toLower() == "filter_smooth")
	{
		Filter_Smooth();
	}
	else if (name.toLower() == "filter_streamline")
	{
		Filter_StreamLine();
	}
}
void ScriptHandler::Filter_Clip()
{
	if ((!flag_openFile) || (script_dataObj==NULL))
		return;
	_mainWindow->script_FilterClip(script_dataObj);
	script_curObj = _mainWindow->get_cur_PipelineObj();
}

void ScriptHandler::Filter_Slice()
{
	if ((!flag_openFile) || (script_dataObj == NULL))
		return;
	_mainWindow->script_FilterSlice(script_dataObj);
	script_curObj = _mainWindow->get_cur_PipelineObj();
}

void ScriptHandler::Filter_Contour()
{
	if ((!flag_openFile) || (script_dataObj == NULL))
		return;
	_mainWindow->script_FilterContour(script_dataObj);
	script_curObj = _mainWindow->get_cur_PipelineObj();
}

void ScriptHandler::Filter_Vector()
{
	if ((!flag_openFile) || (script_dataObj == NULL))
		return;
	_mainWindow->script_FilterVector(script_dataObj);
	script_curObj = _mainWindow->get_cur_PipelineObj();
}

void ScriptHandler::Filter_StreamLine()
{
	if ((!flag_openFile) || (script_dataObj == NULL))
		return;
	_mainWindow->script_FilterStreamLine(script_dataObj);
	script_curObj = _mainWindow->get_cur_PipelineObj();
}

void ScriptHandler::Filter_Reflection()
{
	if ((!flag_openFile) || (script_dataObj == NULL))
		return;
	_mainWindow->script_FilterReflection(script_dataObj);
	script_curObj = _mainWindow->get_cur_PipelineObj();
}
void ScriptHandler::Filter_Calculator()
{
	if ((!flag_openFile) || (script_dataObj == NULL))
		return;
	_mainWindow->script_FilterCalculator(script_dataObj);
	script_curObj = _mainWindow->get_cur_PipelineObj();
}
void ScriptHandler::Filter_Smooth()
{
	if ((!flag_openFile) || (script_dataObj == NULL))
		return;
	_mainWindow->script_FilterSmooth(script_dataObj);
	script_curObj = _mainWindow->get_cur_PipelineObj();
}



void ScriptHandler::Camera_FocalPoint(QString args)
{
	bool ok;
	double pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toDouble(&ok);
		pos[1] = tep_list.at(1).toDouble(&ok);
		pos[2] = tep_list.at(2).toDouble(&ok);
		_mainWindow->script_Camera_FocalPoint(pos);
	}
	
}

void ScriptHandler::Camera_ClippingRange(QString args)
{
	bool ok;
	double pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 2)
	{
		pos[0] = tep_list.at(0).toDouble(&ok);
		pos[1] = tep_list.at(1).toDouble(&ok);
		_mainWindow->script_Camera_ClippingRange(pos);
	}
	
}

void ScriptHandler::Camera_Viewup(QString args)
{
	bool ok;
	double pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toDouble(&ok);
		pos[1] = tep_list.at(1).toDouble(&ok);
		pos[2] = tep_list.at(2).toDouble(&ok);
		_mainWindow->script_Camera_ViewUp(pos);
	}
	
}

void ScriptHandler::Camera_Position(QString args)
{
	bool ok;
	double pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toDouble(&ok);
		pos[1] = tep_list.at(1).toDouble(&ok);
		pos[2] = tep_list.at(2).toDouble(&ok);
		_mainWindow->script_Camera_Position(pos);
	}
	
}

void ScriptHandler::Camera_ViewAngle(QString args)
{
	bool ok;
	double val;
	val= args.toDouble(&ok);
	_mainWindow->script_Camera_ViewAngle(val);
}
void ScriptHandler::Camera_ParalellScale(QString args)
{
	bool ok;
	double val;
	val = args.toDouble(&ok);
	_mainWindow->script_Camera_ParalellScale(val);
}
void ScriptHandler::Camera_Zoom(QString args)
{
	bool ok;
	double val;
	val = args.toDouble(&ok);
	_mainWindow->script_Camera_Zoom(val);
}

void ScriptHandler::Properties_DataSourceZone(QString args)
{
	if (script_curObj == NULL)
		return;
	if (args.split(",", QString::SkipEmptyParts).count() > 0)
	{
		_mainWindow->script_Properties_dataSourceZone(script_curObj, args);
	}
}

void ScriptHandler::Properties_DataSource_DisplayGridOrZone(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val;
	val = args.toInt(&ok, 10);
	if (val)
		val = 1;
	_mainWindow->script_Properties_DataSource_DisplayGridOrZone(script_curObj, val);
}

void ScriptHandler::Properties_planeOrigin(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toDouble(&ok);
		pos[1] = tep_list.at(1).toDouble(&ok);
		pos[2] = tep_list.at(2).toDouble(&ok);
		_mainWindow->script_Properties_planeOrigin(script_curObj, pos);
	}
	
}

void ScriptHandler::Properties_planeNormal(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toDouble(&ok);
		pos[1] = tep_list.at(1).toDouble(&ok);
		pos[2] = tep_list.at(2).toDouble(&ok);
		_mainWindow->script_Properties_planeNormal(script_curObj, pos);
	}
	
}

void ScriptHandler::Properties_planeVisible(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	bool val = false;
	if (args.toInt(&ok, 10)!=0)
		val = true;
	_mainWindow->script_Properties_planeVisible(script_curObj, val);
}
void ScriptHandler::Properties_insideOut(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	bool val = false;
	if (args.toInt(&ok, 10) != 0)
		val = true;
	_mainWindow->script_Properties_insideOut(script_curObj, val);
}
void ScriptHandler::Properties_Opacity(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if (val < 0)
		val = 0.0;
	else if (val>1.0)
		val = 1.0;
	_mainWindow->script_Properties_Opacity(script_curObj, val);
}

void ScriptHandler::Properties_colorColumn(QString args)
{
	if (script_curObj == NULL)
		return;
	_mainWindow->script_Properties_colorColumn(script_curObj, args);
}
void ScriptHandler::Properties_colorColumn_component(QString args)
{
	if (script_curObj == NULL)
		return;
	_mainWindow->script_Properties_colorColumn_component(script_curObj, args);
}
void ScriptHandler::Properties_scalarBarTitle(QString args)
{
	if (script_curObj == NULL)
		return;
	QStringList list = args.split(",");
	if (list.count() != 2)
		return;
	_mainWindow->script_Properties_scalarBarTitle(script_curObj,list.at(0),list.at(1));
}
void ScriptHandler::Properties_scalarBarFontSize(QString args)
{
	if (script_curObj == NULL)
		return;
	QStringList list = args.split(",");
	if (list.count() != 2)
		return;
	bool ok;
	int val = list.at(1).toInt(&ok);
	if ((val < 3) || (val>30))
		val = scalarBar_FontSize;
	_mainWindow->script_Properties_scalarBarFontSize(script_curObj, list.at(0), val);
}
void ScriptHandler::Properties_scalarBarNumLables(QString args)
{
	if (script_curObj == NULL)
		return;
	QStringList list = args.split(",");
	if (list.count() != 2)
		return;
	bool ok;
	int val = list.at(1).toInt(&ok);
	if ((val < 3) || (val>10))
		val = scalarBar_NumLables;
	_mainWindow->script_Properties_scalarBarNumLables(script_curObj, list.at(0),val );
}
void ScriptHandler::Properties_lineWidth(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok);
	if (val < 1)
		val = 1;
	_mainWindow->script_Properties_lineWidth(script_curObj, val);
}

void ScriptHandler::Properties_pointSize(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok);
	if (val < 1)
		val = 1;
	_mainWindow->script_Properties_pointSize(script_curObj, val);
}

void ScriptHandler::Properties_translate(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toDouble(&ok);
		pos[1] = tep_list.at(1).toDouble(&ok);
		pos[2] = tep_list.at(2).toDouble(&ok);
		_mainWindow->script_Properties_translate(script_curObj, pos);
	}
	
}

void ScriptHandler::Properties_origin(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toDouble(&ok);
		pos[1] = tep_list.at(1).toDouble(&ok);
		pos[2] = tep_list.at(2).toDouble(&ok);
		_mainWindow->script_Properties_origin(script_curObj, pos);
	}
	
}

void ScriptHandler::Properties_scale(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toDouble(&ok);
		pos[1] = tep_list.at(1).toDouble(&ok);
		pos[2] = tep_list.at(2).toDouble(&ok);
		_mainWindow->script_Properties_scale(script_curObj, pos);
	}
	
}

void ScriptHandler::Properties_orientation(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toDouble(&ok);
		pos[1] = tep_list.at(1).toDouble(&ok);
		pos[2] = tep_list.at(2).toDouble(&ok);
		_mainWindow->script_Properties_orientation(script_curObj, pos);
	}
	
}

void ScriptHandler::Properties_representation(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok, 10);
	if ((val < 0) || (val>3))
		val = 1;
	_mainWindow->script_Properties_representation(script_curObj, (ERepresentationType)val);
}

void ScriptHandler::Properties_specular(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	_mainWindow->script_Properties_specular(script_curObj, val);
}

void ScriptHandler::Properties_diffuse(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	_mainWindow->script_Properties_diffuse(script_curObj, val);
}

void ScriptHandler::Properties_ambient(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	_mainWindow->script_Properties_ambient(script_curObj, val);
}

void ScriptHandler::Properties_specularPower(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok,10);
	_mainWindow->script_Properties_specularPower(script_curObj, val);
}

void ScriptHandler::Properties_specularColor(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toInt(&ok);
		pos[1] = tep_list.at(1).toInt(&ok);
		pos[2] = tep_list.at(2).toInt(&ok);
		_mainWindow->script_Properties_specularColor(script_curObj, pos);
	}
	
}

void ScriptHandler::Properties_solidColor(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toInt(&ok);
		pos[1] = tep_list.at(1).toInt(&ok);
		pos[2] = tep_list.at(2).toInt(&ok);
		_mainWindow->script_Properties_solidColor(script_curObj, pos);
	}
	
}

void ScriptHandler::Properties_edgeColor(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toInt(&ok);
		pos[1] = tep_list.at(1).toInt(&ok);
		pos[2] = tep_list.at(2).toInt(&ok);
		_mainWindow->script_Properties_edgeColor(script_curObj, pos);
	}

}

void ScriptHandler::Properties_interpolation(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok);
	if (val != 0)
		val = 1;
	_mainWindow->script_Properties_interpolation(script_curObj, (EInterpolationType)val);
}

void ScriptHandler::Properties_visible(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	bool val = false;
	if (args.toInt(&ok, 10) != 0)
		val = true;
	_mainWindow->script_Properties_visible(script_curObj, val);
}

void ScriptHandler::Properties_Flag_scalarBars(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	bool val=false;
	if (args.toInt(&ok,10)!=0)
		val = true;
	_mainWindow->script_Properties_Flag_scalarBar(script_curObj, val);
}
void ScriptHandler::Properties_EnableOpacityMap(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	bool val = false;
	if (args.toInt(&ok, 10) != 0)
		val = true;
	_mainWindow->script_Properties_EnableOpacityMap(script_curObj, val);
}
void ScriptHandler::Properties_LookupTableMap(QString val)
{
	if (script_curObj == NULL)
		return;
	QStringList valList = val.split(",");
	if (valList.count() % 4)
		return;
	_mainWindow->script_Properties_LookupTableMap(script_curObj, val);
}
void ScriptHandler::Properties_LookupOpacityMap(QString val)
{
	if (script_curObj == NULL)
		return;
	QStringList valList = val.split(",");
	if (valList.count() % 2)
		return;
	_mainWindow->script_Properties_LookupOpacityMap(script_curObj, val);
}
void ScriptHandler::Properties_show_scalarBars()
{
	if (script_curObj == NULL)
		return;
	_mainWindow->script_Properties_show_scalarBars(script_curObj);
}
void ScriptHandler::Properties_show_cubeAxes()
{
	if (script_curObj == NULL)
		return;
	_mainWindow->script_Properties_show_cubeAxes(script_curObj);
}

void ScriptHandler::Properties_scalarBarPosition(QString args)
{
	if (script_dataObj == NULL)
		return;
	QStringList valList = args.split(",");
	if (valList.count() != 6)
		return;
	double pos[4];
	QString colName = valList.at(0).trimmed();
	bool ok;
	int oritetation = valList.at(1).toInt(&ok);
	for (int i = 0; i < 4;i++)
		pos[i] = valList.at(2+i).toDouble(&ok);
	_mainWindow->script_Properties_scalarBarPosition(script_dataObj, colName,oritetation,pos);
}

//properties vector
void ScriptHandler::Properties_vector_GlyphVector(QString args)
{
	if (script_curObj == NULL)
		return;
	_mainWindow->script_Properties_vector_GlyphVector(script_curObj, args);
}

void ScriptHandler::Properties_vector_scalar(QString args)
{
	if (script_curObj == NULL)
		return;
	_mainWindow->script_Properties_vector_scalar(script_curObj, args);
}

void ScriptHandler::Properties_vector_normal(QString args)
{
	if (script_curObj == NULL)
		return;
	_mainWindow->script_Properties_vector_normal(script_curObj, args);
}

void ScriptHandler::Properties_vector_numPoints(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok, 10);
	_mainWindow->script_Properties_vector_numPoints(script_curObj, val);
}

void ScriptHandler::Properties_vector_glyph_type(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok, 10);
	if (val != 0)
		val = 0;
	_mainWindow->script_Properties_vector_glyph_type(script_curObj, val);
}

void ScriptHandler::Properties_vector_glyph_tipRes(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok, 10);
	if ((val <1) || (val>128))
		val = 6;
	_mainWindow->script_Properties_vector_glyph_tipRes(script_curObj, val);
}

void ScriptHandler::Properties_vector_glyph_tipRad(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if ((val <= 0.0) || (val>100.0))
		val = 0.1;
	_mainWindow->script_Properties_vector_glyph_tipRad(script_curObj, val);
}

void ScriptHandler::Properties_vector_glyph_tipLen(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if ((val <= 0.0) || (val>100.0))
		val = 0.1;
	_mainWindow->script_Properties_vector_glyph_tipLen(script_curObj, val);
}

void ScriptHandler::Properties_vector_glyph_shaftRes(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok, 10);
	if ((val <= 0) || (val>128))
		val = 6;
	_mainWindow->script_Properties_vector_glyph_shaftRes(script_curObj, val);
}

void ScriptHandler::Properties_vector_glyph_shaftRad(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if ((val <= 0.0) || (val>100.0))
		val = 0.1;
	_mainWindow->script_Properties_vector_glyph_shaftRad(script_curObj, val);
}

//streamline
void ScriptHandler::Properties_streamline_vector(QString args)
{
	if (script_curObj == NULL)
		return;
	_mainWindow->script_Properties_streamline_vector(script_curObj, args);
}

void ScriptHandler::Properties_streamline_integration_direction(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok);
	_mainWindow->script_Properties_streamline_integration_direction(script_curObj, (IntegrationDirection)val);
}

void ScriptHandler::Properties_streamline_integration_type(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok);
	if ((val <0) || (val>2))
		val = 0;
	_mainWindow->script_Properties_streamline_integration_type(script_curObj, (IntegratorType)val);
}

void ScriptHandler::Properties_streamline_integration_stepUnit(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok);
	if (val < 1)
		val = 1;
	else if (val>2)
		val = 2;
	_mainWindow->script_Properties_streamline_integration_stepUnit(script_curObj, (IntegrationStepUnit)val);
}

void ScriptHandler::Properties_streamline_integration_initStepLen(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if (val <= 0)
		val = 0;
	_mainWindow->script_Properties_streamline_integration_initStepLen(script_curObj, val);
}

void ScriptHandler::Properties_streamline_integration_miniStepLen(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if (val <= 0)
		val = 0;
	_mainWindow->script_Properties_streamline_integration_miniStepLen(script_curObj, val);
}

void ScriptHandler::Properties_streamline_integration_maxiStepLen(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if (val <= 0)
		val = 0;
	_mainWindow->script_Properties_streamline_integration_maxiStepLen(script_curObj, val);
}

void ScriptHandler::Properties_streamline_stream_maxiSteps(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if (val <= 0)
		val = 0;
	_mainWindow->script_Properties_streamline_stream_maxiSteps(script_curObj, val);
}

void ScriptHandler::Properties_streamline_stream_maxiStreamLen(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if (val <= 0)
		val = 0;
	_mainWindow->script_Properties_streamline_stream_maxiStreamLen(script_curObj, val);
}

void ScriptHandler::Properties_streamline_stream_terminalSpeed(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if (val <= 0)
		val = 0;
	_mainWindow->script_Properties_streamline_stream_terminalSpeed(script_curObj, val);
}

void ScriptHandler::Properties_streamline_stream_maxiError(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if (val <=0)
		val = 0;
	_mainWindow->script_Properties_streamline_stream_maxiError(script_curObj, val);
}

void ScriptHandler::Properties_streamline_seeds_type(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok);
	if (val !=0)
		val = 1;
	_mainWindow->script_Properties_streamline_seeds_type(script_curObj, (SpeedType)val);
}

void ScriptHandler::Properties_streamline_seeds_mPoint(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toDouble(&ok);
		pos[1] = tep_list.at(1).toDouble(&ok);
		pos[2] = tep_list.at(2).toDouble(&ok);
		_mainWindow->script_Properties_streamline_seeds_mPoint(script_curObj, pos);
	}
}

void ScriptHandler::Properties_streamline_seeds_num_points(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok);
	if (val <= 0)
		val = 0;
	_mainWindow->script_Properties_streamline_seeds_num_points(script_curObj, val);
}

void ScriptHandler::Properties_streamline_seeds_radius(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if (val <0)
		val = 0;
	_mainWindow->script_Properties_streamline_seeds_radius(script_curObj, val);
}

void ScriptHandler::Properties_streamline_vorticity(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok; bool flag = false;
	int val = args.toInt(&ok);
	if (val != 0)
		flag = true;
	_mainWindow->script_Properties_streamline_vorticity(script_curObj, flag);
}

void ScriptHandler::Properties_streamline_interpolatorType(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok);
	if (val != 0)
		val = 1;
	_mainWindow->script_Properties_streamline_interpolatorType(script_curObj, (InterpolatorType)val);
}

void ScriptHandler::Properties_streamline_surface_streamLines(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok; bool flag=false;
	int val = args.toInt(&ok);
	if (val !=0)
		flag = true;
	_mainWindow->script_Properties_streamline_surface_streamLines(script_curObj, flag);
}

//streamline
//contour properties
void ScriptHandler::Properties_contourColumn(QString args)
{
	if (script_curObj == NULL)
		return;
	_mainWindow->script_Properties_contourColumn(script_curObj, args);
}

void ScriptHandler::Properties_contourValue(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if (val <= 0.0)
		val = 6.0;
	_mainWindow->script_Properties_contourValue(script_curObj, val);
}
void ScriptHandler::Properties_contour_reflection(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok);
	if (val)
		val = 1;
	_mainWindow->script_Properties_contour_reflection(script_curObj, val);
}
void ScriptHandler::Properties_contour_reflectionAxes(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok);
	if ((val<0) || (val>8))
		val = 0;
	_mainWindow->script_Properties_contour_reflectionAxes(script_curObj, (ReflectionPlane)val);
}
//reflection properties
void ScriptHandler::Properties_reflectionAxes(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int tep=args.toInt(&ok, 10);
	if ((tep < 0) || (tep>8))
		tep = 0;
	ReflectionPlane val = (ReflectionPlane)tep;
	_mainWindow->script_Properties_reflectionAxes(script_curObj, val);
}
void ScriptHandler::Properties_reflectionCopyInput(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int tep = args.toInt(&ok, 10);
	if (tep!=0)
		tep = 1;
	
	_mainWindow->script_Properties_reflectionCopyInput(script_curObj, tep);
}
void ScriptHandler::Properties_calculatorFormula(QString args)
{
	if (script_curObj == NULL)
		return;
	_mainWindow->script_Properties_calculatorFormula(script_curObj, args);
}
void ScriptHandler::Properties_calculatorResultName(QString args)
{
	if (script_curObj == NULL)
		return;
	_mainWindow->script_Properties_calculatorResultName(script_curObj, args);
}
//smooth properties
void ScriptHandler::Properties_smooth(QString args)//PipelineObject* tep_piplnObj, int type, double coef
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int tep_smoothType = 0;double coef;
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 2)
	{
		if (tep_list.at(0).toInt(&ok) == 0)
			tep_smoothType = 0;
		else
			tep_smoothType = 1;
		coef=tep_list.at(1).toDouble(&ok);
		_mainWindow->script_Properties_smooth(script_curObj, tep_smoothType, coef);
	}
	
}

//mooringLine properties
void ScriptHandler::Properties_mooringLine_displaySeaBottom(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok, 10);
	if (val != 0)
		val = 0;
	_mainWindow->script_Properties_mooringLine_displaySeaBottom(script_curObj, val);
}
void ScriptHandler::Properties_mooringLine_colorSeaBottom(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toInt(&ok);
		pos[1] = tep_list.at(1).toInt(&ok);
		pos[2] = tep_list.at(2).toInt(&ok);
		_mainWindow->script_Properties_mooringLine_colorSeaBottom(script_curObj, pos);
	}
}
void ScriptHandler::Properties_aniDisplace_fileName(QString args)
{
	if (script_curObj == NULL)
		return;
	_mainWindow->script_Properties_aniDisplace_fileName(script_curObj, args);
}
void ScriptHandler::Properties_aniDisplace_startPosition(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok, 10);
	if (val != 0)
		val = 0;
	_mainWindow->script_Properties_aniDisplace_startPosition(script_curObj, val);
}
void ScriptHandler::Properties_aniDisplace_interval(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok, 10);
	if (val != 0)
		val = 0;
	_mainWindow->script_Properties_aniDisplace_interval(script_curObj, val);
}
void ScriptHandler::Properties_aniDisplace_zoomFactor(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	_mainWindow->script_Properties_aniDisplace_zoomFactor(script_curObj, val);
}
void ScriptHandler::Properties_aniDisplace_gravityCenter(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toDouble();
		pos[1] = tep_list.at(1).toDouble();
		pos[2] = tep_list.at(2).toDouble();
		_mainWindow->script_Properties_aniDisplace_gravityCenter(script_curObj, pos);
	}
}
void ScriptHandler::Properties_mooringLine_type(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok, 10);
	if (val != 0)
		val = 0;
	_mainWindow->script_Properties_mooringLine_type(script_curObj, val);
}

void ScriptHandler::Properties_mooringLine_tipRes(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok, 10);
	if ((val <1) || (val>128))
		val = 6;
	_mainWindow->script_Properties_mooringLine_tipRes(script_curObj, val);
}

void ScriptHandler::Properties_mooringLine_tipRad(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if ((val <= 0.0) || (val>100.0))
		val = 0.1;
	_mainWindow->script_Properties_mooringLine_tipRad(script_curObj, val);
}

void ScriptHandler::Properties_mooringLine_tipLen(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if ((val <= 0.0) || (val>100.0))
		val = 0.1;
	_mainWindow->script_Properties_mooringLine_tipLen(script_curObj, val);
}

void ScriptHandler::Properties_mooringLine_shaftRes(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok, 10);
	if ((val <= 0) || (val>128))
		val = 6;
	_mainWindow->script_Properties_mooringLine_shaftRes(script_curObj, val);
}

void ScriptHandler::Properties_mooringLine_shaftRad(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	double val = args.toDouble(&ok);
	if ((val <= 0.0) || (val>100.0))
		val = 0.1;
	_mainWindow->script_Properties_mooringLine_shaftRad(script_curObj, val);
}

void ScriptHandler::Properties_view_backgroundType(QString args)
{
	
	bool ok;
	int val = args.toInt(&ok, 10);
	if (val != 0)
		val = 1;
	_mainWindow->script_Properties_view_backgroundType(val);
}

void ScriptHandler::Properties_view_backgroundColor(QString args)
{
	
	bool ok;
	int pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toInt(&ok);
		pos[1] = tep_list.at(1).toInt(&ok);
		pos[2] = tep_list.at(2).toInt(&ok);
		_mainWindow->script_Properties_view_backgroundColor(QColor(pos[0], pos[1], pos[2]));
	}
	
}

void ScriptHandler::Properties_view_background2Color(QString args)
{
	bool ok;
	int pos[3];
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 3)
	{
		pos[0] = tep_list.at(0).toInt(&ok);
		pos[1] = tep_list.at(1).toInt(&ok);
		pos[2] = tep_list.at(2).toInt(&ok);
		_mainWindow->script_Properties_view_background2Color(QColor(pos[0], pos[1], pos[2]));
	}
	
}

void ScriptHandler::Properties_view_axesVisible(QString args)
{
	bool ok;
	bool val = false;
	if (args.toInt(&ok, 10) != 0)
		val = true;
	_mainWindow->script_Properties_view_axesVisible(val);
}

void ScriptHandler::Properties_view_remarkVisible(QString args)
{
	bool ok;
	bool val = false;
	if (args.toInt(&ok, 10) != 0)
		val = true;
	_mainWindow->script_Properties_view_remarkVisible(val);
}

void ScriptHandler::Properties_view_remarkFontColor(QString args)
{
	bool ok;
	QStringList tep_list = args.split(",", QString::SkipEmptyParts);
	if (tep_list.count() != 3)
		return;
	_mainWindow->script_Properties_view_remarkFontColor(QColor(tep_list.at(0).toInt(&ok, 10), 
		tep_list.at(1).toInt(&ok, 10), tep_list.at(2).toInt(&ok, 10)));
}
void ScriptHandler::Properties_view_remarkText(QString args)
{
	_mainWindow->script_Properties_view_remarkText(args);
}
void ScriptHandler::Properties_view_remarkPosition1(QString args)
{
	QStringList tep_list = args.split(",", QString::SkipEmptyParts);
	if (tep_list.count() != 2)
		return;
	bool ok;
	double pos[2];
	pos[0] = tep_list.at(0).toDouble(&ok);
	pos[1] = tep_list.at(1).toDouble(&ok);
	_mainWindow->script_Properties_view_remarkPosition1(pos);
}
void ScriptHandler::Properties_view_remarkPosition2(QString args)
{
	QStringList tep_list = args.split(",", QString::SkipEmptyParts);
	if (tep_list.count() != 2)
		return;
	bool ok;
	double pos[2];
	pos[0] = tep_list.at(0).toDouble(&ok);
	pos[1] = tep_list.at(1).toDouble(&ok);
	_mainWindow->script_Properties_view_remarkPosition2(pos);
}
void ScriptHandler::Properties_view_cameraParallel(QString args)
{
	bool ok;
	bool val = false;
	if (args.toInt(&ok, 10) != 0)
		val = true;
	_mainWindow->script_Properties_view_cameraParallel(val);
}

void ScriptHandler::Properties_view_interaction(QString args)
{
	bool ok;
	bool val = false;
	if (args.toInt(&ok, 10) != 0)
		val = true;
	_mainWindow->script_Properties_view_interaction(val);
}

void ScriptHandler::Properties_renderView()
{
	_mainWindow->script_Properties_renderView();
}

void ScriptHandler::Properties_viewDirection(QString args)
{
	_mainWindow->script_setViewDirection(args);
}

void ScriptHandler::Properties_resetView()
{
	_mainWindow->script_resetView();
}
void ScriptHandler::Camera_SelfAdaptation()
{
	_mainWindow->script_Camera_SelfAdaptation(true);
}
void ScriptHandler::Save_Image(QString args)
{
	bool ok;
	bool flag_dlg; int wid; int hei; QString fileName;
	QStringList tep_list = args.split(",");
	if (tep_list.count() == 4)
	{
		if (tep_list.at(0).trimmed().toLower() == "true")
			flag_dlg = true;
		else
			flag_dlg = false;
		wid = tep_list.at(1).trimmed().toInt(&ok);
		hei = tep_list.at(2).trimmed().toInt(&ok);
		fileName = tep_list.at(3).trimmed();
		_mainWindow->script_saveImage(flag_dlg, wid, hei, fileName);
	}	
}
void ScriptHandler::clearAllPipelineObjs(QString args)
{
	bool ok;
	int delayTime = args.toInt(&ok);
	emit sig_script_clearAllPipelineObjs(delayTime);
	/*if (delayTime>0)
		QThread::msleep(delayTime);
	_mainWindow->script_clearAllPipelineObjs();*/
}
void ScriptHandler::Exit_SoftWare(QString args)
{
	bool ok;
	int delayTime = args.toInt(&ok);
	if (delayTime>0)
		_mainWindow->script_setExitSoftWare(delayTime);
	/*if (delayTime>0)
		QThread::msleep(delayTime);
	_mainWindow->script_exitSoftWare();*/
}
void ScriptHandler::Set_PlotSize(QString args)
{
	if (flag_commandLine)
		return;
	QStringList tep_list = args.split(",", QString::SkipEmptyParts);
	if (tep_list.count() != 10)
		return;
	bool ok;
	int val[10];
	for (int i = 0; i < 10; i++)
		val[i] = tep_list.at(i).toInt(&ok, 10);
	//emit sig_script_setPlotSize(val);
	_mainWindow->script_setPlotSize(val);
}
void ScriptHandler::ReturnToOldSize()
{
	//_mainWindow->script_ReturnToOldSize();
}
void ScriptHandler::Save_Animate(QString args)
{
	_mainWindow->script_Save_Animate(args);
}

void ScriptHandler::Pick_Point(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	bool val = false;
	if (args.toInt(&ok, 10) != 0)
		val = true;
	_mainWindow->script_draw_PickPoint(script_curObj,(vtkIdType)val);
}

void ScriptHandler::Pick_Cell(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	bool val = false;
	if (args.toInt(&ok, 10) != 0)
		val = true;
	_mainWindow->script_draw_PickCell(script_curObj,(vtkIdType)val);
}

void ScriptHandler::Pick_PointCurve(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	bool val = false;
	if (args.toInt(&ok, 10) != 0)
		val = true;
	_mainWindow->script_draw_PickPointCurve(script_curObj, (vtkIdType)val);
}

void ScriptHandler::Pick_CellCurve(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	bool val = false;
	if (args.toInt(&ok, 10) != 0)
		val = true;
	_mainWindow->script_draw_PickCellCurve(script_curObj, (vtkIdType)val);
}
void ScriptHandler::Properties_streamline_reflection(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok);
	if (val)
		val = 1;
	_mainWindow->script_Properties_streamline_reflection(script_curObj, val);
}
void ScriptHandler::Properties_streamline_reflectionAxes(QString args)
{
	if (script_curObj == NULL)
		return;
	bool ok;
	int val = args.toInt(&ok);
	if ((val<0) || (val>8))
		val = 0;
	_mainWindow->script_Properties_streamline_reflectionAxes(script_curObj, (ReflectionPlane)val);
}