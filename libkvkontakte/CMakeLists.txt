set(libkvkontakte_SRCS
    vkapi.cpp
    getapplicationpermissionsjob.cpp
    authenticationdialog.cpp
    util.cpp

    vkontaktejobs.cpp

    userinfo.cpp
    userinfojob.cpp

    albuminfo.cpp
    albumlistjob.cpp
    createalbumjob.cpp
    editalbumjob.cpp
    deletealbumjob.cpp

    photoinfo.cpp
    photolistjob.cpp
    photojob.cpp
    getphotouploadserverjob.cpp
    photopostjob.cpp
    savephotojob.cpp
    uploadphotosjob.cpp

#     friendlistjob.cpp
#     noteslistjob.cpp
#     allnoteslistjob.cpp
#     notejob.cpp
#     getvariablejob.cpp
#     userinfofulljob.cpp
#     cidsnamesjob.cpp
#     getinfojob.cpp
#     messageinfo.cpp
#     discussionslistjob.cpp
#     messageslistjob.cpp
#     noteinfo.cpp
#     noteaddjob.cpp
#     allmessageslistjob.cpp
#     groupinfo.cpp
#     grouplistjob.cpp
)

add_library(KF5Vkontakte SHARED ${libkvkontakte_SRCS})
generate_export_header(KF5Vkontakte
    BASE_NAME LibKVkontakte
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/libkvkontakte_export.h
)
add_library(KF5::Vkontakte ALIAS KF5Vkontakte)

target_include_directories(KF5Vkontakte
    PUBLIC "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>")
target_include_directories(KF5Vkontakte
    INTERFACE "$<INSTALL_INTERFACE:${KF5_INCLUDE_INSTALL_DIR}/Vkontakte>")

set(CMAKE_INSTALL_NAME_DIR ${LIB_INSTALL_DIR})

target_link_libraries(KF5Vkontakte
    PRIVATE
        KF5::I18n
        KF5::KIOCore
        KF5::WebKit
        KF5::WidgetsAddons
    PUBLIC
        Qt5::Core
        Qt5::Gui
        KF5::CoreAddons
)

set_target_properties(KF5Vkontakte PROPERTIES
    VERSION ${LIBKVKONTAKTE_VERSION_STRING}
    SOVERSION ${LIBKVKONTAKTE_SOVERSION}
    EXPORT_NAME Vkontakte
)
install(TARGETS KF5Vkontakte
    EXPORT KF5VkontakteTargets
    ${KF5_INSTALL_TARGETS_DEFAULT_ARGS}
)


if (NOT DIGIKAMSC_COMPILE_LIBKVKONTAKTE)

    install(FILES
        vkapi.h
        apppermissions.h

        vkontaktejobs.h

        userinfo.h
        userinfojob.h

        albuminfo.h
        albumlistjob.h
        createalbumjob.h
        editalbumjob.h
        deletealbumjob.h

        photoinfo.h
        uploadphotosjob.h

#         getapplicationpermissionsjob.h
#         authenticationdialog.h
#         messageinfo.h
#         noteinfo.h
#         friendlistjob.h
#         allnoteslistjob.h
#         userinfofulljob.h
#         photojob.h
#         noteslistjob.h
#         notejob.h
#         noteaddjob.h
#         allmessageslistjob.h
#         messageslistjob.h
#         discussionslistjob.h
#         getvariablejob.h
#         cidsnamesjob.h
#         getinfojob.h
#         photolistjob.h
#         groupinfo.h
#         grouplistjob.h
        ${CMAKE_CURRENT_BINARY_DIR}/libkvkontakte_export.h
        DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/Vkontakte/libkvkontakte COMPONENT Devel
    )

endif()
