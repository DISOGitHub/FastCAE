win32{

INCLUDEPATH	+=  ../../output/python37/include/  \

Debug:LIBS += -L../../output/python37/libs/  \
-lpython37_d \


Release:LIBS +=  -L../../output/python37/libs/  \
-lpython37   \

}

unix{

INCLUDEPATH	+=  ../../output/python37/include  \

LIBS +=  -L../../output/python37/lib/  \
-lpython3.7m  \

g++:PRE_TARGETDEPS += $$../../output/python37/lib/libpython3.7m.a  \


}