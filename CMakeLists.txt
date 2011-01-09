project(kfacebook)

set(RESOURCE_VERSION "0.1")
set(LIB_VERSION "1.0")
set(LIB_SOVERSION "1")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

find_package(KdepimLibs 4.5.63 REQUIRED)
find_program(XSLTPROC_EXECUTABLE xsltproc)
macro_log_feature(XSLTPROC_EXECUTABLE "xsltproc" "The command line XSLT processor from libxslt" "http://xmlsoft.org/XSLT/" TRUE)
find_package(QJSON)
macro_log_feature(QJSON_FOUND "QJSON" "Qt library for handling JSON data" "http://qjson.sourceforge.net/" TRUE)

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${KDEPIMLIBS_INCLUDE_DIRS}
  ${KDE4_INCLUDES}
  ${QJSON_INCLUDE_DIR}
)

add_subdirectory(libkfacebook)
add_subdirectory(resource)

macro_display_feature_log()
