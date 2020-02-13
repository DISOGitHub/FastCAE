win32{

X64{

Debug:INCLUDEPATH	+=  ../../cgns/cgns_x64_debug/inc/  \

Debug:LIBS +=  -L../../cgns/cgns_x64_debug/lib/ \
-lcgns   \    
-llibhdf5_D \
-llibszip \ 
-llibzlib \


Release:INCLUDEPATH	+=  ../../cgns/cgns_x64_release/inc/  \

Release:LIBS +=  -L../../cgns/cgns_x64_release/lib/ \
-lcgns   \    
-llibhdf5 \
-llibszip \
-llibzlib \

}

X86{
	
}

}

unix{

INCLUDEPATH	+=  ../../cgns/cgns_linux/  \

LIBS +=  -L../../cgns/cgns_linux/ \
-lcgns   \  

}