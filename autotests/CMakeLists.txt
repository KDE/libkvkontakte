include(ECMAddTests)

find_package(Qt5Test ${REQUIRED_QT_VERSION} CONFIG QUIET)

if(NOT Qt5Test_FOUND)
    message(STATUS "Qt5Test not found, autotests will not be built.")
    return()
endif()

add_definitions(-DAUTOTESTS_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
add_definitions(-DAUTOTESTS_API_TOKEN_PATH="${CMAKE_CURRENT_SOURCE_DIR}/api-token.txt")

include_directories(${CMAKE_SOURCE_DIR}/libkvkontakte)

add_library(testhelper STATIC vktestbase.cpp)
target_link_libraries(testhelper Qt5::Core Qt5::Test KF5::Vkontakte)

function(add_vk_test name)
    ecm_add_test(
        ${name}.cpp
        LINK_LIBRARIES testhelper Qt5::Test KF5::Vkontakte
    )
endfunction(add_vk_test)

add_vk_test(test_userinfo)
# add_vk_test(test_albums)
# add_vk_test(test_notes)
# add_vk_test(test_database)
# add_vk_test(test_loginproperties)
# add_vk_test(test_friends)
# add_vk_test(test_messages)
# add_vk_test(test_photos)
add_vk_test(test_frequentrequests)
