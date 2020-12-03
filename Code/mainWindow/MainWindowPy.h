#ifndef _PYMAINWINDOW_H_glyph_shaftRes
#define _PYMAINWINDOW_H_

#include "mainWindowAPI.h"

namespace Py
{
	class PythonAagent;
}

namespace GUI
{
	class MainWindow;
	class SignalHandler;
	class SubWindowManager;

	//静态类，封装C++功能，供C接口调用
	class MAINWINDOWAPI MainWindowPy
	{
	public :
		//static void showFastCAE();
		static void undo();
		static void redo();
		static void init(GUI::MainWindow* m,GUI::SignalHandler* sg);
		static void clearData();
		static void importMesh(char* f, char* s, int modelId);
		static void exportMesh(char* f,char* s, int modelId);
		static void importGeometry(char* f);
		static void exportGeometry(char* f);
		static void openProjectFile(char* f);
		static void saveProjectFile(char* f);
		static void openPost2D();
		static void openPost3D();
		static void openPreWindow();
		static void saveImage(int w, int h, int id, char*win, char* file);
		static void setView(int id, char*win, char* view);
		static void setViewRandValue(int id, char*win, int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3);
		static void quit();
		static void solveProject(int projectIndex, int solverIndex);
		static void createSet(const char* name, const char* type, const char* idstring);
		static void createGeoComponent(char* name, char* type, char* strgIDs, char* striIDs);
		static void createVTKTransform(const char* componentIds, const char* rotate, const char* moveLocation, const char* scale);
		static void findConplanarPorC(const char* seedType, int seedId, double minAngle, int kernalId, const char* setName);		
		static void updateInterface();

		static void script_openFile(int id, char* type, char* file);
		static void script_applyClicked(int id, char* type);
		static void script_Properties_Opacity(int id, char* type, int obj_id, double mOpacity);
		static void script_Properties_colorColumn(int id, char* type, int obj_id, char* mColorColumnStyle);
		static void script_Properties_scalarBarTitle(int id, char* type, int obj_id, char* colName, char* m_title);
		static void script_Properties_scalarBarFontSize(int id, char* type, int obj_id, char* colName, int m_fontSize);
		static void script_Properties_scalarBarNumLables(int id, char* type, int obj_id, char* colName, int m_numLables);
		static void script_Properties_lineWidth(int id, char* type, int obj_id, int mLineWidth);
		static void	script_Properties_pointSize(int id, char* type, int obj_id, int mPointSize);
		static void	script_Properties_translate(int id, char* type, int obj_id, double x, double y, double z);
		static void	script_Properties_origin(int id, char* type, int obj_id, double x, double y, double z);
		static void	script_Properties_scale(int id, char* type, int obj_id, double x, double y, double z);
		static void	script_Properties_orientation(int id, char* type, int obj_id, double x, double y, double z);
		static void	script_Properties_representation(int id, char* type, int obj_id, int m_enum_representationtype);
		static void script_Properties_specular(int id, char* type, int obj_id, double mSpecular);

		static void script_Properties_diffuse(int id, char* type, int obj_id, double mDiffuse);
		static void	script_Properties_ambient(int id, char* type, int obj_id, double mAmbient);
		static void	script_Properties_specularPower(int id, char* type, int obj_id, int mSpecularPower);
		static void	script_Properties_specularColor(int id, char* type, int obj_id, int r, int g, int b);
		static void	script_Properties_solidColor(int id, char* type, int obj_id, int r, int g, int b);
		static void	script_Properties_edgeColor(int id, char* type, int obj_id, int r, int g, int b);
		static void	script_Properties_interpolation(int id, char* type, int obj_id, int m_enum_interpolationtype);
		static void	script_Properties_Flag_scalarBar(int id, char* type, int obj_id, char* mColorColumnStyle);
		static void	script_Properties_EnableOpacityMap(int id, char* type, int obj_id, bool val);
		static void	script_Properties_visible(int id, char* type, int obj_id, bool flag_show_actors);
		static void	script_Properties_show_scalarBars(int id, char* type, int obj_id, bool mScalarBarVisible);
		static void	script_Properties_show_cubeAxes(int id, char* type, int obj_id, bool flag_cubeAxes);
		static void	script_Properties_scalarBarPosition(int id, char* type, int obj_id, char* colName, int tep_orietation, double pos0, double pos1, double pos2, double pos3);

