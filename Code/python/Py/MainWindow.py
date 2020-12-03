#-------关联C++库---------------
import ctypes  
import platform
from ctypes import *
system = platform.system()
if system == "Windows":
  pre = "./"
  suff = ".dll"
else:
  pre = "./lib"
  suff = ".so"

libfile = ctypes.cdll.LoadLibrary
filename = pre+"MainWindow"+suff

mw = libfile(filename)
#---------------------------------
#--------------New_script_FilterClip---------

#-------定义函数------------------
def showFastCAE():
	mw.showFastCAE()
	pass
	
def undo():
	mw.undo()
	pass
	
def redo():
	mw.redo()
	pass
    
def clearData():
    mw.clearData()
    pass
    
def updateInterface():
    mw.updateInterface()
    pass

def importMesh(filename,suffix,modelID):
  str = bytes(filename,encoding='utf-8')
  suf = bytes(suffix,encoding='utf-8')
  mw.importMesh(str,suf,modelID)
  pass
  
def exportMesh(filename,suffix,modelID):
  str = bytes(filename,encoding='utf-8')
  suf = bytes(suffix,encoding='utf-8')
  mw.exportMesh(str,suf,modelID)
  pass

def importGeometry(filename):
  str = bytes(filename,encoding='utf-8')
  mw.importGeometry(str)
  pass
  
def exportGeometry(filename):
  str = bytes(filename,encoding='utf-8')
  mw.exportGeometry(str)
  pass

def openProjectFile(filename):
  str = bytes(filename,encoding='utf-8')
  mw.openProjectFile(str)
  pass

def saveProjectFile(filename):
  str = bytes(filename,encoding='utf-8')
  mw.saveProjectFile(str)
  pass

def saveImage(w,h,id,win,file):
  wi = bytes(win,encoding='utf-8')
  f = bytes(file,encoding='utf-8')
  mw.saveImage(w,h,id,wi,f)
  pass
  
def setView(id,win,view):
  wi = bytes(win,encoding='utf-8')
  vi = bytes(view,encoding='utf-8')
  mw.setView(id,wi,vi)
  pass
  
def setViewRandValue(id,win,x1,x2,x3,y1,y2,y3,z1,z2,z3):
  wi=bytes(win,encoding='utf-8')
  mw.setViewRandValue(id,wi,x1,x2,x3,y1,y2,y3,z1,z2,z3)
  pass
  
def openPost3D():
  mw.openPost3D()
  pass
  
def openPost2D():
  mw.openPost2D()
  pass

def openPreWindow():
  mw.openPreWindow()
  pass
  
def solveProject(projectIndex,solverIndex):
  mw.solveProject(projectIndex,solverIndex)
  pass

def script_openFile(id, type, file):
  stype = bytes(type,encoding='utf-8')
  sfile = bytes(file,encoding='utf-8')
  mw.script_openFile(id, stype, sfile)
  pass
  
def script_applyClicked(id, type):
  stype=bytes(type,encoding='utf-8')
  mw.script_applyClicked(id, stype)
  pass
  
def createSet(name, type, idstring):
  name = bytes(name,encoding='utf-8')
  idstring = bytes(idstring,encoding='utf-8') 
  type = bytes(type,encoding='utf-8')
  mw.createSet(name,type,idstring)
  pass
  
def createGeoComponent(name, type, strgIDs, strItemIDs):
  name = bytes(name,encoding='utf-8')
  type = bytes(type,encoding='utf-8') 
  strgIDs = bytes(strgIDs,encoding='utf-8')
  strItemIDs = bytes(strItemIDs,encoding='utf-8')
  mw.createGeoComponent(name, type, strgIDs, strItemIDs)
  pass
  
def createVTKTransform(componentIds, rotate, moveLocation, scale):  
  componentIds = bytes(componentIds,encoding='utf-8')
  rotate = bytes(rotate,encoding='utf-8')
  moveLocation = bytes(moveLocation,encoding='utf-8') 
  scale = bytes(scale,encoding='utf-8')
  mw.createVTKTransform(componentIds, rotate, moveLocation, scale)
  pass
  
