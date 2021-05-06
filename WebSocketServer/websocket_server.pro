QT = websockets

TARGET = echo_server
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
  main.cpp \
  echo_server.cpp

HEADERS += \
  echo_server.h

EXAMPLE_FILES += echo_client.html
