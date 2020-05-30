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
filename = pre+"MeshData"+suff
lib = libfile(filename)

lib.getKernalNameByIndexPy.restype=ctypes.c_char_p
lib.getKernalNameByKidPy.restype=ctypes.c_char_p
lib.getSetNameByIndexPy.restype=ctypes.c_char_p
lib.getSetNameBySidPy.restype=ctypes.c_char_p

def getKernalCount():
  count = lib.getKernalCountPy()
  return count
  
def getKernalNameByIndex(index):
  name = lib.getKernalNameByIndexPy(index)
  pyname = name.decode()
  return pyname
  
def getKernalNameByKid(kid):
  name = lib.getKernalNameByKidPy(kid)
  pyname = name.decode()
  return pyname
   
def removeKernalByIndex(index):
  lib.removeKernalByIndexPy(index)
  pass
  
def removeKernalByKid(kid):
  lib.removeKernalByKidPy(kid)
  pass
  
def getSetCount():
  count = lib.getSetCountPy()
  return count

def getSetNameByIndex(index):
  name = lib.getSetNameByIndexPy(index)
  pyname = name.decode()
  return pyname
  
def getSetNameBySid(sid):
  name = lib.getSetNameBySidPy(sid)
  pyname = name.decode()
  return pyname
  
def removeSetByIndex(index):
  lib.removeSetByIndexPy(index)
  pass
  
def clear():
  lib.clearPy()
  pass
  
def generateDisplayDataSet():
  lib.generateDisplayDataSetPy()
  pass