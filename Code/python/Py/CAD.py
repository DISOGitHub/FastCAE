import platform
import ctypes
from ctypes import *

system = platform.system()
if system == "Windows":
  pre = "./"
  suff = ".dll"
else:
  pre = "./lib"
  suff = ".so"

libfile = ctypes.cdll.LoadLibrary
filename = pre+"GeometryCommand"+suff

command = libfile(filename)

class Box:
    def setName(self, name):
        self.name = name

    def setEditID(self, id):
        self.editID = id

    def setLocation(self, x,y,z):
        self.x = x
        self.y = y
        self.z = z

    def setPara(self, l,w,h):
        self.l = l
        self.w = w
        self.h = h

    def create(self):
        sname = bytes(self.name, encoding='utf-8')
        command.CreateBox(sname,c_double(self.x),c_double(self.y),c_double(self.z), \
                                c_double(self.l),c_double(self.w),c_double(self.h))
        del self

    def edit(self):
        command.EditBox(c_int(self.editID), c_double(self.x), c_double(self.y), c_double(self.z), \
                                            c_double(self.l), c_double(self.w), c_double(self.h))
        del self
class Cylinder:
    def setName(self, name):
        self.name = name

    def setEditID(self, id):
        self.editID = id

    def setLocation(self, x,y,z):
        self.x = x
        self.y = y
        self.z = z

    def setAxis(self, l,w,h):
        self.l = l
        self.w = w
        self.h = h
		
    def setRadius(self,radius):
        self.radius = radius
	
    def setLength(self,length):
        self.length = length
		
    def create(self):
        sname = bytes(self.name, encoding='utf-8')
        command.CreateCylinder(sname,c_double(self.x),c_double(self.y),c_double(self.z), \
                                c_double(self.l),c_double(self.w),c_double(self.h),\
								c_double(self.radius),\
								c_double(self.length))
        del self

    def edit(self):
        command.EditCylinder(c_int(self.editID), c_double(self.x), c_double(self.y), c_double(self.z), \
                                            c_double(self.l), c_double(self.w), c_double(self.h),\
											c_double(self.radius),\
											c_double(self.length))
        del self
class Cone:
    def setName(self, name):
        self.name = name

    def setEditID(self, id):
        self.editID = id

    def setLocation(self, x,y,z):
        self.x = x
        self.y = y
        self.z = z

    def setAxis(self, l,w,h):
        self.l = l
        self.w = w
        self.h = h
		
    def setRadius(self,radius,radius2):
        self.radius = radius
        self.radius2=radius2
	
    def setLength(self,length):
        self.length = length
		
    def create(self):
        sname = bytes(self.name, encoding='utf-8')
        command.CreateCone(sname,c_double(self.x),c_double(self.y),c_double(self.z), \
                                c_double(self.l),c_double(self.w),c_double(self.h),\
		c_double(self.radius),c_double(self.radius2),\
		c_double(self.length))
        del self

    def edit(self):
        command.EditCone(c_int(self.editID), c_double(self.x), c_double(self.y), c_double(self.z), \
                                            c_double(self.l), c_double(self.w), c_double(self.h),\
		            c_double(self.radius),c_double(self.radius2),\
		            c_double(self.length))
        del self
		
class Sphere:
    def setName(self, name):
        self.name = name

    def setEditID(self, id):
        self.editID = id

    def setLocation(self, x,y,z):
        self.x = x
        self.y = y
        self.z = z
		
    def setRadius(self,radius):
        self.radius = radius
		
    def create(self):
        sname = bytes(self.name, encoding='utf-8')
        command.CreateSphere(sname,c_double(self.x),c_double(self.y),c_double(self.z), \
		c_double(self.radius))
        del self

    def edit(self):
        command.EditSphere(c_int(self.editID), c_double(self.x), c_double(self.y), c_double(self.z), \
		            c_double(self.radius))
        del self

