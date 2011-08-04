project(libkvkontakte)

set(libkvkontakte_SRCS
    authenticationdialog.cpp
    userinfojob.cpp
    userinfo.cpp
    friendlistjob.cpp
    friendjob.cpp
    photojobbase.cpp
    noteslistjob.cpp
    allnoteslistjob.cpp
    notejob.cpp
    getvariablejob.cpp
    currentuserjob.cpp
    profilephotojob.cpp
    userinfofulljob.cpp
    cidsnamesjob.cpp
    longpollserverjob.cpp
    longpollserverinfo.cpp
    messageinfo.cpp
    discussionslistjob.cpp
    messageslistjob.cpp
    noteinfo.cpp
    noteaddjob.cpp
    allmessageslistjob.cpp
    albumlistjob.cpp
    albuminfo.cpp
    createalbumjob.cpp
    deletealbumjob.cpp

    qintlist.cpp
    util.cpp
    vkontaktejobs.cpp  
)

kde4_add_library(kvkontakte SHARED ${libkvkontakte_SRCS})

set(CMAKE_INSTALL_NAME_DIR ${LIB_INSTALL_DIR})

# Yes, QJSON_LIBRARIES and QJSON_LIBRARY are both needed, as otherwise it will break either
# on Linux or on Mac.
target_link_libraries(kvkontakte
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
  ${KDEPIMLIBS_KMIME_LIBS}
)

if(KDEPIM_44_COMPAT)
  target_link_libraries(kvkontakte ${KDEPIMLIBS_KCAL_LIBS})
else(KDEPIM_44_COMPAT)
  target_link_libraries(kvkontakte ${KDEPIMLIBS_KCALCORE_LIBS})
endif(KDEPIM_44_COMPAT)

set_target_properties(kvkontakte PROPERTIES VERSION ${LIB_VERSION} SOVERSION ${LIB_SOVERSION})
install(TARGETS kvkontakte ${INSTALL_TARGETS_DEFAULT_ARGS})
