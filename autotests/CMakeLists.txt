set(userinfo_SRCS test_userinfo.cpp vkapi.cpp)
kde4_add_unit_test(test_userinfo TEST ${userinfo_SRCS})
target_link_libraries(test_userinfo
    kvkontakte
    ${KDE4_KDECORE_LIBS}
    ${QT_QTTEST_LIBRARY}
    ${QT_QTGUI_LIBRARY}
    ${QT_QTCORE_LIBRARY}
)