class Point:
    def setName(self, name):
        self.name = name

    def setEditID(self, id):
        self.editID = id

    def setLocation(self, x,y,z):
        self.x = x
        self.y = y
        self.z = z
		
    def setOffset(self,p1,p2,p3):
        self.p1=p1
        self.p2=p2
        self.p3=p3
		
    def create(self):
        sname = bytes(self.name, encoding='utf-8')
        command.CreatePoint(sname,c_double(self.x),c_double(self.y),c_double(self.z), \
		c_double(self.p1),c_double(self.p2),c_double(self.p3))
        del self

    def edit(self):
        command.EditPoint(c_int(self.editID), c_double(self.x), c_double(self.y), c_double(self.z), \
		            c_double(self.p1),c_double(self.p2),c_double(self.p3))
        del self
		
class Line:
    def __init__(self):
        self.startpoint0=0
        self.startpoint1=0
        self.startpoint2=0
        self.name=""
        self.editID=-1
        self.method=0
        self.coor0=0
        self.coor1=0
        self.coor2=0
        self.len=10
        self.reverse=0
        self.dir0=0
        self.dir1=0
        self.dir2=0
		
    def setName(self, name):
        self.name = name

    def setEditID(self, id):
        self.editID = id

    def setStartPoint(self, startpoint0,startpoint1,startpoint2):
        self.startpoint0 = startpoint0
        self.startpoint1 = startpoint1
        self.startpoint2 = startpoint2
		
    def setEndPoint(self,coor0,coor1,coor2):
        self.method=0
        self.coor0=coor0
        self.coor1=coor1
        self.coor2=coor2
		
    def setDirection(self,dir0,dir1,dir2):
        self.method=1
        self.dir0=dir0
        self.dir1=dir1
        self.dir2=dir2
		
    def setLength(self,len):
        self.len=len;

    def setDirectionReverse(self,reverse):
        self.reverse=reverse
		
    def create(self):
        sname = bytes(self.name, encoding='utf-8')
        command.CreateLine(sname,c_double(self.startpoint0),c_double(self.startpoint1),c_double(self.startpoint2), \
							c_int(self.method),\
							c_double(self.coor0),c_double(self.coor1),c_double(self.coor2),\
							c_double(self.len),\
							c_double(self.dir0),c_double(self.dir1),c_double(self.dir2),\
							c_int(self.reverse))
        del self

    def edit(self):
        command.EditLine(c_int(self.editID),c_double(self.startpoint0),c_double(self.startpoint1),c_double(self.startpoint2), \
							c_int(self.method),\
							c_double(self.coor0),c_double(self.coor1),c_double(self.coor2),\
							c_double(self.len),\
							c_double(self.dir0),c_double(self.dir1),c_double(self.dir2),\
							c_int(self.reverse))
        del self
		
class Face:
    def __init__(self):
        self.edges=dict()
        self.editID = -1
        self.name=""

    def setName(self, name):
        self.name = name

    def setEditID(self, id):
        self.editID = id
		
    def appendEdge(self, geoset, index):
        self.edges.setdefault(geoset, set()).add(index)
		
    def create(self):
         keyList = self.edges.keys()
         strcom = ""
         for key in keyList:
             setstr = ""
             values = self.edges.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
         strcom = strcom[:-1]
         edgestr = bytes(strcom, encoding='utf-8')
         sname = bytes(self.name, encoding='utf-8')
         command.CreateFace(edgestr, sname, c_int(self.editID))

    def edit(self):
        self.create()

class Chamfer:

    def __init__(self):
        self.edges=dict()
        self.editID = -1
        self.type=0
        self.d1=1.0
        self.d2=1.0
        self.typestr="Symmetrical"
		
		
    def setEditID(self, id):
        self.editID = id
        
    def setSectionType(self,typestr):
        self.typestr=typestr

    def setSymmetricalDistance(self,d1):
        self.d1=d1
    
    def setAsymmetricalDistances(self,d1,d2):
        self.d1=d1
        self.d2=d2

    def appendEdge(self, geoset, index):
        self.edges.setdefault(geoset, set()).add(index)
		
    def create(self):
         keyList = self.edges.keys()
         strcom = ""
         for key in keyList:
             setstr = ""
             values = self.edges.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
         strcom = strcom[:-1]
         edgestr = bytes(strcom, encoding='utf-8')
         if self.typestr=='Symmetrical':
          self.type=0
         elif self.typestr=='Asymmetrical':
          self.type=1
         command.CreateChamfer(edgestr, c_int(self.editID),c_double(self.d1),c_double(self.d2),c_int(self.type))
	
    def edit(self):
        self.create()
				
		
		
		

