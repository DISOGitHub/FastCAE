#-------关联C++库---------------
import ctypes  
import platform
system = platform.system()
if system == "Windows":
  pre = "./"
  suff = ".dll"
else:
  pre = "./lib"
  suff = ".so"

libfile = ctypes.cdll.LoadLibrary
filename = pre+"GraphicsAnalyse"+suff
postPro = libfile(filename)

import MainWindow
#---------------------------------

#-------定义函数------------------
def script_openFile(id, type, file):
  MainWindow.script_openFile(id, type, file)
  pass
  
def script_applyClicked(id, type):
  MainWindow.script_applyClicked(id, type)
  pass
  
def script_Properties_Opacity(id, type, obj_id, mOpacity):
  MainWindow.script_Properties_Opacity(id, type, obj_id, mOpacity)
  pass
  
def script_Properties_colorColumn(id, type, obj_id, mColorColumnStyle):
  MainWindow.script_Properties_colorColumn(id, type, obj_id, mColorColumnStyle)
  pass
  
def script_Properties_scalarBarTitle(id, type, obj_id, colName, m_title):
  MainWindow.script_Properties_scalarBarTitle(id, type, obj_id, colName, m_title)
  pass
  
  
def script_Properties_scalarBarFontSize(id, type, obj_id, colName, m_fontSize):
  MainWindow.script_Properties_scalarBarFontSize(id, type, obj_id, colName, m_fontSize)
  pass
  
def script_Properties_scalarBarNumLables(id, type, obj_id, colName, m_numLables):
  MainWindow.script_Properties_scalarBarNumLables(id, type, obj_id, colName, m_numLables)
  pass
  
def script_Properties_lineWidth(id, type, obj_id, mLineWidth):
  MainWindow.script_Properties_lineWidth(id, type, obj_id, mLineWidth)
  pass
   
def script_Properties_pointSize(id, type, obj_id, mPointSize):
  MainWindow.script_Properties_pointSize(id, type, obj_id, mPointSize)
  pass   
  
def script_Properties_translate(id, type, obj_id, x, y, z):
  MainWindow.script_Properties_translate(id, type, obj_id, x, y, z)
  pass
  
def script_Properties_origin(id, type, obj_id, x, y, z):
  MainWindow.script_Properties_origin(id, type, obj_id, x, y, z)
  pass
  
def script_Properties_scale(id, type, obj_id, x, y, z):
  MainWindow.script_Properties_scale(id, type, obj_id, x, y, z)
  pass
  
def script_Properties_orientation(id, type, obj_id, x, y, z):
  MainWindow.script_Properties_orientation(id, type, obj_id, x, y, z)
  pass
  
def script_Properties_representation(id, type, obj_id, m_enum_representationtype):
  MainWindow.script_Properties_representation(id, type, obj_id, m_enum_representationtype)
  pass
  
def script_Properties_specular(id, type, obj_id, mSpecular):
  MainWindow.script_Properties_specular(id, type, obj_id, mSpecular)
  pass
  
def script_Properties_diffuse(id, type, obj_id, mDiffuse):
  MainWindow.script_Properties_diffuse(id, type, obj_id, mDiffuse)
  pass
  
def script_Properties_ambient(id, type, obj_id, mAmbient):
  MainWindow.script_Properties_ambient(id, type, obj_id, mAmbient)
  pass
  
def script_Properties_specularPower(id, type, obj_id, mSpecularPower):
  MainWindow.script_Properties_specularPower(id, type, obj_id, mSpecularPower)
  pass
  
def script_Properties_specularColor(id, type, obj_id, r, g, b):
  MainWindow.script_Properties_specularColor(id, type, obj_id, r, g, b)
  pass
  
def script_Properties_solidColor(id, type, obj_id, r, g, b):
  MainWindow.script_Properties_solidColor(id, type, obj_id, r, g, b)
  pass
  
def script_Properties_edgeColor(id, type, obj_id, r, g, b):
  MainWindow.script_Properties_edgeColor(id, type, obj_id, r, g, b)
  pass
  
def script_Properties_interpolation(id, type, obj_id, m_enum_interpolationtype):
  MainWindow.script_Properties_interpolation(id, type, obj_id, m_enum_interpolationtype)
  pass
  
def script_Properties_Flag_scalarBar(id, type, obj_id, mColorColumnStyle):
  MainWindow.script_Properties_Flag_scalarBar(id, type, obj_id, mColorColumnStyle)
  pass
  
def script_Properties_EnableOpacityMap(id, type, obj_id, val):
  MainWindow.script_Properties_EnableOpacityMap(id, type, obj_id, val)
  pass
  
def script_Properties_visible(id, type, obj_id, flag_show_actors):
  MainWindow.script_Properties_visible(id, type, obj_id, flag_show_actors)
  pass
  