def findConplanarPorC(seedType, seedId, minAngle, kernalId, setName):  
  seedType = bytes(seedType, encoding='utf-8')
  setName = bytes(setName, encoding='utf-8')
  minAngle = c_double(minAngle)
  mw.findConplanarPorC(seedType, seedId, minAngle, kernalId, setName)
  pass
  
def script_Properties_Opacity(id, type, obj_id, mOpacity):
  type=bytes(type,encoding='utf-8')
  mOpacity=c_double(mOpacity);
  mw.script_Properties_Opacity(id, type, obj_id, mOpacity)
  pass

def script_Properties_colorColumn(id, type, obj_id, mColorColumnStyle):
  type=bytes(type,encoding='utf-8')
  mColorColumnStyle=bytes(mColorColumnStyle,encoding='utf-8')
  mw.script_Properties_colorColumn(id, type, obj_id, mColorColumnStyle)
  pass

def script_Properties_scalarBarTitle(id, type, obj_id, colName, m_title):
  type=bytes(type,encoding='utf-8')
  colName=bytes(colName,encoding='utf-8')
  m_title=bytes(m_title,encoding='utf-8')
  mw.script_Properties_scalarBarTitle(id, type, obj_id, colName, m_title)
  pass
 
def script_Properties_scalarBarFontSize(id, type, obj_id, colName, m_fontSize):
  type=bytes(type,encoding='utf-8')
  colName=bytes(colName,encoding='utf-8')
  mw.script_Properties_scalarBarFontSize(id, type, obj_id, colName, m_fontSize)
  pass

def script_Properties_scalarBarNumLables(id, type, obj_id, colName, m_numLables):
  type=bytes(type,encoding='utf-8')
  colName=bytes(colName,encoding='utf-8')
  mw.script_Properties_scalarBarNumLables(id, type, obj_id, colName, m_numLables)
  pass

def script_Properties_lineWidth(id, type, obj_id, mLineWidth):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_lineWidth(id, type, obj_id, mLineWidth)
  pass

def script_Properties_pointSize(id, type, obj_id, mPointSize):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_pointSize(id, type, obj_id, mPointSize)
  pass

def script_Properties_translate(id, type, obj_id, x, y, z):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_translate(id, type, obj_id, x, y, z)
  pass

def script_Properties_origin(id, type, obj_id, x, y, z):
  type=bytes(type,encoding='utf-8')
  x=c_double(x)
  y=c_double(y)
  z=c_double(z)
  mw.script_Properties_origin(id, type, obj_id, x, y, z)
  pass

def script_Properties_scale(id, type, obj_id, x, y, z):
  type=bytes(type,encoding='utf-8')
  x=c_double(x)
  y=c_double(y)
  z=c_double(z)
  mw.script_Properties_scale(id, type, obj_id, x, y, z)
  pass

def script_Properties_orientation(id, type, obj_id, x, y, z):
  type=bytes(type,encoding='utf-8')
  x=c_double(x)
  y=c_double(y)
  z=c_double(z)
  mw.script_Properties_orientation(id, type, obj_id, x, y, z)
  pass

def script_Properties_representation(id, type, obj_id, m_enum_representationtype):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_representation(id, type, obj_id, m_enum_representationtype)
  pass

def script_Properties_specular(id, type, obj_id, mSpecular):
  type=bytes(type,encoding='utf-8')
  mSpecular=c_double(mSpecular)
  mw.script_Properties_specular(id, type, obj_id, mSpecular)
  pass

def script_Properties_diffuse(id, type, obj_id, mDiffuse):
  type=bytes(type,encoding='utf-8')
  mDiffuse=c_double(mDiffuse)
  mw.script_Properties_diffuse(id, type, obj_id, mDiffuse)
  pass

def script_Properties_ambient(id, type, obj_id, mAmbient):
  type=bytes(type,encoding='utf-8')
  mAmbient=c_double(mAmbient)
  mw.script_Properties_ambient(id, type, obj_id, mAmbient)
  pass

