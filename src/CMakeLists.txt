set(vk_SRCS
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

add_library(KF5Vkontakte SHARED ${vk_SRCS})
add_library(KF5::Vkontakte ALIAS KF5Vkontakte)

ecm_generate_headers(vk_CAMELCASE_HEADERS
    HEADER_NAMES
        VkApi
        AppPermissions

        UserInfo
        UserInfoJob

        AlbumInfo
        AlbumListJob
        CreateAlbumJob
        EditAlbumJob
        DeleteAlbumJob

        PhotoInfo
        UploadPhotosJob


    PREFIX           Vkontakte
    REQUIRED_HEADERS vk_HEADERS
)

generate_export_header(KF5Vkontakte
    BASE_NAME LibKVkontakte
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/libkvkontakte_export.h
)

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
        ${vk_HEADERS}
        ${vk_CAMELCASE_HEADERS}
        vkontaktejobs.h

        ${CMAKE_CURRENT_BINARY_DIR}/libkvkontakte_export.h
        DESTINATION ${KF5_INCLUDE_INSTALL_DIR}/Vkontakte COMPONENT Devel
    )

endif()
