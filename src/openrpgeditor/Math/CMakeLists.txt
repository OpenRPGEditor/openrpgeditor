add_library(oremath STATIC
        OREMath/Point.hpp OREMath/Point.cpp
        OREMath/Rect.hpp OREMath/Rect.cpp
        OREMath/Color.hpp OREMath/Color.cpp
        OREMath/Size.hpp OREMath/Size.cpp
)

target_link_libraries(oremath PRIVATE imgui-setup)
target_include_directories(oremath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})