def script_Properties_show_scalarBars(id, type, obj_id, mScalarBarVisible):
  MainWindow.script_Properties_show_scalarBars(id, type, obj_id, mScalarBarVisible)
  pass
  
def script_Properties_show_cubeAxes(id, type, obj_id, flag_cubeAxes):
  MainWindow.script_Properties_show_cubeAxes(id, type, obj_id, flag_cubeAxes)
  pass
  
def script_Properties_scalarBarPosition(id, type, obj_id, colName, tep_orietation, pos0, pos1, pos2, pos3):
  MainWindow.script_Properties_scalarBarPosition(id, type, obj_id,colName, tep_orietation, pos0, pos1, pos2, pos3)
  pass
  
def script_Clip(id, type, obj_id):
  MainWindow.script_FilterClip(id, type, obj_id)
  pass
   
def script_Slice(id, type, obj_id):
  MainWindow.script_FilterSlice(id, type, obj_id)
  pass
   
def script_Contour(id, type, obj_id):
  MainWindow.script_FilterContour(id, type, obj_id)
  pass
   
def script_Vector(id, type, obj_id):
  MainWindow.script_FilterVector(id, type, obj_id)
  pass
   
def script_Reflection(id, type, obj_id):
  MainWindow.script_FilterReflection(id, type, obj_id)
  pass
   
def script_Smooth(id, type, obj_id):
  MainWindow.script_FilterSmooth(id, type, obj_id)
  pass
   
def script_StreamLine(id, type, obj_id):
  MainWindow.script_FilterStreamLine(id, type, obj_id)
  pass
  
###################
   
def script_Vector_GlyphVector(id, type, obj_id, val):
  MainWindow.script_Properties_vector_GlyphVector(id, type, obj_id, val)
  pass
    
def script_Vector_scalar(id, type, obj_id, val):
  MainWindow.script_Properties_vector_scalar(id, type, obj_id, val)
  pass
     
def script_Vector_normal(id, type, obj_id, val):
  MainWindow.script_Properties_vector_normal(id, type, obj_id, val)
  pass
     
def script_Vector_numPoints(id, type, obj_id, val):
  MainWindow.script_Properties_vector_numPoints(id, type, obj_id, val)
  pass
     
def script_Vector_glyph_type(id, type, obj_id, val):
  MainWindow.script_Properties_vector_glyph_type(id, type, obj_id, val)
  pass
     
def script_Vector_glyph_tipRes(id, type, obj_id, val):
  MainWindow.script_Properties_vector_glyph_tipRes(id, type, obj_id, val)
  pass
     
def script_Vector_glyph_tipRad(id, type, obj_id, val):
  MainWindow.script_Properties_vector_glyph_tipRad(id, type, obj_id, val)
  pass
     
def script_Vector_glyph_tipLen(id, type, obj_id, val):
  MainWindow.script_Properties_vector_glyph_tipLen(id, type, obj_id, val)
  pass
     
def script_FilterStreamLine(id, type, obj_id):
  MainWindow.script_FilterStreamLine(id, type, obj_id)
  pass
     
def script_Vector_glyph_shaftRes(id, type, obj_id, val):
  MainWindow.script_Properties_vector_glyph_shaftRes(id, type, obj_id, val)
  pass
     
def script_Vector_glyph_shaftRad(id, type, obj_id, val):
  MainWindow.script_Properties_vector_glyph_shaftRad(id, type, obj_id, val)
  pass
     
def script_Properties_view_backgroundType(id, type, obj_id, val):
  MainWindow.script_Properties_view_backgroundType(id, type, obj_id, val)
  pass
     
def script_Properties_view_backgroundColor(id, type, obj_id, red, green, blue):
  MainWindow.script_Properties_view_backgroundColor(id, type, obj_id, red, green, blue)
  pass
     
def script_Properties_view_background2Color(id, type, obj_id, red, green, blue):
  MainWindow.script_Properties_view_background2Color(id, type, obj_id, red, green, blue)
  pass
     
def script_Properties_view_axesVisible(id, type, a):
  MainWindow.script_Properties_view_axesVisible(id, type, a)
  pass
     
def script_Properties_view_cameraParallel(id, type, a):
  MainWindow.script_Properties_view_cameraParallel(id, type, a)
  pass
     
def script_Properties_view_interaction(id, type, a):
  MainWindow.script_Properties_view_interaction(id, type, a)
  pass
     
def script_Properties_renderView(id, type):
  MainWindow.script_Properties_renderView(id, type)
  pass
     
def script_Camera_Position(id, type, pos0, pos1, pos2):
  MainWindow.script_Camera_Position(id, type, pos0, pos1, pos2)
  pass
     
def script_Camera_FocalPoint(id, type, focalPoint0, focalPoint1, focalPoint2):
  MainWindow.script_Camera_FocalPoint(id, type, focalPoint0, focalPoint1, focalPoint2)
  pass
     