class Fillet:
    def __init__(self):
        self.edges=dict()
        self.editID = -1

    def appendEdge(self, geoset, index):
        self.edges.setdefault(geoset, set()).add(index)

    def setRadius(self, r):
        self.radius = r


    def setEditID(self,id):
        self.editID = id

    def create(self):
         keyList = self.edges.keys()
         strcom = ""
         for key in keyList:
             setstr = ""
             values = self.edges.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
         strcom = strcom[:-1]
         edgestr = bytes(strcom, encoding='utf-8')
         command.CreateFillet(edgestr,  c_double(self.radius), c_int(self.editID))

    def edit(self):
        self.create()
		
		
		
class VariableFillet:
    def __init__(self):
        self.edges=dict()
        self.editID = -1
		
    def setEditID(self, id):
        self.editID = id
		
    def VariableFilletOnEdge(self,setid,edgeindex):
        self.setid=setid
        self.edgeindex=edgeindex
	
    def AppendVariablePoint(self, location, radius):
        self.edges.setdefault(location,radius)
 
    def setBasicRad(self,basicRad):
        self.basicRad=basicRad

	
    def create(self):
        keyList = self.edges.keys()
        strcom = ""
        for key in keyList:
             setstr = "" 
 #            values = self.edges.get(key)
#             for v in values:
             setstr = setstr + str(self.edges[key]) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
        strcom = strcom[:-1]
        edgestr = bytes(strcom, encoding='utf-8')
        command.CreateVariableFillet(edgestr,  c_double(self.basicRad), c_int(self.editID),c_int(self.setid),c_int(self.edgeindex))
	
    def edit(self):
        self.create()
		
class BooLOperation:

    def setEditID(self, id):
        self.editID = id
		
    def setBoolType(self,booltype):
        self.booltype=booltype
		
    def setIndexOfSolid1InGeo(self,set1,body1Index):
        self.set1=set1
        self.body1Index=body1Index
		
    def setIndexOfSolid2InGeo(self,set2,body2Index):
        self.set2=set2
        self.body2Index=body2Index
		
    def create(self):
        typestr = bytes(self.booltype, encoding='utf-8')
        command.CreateBooLOperation(typestr,c_int(self.set1),c_int(self.body1Index),c_int(self.set2),c_int(self.body2Index))
        del self

    def edit(self):
        typestr = bytes(self.booltype, encoding='utf-8')
        command.EditBooLOperation(c_int(self.editID),typestr,c_int(self.set1),c_int(self.body1Index),c_int(self.set2),c_int(self.body2Index))
        del self	
		
class MirrorFeature:
    def __init__(self):
        self.faceindex = 0
        self.facebody = 0
        self.method = 0
        self.planemethod='"XOY"'
        self.basepoint0=0
        self.basepoint1=0
        self.basepoint2=0
        self.random0=0
        self.random1=0
        self.random2=0
        self.save=0
        self.bodys=dict()
		
    def setEditID(self, id):
        self.editID = id
		
    def appendBody(self, setid, bodyindex):
        self.bodys.setdefault(setid,set()).add(bodyindex)
		  
    def SaveOrigin(self, save):
        self.save = save
	
	
    def setSymmetricPlaneMethod(self, method):
        self.method=method
	
    def setFace(self,facebody,faceindex):
        self.faceindex=faceindex
        self.facebody=facebody
		
    def setPlaneMethod(self,planemethod):
        self.planemethod=planemethod		
	
	
    def setDir(self,random0,random1,random2):
        self.random0=random0
        self.random1=random1
        self.random2=random2
		
    def setBasePt(self,basepoint0,basepoint1,basepoint2):
        self.basepoint0=basepoint0
        self.basepoint1=basepoint1
        self.basepoint2=basepoint2
    
    def create(self):
        keyList = self.bodys.keys()
        strcom = ""
        for key in keyList:
             setstr = ""
             values = self.bodys.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
        strcom = strcom[:-1]
        bodystr = bytes(strcom, encoding='utf-8')
        methodstr = bytes(self.method, encoding='utf-8')
        savestr=bytes(self.save, encoding='utf-8')
        planemestr=bytes(self.planemethod, encoding='utf-8')
        command.CreateMirrorFeature(bodystr,methodstr,c_int(self.faceindex),c_int(self.facebody),planemestr,c_double(self.random0),c_double(self.random1),c_double(self.random2),c_double(self.basepoint0),c_double(self.basepoint1),c_double(self.basepoint2),savestr)
        del self

    def edit(self):
        keyList = self.bodys.keys()
        strcom = ""
        for key in keyList:
             setstr = ""
             values = self.bodys.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
        strcom = strcom[:-1]
        bodystr = bytes(strcom, encoding='utf-8')
        methodstr = bytes(self.method, encoding='utf-8')
        savestr=bytes(self.save, encoding='utf-8')
        planemestr=bytes(self.planemethod, encoding='utf-8')
        command.EditMirrorFeature(c_int(self.editID),bodystr,methodstr,c_int(self.faceindex),c_int(self.facebody),planemestr,c_double(self.random0),c_double(self.random1),c_double(self.random2),c_double(self.basepoint0),c_double(self.basepoint1),c_double(self.basepoint2),savestr)
        del self	
		
