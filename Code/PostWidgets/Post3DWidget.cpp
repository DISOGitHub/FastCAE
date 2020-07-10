#include "Post3DWidget.h"
#include "ui_Post3DWidget.h"
#include "mainWindow/mainWindow.h"
#include <QMdiSubWindow>
#include <QDockWidget>
#include "settings/busAPI.h"
#include "GraphicsAnalyse/graph_mainwindow.h"
#include "GraphicsAnalyse/PipelineObjPlotForm.h"
#include "python/PyInterpreter.h"
#include "python/PyAgent.h"
#include <QRegExp>



namespace Post
{
	Post3DWindow::Post3DWindow(GUI::MainWindow* mainwindow,int id) 
		: ModuleBase::GraphWindowBase(mainwindow,id, ModuleBase::GraphWindowType::Post3D), _ui(new Ui::Post3DWidget)
	{
		_ui->setupUi(this);
		_ui->layout->setSpacing(0);
		_graphWindow = new graph_MainWindow;
		_ui->layout->addWidget(_graphWindow->get_pipelineObj_plotWidget());
//		_graphWindow->get_pipelineObj_plotWidget()->show();
		connect(_graphWindow, SIGNAL(sig_script_countAniFrame(int)), this, SIGNAL(animateFrameSig(int)));
		connect(_graphWindow, SIGNAL(sig_script_creatPickCurveWidget(QDockWidget*)), this, SLOT(handlePickCurveWodget(QDockWidget*)));
		connect(_graphWindow, SIGNAL(sig_finish_aviFile(QString)), this, SIGNAL(AVIFileFinishedSig(QString)));
		connect(_graphWindow, SIGNAL(sig_script_creatScalarBarEditWidget(QDockWidget*)), this, SLOT(createScalarBarEditWidget(QDockWidget*)));
		connect(_graphWindow, SIGNAL(sig_refreshView_fromMainWin(QList<PipelineObject*>)), this, SIGNAL(refreshViewFromMainWindowSig(QList<PipelineObject*>)));
		connect(_graphWindow, SIGNAL(sig_execScript(QString)), this, SLOT(handleScriptCode(QString)));

		connect(this, SIGNAL(openFileSig(QString, bool)), this, SLOT(openFile(QString, bool)));
		connect(this, SIGNAL(applyClickSig()), this, SLOT(applyClick()));
		connect(this, SIGNAL(Properties_OpacitySig(int, double)), this, SLOT(Properties_Opacity(int, double)));
		connect(this, SIGNAL(Properties_colorColumnSig(int, QString)), this, SLOT(Properties_colorColumn(int, QString)));
		connect(this, SIGNAL(Properties_scalarBarTitleSig(int, QString, QString)), this, SLOT(Properties_scalarBarTitle(int, QString, QString)));
		connect(this, SIGNAL(Properties_scalarBarFontSizeSig(int, QString, int)), this, SLOT(Properties_scalarBarFontSize(int, QString, int)));
		connect(this, SIGNAL(Properties_scalarBarNumLablesSig(int, QString, int)), this, SLOT(Properties_scalarBarNumLables(int, QString, int)));
		connect(this, SIGNAL(Properties_lineWidthSig(int, int)), this, SLOT(Properties_lineWidth(int, int)));
		connect(this, SIGNAL(Properties_pointSizeSig(int, int)), this, SLOT(Properties_pointSize(int, int)));
		connect(this, SIGNAL(Properties_translateSig(int, double, double, double)), this, SLOT(Properties_translate(int, double, double, double)));
		connect(this, SIGNAL(Properties_originSig(int, double, double, double)), this, SLOT(Properties_origin(int, double, double, double)));
		connect(this, SIGNAL(Properties_scaleSig(int, double, double, double)), this, SLOT(Properties_scale(int, double, double, double)));
		connect(this, SIGNAL(Properties_orientationSig(int, double, double, double)), this, SLOT(Properties_orientation(int, double, double, double)));
		connect(this, SIGNAL(Properties_representationSig(int, int)), this, SLOT(Properties_representation(int, int)));
		connect(this, SIGNAL(Properties_specularSig(int, double)), this, SLOT(Properties_specular(int, double)));
		///////////////
		connect(this, SIGNAL(Properties_diffuseSig(int, double)), this, SLOT(Properties_diffuse(int, double)));
		connect(this, SIGNAL(Properties_ambientSig(int, double)), this, SLOT(Properties_ambient(int, double)));
		connect(this, SIGNAL(Properties_specularPowerSig(int, int)), this, SLOT(Properties_specularPower(int, int)));
		connect(this, SIGNAL(Properties_specularColorSig(int, int, int, int)), this, SLOT(Properties_specularColor(int, int, int, int)));
		connect(this, SIGNAL(Properties_solidColorSig(int, int, int, int)), this, SLOT(Properties_solidColor(int, int, int, int)));
		connect(this, SIGNAL(Properties_edgeColorSig(int, int, int, int)), this, SLOT(Properties_edgeColor(int, int, int, int)));
		connect(this, SIGNAL(Properties_interpolationSig(int, int)), this, SLOT(Properties_interpolation(int, int)));
		connect(this, SIGNAL(Properties_Flag_scalarBarSig(int, QString)), this, SLOT(Properties_Flag_scalarBar(int, QString)));
		connect(this, SIGNAL(Properties_EnableOpacityMapSig(int, bool)), this, SLOT(Properties_EnableOpacityMap(int, bool)));
		connect(this, SIGNAL(Properties_visibleSig(int, bool)), this, SLOT(Properties_visible(int, bool)));
		connect(this, SIGNAL(Properties_show_scalarBarsSig(int, bool)), this, SLOT(Properties_show_scalarBars(int, bool)));
		connect(this, SIGNAL(Properties_show_cubeAxesSig(int, bool)), this, SLOT(Properties_show_cubeAxes(int, bool)));
		connect(this, SIGNAL(Properties_scalarBarPositionSig(int, QString, int, double, double, double, double)), this, SLOT(Properties_scalarBarPosition(int, QString, int, double, double, double, double)));
		///////////connect(this, SIGNAL(), this, SLOT());
		connect(this, SIGNAL(FilterClipSig(int)), this, SLOT(FilterClip(int)));
		connect(this, SIGNAL(FilterSliceSig(int)), this, SLOT(FilterSlice(int)));
		connect(this, SIGNAL(FilterContourSig(int)), this, SLOT(FilterContour(int)));
		connect(this, SIGNAL(FilterVectorSig(int)), this, SLOT(FilterVector(int)));
		connect(this, SIGNAL(FilterReflectionSig(int)), this, SLOT(FilterReflection(int)));
		connect(this, SIGNAL(FilterSmoothSig(int)), this, SLOT(FilterSmooth(int)));
		connect(this, SIGNAL(FilterStreamLineSig(int)), this, SLOT(FilterStreamLine(int)));
		
		connect(this, SIGNAL(Properties_vector_GlyphVectorSig(int, QString)), this, SLOT(Properties_vector_GlyphVector(int, QString)));
		connect(this, SIGNAL(Properties_vector_scalarSig(int, QString)), this, SLOT(Properties_vector_scalar(int, QString)));
		connect(this, SIGNAL(Properties_vector_normalSig(int, QString)), this, SLOT(Properties_vector_normal(int, QString)));
		connect(this, SIGNAL(Properties_vector_numPointsSig(int, int)), this, SLOT(Properties_vector_numPoints(int, int)));
		connect(this, SIGNAL(Properties_vector_glyph_typeSig(int, int)), this, SLOT(Properties_vector_glyph_type(int, int)));
		connect(this, SIGNAL(Properties_vector_glyph_tipResSig(int, int)), this, SLOT(Properties_vector_glyph_tipRes(int, int)));
		connect(this, SIGNAL(Properties_vector_glyph_tipRadSig(int, double)), this, SLOT(Properties_vector_glyph_tipRad(int, double)));
		connect(this, SIGNAL(Properties_vector_glyph_tipLenSig(int, double)), this, SLOT(Properties_vector_glyph_tipLen(int, double)));
		connect(this, SIGNAL(Properties_vector_glyph_shaftResSig(int, int)), this, SLOT(Properties_vector_glyph_shaftRes(int, int)));
		connect(this, SIGNAL(Properties_vector_glyph_shaftRadSig(int, double)), this, SLOT(Properties_vector_glyph_shaftRad(int, double)));

		connect(this, SIGNAL(Properties_view_backgroundTypedSig(int)), this, SLOT(Properties_view_backgroundTyped(int)));
		connect(this, SIGNAL(Properties_view_backgroundColorSig(int, int, int)), this, SLOT(Properties_view_backgroundColor(int, int, int)));
		connect(this, SIGNAL(Properties_view_background2ColorSig(int, int, int)), this, SLOT(Properties_view_background2Color(int, int, int)));
		connect(this, SIGNAL(Properties_view_axesVisibleSig(int)), this, SLOT(Properties_view_axesVisible(int)));
		connect(this, SIGNAL(Properties_view_cameraParallelSig(int)), this, SLOT(Properties_view_cameraParallel(int)));
		connect(this, SIGNAL(Properties_view_interactionSig(int)), this, SLOT(Properties_view_interaction(int)));
		connect(this, SIGNAL(Properties_renderViewSig()), this, SLOT(Properties_renderView()));
		connect(this, SIGNAL(Camera_PositionSig(double, double, double)), this, SLOT(Camera_Position(double, double, double)));
		connect(this, SIGNAL(Camera_FocalPointSig(double, double, double)), this, SLOT(Camera_FocalPoint(double, double, double)));
		connect(this, SIGNAL(Camera_ClippingRangeSig(double, double)), this, SLOT(Camera_ClippingRange(double, double)));
		connect(this, SIGNAL(Camera_ViewUpSig(double, double, double)), this, SLOT(Camera_ViewUp(double, double, double)));
		connect(this, SIGNAL(Camera_ViewAngleSig(double)), this, SLOT(Camera_ViewAngle(double)));
		connect(this, SIGNAL(Camera_ZoomSig(double)), this, SLOT(Camera_Zoom(double)));
		connect(this, SIGNAL(Camera_ResetSig()), this, SLOT(Camera_Reset()));
		connect(this, SIGNAL(Properties_planeOriginSig(int, double, double, double)), this, SLOT(Properties_planeOrigin(int, double, double, double)));
		connect(this, SIGNAL(Properties_planeNormalSig(int,  double, double, double)), this, SLOT(Properties_planeNormal(int, double, double, double)));
		connect(this, SIGNAL(Properties_planeVisibleSig(int, int)), this, SLOT(Properties_planeVisible(int, int)));
		connect(this, SIGNAL(Properties_insideOutSig(int, int)), this, SLOT(Properties_insideOut(int, int)));
		connect(this, SIGNAL(Properties_contourColumnSig(int, QString)), this, SLOT(Properties_contourColumn(int, QString)));
		connect(this, SIGNAL(Properties_contourValueSig(int, double)), this, SLOT(Properties_contourValue(int, double)));
		connect(this, SIGNAL(Properties_contour_reflectionSig(int, int)), this, SLOT(Properties_contour_reflection(int, int)));
		connect(this, SIGNAL(Properties_contour_reflectionAxesSig(int, int)), this, SLOT(Properties_contour_reflectionAxes(int, int)));
		connect(this, SIGNAL(Properties_reflectionAxesSig(int, int)), this, SLOT(Properties_reflectionAxes(int, int)));
		connect(this, SIGNAL(Properties_smoothSig(int, int, double)), this, SLOT(Properties_smooth(int, int, double)));
		connect(this, SIGNAL(Properties_streamline_vectorSig(int, QString)), this, SLOT(Properties_streamline_vector(int, QString)));
		//connect(this, SIGNAL(), this, SLOT());




		connect(this, SIGNAL(Properties_streamline_integration_directionSig(int, int)), this, SLOT(Properties_streamline_integration_direction(int, int)));
		connect(this, SIGNAL(Properties_streamline_integration_typeSig(int, int)), this, SLOT(Properties_streamline_integration_type(int, int)));
		connect(this, SIGNAL(Properties_streamline_integration_stepUnitSig(int, int)), this, SLOT(Properties_streamline_integration_stepUnit(int, int)));
		connect(this, SIGNAL(Properties_streamline_integration_initStepLenSig(int, double)), this, SLOT(Properties_streamline_integration_initStepLen(int, double)));
		connect(this, SIGNAL(Properties_streamline_integration_miniStepLenSig(int, double)), this, SLOT(Properties_streamline_integration_miniStepLen(int, double)));
		connect(this, SIGNAL(Properties_streamline_integration_maxiStepLenSig(int, double)), this, SLOT(Properties_streamline_integration_maxiStepLen(int, double)));
		connect(this, SIGNAL(Properties_streamline_stream_maxiStepsSig(int, int)), this, SLOT(Properties_streamline_stream_maxiSteps(int, int)));
		connect(this, SIGNAL(Properties_streamline_stream_maxiStreamLenSig(int, double)), this, SLOT(Properties_streamline_stream_maxiStreamLen(int, double)));
		connect(this, SIGNAL(Properties_streamline_stream_terminalSpeedSig(int, double)), this, SLOT(Properties_streamline_stream_terminalSpeed(int, double)));
		connect(this, SIGNAL(Properties_streamline_stream_maxiErrorSig(int, double)), this, SLOT(Properties_streamline_stream_maxiError(int, double)));
		connect(this, SIGNAL(Properties_streamline_seeds_typeSig(int, int)), this, SLOT(Properties_streamline_seeds_type(int, int)));
		connect(this, SIGNAL(Properties_streamline_seeds_mPointSig(int, double, double, double)), this, SLOT(Properties_streamline_seeds_mPoint(int, double, double, double)));
		connect(this, SIGNAL(Properties_streamline_seeds_num_pointsSig(int, int)), this, SLOT(Properties_streamline_seeds_num_points(int, int)));
		connect(this, SIGNAL(Properties_streamline_seeds_radiusSig(int, double)), this, SLOT(Properties_streamline_seeds_radius(int, double)));
		connect(this, SIGNAL(Properties_streamline_vorticitySig(int, bool)), this, SLOT(Properties_streamline_vorticity(int, bool)));
		connect(this, SIGNAL(Properties_streamline_interpolatorTypeSig(int, int)), this, SLOT(Properties_streamline_interpolatorType(int, int)));
		connect(this, SIGNAL(Properties_streamline_surface_streamLinesSig(int, bool)), this, SLOT(Properties_streamline_surface_streamLines(int, bool)));
		connect(this, SIGNAL(Properties_streamline_reflectionSig(int, bool)), this, SLOT(Properties_streamline_reflection(int, bool)));
		connect(this, SIGNAL(Properties_streamline_reflectionAxesSig(int, int)), this, SLOT(Properties_streamline_reflectionAxes(int, int)));
		//connect(this, SIGNAL(), this, SLOT());
	}
	Post3DWindow::~Post3DWindow()
	{
//		emit closeWindow(_id, _graphWindow->get_pipelineObj_treeWidget(), _graphWindow->get_pipelineObj_propWidget(),_toolBars);
		if (_graphWindow != nullptr) _graphWindow->close();
		if (_ui != nullptr) delete _ui;
	}


