function(add_vk_test name)
    set(test_SRCS ${name}.cpp vktestbase.cpp)
    kde4_add_unit_test(${name} TEST ${test_SRCS})
    target_link_libraries(${name}
        kvkontakte
        ${KDE4_KDECORE_LIBS}
        ${QT_QTTEST_LIBRARY}
        ${QT_QTGUI_LIBRARY}
        ${QT_QTCORE_LIBRARY}
    )
endfunction(add_vk_test)

add_definitions(-DAUTOTESTS_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
add_definitions(-DAUTOTESTS_API_TOKEN_PATH="${CMAKE_CURRENT_SOURCE_DIR}/api-token.txt")

add_vk_test(test_userinfo)
add_vk_test(test_albums)
add_vk_test(test_notes)
add_vk_test(test_database)
add_vk_test(test_loginproperties)
add_vk_test(test_friends)
add_vk_test(test_messages)
add_vk_test(test_photos)
add_vk_test(test_frequentrequests)