class MoveFeature:
    def __init__(self):
        self.startpt0=0
        self.startpt1=0
        self.startpt2=0
        self.endpt0=0
        self.endpt1=0
        self.endpt2=0
        self.length=10
        self.dir0=0
        self.dir1=0
        self.dir2=0
        self.reverse="false"
        self.bodys=dict()
		
    def setEditID(self, id):
        self.editID = id
	
    def appendBody(self, setid, bodyindex):
        self.bodys.setdefault(setid,set()).add(bodyindex)	
   
		
    def TransformMethod(self,method):
        self.method=method
		
    def setStartPoint(self,startpt0,startpt1,startpt2):
        self.startpt0=startpt0
        self.startpt1=startpt1
        self.startpt2=startpt2
		
    def setEndPoint(self,endpt0,endpt1,endpt2):
        self.endpt0=endpt0
        self.endpt1=endpt1
        self.endpt2=endpt2
		  
    def SaveOrigin(self, saveori):
        self.saveori = saveori

    def setReverse(self,reverse):
        self.reverse=reverse
		
    def setLength(self,length):
        self.length=length
	
    def setDirection(self,dir0,dir1,dir2):
        self.dir0=dir0
        self.dir1=dir1
        self.dir2=dir2		
		
    def create(self):
        keyList = self.bodys.keys()
        strcom = ""
        for key in keyList:
             setstr = ""
             values = self.bodys.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
        strcom = strcom[:-1]
        bodystr = bytes(strcom, encoding='utf-8')
        methstr = bytes(self.method, encoding='utf-8')
        saveoristr = bytes(self.saveori, encoding='utf-8')
        reversestr = bytes(self.reverse, encoding='utf-8')
        command.CreateMoveFeature(bodystr,methstr, c_double(self.startpt0),c_double(self.startpt1),c_double(self.startpt2),c_double(self.endpt0),c_double(self.endpt1),c_double(self.endpt2),\
									saveoristr,reversestr,c_double(self.length),c_double(self.dir0),c_double(self.dir1),c_double(self.dir2))
        del self

    def edit(self):
        keyList = self.bodys.keys()
        strcom = ""
        for key in keyList:
             setstr = ""
             values = self.bodys.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
        strcom = strcom[:-1]
        bodystr = bytes(strcom, encoding='utf-8')
        methstr = bytes(self.method, encoding='utf-8')
        saveoristr = bytes(self.saveori, encoding='utf-8')
        reversestr = bytes(self.reverse, encoding='utf-8')
        command.EditMoveFeature(c_int(self.editID),bodystr,methstr, c_double(self.startpt0),c_double(self.startpt1),c_double(self.startpt2),c_double(self.endpt0),c_double(self.endpt1),c_double(self.endpt2),\
									saveoristr,reversestr,c_double(self.length),c_double(self.dir0),c_double(self.dir1),c_double(self.dir2))
        del self

