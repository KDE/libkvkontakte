cmake_minimum_required(VERSION 2.8.12)
project(kvkontakte)

find_package(ECM 5.18.0 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR})

set(REQUIRED_QT_VERSION "5.2")
find_package(Qt5 ${REQUIRED_QT_VERSION} CONFIG REQUIRED Core Network WebKit Widgets)

include(KDEInstallDirs)
include(KDEFrameworkCompilerSettings NO_POLICY_SCOPE)
include(KDECMakeSettings) # for automoc
include(KDECompilerSettings)

include(FeatureSummary)
include(GenerateExportHeader)

include(ECMSetupVersion)
include(ECMGenerateHeaders)

set(LIBKVKONTAKTE_VERSION "5.0.0")

ecm_setup_version(${LIBKVKONTAKTE_VERSION}
    VARIABLE_PREFIX LIBKVKONTAKTE
    VERSION_HEADER "${CMAKE_CURRENT_BINARY_DIR}/kf5vkontakte_version.h"
    PACKAGE_VERSION_FILE "${CMAKE_CURRENT_BINARY_DIR}/KF5VkontakteConfigVersion.cmake"
    SOVERSION 2)

set(REQUIRED_KF5_VERSION "5.3.0")
find_package(KF5KIO ${REQUIRED_KF5_VERSION} REQUIRED)
find_package(KF5I18n ${REQUIRED_KF5_VERSION} REQUIRED)
find_package(KF5WebKit ${REQUIRED_KF5_VERSION} REQUIRED)
find_package(KF5CoreAddons ${REQUIRED_KF5_VERSION} REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(src)

if (BUILD_TESTING)
    add_subdirectory(autotests)
endif (BUILD_TESTING)


# create a Config.cmake and a ConfigVersion.cmake file and install them
set(CMAKECONFIG_INSTALL_DIR "${CMAKECONFIG_INSTALL_PREFIX}/KF5Vkontakte")

include(ECMPackageConfigHelpers)

ecm_configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/KF5VkontakteConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5VkontakteConfig.cmake"
    INSTALL_DESTINATION ${CMAKECONFIG_INSTALL_DIR}
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/KF5VkontakteConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/KF5VkontakteConfigVersion.cmake"
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    COMPONENT Devel
)

# TODO: is it OK to use the "KF5::" namespace for libraries not included in KDE Frameworks?
install(EXPORT KF5VkontakteTargets
    DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
    FILE KF5VkontakteTargets.cmake
    NAMESPACE KF5::
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/kf5vkontakte_version.h
    DESTINATION ${KF5_INCLUDE_INSTALL_DIR} COMPONENT Devel
)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
