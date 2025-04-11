project(imgui-setup LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

add_library(imgui-setup STATIC
        ${implot_SOURCE_DIR}/implot.h ${implot_SOURCE_DIR}/implot.cpp
        ${implot_SOURCE_DIR}/implot_demo.cpp
        ${implot_SOURCE_DIR}/implot_internal.h ${implot_SOURCE_DIR}/implot_items.cpp
        ${imgui_SOURCE_DIR}/imgui.cpp ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imgui_stacklayout.cpp ${imgui_SOURCE_DIR}/imgui_stacklayout.h
        ${imgui_SOURCE_DIR}/imgui_stacklayout_internal.h
        ${imgui_SOURCE_DIR}/imconfig.h ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imgui_tables.cpp ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imstb_rectpack.h ${imgui_SOURCE_DIR}/imstb_textedit.h
        ${imgui_SOURCE_DIR}/imstb_truetype.h
        ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.h ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
        ${imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.h ${imgui_SOURCE_DIR}/misc/freetype/imgui_freetype.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.h ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.h ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
)

target_include_directories(imgui-setup PUBLIC ${imgui_SOURCE_DIR} ${implot_SOURCE_DIR} "${freetype2_SOURCE_DIR}/include")
target_compile_features(imgui-setup PRIVATE cxx_std_20)
target_compile_definitions(imgui-setup PUBLIC IMGUI_HAS_STACK_LAYOUT IMGUI_DEFINE_MATH_OPERATORS IMGUI_ENABLE_FREETYPE)
target_link_libraries(imgui-setup PUBLIC SDL3::SDL3 freetype)