class RotateFeature:
    def __init__(self):
        self.saveOri = 0
        self.rever = 0
        self.body = 0
        self.edge = 0
        self.axisx = 0
        self.axisy = 0
        self.axisz = 0
        self.axisMethod = 0
        self.bodys=dict()

    def saveOrigin(self):
        self.saveOri = 1
        
    def appendBody(self, setid, bodyindex):
        self.bodys.setdefault(setid,set()).add(bodyindex)	

    def setBasicPoint(self, x, y, z):
        self.basicx = x
        self.basicy = y
        self.basicz = z

    def setAxisFromBody(self, body, edge):
        self.axisMethod = 0
        self.body = body
        self.edge = edge

    def setAxis(self, x, y ,z):
        self.axisMethod = 1
        self.axisx = x
        self.axisy = y
        self.axisz = z

    def reverse(self):
        self.rever = 1

    def setAngle(self,ang):
        self.angle = ang


    def setEditID(self, id):
        self.editID = id

    def rotate(self):
        keyList = self.bodys.keys()
        strcom = ""
        for key in keyList:
             setstr = ""
             values = self.bodys.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
        strcom = strcom[:-1]
        objstr = bytes(strcom, encoding='utf-8')
        command.RotateFeature(objstr, c_double(self.basicx), c_double(self.basicy), c_double(self.basicz), \
                              c_int(self.axisMethod),c_int(self.body),c_int(self.edge), c_double(self.axisx),c_double(self.axisy), c_double(self.axisz), c_int(self.rever), \
                              c_double(self.angle),c_int(self.saveOri))
        del self


    def edit(self):
        keyList = self.bodys.keys()
        strcom = ""
        for key in keyList:
             setstr = ""
             values = self.bodys.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
        strcom = strcom[:-1]
        objstr = bytes(strcom, encoding='utf-8')
        command.EditRotateFeature(c_int(self.editID), objstr, c_double(self.basicx), c_double(self.basicy), c_double(self.basicz), \
                              c_int(self.axisMethod), c_int(self.body), c_int(self.edge), c_double(self.axisx),
                              c_double(self.axisy), c_double(self.axisz), c_int(self.rever), \
                              c_double(self.angle), c_int(self.saveOri))
        del self
		
