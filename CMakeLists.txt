cmake_minimum_required(VERSION 2.8.12)
project(kvkontakte)

# the version number, needed for
# - the library version
# - version detection by cmake
set(LIBKVKONTAKTE_VERSION_MAJOR 4)
set(LIBKVKONTAKTE_VERSION_MINOR 12)
set(LIBKVKONTAKTE_VERSION_PATCH 0)
set(LIBKVKONTAKTE_VERSION "${LIBKVKONTAKTE_VERSION_MAJOR}.${LIBKVKONTAKTE_VERSION_MINOR}.${LIBKVKONTAKTE_VERSION_PATCH}")

# ABI in 4.x releases did not break since the very first version from digiKam SC 2.1.0
set(LIBKVKONTAKTE_SOVERSION 1)

if(NOT LIB_INSTALL_DIR)
	set(LIB_INSTALL_DIR lib${LIB_SUFFIX})
endif(NOT LIB_INSTALL_DIR)
set(CMAKECONFIG_INSTALL_DIR ${LIB_INSTALL_DIR}/cmake/LibKVkontakte )

# Use a versioned install directory for the headers so multiple versions can be installed in parallel
set(INCLUDE_INSTALL_DIR include )


# === Closer to the code, close to the body ===
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

set(QT_MIN_VERSION  "4.8.0") # Qt 4.8 required for QHttpMultiPart
set(KDE_MIN_VERSION "4.4.0")
find_package(Qt4  ${QT_MIN_VERSION}  REQUIRED)
find_package(KDE4 ${KDE_MIN_VERSION} REQUIRED)

include(KDE4Defaults)
include(MacroLibrary)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}")

set(QJSON_VERSION_REQUIRED 0.7)
find_package(QJSON ${QJSON_VERSION_REQUIRED})
macro_log_feature(QJSON_FOUND "QJSON" "Qt library for handling JSON data" "http://qjson.sourceforge.net/" TRUE "${QJSON_VERSION_REQUIRED}")

add_definitions(${QT_DEFINITIONS} ${KDE4_DEFINITIONS})

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${KDE4_INCLUDES}
  ${QJSON_INCLUDE_DIR}
)

add_subdirectory(libkvkontakte)
# === ------------------------------------- ===

# This "exports" all targets which have been put into the export set "LibKVkontakteExport".
# This means that cmake generates a file with the given filename, which can later on be loaded
# by projects using this package.
# This file contains add_library(bar IMPORTED) statements for each target in the export set, so
# when loaded later on cmake will create "imported" library targets from these, which can be used
# in many ways in the same way as a normal library target created via a normal add_library().
install(EXPORT LibKVkontakteExport DESTINATION ${CMAKECONFIG_INSTALL_DIR} FILE LibKVkontakteTargets.cmake )

# figure out the relative path from the installed Config.cmake file to the install prefix (which may be at
# runtime different from the chosen CMAKE_INSTALL_PREFIX if under Windows the package was installed anywhere)
# This relative path will be configured into the LibKVkontakteConfig.cmake
file(RELATIVE_PATH relInstallDir ${CMAKE_INSTALL_PREFIX}/${CMAKECONFIG_INSTALL_DIR} ${CMAKE_INSTALL_PREFIX} )

# Create a LibKVkontakteConfig.cmake file. <name>Config.cmake files are searched by find_package()
# automatically. We configure that file so that we can put any information we want in it,
# e.g. version numbers, include directories, etc.
configure_file(LibKVkontakteConfig.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/LibKVkontakteConfig.cmake @ONLY )

# Additionally, when cmake has found a LibKVkontakteConfig.cmake, it can check for a LibKVkontakteConfigVersion.cmake
# in the same directory when figuring out the version of the package when a version
# has been specified in the find_package() call, e.g. find_package(LibKVkontakte 1.0)
configure_file(LibKVkontakteConfigVersion.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/LibKVkontakteConfigVersion.cmake @ONLY )

# Install these two files into the same directory as the generated exports-file.
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/LibKVkontakteConfig.cmake ${CMAKE_CURRENT_BINARY_DIR}/LibKVkontakteConfigVersion.cmake
        DESTINATION ${CMAKECONFIG_INSTALL_DIR} )


macro_display_feature_log()

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif (BUILD_TESTING)
