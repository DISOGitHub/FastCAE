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
    self.solidList = dict()
    self.surfaceList = dict()
    self.pointsSizeList = list()
    self.fieldsList = list()
#    self.physicalsList = list()
    self.method = -1
    self.dim = -1
    self.isGridCoplanar=False
    self.selectall=False
    self.selectvisible=False
    self.cleanGeome = False    

  def appendSurface(self, geoset, index):
    self.surfaceList.setdefault(geoset, set()).add(index)  

  def appendSolid(self, geoset, index):
    self.solidList.setdefault(geoset, set()).add(index)

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
    self.cleanGeome = True

  def setSmoothIteration(self, it):
    self.smoothIteration = it
    
  def setGridCoplanar(self):
    self.isGridCoplanar = True
  
  def appendSizeFiled(self,px,py,pz,pv):
    pointsList = [px,py,pz,pv]
    self.pointsSizeList.append(pointsList)
    
  def appendBoxFields(self,tp,v1,v2,v3,v4,v5,v6,v7,v8,v9,bg):
    boxlist = [tp,v1,v2,v3,v4,v5,v6,v7,v8,v9,bg]
    self.fieldsList.append(boxlist)
    
  def appendBallFields(self,tp,v1,v2,v3,v4,v5,v6,v7,bg):
    balllist = [tp,v1,v2,v3,v4,v5,v6,v7,bg]
    self.fieldsList.append(balllist)
    
  def appendCylinderFields(self,tp,v1,v2,v3,v4,v5,v6,v7,v8,v9,bg):
    cylinderlist = [tp,v1,v2,v3,v4,v5,v6,v7,v8,v9,bg]
    self.fieldsList.append(cylinderlist)
    
#  def appendPhysicals(self,type,name,geoid,index):
#    physicallist = [type,name,geoid,index]
#    self.physicalsList.append(physicallist)
    
  def selectedAll(self):
    self.selectall = True
    
  def selectedVisible(self):
    self.selectvisible = True
    
  def setDim(self,dim):
    self.dim = dim

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
    
    fields = ""
    for fieldlist in self.fieldsList:
      fieldstr = ""
      for field in fieldlist:
        fieldstr = fieldstr + str(field) + ","
      fieldstr = fieldstr[:-1]
      fields = fields + fieldstr + ";"
    fields = fields[:-1]
    fieldsStr = bytes(fields, encoding = 'utf-8')
    
    
#    physicals = ""
#    for grouplist in self.physicalsList:
#      groupstr = ""
#      for group in grouplist:
#        groupstr = groupstr + str(group) + ","
#      groupstr = groupstr[:-1]
#      physicals = physicals + groupstr + ";"
#    physicals = physicals[:-1]
#    physicalsStr = bytes(physicals, encoding = 'utf-8')
    
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
                               c_double(self.minSize), c_double(self.maxSize), c_bool(self.cleanGeome),c_bool(self.isGridCoplanar),\
                               pointsizestr,fieldsStr,c_bool(self.selectall),c_bool(self.selectvisible))

    elif self.dim == 3:
      keyList = self.solidList.keys()
      strcom = ""
      for key in keyList:
        setstr = ""
        values = self.solidList.get(key)
        for v in values:
          setstr = setstr + str(v) + ","
        setstr = str(key) + ":" + setstr[:-1]
        strcom = strcom + setstr + ";"
      strcom = strcom[:-1]
      objstr = bytes(strcom, encoding='utf-8')
      gmshPlugin.GenerateMesh3D(objstr, eletypestr, c_int(self.elementOrder), c_int(self.method), c_double(self.sizeFactor), \
                                c_double(self.minSize), c_double(self.maxSize), c_bool(self.cleanGeome),c_bool(self.isGridCoplanar),\
                                pointsizestr,fieldsStr,c_bool(self.selectall),c_bool(self.selectvisible))


    del self