class MakeMatrix:
    def __init__(self):
        self.optionindex=0
        self.dir10=0
        self.dir11=0
        self.dir12=0
        self.reverse1=0
        self.dis1=15
        self.count1=2
        self.showdir2=0
        self.dir20=0
        self.dir21=0
        self.dir22=0
        self.reverse2=0
        self.dis2=15
        self.count2=2
        self.basept0=0
        self.basept1=0
        self.basept2=0
        self.axis0=0
        self.axis1=0
        self.axis2=0
        self.wirereverse=0
        self.wirecount=2
        self.degree=30
        self.bodys=dict()
       
		
    def setEditID(self, id):
        self.editID = id
		
    def appendBody(self, setid, bodyindex):
        self.bodys.setdefault(setid,set()).add(bodyindex)	
		
    def setOptionMethod(self,optionstr):
        if (optionstr=='Liear Matrix'):
          self.optionindex=0
        elif (optionstr=='Wire Matrix'):
          self.optionindex=1
			
    def setDirection1(self,dir10,dir11,dir12):
        self.dir10=dir10
        self.dir11=dir11
        self.dir12=dir12
		  
    def setReverseOfDirection1(self,revstr):
        if (revstr=='Yes'):
          self.reverse1=1
        elif (revstr=='No'):
          self.reverse1=0
        
		
    def setDistance1(self,dis1):
        self.dis1=dis1
		
    def setCount1(self,count1):
        self.count1=count1
		
    def showDirection2(self,showdir2str):
        if (showdir2str=='Yes'):
          self.showdir2=1
        elif (showdir2str=='No'):
          self.showdir2=0
		
    def setDirection2(self,dir20,dir21,dir22):
        self.dir20=dir20
        self.dir21=dir21
        self.dir22=dir22
		  
    def setReverseOfDirection2(self,revstr):
        if (revstr=='Yes'):
          self.reverse2=1
        elif (revstr=='No'):
          self.reverse2=0
                          
    def setDistance2(self,dis2):
        self.dis2=dis2
		
    def setCount2(self,count2):
        self.count2=count2
		
    def setBasicPoint(self,basepoint0,basepoint1,basepoint2):
        self.basept0 = basepoint0
        self.basept1 = basepoint1
        self.basept2 = basepoint2
		  
    def setAxis(self, axis0,axis1,axis2):
        self.axis0 = axis0
        self.axis1 = axis1
        self.axis2 = axis2
		
    def setWireReverse(self, wirerestr):
        if (wirerestr=='Yes'):
          self.wirereverse=1
        elif (wirerestr=='No'):
          self.wirereverse=0
		
    def setWireCount(self,wirecount):
        self.wirecount=wirecount
		  
    def setDegree(self,degree):
        self.degree=degree
		
    def create(self):
        keyList = self.bodys.keys()
        strcom = ""
        for key in keyList:
             setstr = ""
             values = self.bodys.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
        strcom = strcom[:-1]
        bodystr = bytes(strcom, encoding='utf-8')
        command.MakeMatrix(bodystr,c_int(self.optionindex),c_double(self.dir10),c_double(self.dir11),c_double(self.dir12),c_int(self.reverse1),c_double(self.dis1),c_int(self.count1),\
								c_int(self.showdir2),c_double(self.dir20),c_double(self.dir21),c_double(self.dir22),c_int(self.reverse2),c_double(self.dis2),c_int(self.count2),\
								c_double(self.basept0),c_double(self.basept1),c_double(self.basept2),c_double(self.axis0),c_double(self.axis1),c_double(self.axis2),\
								c_int(self.wirereverse),c_int(self.wirecount),c_double(self.degree))
        del self

    def edit(self):
        keyList = self.bodys.keys()
        strcom = ""
        for key in keyList:
             setstr = ""
             values = self.bodys.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
        strcom = strcom[:-1]
        bodystr = bytes(strcom, encoding='utf-8')
        command.EditMatrix(c_int(self.editID),bodystr,c_int(self.optionindex),c_double(self.dir10),c_double(self.dir11),c_double(self.dir12),c_int(self.reverse1),c_double(self.dis1),c_int(self.count1),\
								c_int(self.showdir2),c_double(self.dir20),c_double(self.dir21),c_double(self.dir22),c_int(self.reverse2),c_double(self.dis2),c_int(self.count2),\
								c_double(self.basept0),c_double(self.basept1),c_double(self.basept2),c_double(self.axis0),c_double(self.axis1),c_double(self.axis2),\
								c_int(self.wirereverse),c_int(self.wirecount),c_double(self.degree))
        del self
		  
class Extrusion:
    def __init__(self):
        self.edges=dict()
        self.editID = -1
        self.pt0=0
        self.pt1=0
        self.pt2=0
        self.distance=10
        self.reverse="No"
        self.solid="Yes"
		
    def setEditID(self, id):
        self.editID = id
    def setName(self,name):
        self.name=name
		
    def setDistance(self,distance):
        self.distance=distance
		
    def setDirection(self,pt0,pt1,pt2):
        self.pt0=pt0
        self.pt1=pt1
        self.pt2=pt2
		
    def appendEdge(self, geoset, index):
        self.edges.setdefault(geoset, set()).add(index)
		
    def Reverse(self,reverse):
        self.reverse=reverse
		
    def GenerateSolid(self,solid):
        self.solid=solid
		
    def create(self):
        keyList = self.edges.keys()
        strcom = ""
        for key in keyList:
             setstr = ""
             values = self.edges.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
        strcom = strcom[:-1]
        edgestr = bytes(strcom, encoding='utf-8')
        reversestr=bytes(self.reverse, encoding='utf-8')
        solidstr=bytes(self.solid, encoding='utf-8')
        namestr=bytes(self.name, encoding='utf-8')
		 
        command.CreateExtrusion( c_int(self.editID),namestr,edgestr,c_double(self.distance),c_double(self.pt0),c_double(self.pt1),c_double(self.pt2),reversestr,solidstr)
	
    def edit(self):
        self.create()

