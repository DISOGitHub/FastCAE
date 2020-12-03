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
filename = pre+"MainWidgets"+suff

lib = libfile(filename)
#---------------------------------

#-------定义函数------------------
def createCase(name, typ):
  sname = bytes(name,encoding='utf-8')
  stype = bytes(typ,encoding='utf-8')
  lib.createCase(sname, stype)
  pass
  
def deleteCase(caseid):
  lib.deleteCase(caseid)
  pass

def startMesher(mesher):
  smesher = bytes(mesher,encoding='utf-8')
  lib.startMesher(smesher)
  pass
  
def importProject(id):
  lib.importProject(id)
  pass
  
def caseRename(pid,newname):
  newname = bytes(newname,encoding='utf-8')
  lib.caseRename(pid,newname)
  pass
  
def updateComponentSubTree(caseid):
  lib.updateComponentSubTree(caseid)
  pass
  
def updateBCSubTree(caseid):
  lib.updateBCSubTree(caseid)
  pass
  
def updatePostTree(caseid):
  lib.updatePostTree(caseid)
  pass
  
def updateGeometrySubTree(caseid):
  lib.updateGeometrySubTree(caseid)
  pass
  
def loadFromMaterialLib(namelist):
  namelist=bytes(namelist,encoding='utf-8')
  lib.loadFromMaterialLib(namelist)

def CreateMaterial(name,type):
  name=bytes(name,encoding='utf-8')
  type=bytes(type,encoding='utf-8')
  lib.CreateMaterial(name, type)
  pass
  
def RemoveFromMaterialLib(namelist):
  namelist=bytes(namelist,encoding='utf-8')
  lib.RemoveFromMaterialLib(namelist)
  pass
  
def DeleteMaterial(id, name):
  name=bytes(name,encoding='utf-8')
  lib.DeleteMaterial(id,name)
  pass
  
def AddMaterialToLib(id, name):
  name=bytes(name,encoding='utf-8')
  lib.AddMaterialToLib(id,name)
  pass

  