	void Post3DWindow::applyClick()
	{
		Py::PythonAagent::getInstance()->appendOff();
		_graphWindow->script_applyClicked();
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::openFile(QString files, bool apply)
	{
		Py::PythonAagent::getInstance()->appendOff();
		_graphWindow->script_openFile(files);
		if (apply)
			_graphWindow->script_applyClicked();
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}
	QWidget* Post3DWindow::getTreeWidget()
	{
		QDockWidget* dw = _graphWindow->get_pipelineObj_treeWidget();
		QWidget* old = dw->titleBarWidget();
		QWidget* empty = new QWidget;
		dw->setTitleBarWidget(empty);
		delete old;
		return dw;
	}
	QWidget* Post3DWindow::getPropWidget()
	{
		QDockWidget* dw = _graphWindow->get_pipelineObj_propWidget();
		QWidget* old = dw->titleBarWidget();
		QWidget* empty = new QWidget;
		dw->setTitleBarWidget(empty);
		delete old;
		return dw;
	}
	QList<QToolBar*> Post3DWindow::getToolBars()
	{
		QList<QToolBar*> toolBars;
		toolBars.append(_graphWindow->script_get_savePngAviToolBar());
		toolBars.append(_graphWindow->script_get_pipelineObjPropToolBar());
		toolBars.append(_graphWindow->script_get_filterToolBar());
//		toolBars.append(_graphWindow->script_get_AniToolBar());
		toolBars.append(_graphWindow->script_get_mousePickToolBar());
//		toolBars.append(_graphWindow->script_get_viewDirectionToolBar());
		return toolBars;
	}
	void Post3DWindow::setViewValue(int x1,int x2,int x3,int y1,int y2,int y3,int z1,int z2,int z3)
	{
		_graphWindow->script_setViewValueDirection( x1,x2,x3,y1,y2,y3,z1,z2,z3);
	}
	void Post3DWindow::fitView()
	{
		_graphWindow->script_resetView();
	}
	void Post3DWindow::setViewXPlus()
	{
		_graphWindow->script_setViewDirection("xplus");
	}
	void Post3DWindow::setViewXMiuns()
	{
		_graphWindow->script_setViewDirection("xminus");
	}
	void Post3DWindow::setViewYPlus()
	{
		_graphWindow->script_setViewDirection("yplus");
	}
	void Post3DWindow::setViewYMiuns()
	{
		_graphWindow->script_setViewDirection("yminus");
	}
	void Post3DWindow::setViewZPlus()
	{
		_graphWindow->script_setViewDirection("zplus");
	}
	void Post3DWindow::setViewZMiuns()
	{
		_graphWindow->script_setViewDirection("zminus");
	}
	bool Post3DWindow::startAnimate()
	{
//		_graphWindow->slot_animate();
		return _graphWindow->script_start_animate();
	}
	bool Post3DWindow::stopAnimate()
	{
		return _graphWindow->script_stop_animate();
	}
	void Post3DWindow::clearAll()
	{
// 		QList<PipelineObject*> objList = _graphWindow->script_get_pipelineObject_list();
// 		for (int i = 0; i < objList.size(); ++i)
// 		{
// 			PipelineObject* p = objList.at(i);
// 			_graphWindow->script_delPipelineObj(p);
// 		}
		_graphWindow->script_clearAllPipelineObjs();
	}
	void Post3DWindow::setBackGround(QColor color1, QColor color2)
	{
		_graphWindow->script_Properties_view_backgroundType(1);
		_graphWindow->script_Properties_view_backgroundColor(color1);
		_graphWindow->script_Properties_view_background2Color(color2);
		_graphWindow->script_Properties_renderView();

	}
	void Post3DWindow::handlePickCurveWodget(QDockWidget* w)
	{
		QWidget *oritit = w->titleBarWidget();
		QWidget* nt = new QWidget();
		w->setTitleBarWidget(nt);
		delete oritit;
		emit createPickCurveWidgetSig(w);
	}
	void Post3DWindow::saveImage(QString fileName, int width, int heigh, bool showDlg)
	{
		_graphWindow->script_saveImage(showDlg, width, heigh, fileName);
	}
	void Post3DWindow::updatePipelineObjeDataSet(PipelineObject* obj, QString fileName)
	{
		_graphWindow->script_updatePipelineObjeDataSet(obj, fileName);
	}
	PipelineObject* Post3DWindow::getCurrentPipelineObj()
	{
		return _graphWindow->get_cur_PipelineObj();
	}
	void Post3DWindow::createScalarBarEditWidget(QDockWidget *w)
	{
		int width = _graphWindow->width();
		w->setFixedWidth(width);
		_ui->layout->addWidget(w);
	}

	void Post3DWindow::handleScriptCode(QString code)
	{
		int caseid = getID();
		QString wintype = getStringGraphWindowType();
		QRegExp reg("[()]");
		QString midcon = code.section(reg, 1, 1);
		QString newcode("");
		if (midcon=="")
		{
			QString idaddtype = QString("(%1,\"%2\"").arg(caseid).arg(wintype);
		    newcode = code.replace(QRegExp("\\("), idaddtype);
		}
		else
		{
			QString idaddtype = QString("(%1,\"%2\",").arg(caseid).arg(wintype);
			newcode = code.replace(QRegExp("\\("), idaddtype);	
		}
		qDebug() << newcode;
		Py::PythonAagent::getInstance()->appCodeList(newcode);
// 		QStringList qlist = Py::PythonAagent::getInstance()->getcodelist();
// 		for (int i = 0; i < qlist.size(); i++)
// 		{
// 			qDebug() << qlist.at(i);
// 		}
// 		Py::PythonAagent::getInstance()->unLock();
		
	}
	void Post3DWindow::reTranslate()
	{
		_ui->retranslateUi(this);
		QString l = Setting::BusAPI::instance()->getLanguage();
		_graphWindow->ChangeLanguage(l);
	}
	void Post3DWindow::getScalarRange(PipelineObject* obj, double value[2])
	{
		_graphWindow->script_getScalarRange(obj, value);
	}
	void Post3DWindow::reScaleScalarRange(PipelineObject* obj, double min, double max)
	{
		_graphWindow->script_rescaleScalarRange(obj, min, max);
	}
	void Post3DWindow::displayLegendBar(bool on)
	{
//		_graphWindow->script_sca
		PipelineObject* obj = getCurrentPipelineObj();
		_graphWindow->script_Properties_Flag_scalarBar(obj, on);
		_graphWindow->script_Properties_show_scalarBars(obj);
	}
	void Post3DWindow::viewCounter(PipelineObject* obj, QString var)
	{
		_graphWindow->script_Properties_colorColumn(obj, var);
//		_graphWindow->script_FilterContour(obj);
	}
	void Post3DWindow::viewVector(PipelineObject* obj, QString var)
	{
		_graphWindow->script_Properties_vector_normal(obj, var);
	}

	vtkDataSet* Post3DWindow::getPipelineObjDataSet(PipelineObject* obj)
	{
		return _graphWindow->script_get_pipelineObj_dataSet(obj);
	}

	int Post3DWindow::getNumOfGeometry()
	{
		return _graphWindow->get_pipelineObj_treeWidget()->getNodeNum();
	}

	PipelineObject * Post3DWindow::getGeometryByIndex(int index)
	{
		return _graphWindow->get_pipelineObj_treeWidget()->getNodeByIndex(index);
	}
	
	
	void Post3DWindow::Properties_Opacity(int obj_id, double mOpacity)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_Opacity(pipobj, mOpacity);
		//_graphWindow->script_Properties_Opacity();

		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}
	//pass
	void Post3DWindow::Properties_colorColumn(int obj_id, QString mColorColumnStyle)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_colorColumn(pipobj,mColorColumnStyle);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_scalarBarTitle(int obj_id, QString scolName, QString sm_titile)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_scalarBarTitle(pipobj, scolName, sm_titile);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
		
	}

	void Post3DWindow::Properties_scalarBarFontSize(int obj_id, QString scolName, int m_fontSize)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_scalarBarFontSize(pipobj, scolName, m_fontSize);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_scalarBarNumLables(int obj_id, QString scolName, int m_numLables)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_scalarBarNumLables(pipobj, scolName, m_numLables);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_lineWidth(int obj_id, int mLineWidth)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_lineWidth(pipobj, mLineWidth);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_pointSize(int obj_id, int mPointSize)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_pointSize(pipobj, mPointSize);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_translate(int obj_id, double x, double y, double z)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		double val[3] = { x, y, z };
		_graphWindow->script_Properties_translate(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_origin(int obj_id, double x, double y, double z)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		double val[3] = { x, y, z };
		_graphWindow->script_Properties_origin(pipobj,val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_scale(int obj_id, double x, double y, double z)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		double val[3] = { x, y, z };
		_graphWindow->script_Properties_scale(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_orientation(int obj_id, double x, double y, double z)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		double val[3] = { x, y, z };
		_graphWindow->script_Properties_orientation(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_representation(int obj_id, int m_enum_representationtype)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		ERepresentationType t =ERepresentationType(m_enum_representationtype);
		_graphWindow->script_Properties_representation(pipobj, t);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_specular(int obj_id, double mSpecular)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_specular(pipobj, mSpecular);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}
	///////////////////////////////////
	void Post3DWindow::Properties_diffuse(int obj_id, double mDiffuse)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_diffuse(pipobj, mDiffuse);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_ambient(int obj_id, double mAmbient)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_ambient(pipobj, mAmbient);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_specularPower(int obj_id, int mSpecularPower)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_specularPower(pipobj, mSpecularPower);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_specularColor(int obj_id, int r, int g, int b)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		int val[3] = { r, g, b };
		_graphWindow->script_Properties_specularColor(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_solidColor(int obj_id, int r, int g, int b)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		int val[3] = { r, g, b };
		_graphWindow->script_Properties_solidColor(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_edgeColor(int obj_id, int r, int g, int b)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		int val[3] = { r, g, b };
		_graphWindow->script_Properties_edgeColor(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_interpolation(int obj_id, int m_enum_interpolationtype)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		EInterpolationType t = EInterpolationType(m_enum_interpolationtype);
		_graphWindow->script_Properties_interpolation(pipobj, t);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_Flag_scalarBar(int obj_id, QString smColorColumnStyle)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		bool b = pipobj->mPipeLineObjProp.pipelineObj_base_propData.scalarBar_showMap.value(smColorColumnStyle);
		_graphWindow->script_Properties_Flag_scalarBar(pipobj, b);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_EnableOpacityMap(int obj_id, bool val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_EnableOpacityMap(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_visible(int obj_id, bool flag_show_actors)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_visible(pipobj, flag_show_actors);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_show_scalarBars(int obj_id, bool mScalarBarVisible)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		pipobj->mPipeLineObjProp.pipelineObj_base_propData.mScalarBarVisible = mScalarBarVisible;
		_graphWindow->script_Properties_show_scalarBars(pipobj);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_show_cubeAxes(int obj_id, bool flag_cubeAxes)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		pipobj->mPipeLineObjProp.pipelineObj_base_propData.flag_cubeAxes = flag_cubeAxes;
		_graphWindow->script_Properties_show_cubeAxes(pipobj);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_scalarBarPosition(int obj_id, QString scolName, int tep_orietation, double pos0, double pos1, double pos2, double pos3)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		double val[4] = { pos0, pos1, pos2, pos3 };
		_graphWindow->script_Properties_scalarBarPosition(pipobj, scolName,tep_orietation,val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::FilterClip(int obj_id)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_FilterClip(pipobj);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::FilterSlice(int obj_id)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_FilterSlice(pipobj);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::FilterContour(int obj_id)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_FilterContour(pipobj);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::FilterVector(int obj_id)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_FilterVector(pipobj);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::FilterReflection(int obj_id)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_FilterReflection(pipobj);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::FilterSmooth(int obj_id)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_FilterSmooth(pipobj);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::FilterStreamLine(int obj_id)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_FilterStreamLine(pipobj);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_vector_GlyphVector(int obj_id, QString sval)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_vector_GlyphVector(pipobj, sval);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_vector_scalar(int obj_id, QString sval)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_vector_scalar(pipobj, sval);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_vector_normal(int obj_id, QString sval)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_vector_normal(pipobj, sval);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_vector_numPoints(int obj_id, int val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_vector_numPoints(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_vector_glyph_type(int obj_id, int val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_vector_glyph_type(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_vector_glyph_tipRes(int obj_id, int val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_vector_glyph_tipRes(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_vector_glyph_tipRad(int obj_id, double val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_vector_glyph_tipRad(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_vector_glyph_tipLen(int obj_id, double val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_vector_glyph_tipLen(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_vector_glyph_shaftRes(int obj_id, int val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_vector_glyph_shaftRes(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_vector_glyph_shaftRad(int obj_id, double val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_vector_glyph_shaftRad(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}
	////////////////
	void Post3DWindow::Properties_view_backgroundTyped(int val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		_graphWindow->script_Properties_view_backgroundType(val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_view_backgroundColor(int red, int green, int blue)
	{   
		Py::PythonAagent::getInstance()->appendOff();
		QColor qc(red, green, blue);
		_graphWindow->script_Properties_view_backgroundColor(qc);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_view_background2Color(int red, int green, int blue)
	{
		Py::PythonAagent::getInstance()->appendOff();
		QColor qc(red, green, blue);
		_graphWindow->script_Properties_view_background2Color(qc);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_view_axesVisible(int a)
	{
		Py::PythonAagent::getInstance()->appendOff();
		_graphWindow->script_Properties_view_axesVisible(a);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_view_cameraParallel(int a)
	{
		Py::PythonAagent::getInstance()->appendOff();
		_graphWindow->script_Properties_view_cameraParallel(a);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_view_interaction(int a)
	{
		Py::PythonAagent::getInstance()->appendOff();
		_graphWindow->script_Properties_view_interaction(a);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_renderView()
	{
		Py::PythonAagent::getInstance()->appendOff();
		_graphWindow->script_Properties_renderView();
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Camera_Position(double pos0, double pos1, double pos2)
	{
		Py::PythonAagent::getInstance()->appendOff();
		double pos[3] = { pos0, pos1, pos2 };
		_graphWindow->script_Camera_Position(pos);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Camera_FocalPoint(double focalPoint0, double focalPoint1, double focalPoint2)
	{
		Py::PythonAagent::getInstance()->appendOff();
		double focalPoint[3] = { focalPoint0, focalPoint1, focalPoint2 };
		_graphWindow->script_Camera_FocalPoint(focalPoint);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Camera_ClippingRange(double clippingRange0, double clippingRange1)
	{
		Py::PythonAagent::getInstance()->appendOff();
		double clippingRange[2] = { clippingRange0, clippingRange1 };
		_graphWindow->script_Camera_ClippingRange(clippingRange);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Camera_ViewUp(double viewup0, double viewup1, double viewup2)
	{
		Py::PythonAagent::getInstance()->appendOff();
		double viewup[3] = { viewup0, viewup1, viewup2 };
		_graphWindow->script_Camera_ViewUp(viewup);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Camera_ViewAngle(double angle)
	{
		Py::PythonAagent::getInstance()->appendOff();
		_graphWindow->script_Camera_ViewAngle(angle);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Camera_Zoom(double zoom)
	{
		Py::PythonAagent::getInstance()->appendOff();
		_graphWindow->script_Camera_Zoom(zoom);
		Py::PythonAagent::getInstance()->appendOn(); 
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Camera_Reset()
	{
		Py::PythonAagent::getInstance()->appendOff();
		_graphWindow->script_Camera_Reset();
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_planeOrigin(int obj_id, double x, double y, double z)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		double val[3] = { x, y, z };
		_graphWindow->script_Properties_planeOrigin(pipobj,val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_planeNormal(int obj_id,  double x, double y, double z)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		double val[3] = { x, y, z };
		_graphWindow->script_Properties_planeNormal(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_planeVisible(int obj_id, int a)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_planeVisible(pipobj, a);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_insideOut(int obj_id, int a)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_insideOut(pipobj, a);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_contourColumn(int obj_id, QString sval)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_contourColumn(pipobj, sval);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_contourValue(int obj_id, double val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_contourValue(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_contour_reflection(int obj_id, int aaa)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_vector_glyph_shaftRad(pipobj, aaa);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_contour_reflectionAxes(int obj_id, int val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_vector_glyph_shaftRad(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}
	
	void Post3DWindow::Properties_reflectionAxes(int obj_id, int reflection_axis)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		ReflectionPlane ref = ReflectionPlane(reflection_axis);
		_graphWindow->script_Properties_reflectionAxes(pipobj, ref);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_smooth(int obj_id, int smotype, double coef)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_smooth(pipobj, smotype, coef);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}
	//////////
	void Post3DWindow::Properties_streamline_vector(int obj_id, QString sval)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_streamline_vector(pipobj, sval);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_integration_direction(int obj_id, int val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		IntegrationDirection sval = IntegrationDirection(val);
		_graphWindow->script_Properties_streamline_integration_direction(pipobj, sval);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_integration_type(int obj_id, int val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		IntegratorType sval = IntegratorType(val);
		_graphWindow->script_Properties_streamline_integration_type(pipobj, sval);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_integration_stepUnit(int obj_id, int val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		IntegrationStepUnit sval = IntegrationStepUnit(val);
		_graphWindow->script_Properties_streamline_integration_stepUnit(pipobj, sval);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_integration_initStepLen(int obj_id, double val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_streamline_integration_initStepLen(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_integration_miniStepLen(int obj_id, double val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_streamline_integration_miniStepLen(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_integration_maxiStepLen(int obj_id, double val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_streamline_integration_maxiStepLen(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();

	}

	void Post3DWindow::Properties_streamline_stream_maxiSteps(int obj_id, int val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_streamline_stream_maxiSteps(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_stream_maxiStreamLen(int obj_id, double val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_streamline_stream_maxiStreamLen(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_stream_terminalSpeed(int obj_id, double val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_streamline_stream_terminalSpeed(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_stream_maxiError(int obj_id, double val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_streamline_stream_maxiError(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_seeds_type(int obj_id, int val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		SpeedType sval = SpeedType(val);
		_graphWindow->script_Properties_streamline_seeds_type(pipobj, sval);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_seeds_mPoint(int obj_id, double val0, double val1, double val2)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		double val[3] = { val0, val1, val2 };
		_graphWindow->script_Properties_streamline_seeds_mPoint(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_seeds_num_points(int obj_id, int val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_streamline_seeds_num_points(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_seeds_radius(int obj_id, double val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_streamline_seeds_radius(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_vorticity(int obj_id, bool val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_streamline_vorticity(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_interpolatorType(int obj_id, int val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		InterpolatorType sval = InterpolatorType(val);
		_graphWindow->script_Properties_streamline_interpolatorType(pipobj, sval);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_surface_streamLines(int obj_id, bool val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_streamline_surface_streamLines(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();

	}

	void Post3DWindow::Properties_streamline_reflection(int obj_id, bool val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		_graphWindow->script_Properties_streamline_reflection(pipobj, val);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::Properties_streamline_reflectionAxes(int obj_id, int val)
	{
		Py::PythonAagent::getInstance()->appendOff();
		PipelineObject* pipobj = _graphWindow->script_getPipelineObj_byID(obj_id);
		ReflectionPlane sval = ReflectionPlane(val);
		_graphWindow->script_Properties_streamline_reflectionAxes(pipobj, sval);
		Py::PythonAagent::getInstance()->appendOn();
		Py::PythonAagent::getInstance()->unLock();
	}

	void Post3DWindow::showAnimationToolBar(bool show)
	{
		QToolBar* b = _graphWindow->script_get_AniToolBar();
		if (b != nullptr)
			b->setVisible(show);

	}

}
