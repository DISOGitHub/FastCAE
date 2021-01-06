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
filename = pre+"Geometry"+suff
lib = libfile(filename)

lib.getGeometrySetNameByIndex.restype=ctypes.c_char_p
lib.getGeometrySetNameByID.restype=ctypes.c_char_p

def getGeometrySetCount():
  c = lib.getGeometrySetCount()
  return c
  
def isEmpty():
  b = bool(geometry.isEmpty())
  return b
  
def getGeometrySetNameByIndex(index):
  name = lib.getGeometrySetNameByIndex(index)
  nstr = name.decode()
  return nstr
  
def getGeometrySetNameByID(sid):
  name = lib.getGeometrySetNameByID(sid)
  nstr = name.decode()
  return nstr
   
def getIndexOfGeoometrySet(name):
  setname = bytes(name,encoding='utf-8')
  index = lib.getIndexOfGeoometrySet(setname)
  return index
  
def getIDOfGeoometrySet(name):
  setname = bytes(name,encoding='utf-8')
  sid = lib.getIDOfGeoometrySet(setname)
  return sid
  
def getIDByIndex(index):
  sid = lib.getIDByIndex(index)
  return sid

def getIndexByID(sid):
  index = lib.getIndexByID(sid)
  return index
  
def removeGeometrySetByIndex(index):
  lib.removeGeometrySetByIndex(index)
  pass
  
def removeGeometrySetByID(sid):
  lib.removeGeometrySetByID(sid)
  pass
  
def clear():
  lib.clear()
  pass