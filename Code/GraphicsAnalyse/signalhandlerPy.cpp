#include "signalhandlerPy.h"
#include "ui_graph_mainwindow.h"
#include "graph_mainwindow.h"
#include <QTreeWidgetItem>
#include <QFileDialog>
#include <QDir>
#include <QProcess>
#include <QMessageBox>
#include <QDebug>
#include <assert.h>
#include <QFileDialog>
#include "qobject.h"
#include "signalhandlerPy.h"
#include <QMessageBox>
#include "pipelineobjpropwidget.h"

	SignalHandlerPy::SignalHandlerPy(graph_MainWindow* mainwindow) : _mainWindow(mainwindow)
	{
		/*void script_Properties_Flag_scalarBar(PipelineObject* tep_piplnObj, bool val);
	void script_Properties_visible(PipelineObject* tep_piplnObj, bool val);*/
		//connect(mainwindow->getUi()->actionEnglish, SIGNAL(triggered()), this, SLOT(on_actionEnglish()));
		//connect(mainwindow->getUi()->actionChinese, SIGNAL(triggered()), this, SLOT(on_actionChinese()));
		connect(mainwindow, SIGNAL(sig_python_openFile(QString)), this, SLOT(script_openFile(QString)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_applyClicked()), this, SLOT(script_applyClicked()));
		//display property
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_opacity(PipelineObject *)), this, SLOT(script_Properties_Opacity(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_color(PipelineObject *)), this, SLOT(script_Properties_colorColumn(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_color_component(PipelineObject *)), this, SLOT(script_Properties_colorColumn_component(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_pointSize(PipelineObject *)), this, SLOT(script_Properties_pointSize(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_lineWidth(PipelineObject *)), this, SLOT(script_Properties_lineWidth(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_translate(PipelineObject *)), this, SLOT(script_Properties_translate(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_scale(PipelineObject *)), this, SLOT(script_Properties_scale(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_origin(PipelineObject *)), this, SLOT(script_Properties_origin(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_orientation(PipelineObject *)), this, SLOT(script_Properties_orientation(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_representation(PipelineObject *)), this, SLOT(script_Properties_representation(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_specular(PipelineObject *)), this, SLOT(script_Properties_specular(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_diffuse(PipelineObject *)), this, SLOT(script_Properties_diffuse(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_ambient(PipelineObject *)), this, SLOT(script_Properties_ambient(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_specularPower(PipelineObject *)), this, SLOT(script_Properties_specularPower(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_specularColor(PipelineObject *)), this, SLOT(script_Properties_specularColor(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_solidColor(PipelineObject *)), this, SLOT(script_Properties_solidColor(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_interpolation(PipelineObject *)), this, SLOT(script_Properties_interpolation(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_scalarBar(PipelineObject *)), this, SLOT(script_Properties_show_scalarBars(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_edgeColor(PipelineObject *)), this, SLOT(script_Properties_edgeColor(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_cubeAxes(PipelineObject *)), this, SLOT(script_Properties_show_cubeAxes(PipelineObject *)));
		
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_vector_GlyphVector(PipelineObject *, QString)), this, SLOT(script_Properties_vector_GlyphVector(PipelineObject *, QString)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_vector_scalar(PipelineObject *, QString)), this, SLOT(script_Properties_vector_scalar(PipelineObject *, QString)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_vector_normal(PipelineObject *, QString)), this, SLOT(script_Properties_vector_normal(PipelineObject *, QString)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_vector_numPoints(PipelineObject *, int)), this, SLOT(script_Properties_vector_numPoints(PipelineObject *, int)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_vector_glyph_type(PipelineObject *, int)), this, SLOT(script_Properties_vector_glyph_type(PipelineObject *, int)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_vector_glyph_tipRes(PipelineObject *, int)), this, SLOT(script_Properties_vector_glyph_tipRes(PipelineObject *, int)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_vector_glyph_tipRad(PipelineObject *, double)), this, SLOT(script_Properties_vector_glyph_tipRad(PipelineObject *, double)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_vector_glyph_tipLen(PipelineObject *, double)), this, SLOT(script_Properties_vector_glyph_tipLen(PipelineObject *, double)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_vector_glyph_shaftRes(PipelineObject *, int)), this, SLOT(script_Properties_vector_glyph_shaftRes(PipelineObject *, int)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_vector_glyph_shaftRad(PipelineObject *, double)), this, SLOT(script_Properties_vector_glyph_shaftRad(PipelineObject *, double)));

		connect(mainwindow, SIGNAL(sig_python_FilterSlice(PipelineObject *)), this, SLOT(script_FilterSlice(PipelineObject *)));
		connect(mainwindow, SIGNAL(sig_python_FilterClip(PipelineObject *)), this, SLOT(script_FilterClip(PipelineObject *)));
		connect(mainwindow, SIGNAL(sig_python_FilterContour(PipelineObject *)), this, SLOT(script_FilterContour(PipelineObject *)));
		connect(mainwindow, SIGNAL(sig_python_FilterVector(PipelineObject *)), this, SLOT(script_FilterVector(PipelineObject *)));
		connect(mainwindow, SIGNAL(sig_python_FilterReflection(PipelineObject *)), this, SLOT(script_FilterReflection(PipelineObject *)));
		connect(mainwindow, SIGNAL(sig_python_FilterCalculator(PipelineObject *)), this, SLOT(script_FilterCalculator(PipelineObject *)));
		connect(mainwindow, SIGNAL(sig_python_FilterSmooth(PipelineObject *)), this, SLOT(script_FilterSmooth(PipelineObject *)));
		connect(mainwindow, SIGNAL(sig_python_FilterStreamLine(PipelineObject *)), this, SLOT(script_FilterStreamLine(PipelineObject *)));

		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_view_backgroundType(int)), this, SLOT(script_Properties_view_backgroundType(int)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_view_backgroundColor(QColor)), this, SLOT(script_Properties_view_backgroundColor(QColor)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_view_background2Color(QColor)), this, SLOT(script_Properties_view_background2Color(QColor)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_view_axesVisible(bool)), this, SLOT(script_Properties_view_axesVisible(bool)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_view_remarkVisible(bool)), this, SLOT(script_Properties_view_remarkVisible(bool)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_view_remarkFontColor(QColor)), this, SLOT(script_Properties_view_remarkFontColor(QColor)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_view_remarkText(QString)), this, SLOT(script_Properties_view_remarkText(QString)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_view_cameraParallel(bool)), this, SLOT(script_Properties_view_cameraParallel(bool)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_view_interaction(bool)), this, SLOT(script_Properties_view_interaction(bool)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_renderView()), this, SLOT(script_Properties_renderView()));

		connect(mainwindow, SIGNAL(sig_python_Camera_Position(double*)), this, SLOT(script_Camera_Position(double*)));
		connect(mainwindow, SIGNAL(sig_python_Camera_FocalPoint(double*)), this, SLOT(script_Camera_FocalPoint(double*)));
		connect(mainwindow, SIGNAL(sig_python_Camera_ClippingRange(double*)), this, SLOT(script_Camera_ClippingRange(double*)));
		connect(mainwindow, SIGNAL(sig_python_Camera_ViewUp(double*)), this, SLOT(script_Camera_ViewUp(double*)));
		connect(mainwindow, SIGNAL(sig_python_Camera_ViewAngle(double)), this, SLOT(script_Camera_ViewAngle(double)));
		connect(mainwindow, SIGNAL(sig_python_Camera_ParalellScale(double)), this, SLOT(script_Camera_ParalellScale(double)));
		connect(mainwindow, SIGNAL(sig_python_Camera_Zoom(double)), this, SLOT(script_Camera_Zoom(double)));
		connect(mainwindow, SIGNAL(sig_python_Camera_Reset()), this, SLOT(script_Camera_Reset()));

		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_planeOrigin(PipelineObject *)), this, SLOT(script_Properties_planeOrigin(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_planeNormal(PipelineObject *)), this, SLOT(script_Properties_planeNormal(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_planeVis(PipelineObject *)), this, SLOT(script_Properties_planeVisible(PipelineObject *)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_insideOut(PipelineObject *)), this, SLOT(script_Properties_insideOut(PipelineObject *)));
		
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_contourColumn(PipelineObject *, QString)), this, SLOT(script_Properties_contourColumn(PipelineObject *, QString)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_contourValue(PipelineObject *, double)), this, SLOT(script_Properties_contourValue(PipelineObject *, double)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_contour_reflection(PipelineObject *, int)), this, SLOT(script_Properties_contour_reflection(PipelineObject *, int)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_contour_reflectionAxes(PipelineObject *, int)), this, SLOT(script_Properties_contour_reflectionAxes(PipelineObject *, int)));

		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_reflectionCopyInput(PipelineObject*)), this, SLOT(script_Properties_reflectionCopyInput(PipelineObject*)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_reflectionAxes(PipelineObject*)), this, SLOT(script_Properties_reflectionAxes(PipelineObject*)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_smooth(PipelineObject*, int, double)), this, SLOT(script_Properties_smooth(PipelineObject*, int, double)));

		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_vector(PipelineObject*, QString)), this, SLOT(script_Properties_streamline_vector(PipelineObject*, QString)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_integration_direction(PipelineObject*, IntegrationDirection)), this, SLOT(script_Properties_streamline_integration_direction(PipelineObject*, IntegrationDirection)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_integration_type(PipelineObject*, IntegratorType)), this, SLOT(script_Properties_streamline_integration_type(PipelineObject*, IntegratorType)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_integration_stepUnit(PipelineObject*, IntegrationStepUnit)), this, SLOT(script_Properties_streamline_integration_stepUnit(PipelineObject*, IntegrationStepUnit)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_integration_initStepLen(PipelineObject*, double)), this, SLOT(script_Properties_streamline_integration_initStepLen(PipelineObject*, double)));
		
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_integration_miniStepLen(PipelineObject*, double)), this, SLOT(script_Properties_streamline_integration_miniStepLen(PipelineObject*, double)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_integration_maxiStepLen(PipelineObject*, double)), this, SLOT(script_Properties_streamline_integration_maxiStepLen(PipelineObject*, double)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_stream_maxiSteps(PipelineObject*, int)), this, SLOT(script_Properties_streamline_stream_maxiSteps(PipelineObject*, int)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_stream_maxiStreamLen(PipelineObject*, double)), this, SLOT(script_Properties_streamline_stream_maxiStreamLen(PipelineObject*, double)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_stream_terminalSpeed(PipelineObject*, double)), this, SLOT(script_Properties_streamline_stream_terminalSpeed(PipelineObject*, double)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_stream_maxiError(PipelineObject*, double)), this, SLOT(script_Properties_streamline_stream_maxiError(PipelineObject*, double)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_seeds_type(PipelineObject*, SpeedType)), this, SLOT(script_Properties_streamline_seeds_type(PipelineObject*, SpeedType)));		
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_seeds_radius(PipelineObject*, double)), this, SLOT(script_Properties_streamline_seeds_radius(PipelineObject*, double)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_seeds_num_points(PipelineObject*, int)), this, SLOT(script_Properties_streamline_seeds_num_points(PipelineObject*, int)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_vorticity(PipelineObject*, bool)), this, SLOT(script_Properties_streamline_vorticity(PipelineObject*, bool)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_interpolatorType(PipelineObject*, InterpolatorType)), this, SLOT(script_Properties_streamline_interpolatorType(PipelineObject*, InterpolatorType)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_surface_streamLines(PipelineObject*, bool)), this, SLOT(script_Properties_streamline_surface_streamLines(PipelineObject*, bool)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_seeds_mPoint(PipelineObject *, double*)), this, SLOT(script_Properties_streamline_seeds_mPoint(PipelineObject *, double*)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_reflection(PipelineObject *, int)), this, SLOT(script_Properties_streamline_reflection(PipelineObject *, int)));
		connect(mainwindow->get_pipelineObj_propWidget()->get_propWidget(), SIGNAL(sig_python_streamline_reflectionAxes(PipelineObject *, int)), this, SLOT(script_Properties_streamline_reflectionAxes(PipelineObject *, int)));

		connect(mainwindow, SIGNAL(sig_python_scalarBar_title(PipelineObject*, QString, QString)), this, SLOT(script_Properties_scalarBarTitle(PipelineObject*, QString, QString)));
		connect(mainwindow, SIGNAL(sig_python_scalarBar_fontszie(PipelineObject*, QString, QString)), this, SLOT(script_Properties_scalarBarFontSize(PipelineObject*, QString, QString)));
		connect(mainwindow, SIGNAL(sig_python_scalarBar_numlables(PipelineObject*, QString, QString)), this, SLOT(script_Properties_scalarBarNumLables(PipelineObject*, QString, QString)));
		
		connect(mainwindow, SIGNAL(sig_python_resetView()), this, SLOT(script_resetView()));
		connect(mainwindow, SIGNAL(sig_python_view_XPlus()), this, SLOT(script_view_XPlus()));
		connect(mainwindow, SIGNAL(sig_python_view_YPlus()), this, SLOT(script_view_YPlus()));
		connect(mainwindow, SIGNAL(sig_python_view_ZPlus()), this, SLOT(script_view_ZPlus()));
		connect(mainwindow, SIGNAL(sig_python_view_XMinus()), this, SLOT(script_view_XMinus()));
		connect(mainwindow, SIGNAL(sig_python_view_YMinus()), this, SLOT(script_view_YMinus()));
		connect(mainwindow, SIGNAL(sig_python_view_ZMinus()), this, SLOT(script_view_ZMinus()));

		connect(mainwindow, SIGNAL(sig_python_exitSoftWare()), this, SLOT(script_exitSoftWare()));
		connect(mainwindow, SIGNAL(sig_python_setPlotSize(int*)), this, SLOT(script_setPlotSize(int*)));
		//vector property
		
	}
	SignalHandlerPy::~SignalHandlerPy()
	{
		/*clearData();
		ConfigOption::ConfigOption::getInstance()->clearAllConfig();*/
	}
	
	void SignalHandlerPy::on_actionEnglish()
	{
		//_mainWindow->ChangeLanguage("English");
	}
	void SignalHandlerPy::on_actionChinese()
	{
		//_mainWindow->ChangeLanguage("Chinese");
	}
	
	void SignalHandlerPy::script_openFile(QString args)
	{
		QString pycode = QString("PostProcess.script_openFile(\"%1\")").arg(args);
		qDebug() << pycode;
		emit _mainWindow->sig_execScript(pycode);
	}

	/*void SignalHandlerPy::script_applyClicked()
	{
		return _mainWindow->script_applyClicked();
	}*/
	void SignalHandlerPy::script_applyClicked()
	{
		QString pycode = QString("PostProcess.script_applyClicked()");
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
		//Py::PythonAagent::getInstance()->submit(pycode);
	}
	void SignalHandlerPy::script_Properties_Opacity(PipelineObject *tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_Opacity(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mOpacity);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_colorColumn(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_colorColumn(%1,\"%2\")").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_colorColumn_component(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_colorColumn_component(%1,\"%2\")").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle_XYZ);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_lineWidth(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_lineWidth(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mLineWidth);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_pointSize(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_pointSize(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mPointSize);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_translate(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_translate(%1,%2,%3,%4)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_transforming.x)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_transforming.y)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_transforming.z);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_origin(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_origin(%1,%2,%3,%4)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_origin.x)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_origin.y)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_origin.z);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_scale(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_scale(%1,%2,%3,%4)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_scale.x)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_scale.y)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_scale.z);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_orientation(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_orientation((%1,%2,%3,%4)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_orientation.x)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_orientation.y)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_struct_orientation.z);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_representation(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_representation(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_enum_representationtype);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_specular(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_specular(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSpecular);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_diffuse(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_diffuse(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mDiffuse);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_ambient(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_ambient(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mAmbient);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_specularPower(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_specularPower(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSpecularPower);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_specularColor(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_specularColor(%1,%2,%3,%4)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSpecularColor.r)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSpecularColor.g)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSpecularColor.b);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_solidColor(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_solidColor(%1,%2,%3,%4)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSolidColor.r)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSolidColor.g)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mSolidColor.b);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_edgeColor(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_edgeColor(%1,%2,%3,%4)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.r)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.g)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mEdgeColor.b);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_interpolation(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_interpolation(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.m_enum_interpolationtype);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_Flag_scalarBar(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_Flag_scalarBar(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap[tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle]);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_EnableOpacityMap(PipelineObject* tep_piplnObj)//20181205 aaaaaaaaaaa
	{
		PipelineObject* tep_obj = NULL;
		if (tep_piplnObj->GetObjectType() == dNumeric_DataSource)
			tep_obj = tep_piplnObj;
		else
			tep_obj = tep_piplnObj->GetObjParent();
		QString tep_colName = tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mColorColumnStyle;
		if (tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap.contains(tep_colName))
		{
			bool val = tep_obj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_opacityMap[tep_colName];
			QString pycode = QString("PostProcess.script_Properties_EnableOpacityMap(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
				.arg(val);
			emit _mainWindow->sig_execScript(pycode);
			qDebug(pycode.toStdString().data()); qDebug("\n");
		}
	}
	void SignalHandlerPy::script_Properties_visible(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_visible(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.flag_show_actors);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_show_scalarBars(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_show_scalarBars(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_show_cubeAxes(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_show_cubeAxes(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.flag_cubeAxes);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}

	void SignalHandlerPy::script_Properties_vector_GlyphVector(PipelineObject* tep_piplnObj, QString val)
	{
		QString pycode = QString("PostProcess.script_Properties_vector_GlyphVector(%1,\"%2\")").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_vector_scalar(PipelineObject* tep_piplnObj, QString val)
	{
		QString pycode = QString("PostProcess.script_Properties_vector_scalar(%1,\"%2\")").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_vector_normal(PipelineObject* tep_piplnObj, QString val)
	{
		QString pycode = QString("PostProcess.script_Properties_vector_normal(%1,\"%2\")").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_vector_numPoints(PipelineObject* tep_piplnObj, int val)
	{
		QString pycode = QString("PostProcess.script_Properties_vector_numPoints(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_vector_glyph_type(PipelineObject* tep_piplnObj, int val)
	{
		QString pycode = QString("PostProcess.script_Properties_vector_glyph_type(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_vector_glyph_tipRes(PipelineObject* tep_piplnObj, int val)
	{
		QString pycode = QString("PostProcess.script_Properties_vector_glyph_tipRes(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_vector_glyph_tipRad(PipelineObject* tep_piplnObj, double val)
	{
		QString pycode = QString("PostProcess.script_Properties_vector_glyph_tipRad(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_vector_glyph_tipLen(PipelineObject* tep_piplnObj, double val)
	{
		QString pycode = QString("PostProcess.script_Properties_vector_glyph_tipLen(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_vector_glyph_shaftRes(PipelineObject* tep_piplnObj, int val)
	{
		QString pycode = QString("PostProcess.script_Properties_vector_glyph_shaftRes(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_vector_glyph_shaftRad(PipelineObject* tep_piplnObj, double val)
	{
		QString pycode = QString("PostProcess.script_Properties_vector_glyph_shaftRad(%1,%2)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}

	void SignalHandlerPy::script_FilterClip(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_FilterClip(%1)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_FilterSlice(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_FilterSlice(%1)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_FilterContour(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_FilterContour(%1)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_FilterVector(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_FilterVector(%1)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_FilterReflection(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_FilterReflection(%1)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_FilterCalculator(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_FilterCalculator(%1)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_FilterSmooth(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_FilterSmooth(%1)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_FilterStreamLine(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_FilterStreamLine(%1)").arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}

	
	void SignalHandlerPy::script_Properties_view_backgroundType(int val)
	{
		QString pycode = QString("PostProcess.script_Properties_view_backgroundType(%1)").arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_view_backgroundColor(QColor color_val)
	{
		QString pycode = QString("PostProcess.script_Properties_view_backgroundColor(%1,%2,%3)").arg(color_val.red()).arg(color_val.green()).arg(color_val.blue());
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_view_background2Color(QColor color_val)
	{
		QString pycode = QString("PostProcess.script_Properties_view_background2Color(%1,%2,%3)").arg(color_val.red()).arg(color_val.green()).arg(color_val.blue());
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_view_axesVisible(bool val)
	{
		int a = 0;
		if (val)
			a = 1;
		QString pycode = QString("PostProcess.script_Properties_view_axesVisible(%1)").arg(a);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_view_remarkVisible(bool val)
	{
		int a = 0;
		if (val)
			a = 1;
		QString pycode = QString("PostProcess.script_Properties_view_remarkVisible(%1)").arg(a);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_view_remarkFontColor(QColor color_val)
	{
		QString pycode = QString("PostProcess.script_Properties_view_remarkFontColor(%1,%2,%3)")
			.arg(color_val.red()).arg(color_val.green()).arg(color_val.blue());
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_view_remarkText(QString text)
	{
		QString pycode = QString("PostProcess.script_Properties_view_remarkText(%1)").arg(text);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_view_remarkPosition1(double *pos)
	{
		QString pycode = QString("PostProcess.script_Properties_view_remarkPosition1(%1,%2)").arg(pos[0]).arg(pos[1]);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_view_remarkPosition2(double *pos)
	{
		QString pycode = QString("PostProcess.script_Properties_view_remarkPosition2(%1,%2)").arg(pos[0]).arg(pos[1]);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_view_cameraParallel(bool val)
	{
		int a = 0;
		if (val)
			a = 1;
		QString pycode = QString("PostProcess.script_Properties_view_cameraParallel(%1)").arg(a);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_view_interaction(bool val)
	{
		int a = 0;
		if (val)
			a = 1;
		QString pycode = QString("PostProcess.script_Properties_view_interaction(%1)").arg(a);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_renderView()
	{
		QString pycode = QString("PostProcess.script_Properties_renderView()");
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}

	void SignalHandlerPy::script_Camera_Position(double pos[3])
	{
		QString pycode = QString("PostProcess.script_Camera_Position(%1,%2,%3)").arg(pos[0]).arg(pos[1]).arg(pos[2]);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Camera_FocalPoint(double focalPoint[3])
	{
		QString pycode = QString("PostProcess.script_Camera_FocalPoint(%1,%2,%3)").arg(focalPoint[0]).arg(focalPoint[1]).arg(focalPoint[2]);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Camera_ClippingRange(double clippingRange[2])
	{
		QString pycode = QString("PostProcess.script_Camera_ClippingRange(%1,%2)").arg(clippingRange[0]).arg(clippingRange[1]);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Camera_ViewUp(double viewup[3])
	{
		QString pycode = QString("PostProcess.script_Camera_ViewUp(%1,%2,%3)").arg(viewup[0]).arg(viewup[1]).arg(viewup[2]);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Camera_ViewAngle(double angle)
	{
		QString pycode = QString("PostProcess.vscript_Camera_ViewAngle(%1)").arg(angle);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Camera_ParalellScale(double scale)
	{
		QString pycode = QString("PostProcess.script_Camera_ParalellScale(%1)").arg(scale);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Camera_Zoom(double zoom)
	{
		QString pycode = QString("PostProcess.script_Camera_Zoom(%1)").arg(zoom);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Camera_Reset()
	{
		QString pycode = QString("PostProcess.script_Camera_Reset(%1)");
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}

	void SignalHandlerPy::script_Properties_planeOrigin(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_planeOrigin(%1,%2,%3,%4)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.plane_propData.m_plane_origin.x)
			.arg(tep_piplnObj->mPipeLineObjProp.plane_propData.m_plane_origin.y)
			.arg(tep_piplnObj->mPipeLineObjProp.plane_propData.m_plane_origin.z);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_planeNormal(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_planeNormal(%1,%2,%3,%4)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.plane_propData.m_plane_normal.x)
			.arg(tep_piplnObj->mPipeLineObjProp.plane_propData.m_plane_normal.y)
			.arg(tep_piplnObj->mPipeLineObjProp.plane_propData.m_plane_normal.z);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_planeVisible(PipelineObject* tep_piplnObj)
	{
		int a = 0;
		if (tep_piplnObj->mPipeLineObjProp.plane_propData.flag_plane)
			a = 1;
		QString pycode = QString("PostProcess.script_Properties_planeVisible(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(a);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_insideOut(PipelineObject* tep_piplnObj)
	{
		int a = 0;
		if (tep_piplnObj->mPipeLineObjProp.plane_propData.flag_insideOut)
			a = 1;
		QString pycode = QString("PostProcess.script_Properties_insideOut(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(a);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}

	void SignalHandlerPy::script_Properties_contourColumn(PipelineObject* tep_piplnObj, QString val)
	{
		QString pycode = QString("PostProcess.script_Properties_contourColumn(%1,\"%2\")")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_contourValue(PipelineObject* tep_piplnObj, double val)
	{
		QString pycode = QString("PostProcess.script_Properties_contourValue(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_contour_reflection(PipelineObject* tep_piplnObj, bool val)
	{
		int aaa = 0;
		if (val)
			aaa = 1;
		QString pycode = QString("PostProcess.script_Properties_contour_reflection(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(aaa);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_calculatorFormula(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_calculatorFormula(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.calculatorFilter_propData.formula);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_calculatorResultName(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_calculatorResultName(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.calculatorFilter_propData.resultName);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_contour_reflectionAxes(PipelineObject* tep_piplnObj, ReflectionPlane val)
	{
		QString pycode = QString("PostProcess.script_Properties_contour_reflectionAxes(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg((int)val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}

	void SignalHandlerPy::script_Properties_reflectionAxes(PipelineObject* tep_piplnObj)
	{
		QString pycode = QString("PostProcess.script_Properties_reflectionAxes(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(tep_piplnObj->mPipeLineObjProp.reflectionFilter_propData.reflection_axis);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}

	void SignalHandlerPy::script_Properties_reflectionCopyInput(PipelineObject* tep_piplnObj)
	{
		int val = 0;
		if (tep_piplnObj->mPipeLineObjProp.reflectionFilter_propData.flag_copyInput)
			val = 1;
		QString pycode = QString("PostProcess.script_Properties_reflectionCopyInput(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id)
			.arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}

	void SignalHandlerPy::script_Properties_smooth(PipelineObject* tep_piplnObj, int type, double coef)
	{
		QString pycode = QString("PostProcess.script_Properties_smooth(%1,%2,%3)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(type).arg(coef);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}

	void SignalHandlerPy::script_Properties_streamline_vector(PipelineObject* tep_piplnObj, QString val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_vector(%1,\"%2\")")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_integration_direction(PipelineObject* tep_piplnObj, IntegrationDirection val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_integration_direction(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_integration_type(PipelineObject* tep_piplnObj, IntegratorType val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_integration_type(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_integration_stepUnit(PipelineObject* tep_piplnObj, IntegrationStepUnit val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_integration_stepUnit(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_integration_initStepLen(PipelineObject* tep_piplnObj, double val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_integration_initStepLen(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_integration_miniStepLen(PipelineObject* tep_piplnObj, double val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_integration_miniStepLen(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_integration_maxiStepLen(PipelineObject* tep_piplnObj, double val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_integration_maxiStepLen(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_stream_maxiSteps(PipelineObject* tep_piplnObj, int val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_stream_maxiSteps(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_stream_maxiStreamLen(PipelineObject* tep_piplnObj, double val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_stream_maxiStreamLen(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_stream_terminalSpeed(PipelineObject* tep_piplnObj, double val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_stream_terminalSpeed(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_stream_maxiError(PipelineObject* tep_piplnObj, double val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_stream_maxiError(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_seeds_type(PipelineObject* tep_piplnObj, SpeedType val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_seeds_type(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_seeds_mPoint(PipelineObject* tep_piplnObj, double* val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_seeds_mPoint(%1,%2,%3,%4)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val[0]).arg(val[1]).arg(val[2]);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_seeds_num_points(PipelineObject* tep_piplnObj, int val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_seeds_num_points(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_seeds_radius(PipelineObject* tep_piplnObj, double val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_seeds_radius(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_vorticity(PipelineObject* tep_piplnObj, bool val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_vorticity(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_interpolatorType(PipelineObject* tep_piplnObj, InterpolatorType val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_interpolatorType(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_surface_streamLines(PipelineObject* tep_piplnObj, bool val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_surface_streamLines(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}

	void SignalHandlerPy::script_Properties_scalarBarTitle(PipelineObject* tep_piplnObj, QString colName, QString m_title)
	{
		QString pycode = QString("PostProcess.script_Properties_scalarBarTitle(%1,\"%2\",\"%3\")")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(colName).arg(m_title);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}

	void SignalHandlerPy::script_Properties_scalarBarFontSize(PipelineObject* tep_piplnObj, QString colName, int m_fontSize)
	{
		QString pycode = QString("PostProcess.script_Properties_scalarBarFontSize(%1,\"%2\",%3)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(colName).arg(m_fontSize);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}

	void SignalHandlerPy::script_Properties_scalarBarNumLables(PipelineObject* tep_piplnObj, QString colName, int m_numLables)
	{
		QString pycode = QString("PostProcess.script_Properties_scalarBarNumLables(%1,\"%2\",%3)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(colName).arg(m_numLables);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}

	void SignalHandlerPy::script_resetView()
	{
		QString pycode = QString("PostProcess.script_resetView()");
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_view_XPlus()
	{
		QString pycode = QString("PostProcess.script_view_XPlus()");
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_view_YPlus()
	{
		QString pycode = QString("PostProcess.script_view_YPlus()");
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_view_ZPlus()
	{
		QString pycode = QString("PostProcess.script_view_ZPlus()");
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_view_XMinus()
	{
		QString pycode = QString("PostProcess.script_view_XMinus()");
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_view_YMinus()
	{
		QString pycode = QString("PostProcess.script_view_YMinus()");
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_view_ZMinus()
	{
		QString pycode = QString("PostProcess.script_view_ZMinus()");
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}

	void SignalHandlerPy::script_exitSoftWare()
	{
		QString pycode = QString("PostProcess.script_exitSoftWare()");
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_setExitSoftWare(int ms)
	{
		QString pycode = QString("PostProcess.script_setExitSoftWare(%1)").arg(ms);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_setPlotSize(int* val)
	{
		QString pycode = QString("PostProcess.script_setPlotSize(%1,%2,%3,%4,%5,%6,%7,%8)").arg(val[0]).arg(val[1])
			.arg(val[2]).arg(val[3])
			.arg(val[4]).arg(val[5])
			.arg(val[6]).arg(val[7]);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_scalarBarPosition(PipelineObject* tep_piplnObj, QString colName,int tep_orietation, double pos[4])
	{
		QString pycode = QString("PostProcess.script_Properties_scalarBarPosition(%1,%2,%3,%4,%5,%6,%7)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(colName).arg(tep_orietation)
			.arg(pos[0]).arg(pos[1]).arg(pos[2]).arg(pos[3]);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_reflection(PipelineObject* tep_piplnObj, bool val)
	{
		int aaa = 0;
		if (val)
			aaa = 1;
		QString pycode = QString("PostProcess.script_Properties_streamline_reflection(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg(aaa);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
	void SignalHandlerPy::script_Properties_streamline_reflectionAxes(PipelineObject* tep_piplnObj, ReflectionPlane val)
	{
		QString pycode = QString("PostProcess.script_Properties_streamline_reflectionAxes(%1,%2)")
			.arg(tep_piplnObj->mPipeLineObjProp.pipelineObj_base_propData.obj_id).arg((int)val);
		emit _mainWindow->sig_execScript(pycode);
		qDebug(pycode.toStdString().data()); qDebug("\n");
	}
