project(libkfacebook)

set(libkfacebook_SRCS
  authenticationdialog.cpp
  userinfojob.cpp
  userinfo.cpp
  friendlistjob.cpp
  facebookjob.cpp
  friendjob.cpp
  photojob.cpp
  eventslistjob.cpp
  eventinfo.cpp
  util.cpp
  alleventslistjob.cpp
  eventjob.cpp
)

kde4_add_library(kfacebook SHARED ${libkfacebook_SRCS})

set(CMAKE_INSTALL_NAME_DIR ${LIB_INSTALL_DIR})

# Yes, QJSON_LIBRARIES and QJSON_LIBRARY are both needed, as otherwise it will break either
# on Linux or on Mac.
target_link_libraries(kfacebook
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KDEUI_LIBS}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
  ${KDE4_KDEWEBKIT_LIBS}
  ${KDE4_KIO_LIBS}
  ${QJSON_LIBRARIES}
  ${QJSON_LIBRARY}
  ${KDEPIMLIBS_KABC_LIBS}
  ${KDEPIMLIBS_KPIMUTILS_LIBS}
)

if(KDEPIM_44_COMPAT)
  target_link_libraries(kfacebook ${KDEPIMLIBS_KCAL_LIBS})
else(KDEPIM_44_COMPAT)
  target_link_libraries(kfacebook ${KDEPIMLIBS_KCALCORE_LIBS})
endif(KDEPIM_44_COMPAT)

set_target_properties(kfacebook PROPERTIES VERSION ${LIB_VERSION} SOVERSION ${LIB_SOVERSION})
install(TARGETS kfacebook ${INSTALL_TARGETS_DEFAULT_ARGS})
