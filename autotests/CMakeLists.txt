function(add_vk_test name)
    set(test_SRCS ${name}.cpp vktestbase.cpp vkapi.cpp)
    kde4_add_unit_test(${name} TEST ${test_SRCS})
    target_link_libraries(${name}
        kvkontakte
        ${KDE4_KDECORE_LIBS}
        ${QT_QTTEST_LIBRARY}
        ${QT_QTGUI_LIBRARY}
        ${QT_QTCORE_LIBRARY}
    )
endfunction(add_vk_test)

set(AUTOTESTS_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/data")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/autotestsconfig.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/autotestsconfig.h"
)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_vk_test(test_userinfo)
add_vk_test(test_albums)
add_vk_test(test_notes)
add_vk_test(test_database)
add_vk_test(test_loginproperties)
add_vk_test(test_friends)
add_vk_test(test_messages)
add_vk_test(test_photos)
