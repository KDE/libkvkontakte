set(libkvkontakte_SRCS
    authenticationdialog.cpp
    userinfojob.cpp
    userinfo.cpp
    friendlistjob.cpp
    photojob.cpp
    noteslistjob.cpp
    allnoteslistjob.cpp
    notejob.cpp
    getvariablejob.cpp
    userinfofulljob.cpp
    cidsnamesjob.cpp
    getinfojob.cpp
    messageinfo.cpp
    discussionslistjob.cpp
    messageslistjob.cpp
    noteinfo.cpp
    noteaddjob.cpp
    allmessageslistjob.cpp
    albumlistjob.cpp
    albuminfo.cpp
    createalbumjob.cpp
    editalbumjob.cpp
    deletealbumjob.cpp
    getphotouploadserverjob.cpp
    photopostjob.cpp
    savephotojob.cpp
    uploadphotosjob.cpp
    photoinfo.cpp
    photolistjob.cpp
    getapplicationpermissionsjob.cpp
    groupinfo.cpp
    grouplistjob.cpp

    qintlist.cpp
    util.cpp
    vkontaktejobs.cpp
    vkapi.cpp
)

kde4_add_library(kvkontakte SHARED ${libkvkontakte_SRCS})
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/version.h)

# Yes, QJSON_LIBRARIES and QJSON_LIBRARY are both needed, as otherwise it will break either
# on Linux or on Mac.
target_link_libraries(kvkontakte
    PRIVATE
        ${KDE4_KDECORE_LIBS}
        ${KDE4_KDEUI_LIBS}
        ${QT_QTGUI_LIBRARY}
        ${QT_QTWEBKIT_LIBRARY}
        ${KDE4_KDEWEBKIT_LIBS}
        ${KDE4_KIO_LIBS}
        ${QJSON_LIBRARIES}
        ${QJSON_LIBRARY}
        ${qjson_LIBRARIES}
        ${KDEPIMLIBS_KABC_LIBS}
        ${KDEPIMLIBS_KPIMUTILS_LIBS}
        ${KDEPIMLIBS_KMIME_LIBS}
)

set_target_properties(kvkontakte PROPERTIES
    VERSION ${LIBKVKONTAKTE_VERSION}
    SOVERSION ${LIBKVKONTAKTE_SOVERSION}
)

install(TARGETS kvkontakte
    EXPORT LibKVkontakteExport
    ${INSTALL_TARGETS_DEFAULT_ARGS}
)

if (NOT DIGIKAMSC_COMPILE_LIBKVKONTAKTE)

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/version.h
        libkvkontakte_export.h
        albuminfo.h
        getapplicationpermissionsjob.h
        vkontaktejobs.h
        authenticationdialog.h
        albumlistjob.h
        qintlist.h
        createalbumjob.h
        editalbumjob.h
        uploadphotosjob.h
        photoinfo.h
        userinfo.h
        messageinfo.h
        noteinfo.h
        friendlistjob.h
        allnoteslistjob.h
        userinfofulljob.h
        userinfojob.h
        photojob.h
        noteslistjob.h
        notejob.h
        noteaddjob.h
        allmessageslistjob.h
        messageslistjob.h
        discussionslistjob.h
        getvariablejob.h
        deletealbumjob.h
        cidsnamesjob.h
        getinfojob.h
        photolistjob.h
        groupinfo.h
        grouplistjob.h
        apppermissions.h
        vkapi.h
        DESTINATION ${INCLUDE_INSTALL_DIR}/libkvkontakte COMPONENT Devel)

endif()
