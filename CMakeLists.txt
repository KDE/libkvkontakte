project(kvkontakte)

set(RESOURCE_VERSION "0.1")
set(LIB_VERSION "1.0")
set(LIB_SOVERSION "1")

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )

find_program(XSLTPROC_EXECUTABLE xsltproc)
macro_log_feature(XSLTPROC_EXECUTABLE "xsltproc" "The command line XSLT processor from libxslt" "http://xmlsoft.org/XSLT/" TRUE)
set(QJSON_VERSION_REQUIRED 0.7)
find_package(QJSON ${QJSON_VERSION_REQUIRED})
macro_log_feature(QJSON_FOUND "QJSON" "Qt library for handling JSON data" "http://qjson.sourceforge.net/" TRUE "${QJSON_VERSION_REQUIRED}")

configure_file(config.h.cmake "${CMAKE_CURRENT_BINARY_DIR}/config.h")

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${QJSON_INCLUDE_DIR}
  ${Boost_INCLUDE_DIR}
)

add_subdirectory(libkvkontakte)
add_subdirectory(resource)

macro_display_feature_log()

