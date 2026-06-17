project(imgui-texteditor-setup LANGUAGES CXX)

add_library(imgui-texteditor-setup STATIC
        ${imguicolortextedit_SOURCE_DIR}/TextEditor.cpp)

target_include_directories(imgui-texteditor-setup PUBLIC ${imguicolortextedit_SOURCE_DIR})
target_link_libraries(imgui-texteditor-setup PUBLIC imgui-setup)
target_compile_features(imgui-texteditor-setup PRIVATE cxx_std_17)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GCC" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" OR CMAKE_CXX_COMPILER_ID STREQUAL "CLANG")
    target_compile_options(imgui-texteditor-setup PRIVATE -Wno-error)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(imgui-texteditor-setup PRIVATE /WX-)
endif ()