def script_Properties_specularPower(id, type, obj_id, mSpecularPower):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_specularPower(id, type, obj_id, mSpecularPower)
  pass

def script_Properties_specularColor(id, type, obj_id, r, g, b):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_specularColor(id, type, obj_id, r, g, b)
  pass                          
 
def script_Properties_solidColor(id, type, obj_id, r, g, b):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_solidColor(id, type, obj_id, r, g, b)
  pass

def script_Properties_edgeColor(id, type, obj_id, r, g, b):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_edgeColor(id, type, obj_id, r, g, b)
  pass

def script_Properties_interpolation(id, type, obj_id, m_enum_interpolationtype):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_interpolation(id, type, obj_id, m_enum_interpolationtype)
  pass

def script_Properties_Flag_scalarBar(id, type, obj_id, mColorColumnStyle):
  type=bytes(type,encoding='utf-8')
  mColorColumnStyle=bytes(mColorColumnStyle,encoding='utf-8')
  mw.script_Properties_Flag_scalarBar(id, type, obj_id, mColorColumnStyle)
  pass

def script_Properties_EnableOpacityMap(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_EnableOpacityMap(id, type, obj_id, val)
  pass

def script_Properties_visible(id, type, obj_id, flag_show_actors):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_visible(id, type, obj_id, flag_show_actors)
  pass

def script_Properties_show_scalarBars(id, type, obj_id, mScalarBarVisible):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_show_scalarBars(id, type, obj_id, mScalarBarVisible)
  pass

def script_Properties_show_cubeAxes(id, type, obj_id, flag_cubeAxes):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_show_cubeAxes(id, type, obj_id, flag_cubeAxes)
  pass

def script_Properties_scalarBarPosition(id, type, obj_id, colName, tep_orietation, pos0, pos1, pos2, pos3):
  type=bytes(type,encoding='utf-8')
  pos0=c_double(pos0)
  pos1=c_double(pos1)
  pos2=c_double(pos2)
  pos3=c_double(pos3)
  colName=bytes(colName,encoding='utf-8')
  mw.script_Properties_scalarBarPosition(id, type, obj_id, colName, tep_orietation, pos0, pos1, pos2, pos3)
  pass
  
def script_FilterClip(id, type, obj_id):
  type=bytes(type,encoding='utf-8')
  mw.script_FilterClip(id, type, obj_id)
  pass
   
def script_FilterSlice(id, type, obj_id):
  type=bytes(type,encoding='utf-8')
  mw.script_FilterSlice(id, type, obj_id)
  pass
    
def script_FilterContour(id, type, obj_id):
  type=bytes(type,encoding='utf-8')
  mw.script_FilterContour(id, type, obj_id)
  pass
    
def script_FilterVector(id, type, obj_id):
  type=bytes(type,encoding='utf-8')
  mw.script_FilterVector(id, type, obj_id)
  pass
    
def script_FilterReflection(id, type, obj_id):
  type=bytes(type,encoding='utf-8')
  mw.script_FilterReflection(id, type, obj_id)
  pass
    
def script_FilterSmooth(id, type, obj_id):
  type=bytes(type,encoding='utf-8')
  mw.script_FilterSmooth(id, type, obj_id)
  pass
    
def script_FilterStreamLine(id, type, obj_id):
  type=bytes(type,encoding='utf-8')
  mw.script_FilterStreamLine(id, type, obj_id)
  pass
#######  ###############################################
def script_Properties_vector_GlyphVector(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=bytes(val,encoding='utf-8')
  mw.script_Properties_vector_GlyphVector(id, type, obj_id, val)
  pass
    
def script_Properties_vector_scalar(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=bytes(val,encoding='utf-8')
  mw.script_Properties_vector_scalar(id, type, obj_id, val)
  pass
     
def script_Properties_vector_normal(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=bytes(val,encoding='utf-8')
  mw.script_Properties_vector_normal(id, type, obj_id, val)
  pass
     
def script_Properties_vector_numPoints(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_vector_numPoints(id, type, obj_id, val)
  pass
     
def script_Properties_vector_glyph_type(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_vector_glyph_type(id, type, obj_id, val)
  pass
     
def script_Properties_vector_glyph_tipRes(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_vector_glyph_tipRes(id, type, obj_id, val)
  pass
     
def script_Properties_vector_glyph_tipRad(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=c_double(val)
  mw.script_Properties_vector_glyph_tipRad(id, type, obj_id, val)
  pass
     
def script_Properties_vector_glyph_tipLen(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=c_double(val)
  mw.script_Properties_vector_glyph_tipLen(id, type, obj_id, val)
  pass
        
def script_Properties_vector_glyph_shaftRes(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_vector_glyph_shaftRes(id, type, obj_id, val)
  pass
     
def script_Properties_vector_glyph_shaftRad(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=c_double(val)
  mw.script_Properties_vector_glyph_shaftRad(id, type, obj_id, val)
  pass
     
def script_Properties_view_backgroundType(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_view_backgroundType(id, type, obj_id, val)
  pass
     
def script_Properties_view_backgroundColor(id, type, obj_id, red, green, blue):
  mw.script_Properties_view_backgroundColor(id, type, obj_id, red, green, blue)
  pass
     
def script_Properties_view_background2Color(id, type, obj_id, red, green, blue):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_view_background2Color(id, type, obj_id, red, green, blue)
  pass
     
def script_Properties_view_axesVisible(id, type, a):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_view_axesVisible(id, type, a)
  pass
     
def script_Properties_view_cameraParallel(id, type, a):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_view_cameraParallel(id, type, a)
  pass
     
def script_Properties_view_interaction(id, type, a):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_view_interaction(id, type, a)
  pass
     
def script_Properties_renderView(id, type):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_renderView(id, type)
  pass
     
def script_Camera_Position(id, type, pos0, pos1, pos2):
  type=bytes(type,encoding='utf-8')
  pos0=c_double(pos0)
  pos1=c_double(pos1)
  pos2=c_double(pos2)
  mw.script_Camera_Position(id, type, pos0, pos1, pos2)
  pass
     
def script_Camera_FocalPoint(id, type, focalPoint0, focalPoint1, focalPoint2):
  type=bytes(type,encoding='utf-8')
  focalPoint0=c_double(focalPoint0)
  focalPoint1=c_double(focalPoint1)
  focalPoint2=c_double(focalPoint2)
  mw.script_Camera_FocalPoint(id, type, focalPoint0, focalPoint1, focalPoint2)
  pass
     
def script_Camera_ClippingRange(id, type, clippingRange0, clippingRange1):
  type=bytes(type,encoding='utf-8')
  clippingRange0=c_double(clippingRange0)
  clippingRange1=c_double(clippingRange1)
  mw.script_Camera_ClippingRange(id, type, clippingRange0, clippingRange1)
  pass
     
def script_Camera_ViewUp(id, type, viewup0, viewup1, viewup2):
  type=bytes(type,encoding='utf-8')
  viewup0=c_double(viewup0)
  viewup1=c_double(viewup1)
  viewup2=c_double(viewup2)
  mw.script_Camera_ViewUp(id, type, viewup0, viewup1, viewup2)
  pass
     
def script_Camera_ViewAngle(id, type, angle):
  type=bytes(type,encoding='utf-8')
  angle=c_double(angle)
  mw.script_Camera_ViewAngle(id, type, angle)
  pass
     
def script_Camera_Zoom(id, type, zoom):
  type=bytes(type,encoding='utf-8')
  zoom=c_double(zoom)
  mw.script_Camera_Zoom(id, type, zoom)
  pass
     
def script_Camera_Reset(id, type):
  type=bytes(type,encoding='utf-8')
  mw.script_Camera_Reset(id, type,)
  pass
     
def script_Properties_planeOrigin(id, type, obj_id, x, y, z):
  type=bytes(type,encoding='utf-8')
  x=c_double(x)
  y=c_double(y)
  z=c_double(z)
  mw.script_Properties_planeOrigin(id, type, obj_id, x, y, z)
  pass
     
def script_Properties_planeNormal(id, type, obj_id, x, y, z):
  type=bytes(type,encoding='utf-8')
  x=c_double(x)
  y=c_double(y)
  z=c_double(z)
  mw.script_Properties_planeNormal(id, type, obj_id, x, y, z)
  pass
  
def script_Properties_planeVisible(id, type, obj_id, a):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_planeVisible(id, type, obj_id, a)
  pass    
def script_Properties_insideOut(id, type, obj_id, a):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_insideOut(id, type, obj_id, a)
  pass
     
def script_Properties_contourColumn(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=bytes(val,encoding='utf-8')
  mw.script_Properties_contourColumn(id, type, obj_id, val)
  pass
     
def script_Properties_contourValue(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=c_double(val)
  mw.script_Properties_contourValue(id, type, obj_id, val)
  pass
     
def script_Properties_contour_reflection(id, type, obj_id, aaa):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_contour_reflection(id, type, obj_id, aaa)
  pass
     
def script_Properties_contour_reflectionAxes(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_contour_reflectionAxes(id, type, obj_id, val)
  pass
     
def script_Properties_reflectionAxes(id, type, obj_id, reflection_axis):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_reflectionAxes(id, type, obj_id, reflection_axis)
  pass
     
def script_Properties_smooth(id, type, obj_id, smotype, coef):
  type=bytes(type,encoding='utf-8')
  coef=c_double(coef)
  mw.script_Properties_smooth(id, type, obj_id, smotype, coef)
  pass
     
def script_Properties_streamline_vector(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=bytes(val,encoding='utf-8')
  mw.script_Properties_streamline_vector(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_integration_direction(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_streamline_integration_direction(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_integration_type(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_streamline_integration_type(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_integration_stepUnit(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_streamline_integration_stepUnit(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_integration_initStepLen(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=c_double(val)
  mw.script_Properties_streamline_integration_initStepLen(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_integration_miniStepLen(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=c_double(val)
  mw.script_Properties_streamline_integration_miniStepLen(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_integration_maxiStepLen(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=c_double(val)
  mw.script_Properties_streamline_integration_maxiStepLen(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_stream_maxiSteps(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_streamline_stream_maxiSteps(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_stream_maxiStreamLen(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=c_double(val)
  mw.script_Properties_streamline_stream_maxiStreamLen(id, type, obj_id, val)
  pass
 ###########  
  
def script_Properties_streamline_stream_terminalSpeed(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=c_double(val)
  mw.script_Properties_streamline_stream_terminalSpeed(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_stream_maxiError(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=c_double(val)
  mw.script_Properties_streamline_stream_maxiError(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_seeds_type(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_streamline_seeds_type(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_seeds_mPoint(id, type, obj_id, val0, val1, val2):
  type=bytes(type,encoding='utf-8')
  val0=c_double(val0)
  val1=c_double(val1)
  val2=c_double(val2)
  mw.script_Properties_streamline_seeds_mPoint(id, type, obj_id, val0, val1, val2)
  pass
     
def script_Properties_streamline_seeds_num_points(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_streamline_seeds_num_points(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_seeds_radius(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  val=c_double(val)
  mw.script_Properties_streamline_seeds_radius(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_vorticity(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_streamline_vorticity(id, type, obj_id, val)
  pass
def script_Properties_streamline_interpolatorType(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_streamline_interpolatorType(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_surface_streamLines(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_streamline_surface_streamLines(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_reflection(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_streamline_reflection(id, type, obj_id, val)
  pass
     
def script_Properties_streamline_reflectionAxes(id, type, obj_id, val):
  type=bytes(type,encoding='utf-8')
  mw.script_Properties_streamline_reflectionAxes(id, type, obj_id, val)
  pass
 
  
  
  
  
  
  
