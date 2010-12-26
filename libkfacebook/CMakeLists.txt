project(libkfacebook)

set(libkfacebook_SRCS
  facebook.cpp
)

kde4_add_library(libkfacebook SHARED ${libkfacebook_SRCS})

target_link_libraries(libkfacebook
  ${KDE4_KDECORE_LIBS}
)

set_target_properties(libkfacebook PROPERTIES VERSION ${GENERIC_LIB_VERSION} SOVERSION ${GENERIC_LIB_SOVERSION})
install(TARGETS libkfacebook ${INSTALL_TARGETS_DEFAULT_ARGS})
