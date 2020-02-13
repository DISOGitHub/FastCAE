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
filename = pre+"Material"+suff

ma = libfile(filename)

import ControlPanel
#---------------------------------

def setValue(Id,svariable,stype,sValue):
  variable = bytes(svariable,encoding='utf-8')
  type = bytes(stype,encoding='utf-8')
  value = bytes(sValue,encoding='utf-8')
  ma.setValue(Id, variable, type, value)
  pass