		static void	script_FilterClip(int id, char* type, int obj_id);
		static void	script_FilterSlice(int id, char* type, int obj_id);
		static void	script_FilterContour(int id, char* type, int obj_id);
		static void	script_FilterVector(int id, char* type, int obj_id);
		static void	script_FilterReflection(int id, char* type, int obj_id);
		static void	script_FilterSmooth(int id, char* type, int obj_id);
		static void	script_FilterStreamLine(int id, char* type, int obj_id);
		//121211
		static void script_Properties_vector_GlyphVector(int id, char* type, int obj_id, char* val);
		static void	script_Properties_vector_scalar(int id, char* type, int obj_id, char* val);
		static void	script_Properties_vector_normal(int id, char* type, int obj_id, char* val);
		static void	script_Properties_vector_numPoints(int id, char* type, int obj_id, int val);
		static void	script_Properties_vector_glyph_type(int id, char* type, int obj_id, int val);
		static void	script_Properties_vector_glyph_tipRes(int id, char* type, int obj_id, int val);
		static void	script_Properties_vector_glyph_tipRad(int id, char* type, int obj_id, double val);
		static void	script_Properties_vector_glyph_tipLen(int id, char* type, int obj_id, double val);
		static void	script_Properties_vector_glyph_shaftRes(int id, char* type, int obj_id, int val);
		static void	script_Properties_vector_glyph_shaftRad(int id, char* type, int obj_id, double val);

		///
		static void script_Properties_view_backgroundType(int id, char* type, int val);
		static void	script_Properties_view_backgroundColor(int id, char* type, int red, int green, int blue);
		static void	script_Properties_view_background2Color(int id, char* type, int red, int green, int blue);
		static void	script_Properties_view_axesVisible(int id, char* type, int a);
		static void	script_Properties_view_cameraParallel(int id, char* type, int a);
		static void	script_Properties_view_interaction(int id, char* type, int a);
		static void	script_Properties_renderView(int id, char* type);

		static void	script_Camera_Position(int id, char* type, double pos0, double pos1, double pos2);
		static void	script_Camera_FocalPoint(int id, char* type, double focalPoint0, double focalPoint1, double focalPoint2);
		static void	script_Camera_ClippingRange(int id, char* type, double clippingRange0, double clippingRange1);
		static void	script_Camera_ViewUp(int id, char* type, double viewup0, double viewup1, double viewup2);
		static void	script_Camera_ViewAngle(int id, char* type, double angle);
		static void	script_Camera_Zoom(int id, char* type, double zoom);
		static void	script_Camera_Reset(int id, char* type);

		static void	script_Properties_planeOrigin(int id, char* type, int obj_id, double x, double y, double z);
		static void	script_Properties_planeNormal(int id, char* type, int obj_id, double x, double y, double z);
		static void	script_Properties_planeVisible(int id, char* type, int obj_id, int a);
		static void	script_Properties_insideOut(int id, char* type, int obj_id, int a);

		static void	script_Properties_contourColumn(int id, char* type, int obj_id, char* val);
		static void	script_Properties_contourValue(int id, char* type, int obj_id, double val);
		static void	script_Properties_contour_reflection(int id, char* type, int obj_id, int aaa);
		static void	script_Properties_contour_reflectionAxes(int id, char* type, int obj_id, int val);

		static void	script_Properties_reflectionAxes(int id, char* type, int obj_id, int reflection_axis);

		static void	script_Properties_smooth(int id, char* type, int obj_id, int smotype, double coef);


		static void	script_Properties_streamline_vector(int id, char* type, int obj_id, char* val);
		static void	script_Properties_streamline_integration_direction(int id, char* type, int obj_id, int val);
		static void	script_Properties_streamline_integration_type(int id, char* type, int obj_id, int val);
		static void	script_Properties_streamline_integration_stepUnit(int id, char* type, int obj_id, int val);
		static void	script_Properties_streamline_integration_initStepLen(int id, char* type, int obj_id, double val);
		static void	script_Properties_streamline_integration_miniStepLen(int id, char* type, int obj_id, double val);
		static void	script_Properties_streamline_integration_maxiStepLen(int id, char* type, int obj_id, double val);
		static void	script_Properties_streamline_stream_maxiSteps(int id, char* type, int obj_id, int val);
		static void	script_Properties_streamline_stream_maxiStreamLen(int id, char* type, int obj_id, double val);
		static void	script_Properties_streamline_stream_terminalSpeed(int id, char* type, int obj_id, double val);
		static void	script_Properties_streamline_stream_maxiError(int id, char* type, int obj_id, double val);
		static void	script_Properties_streamline_seeds_type(int id, char* type, int obj_id, int val);
		static void	script_Properties_streamline_seeds_mPoint(int id, char* type, int obj_id, double val0, double val1, double val2);
		static void	script_Properties_streamline_seeds_num_points(int id, char* type, int obj_id, int val);
		static void	script_Properties_streamline_seeds_radius(int id, char* type, int obj_id, double val);
		static void	script_Properties_streamline_vorticity(int id, char* type, int obj_id, bool val);
		static void	script_Properties_streamline_interpolatorType(int id, char* type, int obj_id, int val);
		static void	script_Properties_streamline_surface_streamLines(int id, char* type, int obj_id, bool val);
		static void	script_Properties_streamline_reflection(int id, char* type, int obj_id, bool val);
		static void	script_Properties_streamline_reflectionAxes(int id, char* type, int obj_id, int val);
		

