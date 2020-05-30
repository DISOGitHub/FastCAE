#-------关联C++库---------------
import ctypes  
import platform
import os
from ctypes import *

system = platform.system()
if system == "Windows":
  pre = "./"
  suff = ".dll"
else:
  pre = "./lib"
  suff = ".so"

libfile = ctypes.cdll.LoadLibrary

currentPath = os.path.abspath(os.path.dirname(__file__))
plugdllPath = currentPath + "/" + pre+"GmshModule"+suff

gmshPlugin = libfile(plugdllPath)

filename = pre+"ConfigOptions"+suff
configOp = libfile(filename)

#---------------------------------
#--------------New_script_FilterClip---------


def setValue(index, name, stype, svalue):
  name = bytes(name,encoding='utf-8')
  stype=bytes(stype,encoding='utf-8')
  svalue=bytes(svalue,encoding='utf-8')
  configOp.setValue(index, name, stype, svalue)
  pass

class Gmsher:
  def __init__(self):
    self.solidList = list()
    self.surfaceList = dict()
    self.pointsSizeList = list()
    self.method = -1
    self.dim = -1
 #   self.cleanGeo = False
    self.gridCoplanar = False
    

  def appendSurface(self, geoset, index):
    self.surfaceList.setdefault(geoset, set()).add(index)
    self.dim = 2

  def appendSolid(self, id):
    self.solidList.append(id)
    self.dim = 3

  def setElementType(self, etype):
    self.elementType = etype

  def setElementOrder(self,order):
    self.elementOrder = order

  def setMethod(self,m):
    self.method = m

  def setSizeFactor(self, f):
    self.sizeFactor = f

  def setMinSize(self, min):
    self.minSize = min

  def setMaxSize(self, max):
    self.maxSize = max

  def cleanGeo(self):
    self.cleanGeo = True

  def setSmoothIteration(self, it):
    self.smoothIteration = it
    
  def setGridCoplanar(self, gridc):
    self.isGridCoplanar = gridc
  
  def appendSizeFiled(self,px,py,pz,pv):
    pointsList = [px,py,pz,pv]
    self.pointsSizeList.append(pointsList)

  def startGenerationThread(self):
    eletypestr = bytes(self.elementType,encoding='utf-8')
    
    pointsize = ""
    for pointlist in self.pointsSizeList:
      pointstr = ""
      for point in pointlist:
        pointstr = pointstr + str(point) + ","    
      pointstr = pointstr[:-1]
      pointsize = pointsize + pointstr + ";"
    pointsize = pointsize[:-1]
    pointsizestr = bytes(pointsize, encoding='utf-8')
    
    if self.dim == 2:
      keyList = self.surfaceList.keys()
      strcom = ""
      for key in keyList:
        setstr = ""
        values = self.surfaceList.get(key)
        for v in values:
          setstr = setstr + str(v) + ","
        setstr = str(key) + ":" + setstr[:-1]
        strcom = strcom + setstr + ";"
      strcom = strcom[:-1]
      surfacestr = bytes(strcom, encoding='utf-8')
      gmshPlugin.GenerateMesh2D(surfacestr, eletypestr, c_int(self.elementOrder), c_int(self.method), \
                               c_int(self.smoothIteration), c_double(self.sizeFactor), \
                               c_double(self.minSize), c_double(self.maxSize), c_bool(self.cleanGeo),c_bool(self.isGridCoplanar),\
                               pointsizestr)

    elif self.dim == 3:
      strbody = ""
      for body in self.solidList:
        strbody = strbody + str(body) + ","
      strbody = strbody[:-1]
      objstr = bytes(strbody, encoding='utf-8')
      gmshPlugin.GenerateMesh3D(objstr, eletypestr, c_int(self.elementOrder), c_int(self.method), c_double(self.sizeFactor), \
                                c_double(self.minSize), c_double(self.maxSize), c_bool(self.cleanGeo),c_bool(self.isGridCoplanar),\
                                pointsizestr)


    del self