class Revol:
    def __init__(self):
        self.edges=dict()
        self.editID = -1
        self.basicpt0=0
        self.basicpt1=0
        self.basicpt2=0
        self.coor0=0
        self.coor1=0
        self.coor2=0
        self.degree=90
        self.reverse="No"
        self.solid="Yes"
        self.axisset=-1
        self.edgeindex=-1

		
    def setEditID(self, id):
        self.editID = id
		
    def setName(self,name):
        self.name=name
		
    def setDegree(self,degree):
        self.degree=degree
		
    def setBasicPoint(self,basicpt0,basicpt1,basicpt2):
        self.basicpt0=basicpt0
        self.basicpt1=basicpt1
        self.basicpt2=basicpt2
		
    def setAxisMethod(self,optionindex):
        self.optionindex=optionindex
		
    def setCoordinate(self,coor0,coor1,coor2):
        self.coor0=coor0
        self.coor1=coor1
        self.coor2=coor2
		
    def selectAxisOnGeo(self,axisset,edgeindex):
        self.axisset=axisset
        self.edgeindex=edgeindex
	
    def appendEdge(self, geoset, index):
        self.edges.setdefault(geoset, set()).add(index)
		
    def Reverse(self,reverse):
        self.reverse=reverse
		
    def GenerateSolid(self,solid):
        self.solid=solid
		
    def create(self):
        keyList = self.edges.keys()
        strcom = ""
        for key in keyList:
             setstr = ""
             values = self.edges.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
        strcom = strcom[:-1]
        edgestr = bytes(strcom, encoding='utf-8')
        reversestr=bytes(self.reverse, encoding='utf-8')
        solidstr=bytes(self.solid, encoding='utf-8')
        namestr=bytes(self.name, encoding='utf-8')
        optionindexstr= bytes(self.optionindex, encoding='utf-8')
        command.CreateRevol( c_int(self.editID),namestr,edgestr,c_double(self.basicpt0),c_double(self.basicpt1),c_double(self.basicpt2),c_double(self.degree),optionindexstr,c_int(self.axisset),c_int(self.edgeindex),\
								c_double(self.coor0),c_double(self.coor1),c_double(self.coor2),reversestr,solidstr)
	
    def edit(self):
        self.create()		
		
class Loft:
    def __init__(self):
        self.editID = -1
        self.solid="No"	
        self.secstr=""
		
    def setEditID(self, id):
        self.editID = id
		
    def setName(self,name):
        self.name=name	
		
    def isSolid(self,solid):
        self.solid=solid	
    def appendSection(self,sec):
        self.secstr+=sec
        self.secstr+=" "
	
	
    def create(self):
        
        sectionstr = bytes(self.secstr, encoding='utf-8')
        solidstr=bytes(self.solid, encoding='utf-8')
        namestr=bytes(self.name, encoding='utf-8')
        command.CreateLoft( c_int(self.editID),namestr,solidstr,sectionstr)
		
    def edit(self):
        self.create()			
		
class Sweep:
    def __init__(self):
        self.edges=dict()
        self.editID = -1
        self.solid="No"	

    def appendEdge(self, geoset, index):
        self.edges.setdefault(geoset, set()).add(index)

    def setEditID(self,id):
        self.editID = id
	
    def setPath(self,pathset,pathedge):
        self.pathset=pathset
        self.pathedge=pathedge
	
    def isSolid(self,solid):
        self.solid=solid	
		
    def create(self):
         keyList = self.edges.keys()
         strcom = ""
         for key in keyList:
             setstr = ""
             values = self.edges.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
         strcom = strcom[:-1]
         edgestr = bytes(strcom, encoding='utf-8')
         solidstr = bytes(self.solid, encoding='utf-8')
         command.CreateSweep(c_int(self.editID), edgestr, solidstr, c_int(self.pathset),c_int(self.pathedge),)

    def edit(self):
        self.create()
		
