add_library(i18n_dummy
        orei18n.hpp
)
target_link_libraries(i18n_dummy PUBLIC
        moloader::moloader
)
target_include_directories(i18n_dummy PUBLIC
        ${moloader_SOURCE_DIR}/include
)