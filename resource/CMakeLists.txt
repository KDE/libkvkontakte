project(facebookresource)

find_package(Boost 1.34.0)
macro_log_feature(Boost_FOUND "Boost" "Boost C++ Libraries" "http://www.boost.org" TRUE "1.34.0" "Required by Akonadi.")

add_definitions(-DRESOURCE_VERSION=\\"${RESOURCE_VERSION}\\")
if (XSLTPROC_EXECUTABLE)
  macro( kcfg_generate_dbus_interface _kcfg _name )
    add_custom_command(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${_name}.xml
      COMMAND ${XSLTPROC_EXECUTABLE} --stringparam interfaceName ${_name}
              ${KDEPIMLIBS_DATA_DIR}/akonadi-kde/kcfg2dbus.xsl
              ${_kcfg}
              > ${CMAKE_CURRENT_BINARY_DIR}/${_name}.xml
      DEPENDS ${KDEPIMLIBS_DATA_DIR}/akonadi-kde/kcfg2dbus.xsl
              ${_kcfg}
    )
  endmacro( kcfg_generate_dbus_interface )
endif (XSLTPROC_EXECUTABLE)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}" )

set( facebookresource_SRCS
  facebookresource.cpp
  settings.cpp
  settingsdialog.cpp
)

install( FILES facebookresource.desktop DESTINATION "${CMAKE_INSTALL_PREFIX}/share/akonadi/agents" )

kde4_add_ui_files( facebookresource_SRCS settingsdialog.ui)
kde4_add_kcfg_files(facebookresource_SRCS settingsbase.kcfgc)
kcfg_generate_dbus_interface(${CMAKE_CURRENT_SOURCE_DIR}/settingsbase.kcfg org.kde.Akonadi.Facebook.Settings)
qt4_add_dbus_adaptor(facebookresource_SRCS
  ${CMAKE_CURRENT_BINARY_DIR}/org.kde.Akonadi.Facebook.Settings.xml settings.h Settings
)

kde4_add_executable(akonadi_facebook_resource RUN_UNINSTALLED ${facebookresource_SRCS})

if (Q_WS_MAC)
  set_target_properties(akonadi_facebook_resource PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.template)
  set_target_properties(akonadi_facebook_resource PROPERTIES MACOSX_BUNDLE_GUI_IDENTIFIER "org.kde.Akonadi.Facebook")
  set_target_properties(akonadi_facebook_resource PROPERTIES MACOSX_BUNDLE_BUNDLE_NAME "KDE Akonadi Facebook Resource")
endif (Q_WS_MAC)

include_directories (
  ${Boost_INCLUDE_DIR}
)

target_link_libraries( akonadi_facebook_resource
  ${KDEPIMLIBS_AKONADI_LIBS}
  ${QT_QTCORE_LIBRARY}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTDBUS_LIBRARY}
  ${KDE4_KDECORE_LIBS}
  libkfacebook
)

add_subdirectory(icons)

install(TARGETS akonadi_facebook_resource ${INSTALL_TARGETS_DEFAULT_ARGS})
