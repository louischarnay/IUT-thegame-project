TEMPLATE = subdirs
SUBDIRS = ServerAsync ClientAsync #HediServer
#Server.depends = .
#Client.depends = .

HEADERS += \
    socket.h\
    talk.h