	private: 		
		static MainWindow* _mainWindow;
		static SignalHandler* _signalHander;
		static SubWindowManager* _subWindowManager;
		static Py::PythonAagent* _pyAgent;
	};
}


//声明为C接口，供Python脚本调用
extern "C"
{
	//void MAINWINDOWAPI showFastCAE();
	void MAINWINDOWAPI undo();
	void MAINWINDOWAPI redo();
	void MAINWINDOWAPI clearData();
	void MAINWINDOWAPI importMesh(char* f, char* s, int modelId);
	void MAINWINDOWAPI exportMesh(char* f, char* s, int modelId);
	void MAINWINDOWAPI importGeometry(char* f);
	void MAINWINDOWAPI exportGeometry(char* f);
	void MAINWINDOWAPI openProjectFile(char* f);
	void MAINWINDOWAPI saveProjectFile(char* f);
	void MAINWINDOWAPI updateInterface();
	void MAINWINDOWAPI quit();
	void MAINWINDOWAPI saveImage(int w, int h, int id, char*win, char* file);
	void MAINWINDOWAPI setView( int id, char*win, char* view);//设置特定视角
	void MAINWINDOWAPI setViewRandValue(int id, char*win, int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3);//后处理自由设置视角
	//void MAINWINDOWAPI setViewPreValue(int id, char*win, int x1, int x2, int x3, int y1, int y2, int y3, int z1, int z2, int z3);//前处理自由设置视角
	void MAINWINDOWAPI openPost3D();//打开3d窗口
	void MAINWINDOWAPI openPost2D();//打开2d窗口
	void MAINWINDOWAPI openPreWindow();//打开前处理窗口
	void MAINWINDOWAPI solveProject(int projectIndex, int solverIndex);//求解
	void MAINWINDOWAPI createSet(char* name, char* type, char* idstring);
	void MAINWINDOWAPI createGeoComponent(char* name, char* type, char* strgIDs, char* striIDs);
	void MAINWINDOWAPI createVTKTransform(char* componentIds, char* rotate, char* moveLocation, char* scale);	
	void MAINWINDOWAPI findConplanarPorC(const char* seedType, int seedId, double minAngle, int kernalId, const char* setName);
	void MAINWINDOWAPI script_openFile(int id, char* type, char* file);
	void MAINWINDOWAPI script_applyClicked(int id, char* type);
	void MAINWINDOWAPI script_Properties_Opacity(int id, char* type, int obj_id, double mOpacity);
	void MAINWINDOWAPI script_Properties_colorColumn(int id, char* type, int obj_id, char* mColorColumnStyle);
	void MAINWINDOWAPI script_Properties_scalarBarTitle(int id, char* type, int obj_id, char* colName, char* m_title);
	void MAINWINDOWAPI script_Properties_scalarBarFontSize(int id, char* type, int obj_id, char* colName, int m_fontSize);
	void MAINWINDOWAPI script_Properties_scalarBarNumLables(int id, char* type, int obj_id, char* colName, int m_numLables);
	void MAINWINDOWAPI script_Properties_lineWidth(int id, char* type, int obj_id, int mLineWidth);
	void MAINWINDOWAPI script_Properties_pointSize(int id, char* type, int obj_id, int mPointSize);
	void MAINWINDOWAPI script_Properties_translate(int id, char* type,int obj_id, double x, double y, double z);
	void MAINWINDOWAPI script_Properties_origin(int id, char* type, int obj_id, double x, double y, double z);
	void MAINWINDOWAPI script_Properties_scale(int id, char* type, int obj_id, double x, double y, double z);
	void MAINWINDOWAPI script_Properties_orientation(int id, char* type, int obj_id, double x, double y, double z);
	void MAINWINDOWAPI script_Properties_representation(int id, char* type, int obj_id, int m_enum_representationtype);
	void MAINWINDOWAPI script_Properties_specular(int id, char* type, int obj_id, double mSpecular);

	void MAINWINDOWAPI script_Properties_diffuse(int id, char* type, int obj_id, double mDiffuse);
	void MAINWINDOWAPI script_Properties_ambient(int id, char* type, int obj_id, double mAmbient);
	void MAINWINDOWAPI script_Properties_specularPower(int id, char* type, int obj_id, int mSpecularPower);
	void MAINWINDOWAPI script_Properties_specularColor(int id, char* type, int obj_id, int r, int g, int b);
	void MAINWINDOWAPI script_Properties_solidColor(int id, char* type, int obj_id, int r, int g, int b);
	void MAINWINDOWAPI script_Properties_edgeColor(int id, char* type, int obj_id, int r, int g, int b);
	void MAINWINDOWAPI script_Properties_interpolation(int id, char* type, int obj_id, int m_enum_interpolationtype);
	void MAINWINDOWAPI script_Properties_Flag_scalarBar(int id, char* type, int obj_id, char* mColorColumnStyle);
	void MAINWINDOWAPI script_Properties_EnableOpacityMap(int id, char* type, int obj_id, bool val);
	void MAINWINDOWAPI script_Properties_visible(int id, char* type, int obj_id, bool flag_show_actors);
	void MAINWINDOWAPI script_Properties_show_scalarBars(int id, char* type, int obj_id, bool mScalarBarVisible);
	void MAINWINDOWAPI script_Properties_show_cubeAxes(int id, char* type, int obj_id, bool flag_cubeAxes);
	void MAINWINDOWAPI script_Properties_scalarBarPosition(int id, char* type, int obj_id, char* colName, int tep_orietation, double pos0, double pos1, double pos2, double pos3);

	void MAINWINDOWAPI script_FilterClip(int id, char* type, int obj_id);
	void MAINWINDOWAPI script_FilterSlice(int id, char* type, int obj_id);
	void MAINWINDOWAPI script_FilterContour(int id, char* type, int obj_id);
	void MAINWINDOWAPI script_FilterVector(int id, char* type, int obj_id);
	void MAINWINDOWAPI script_FilterReflection(int id, char* type, int obj_id);
	void MAINWINDOWAPI script_FilterSmooth(int id, char* type, int obj_id);
	void MAINWINDOWAPI script_FilterStreamLine(int id, char* type, int obj_id);
	//1212
	void MAINWINDOWAPI script_Properties_vector_GlyphVector(int id, char* type, int obj_id, char* val);
	void MAINWINDOWAPI script_Properties_vector_scalar(int id, char* type, int obj_id, char* val);
	void MAINWINDOWAPI script_Properties_vector_normal(int id, char* type, int obj_id, char* val);
	void MAINWINDOWAPI script_Properties_vector_numPoints(int id, char* type, int obj_id, int val);
	void MAINWINDOWAPI script_Properties_vector_glyph_type(int id, char* type, int obj_id, int val);
	void MAINWINDOWAPI script_Properties_vector_glyph_tipRes(int id, char* type, int obj_id, int val);
	void MAINWINDOWAPI script_Properties_vector_glyph_tipRad(int id, char* type, int obj_id, double val);
	void MAINWINDOWAPI script_Properties_vector_glyph_tipLen(int id, char* type, int obj_id, double val);
	void MAINWINDOWAPI script_Properties_vector_glyph_shaftRes(int id, char* type, int obj_id, int val);
	void MAINWINDOWAPI script_Properties_vector_glyph_shaftRad(int id, char* type, int obj_id, double val);

	///

	void MAINWINDOWAPI script_Properties_view_backgroundType(int id, char* type, int val);
	void MAINWINDOWAPI script_Properties_view_backgroundColor(int id, char* type, int red, int green, int blue);
	void MAINWINDOWAPI script_Properties_view_background2Color(int id, char* type, int red, int green, int blue);
	void MAINWINDOWAPI script_Properties_view_axesVisible(int id, char* type, int a);
	void MAINWINDOWAPI script_Properties_view_cameraParallel(int id, char* type, int a);
	void MAINWINDOWAPI script_Properties_view_interaction(int id, char* type, int a);
	void MAINWINDOWAPI script_Properties_renderView(int id, char* type);

	void MAINWINDOWAPI	script_Camera_Position(int id, char* type, double pos0, double pos1, double pos2);
	void MAINWINDOWAPI	script_Camera_FocalPoint(int id, char* type, double focalPoint0, double focalPoint1, double focalPoint2);
	void MAINWINDOWAPI	script_Camera_ClippingRange(int id, char* type, double clippingRange0, double clippingRange1);
	void MAINWINDOWAPI	script_Camera_ViewUp(int id, char* type, double viewup0, double viewup1, double viewup2);
	void MAINWINDOWAPI	script_Camera_ViewAngle(int id, char* type, double angle);
	void MAINWINDOWAPI   script_Camera_Zoom(int id, char* type, double zoom);
	void MAINWINDOWAPI   script_Camera_Reset(int id, char* type);

	void MAINWINDOWAPI  script_Properties_planeOrigin(int id, char* type, int obj_id, double x, double y, double z);
	void MAINWINDOWAPI	script_Properties_planeNormal(int id, char* type, int obj_id, double x, double y, double z);
	void MAINWINDOWAPI	script_Properties_planeVisible(int id, char* type, int obj_id, int a);
	void MAINWINDOWAPI	script_Properties_insideOut(int id, char* type, int obj_id, int a);

	void MAINWINDOWAPI	script_Properties_contourColumn(int id, char* type, int obj_id, char* val);
	void MAINWINDOWAPI	script_Properties_contourValue(int id, char* type, int obj_id, double val);
	void MAINWINDOWAPI	script_Properties_contour_reflection(int id, char* type, int obj_id, int aaa);
	void MAINWINDOWAPI	script_Properties_contour_reflectionAxes(int id, char* type, int obj_id, int val);;
		
	void MAINWINDOWAPI	script_Properties_reflectionAxes(int id, char* type, int obj_id, int reflection_axis);

	void MAINWINDOWAPI	script_Properties_smooth(int id, char* type, int obj_id, int smotype, double coef);

	void MAINWINDOWAPI  script_Properties_streamline_vector(int id, char* type, int obj_id, char* val);
	void MAINWINDOWAPI	script_Properties_streamline_integration_direction(int id, char* type, int obj_id, int val);
	void MAINWINDOWAPI	script_Properties_streamline_integration_type(int id, char* type, int obj_id, int val);
	void MAINWINDOWAPI	script_Properties_streamline_integration_stepUnit(int id, char* type, int obj_id, int val);
	void MAINWINDOWAPI	script_Properties_streamline_integration_initStepLen(int id, char* type, int obj_id, double val);
	void MAINWINDOWAPI	script_Properties_streamline_integration_miniStepLen(int id, char* type, int obj_id, double val);
	void MAINWINDOWAPI	script_Properties_streamline_integration_maxiStepLen(int id, char* type, int obj_id, double val);
	void MAINWINDOWAPI	script_Properties_streamline_stream_maxiSteps(int id, char* type, int obj_id, int val);
	void MAINWINDOWAPI	script_Properties_streamline_stream_maxiStreamLen(int id, char* type, int obj_id, double val);
	void MAINWINDOWAPI	script_Properties_streamline_stream_terminalSpeed(int id, char* type, int obj_id, double val);
	void MAINWINDOWAPI	script_Properties_streamline_stream_maxiError(int id, char* type, int obj_id, double val);
	void MAINWINDOWAPI	script_Properties_streamline_seeds_type(int id, char* type, int obj_id, int val);
	void MAINWINDOWAPI	script_Properties_streamline_seeds_mPoint(int id, char* type, int obj_id, double val0, double val1, double val2);
	void MAINWINDOWAPI	script_Properties_streamline_seeds_num_points(int id, char* type, int obj_id, int val);
	void MAINWINDOWAPI	script_Properties_streamline_seeds_radius(int id, char* type, int obj_id, double val);
	void MAINWINDOWAPI	script_Properties_streamline_vorticity(int id, char* type, int obj_id, bool val);
	void MAINWINDOWAPI	script_Properties_streamline_interpolatorType(int id, char* type, int obj_id, int val);
	void MAINWINDOWAPI	script_Properties_streamline_surface_streamLines(int id, char* type, int obj_id, bool val);
	void MAINWINDOWAPI	script_Properties_streamline_reflection(int id, char* type, int obj_id, bool val);
	void MAINWINDOWAPI	script_Properties_streamline_reflectionAxes(int id, char* type, int obj_id, int val);


	
	
	
	
}


#endif