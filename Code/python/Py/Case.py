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
filename = pre+"ModelData"+suff

md = libfile(filename)

import ControlPanel
#---------------------------------

#-------定义函数------------------
def setMaterial(modelID, strcpIDs ,mid):
  strcpIDs = bytes(strcpIDs,encoding='utf-8')
  md.setMaterial(modelID, strcpIDs ,mid)
  pass

def importComponents(caseId,addcomponentsId):
  addcomponentsId = bytes(addcomponentsId,encoding='utf-8')
  md.importComponents(caseId,addcomponentsId)
  ControlPanel.updateComponentSubTree(caseId)
  ControlPanel.updateBCSubTree(caseId)
  pass
  
def importGeometry(caseId,addcomponentsId):
  addcomponentsId = bytes(addcomponentsId,encoding='utf-8')
  md.importGeometry(caseId,addcomponentsId)
  ControlPanel.updateGeometrySubTree(caseId)
  pass
  
def addBC(caseId,id,bctypetostring):
  bctypetostring = bytes(bctypetostring,encoding='utf-8')
  md.addBC(caseId,id,bctypetostring)
  ControlPanel.updateBCSubTree(caseId)
  pass
def objValChanged(value,describe,stype):
  describe = bytes(describe,encoding='utf-8')
  stype = bytes(stype,encoding='utf-8')
  md.objValChanged(value,describe,stype)
  pass
  
def setValue(caseId,svariable,stype,sValue):
  variable = bytes(svariable,encoding='utf-8')
  type = bytes(stype,encoding='utf-8')
  value = bytes(sValue,encoding='utf-8')
  md.setValue(caseId, variable, type, value)
  pass
  
def setBCValue(caseId,index,svariable,stype,sValue):
  variable = bytes(svariable,encoding='utf-8')
  type = bytes(stype,encoding='utf-8')
  value = bytes(sValue,encoding='utf-8')
  md.setBCValue(caseId, index,variable, type, value)
  pass
  
  