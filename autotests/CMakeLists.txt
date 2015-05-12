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

add_vk_test(test_userinfo)
add_vk_test(test_albums)
add_vk_test(test_notes)
