TEMPLATE = app

CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG+=thread
LIBS+= $(SUBLIBS) -lpthread -pthread -lncurses

INCLUDEPATH += ../

SOURCES += \
        Test.cpp \
        ../socket.cpp \
        ../talk.cpp \
        ../Player.cpp \
        ../Stack.cpp \
        
        
