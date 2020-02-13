#ifndef _SIGNALHANDLERPY_H_
#define _SIGNALHANDLERPY_H_

#include <QObject>
#include "graph_mainwindow.h"
class QTreeWidgetItem;
class vtkDataSet;

namespace Ui {
	class SignalHandlerPy;
}

	class graph_MainWindow;

	class SignalHandlerPy : public QObject
	{
		Q_OBJECT
	public:
		SignalHandlerPy(graph_MainWindow* mainwindow);
		~SignalHandlerPy();
	public slots:
		/*切换为英语 */
		void on_actionEnglish();
		/*切换为中文 */
		void on_actionChinese();
		
		void script_openFile(QString args);
		void script_applyClicked();
		void script_Properties_Opacity(PipelineObject *tep_piplnObj);
		void script_Properties_colorColumn(PipelineObject* tep_piplnObj);
		void script_Properties_colorColumn_component(PipelineObject* tep_piplnObj);
		void script_Properties_scalarBarTitle(PipelineObject* tep_piplnObj, QString colName, QString m_title);
		void script_Properties_scalarBarFontSize(PipelineObject* tep_piplnObj, QString colName, int m_fontSize);
		void script_Properties_scalarBarNumLables(PipelineObject* tep_piplnObj, QString colName, int m_numLables);
		void script_Properties_lineWidth(PipelineObject* tep_piplnObj);
		void script_Properties_pointSize(PipelineObject* tep_piplnObj);
		void script_Properties_translate(PipelineObject* tep_piplnObj);
		void script_Properties_origin(PipelineObject* tep_piplnObj);
		void script_Properties_scale(PipelineObject* tep_piplnObj);
		void script_Properties_orientation(PipelineObject* tep_piplnObj);
		void script_Properties_representation(PipelineObject* tep_piplnObj);
		void script_Properties_specular(PipelineObject* tep_piplnObj);
		void script_Properties_diffuse(PipelineObject* tep_piplnObj);
		void script_Properties_ambient(PipelineObject* tep_piplnObj);
		void script_Properties_specularPower(PipelineObject* tep_piplnObj);
		void script_Properties_specularColor(PipelineObject* tep_piplnObj);
		void script_Properties_solidColor(PipelineObject* tep_piplnObj);
		void script_Properties_edgeColor(PipelineObject* tep_piplnObj);
		void script_Properties_interpolation(PipelineObject* tep_piplnObj);
		void script_Properties_Flag_scalarBar(PipelineObject* tep_piplnObj);
		void script_Properties_EnableOpacityMap(PipelineObject* tep_piplnObj);
		void script_Properties_visible(PipelineObject* tep_piplnObj);
		void script_Properties_show_scalarBars(PipelineObject* tep_piplnObj);
		void script_Properties_show_cubeAxes(PipelineObject* tep_piplnObj);

		void script_Properties_scalarBarPosition(PipelineObject* tep_piplnObj, QString colName, int tep_orietation,double pos[4]);

		void script_FilterClip(PipelineObject* tep_piplnObj);
		void script_FilterSlice(PipelineObject* tep_piplnObj);
		void script_FilterContour(PipelineObject* tep_piplnObj);
		void script_FilterVector(PipelineObject* tep_piplnObj);
		void script_FilterReflection(PipelineObject* tep_piplnObj);
		void script_FilterCalculator(PipelineObject* tep_piplnObj);
		void script_FilterSmooth(PipelineObject* tep_piplnObj);
		void script_FilterStreamLine(PipelineObject* tep_piplnObj);

		void script_Properties_vector_GlyphVector(PipelineObject* tep_piplnObj, QString val);
		void script_Properties_vector_scalar(PipelineObject* tep_piplnObj, QString val);
		void script_Properties_vector_normal(PipelineObject* tep_piplnObj, QString val);
		void script_Properties_vector_numPoints(PipelineObject* tep_piplnObj, int val);
		void script_Properties_vector_glyph_type(PipelineObject* tep_piplnObj, int val);
		void script_Properties_vector_glyph_tipRes(PipelineObject* tep_piplnObj, int val);
		void script_Properties_vector_glyph_tipRad(PipelineObject* tep_piplnObj, double val);
		void script_Properties_vector_glyph_tipLen(PipelineObject* tep_piplnObj, double val);
		void script_Properties_vector_glyph_shaftRes(PipelineObject* tep_piplnObj, int val);
		void script_Properties_vector_glyph_shaftRad(PipelineObject* tep_piplnObj, double val);

		void script_Properties_view_backgroundType(int val);
		void script_Properties_view_backgroundColor(QColor color_val);
		void script_Properties_view_background2Color(QColor color_val);
		void script_Properties_view_axesVisible(bool val);
		void script_Properties_view_remarkVisible(bool val);
		void script_Properties_view_remarkFontColor(QColor color_val);
		void script_Properties_view_remarkText(QString text);
		void script_Properties_view_remarkPosition1(double *pos);
		void script_Properties_view_remarkPosition2(double *pos);
		void script_Properties_view_cameraParallel(bool val);
		void script_Properties_view_interaction(bool val);
		void script_Properties_renderView();

		void script_Camera_Position(double pos[3]);
		void script_Camera_FocalPoint(double focalPoint[3]);
		void script_Camera_ClippingRange(double clippingRange[2]);
		void script_Camera_ViewUp(double viewup[3]);
		void script_Camera_ViewAngle(double angle);
		void script_Camera_ParalellScale(double scale);
		void script_Camera_Zoom(double zoom);
		void script_Camera_Reset();

		void script_Properties_planeOrigin(PipelineObject* tep_piplnObj);
		void script_Properties_planeNormal(PipelineObject* tep_piplnObj);
		void script_Properties_planeVisible(PipelineObject* tep_piplnObj);
		void script_Properties_insideOut(PipelineObject* tep_piplnObj);

		void script_Properties_contourColumn(PipelineObject* tep_piplnObj, QString val);
		void script_Properties_contourValue(PipelineObject* tep_piplnObj, double val);
		void script_Properties_contour_reflection(PipelineObject* tep_piplnObj, bool val);
		void script_Properties_contour_reflectionAxes(PipelineObject* tep_piplnObj, ReflectionPlane val);

		void script_Properties_reflectionAxes(PipelineObject* tep_piplnObj);
		void script_Properties_reflectionCopyInput(PipelineObject* tep_piplnObj);
		
		void script_Properties_smooth(PipelineObject* tep_piplnObj, int type, double coef);

		void script_Properties_calculatorFormula(PipelineObject* tep_piplnObj);
		void script_Properties_calculatorResultName(PipelineObject* tep_piplnObj);

		void script_Properties_streamline_vector(PipelineObject* tep_piplnObj, QString val);
		void script_Properties_streamline_integration_direction(PipelineObject* tep_piplnObj, IntegrationDirection val);
		void script_Properties_streamline_integration_type(PipelineObject* tep_piplnObj, IntegratorType val);
		void script_Properties_streamline_integration_stepUnit(PipelineObject* tep_piplnObj, IntegrationStepUnit val);
		void script_Properties_streamline_integration_initStepLen(PipelineObject* tep_piplnObj, double val);
		void script_Properties_streamline_integration_miniStepLen(PipelineObject* tep_piplnObj, double val);
		void script_Properties_streamline_integration_maxiStepLen(PipelineObject* tep_piplnObj, double val);
		void script_Properties_streamline_stream_maxiSteps(PipelineObject* tep_piplnObj, int val);
		void script_Properties_streamline_stream_maxiStreamLen(PipelineObject* tep_piplnObj, double val);
		void script_Properties_streamline_stream_terminalSpeed(PipelineObject* tep_piplnObj, double val);
		void script_Properties_streamline_stream_maxiError(PipelineObject* tep_piplnObj, double val);
		void script_Properties_streamline_seeds_type(PipelineObject* tep_piplnObj, SpeedType val);
		void script_Properties_streamline_seeds_mPoint(PipelineObject* tep_piplnObj, double* val);
		void script_Properties_streamline_seeds_num_points(PipelineObject* tep_piplnObj, int val);
		void script_Properties_streamline_seeds_radius(PipelineObject* tep_piplnObj, double val);
		void script_Properties_streamline_vorticity(PipelineObject* tep_piplnObj, bool val);
		void script_Properties_streamline_interpolatorType(PipelineObject* tep_piplnObj, InterpolatorType val);
		void script_Properties_streamline_surface_streamLines(PipelineObject* tep_piplnObj, bool val);
		void script_Properties_streamline_reflection(PipelineObject* tep_piplnObj, bool val);
		void script_Properties_streamline_reflectionAxes(PipelineObject* tep_piplnObj, ReflectionPlane val);

		void script_resetView();
		void script_view_XPlus();
		void script_view_YPlus();
		void script_view_ZPlus();
		void script_view_XMinus();
		void script_view_YMinus();
		void script_view_ZMinus();

		void script_exitSoftWare();
		void script_setExitSoftWare(int ms);
		void script_setPlotSize(int* val);
	private:
		graph_MainWindow* _mainWindow{};

	};


#endif