class GeoSplitter:
    def __init__(self):
        self.faceindex = -1
        self.facebody = -1
        self.editID = -1
        self.method = 0
        self.planemethod='"XOY"'
        self.basepoint0=0
        self.basepoint1=0
        self.basepoint2=0
        self.random0=0
        self.random1=0
        self.random2=0
        self.bodys=dict()
	
    def appendBody(self, setid, bodyindex):
        self.bodys.setdefault(setid,set()).add(bodyindex)

    def setSymmetricPlaneMethod(self, method):
        self.method=method
		
    def setEditID(self, id):
        self.editID = id
		
    def setFace(self,facebody,faceindex):
        self.facebody=facebody
        self.faceindex=faceindex
		
    def setPlaneMethod(self,planemethod):
        self.planemethod=planemethod		
	
	
    def setDir(self,random0,random1,random2):
        self.random0=random0
        self.random1=random1
        self.random2=random2
		
    def setBasePt(self,basepoint0,basepoint1,basepoint2):
        self.basepoint0=basepoint0
        self.basepoint1=basepoint1
        self.basepoint2=basepoint2
     
    def create(self):
        keyList = self.bodys.keys()
        strcom = ""
        for key in keyList:
             setstr = ""
             values = self.bodys.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
        strcom = strcom[:-1]
        bodystr = bytes(strcom, encoding='utf-8')
        methodstr = bytes(self.method, encoding='utf-8')
        planemestr=bytes(self.planemethod, encoding='utf-8')
        command.MakeGeoSplitter(bodystr,methodstr,c_int(self.faceindex),c_int(self.facebody),planemestr,c_double(self.random0),c_double(self.random1),c_double(self.random2),c_double(self.basepoint0),c_double(self.basepoint1),c_double(self.basepoint2))
        del self

    def edit(self):
        keyList = self.bodys.keys()
        strcom = ""
        for key in keyList:
             setstr = ""
             values = self.bodys.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
        strcom = strcom[:-1]
        bodystr = bytes(strcom, encoding='utf-8')
        methodstr = bytes(self.method, encoding='utf-8')
        planemestr=bytes(self.planemethod, encoding='utf-8')
        command.EditGeoSplitter(c_int(self.editID),bodystr,methodstr,c_int(self.faceindex),c_int(self.facebody),planemestr,c_double(self.random0),c_double(self.random1),c_double(self.random2),c_double(self.basepoint0),c_double(self.basepoint1),c_double(self.basepoint2))
        del self			
		
class FillHole:
    def __init__(self):
        self.faces=dict()
        self.editID = -1

    def appendFace(self, geoset, index):
        self.faces.setdefault(geoset, set()).add(index)

    def setEditID(self,id):
        self.editID = id

    def create(self):
         keyList = self.faces.keys()
         strcom = ""
         for key in keyList:
             setstr = ""
             values = self.faces.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
         strcom = strcom[:-1]
         facestr = bytes(strcom, encoding='utf-8')
         command.MakeFillHole(facestr, c_int(self.editID))

    def edit(self):
        self.create()				
class RemoveSurface:
    def __init__(self):
        self.faces=dict()
        self.editID = -1

    def appendFace(self, geoset, index):
        self.faces.setdefault(geoset, set()).add(index)

    def setEditID(self,id):
        self.editID = id

    def create(self):
         keyList = self.faces.keys()
         strcom = ""
         for key in keyList:
             setstr = ""
             values = self.faces.get(key)
             for v in values:
                 setstr = setstr + str(v) + ","
             setstr =  str(key) +  ":" + setstr[:-1]
             strcom = strcom + setstr+";"
         strcom = strcom[:-1]
         facestr = bytes(strcom, encoding='utf-8')
         command.MakeRemoveSurface(facestr, c_int(self.editID))

    def edit(self):
        self.create()				
				
class FillGap:

    def setEditID(self, id):
        self.editID = id
		
    def setFillGapType(self,type):
        self.fillgaptype=type
		
    def setIndexOfShape1(self,set1,body1Index):
        self.set1=set1
        self.body1Index=body1Index
		
    def setIndexOfShape2(self,set2,body2Index):
        self.set2=set2
        self.body2Index=body2Index
		
    def create(self):
        typestr = bytes(self.fillgaptype, encoding='utf-8')
        command.CreateFillGap(typestr,c_int(self.set1),c_int(self.body1Index),c_int(self.set2),c_int(self.body2Index))
        del self

    def edit(self):
        typestr = bytes(self.fillgaptype, encoding='utf-8')
        command.EditFillGap(c_int(self.editID),typestr,c_int(self.set1),c_int(self.body1Index),c_int(self.set2),c_int(self.body2Index))
        del self		
		
		
		