def script_Camera_ClippingRange(id, type, clippingRange0, clippingRange1):
  MainWindow.script_Camera_ClippingRange(id, type, clippingRange0, clippingRange1)
  pass
     
def script_Camera_ViewUp(id, type, viewup0, viewup1, viewup2):
  MainWindow.script_Camera_ViewUp(id, type, viewup0, viewup1, viewup2)
  pass
     
def script_Camera_ViewAngle(id, type, angle):
  MainWindow.script_Camera_ViewAngle(id, type, angle)
  pass
     
def script_Camera_Zoom(id, type, zoom):
  MainWindow.script_Camera_Zoom(id, type, zoom)
  pass
     
def script_Camera_Reset(id, type,):
  MainWindow.script_Camera_Reset(id, type,)
  pass
     
def script_Properties_planeOrigin(id, type, obj_id, x, y, z):
  MainWindow.script_Properties_planeOrigin(id, type, obj_id, x, y, z)
  pass
def script_Properties_planeNormal(id, type, obj_id, x, y, z):
  MainWindow.script_Properties_planeNormal(id, type, obj_id, x, y, z)
  pass    
  
def script_Properties_planeVisible(id, type, obj_id, a):
  MainWindow.script_Properties_planeVisible(id, type, obj_id, a)
  pass 
  
def script_Properties_insideOut(id, type, obj_id, a):
  MainWindow.script_Properties_insideOut(id, type, obj_id, a)
  pass
     
def script_Contour_Column(id, type, obj_id, val):
  MainWindow.script_Properties_contourColumn(id, type, obj_id, val)
  pass
     
def script_Contour_value(id, type, obj_id, val):
  MainWindow.script_Properties_contourValue(id, type, obj_id, val)
  pass
     
def script_Contour_reflection(id, type, obj_id, aaa):
  MainWindow.script_Properties_contour_reflection(id, type, obj_id, aaa)
  pass
     
def script_Contour_reflectionAxes(id, type, obj_id, val):
  MainWindow.script_Properties_contour_reflectionAxes(id, type, obj_id, val)
  pass
     
def script_Properties_reflectionAxes(id, type, obj_id, reflection_axis):
  MainWindow.script_Properties_reflectionAxes(id, type, obj_id, reflection_axis)
  pass
     
def Smooth_smooth(id, type, obj_id, smotype, coef):
  MainWindow.script_Properties_smooth(id, type, obj_id, smotype, coef)
  pass
     
def script_Streamline_vector(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_vector(id, type, obj_id, val)
  pass
     
def script_Streamline_integration_type(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_integration_type(id, type, obj_id, val)
  pass
  
def script_Streamline_integration_direction(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_integration_direction(id, type, obj_id, val)
  pass
        
def script_Streamline_integration_stepUnit(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_integration_type(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_integration_stepUnit(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_integration_stepUnit(id, type, obj_id, val)
  pass
     
def script_Streamline_integration_initStepLen(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_integration_initStepLen(id, type, obj_id, val)
  pass
     
def script_Streamline_integration_miniStepLen(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_integration_miniStepLen(id, type, obj_id, val)
  pass
     
def script_Streamline_integration_maxiStepLen(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_integration_maxiStepLen(id, type, obj_id, val)
  pass
     
def script_Streamline_stream_maxiSteps(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_stream_maxiSteps(id, type, obj_id, val)
  pass
     
def script_Streamline_stream_maxiStreamLen(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_stream_maxiStreamLen(id, type, obj_id, val)
  pass
 ###########  
  
def script_Streamline_stream_terminalSpeed(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_stream_terminalSpeed(id, type, obj_id, val)
  pass
     
def script_Streamline_stream_maxiError(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_stream_maxiError(id, type, obj_id, val)
  pass
     
def script_Streamline_seeds_type(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_seeds_type(id, type, obj_id, val)
  pass
     
def script_Streamline_seeds_mPoint(id, type, obj_id, val0, val1, val2):
  MainWindow.script_Properties_streamline_seeds_mPoint(id, type, obj_id, val0, val1, val2)
  pass
     
def script_Streamline_seeds_num_points(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_seeds_num_points(id, type, obj_id, val)
  pass
     
def script_Streamline_seeds_radius(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_seeds_radius(id, type, obj_id, val)
  pass
     
def script_Streamline_vorticity(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_vorticity(id, type, obj_id, val)
  pass
def script_Streamline_interpolatorType(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_interpolatorType(id, type, obj_id, val)
  pass
     
def script_Streamline_surface_streamLines(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_surface_streamLines(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_reflection(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_reflection(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_reflectionAxes(id, type, obj_id, val):
  MainWindow.script_Properties_streamline_reflectionAxes(id, type, obj_id, val)
  pass
 
