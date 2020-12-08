win32{

INCLUDEPATH	+=  ../../cgns/include  \

Debug:LIBS +=  -L../../cgns/libd/ \
-lcgns   \    
-lcgnsdll \


Release:LIBS +=  -L../../cgns/lib/ \
-lcgns   \    
-lcgnsdll \

}

unix{

INCLUDEPATH	+=  ../../cgns/cgns_linux/  \

LIBS +=  -L../../cgns/cgns_linux/ \
-lcgns   \  

}