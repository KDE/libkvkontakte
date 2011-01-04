project(libkfacebook)

set(libkfacebook_SRCS
  authenticationdialog.cpp
  userinfojob.cpp
  userinfo.cpp
  friendlistjob.cpp
  facebookjob.cpp
  friendjob.cpp
  photojob.cpp
)

kde4_add_library(libkfacebook SHARED ${libkfacebook_SRCS})

target_link_libraries(libkfacebook
  ${KDE4_KDECORE_LIBS}
  ${KDE4_KDEUI_LIBS}
  ${QT_QTGUI_LIBRARY}
  ${QT_QTWEBKIT_LIBRARY}
  ${KDE4_KDEWEBKIT_LIBS}
  ${KDE4_KIO_LIBS}
  ${QJSON_LIBRARY}
  ${KDEPIMLIBS_KABC_LIBS}
)

set_target_properties(libkfacebook PROPERTIES VERSION ${LIB_VERSION} SOVERSION ${LIB_SOVERSION})
install(TARGETS libkfacebook ${INSTALL_TARGETS_DEFAULT_ARGS})
