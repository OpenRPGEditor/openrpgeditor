add_library(oremath STATIC
        OREMath/Point.hpp OREMath/Point.cpp
        OREMath/Rect.hpp OREMath/Rect.cpp
        OREMath/Color.hpp OREMath/Color.cpp
        OREMath/Size.hpp OREMath/Size.cpp
        OREMath/Matrix4x4.cpp
        OREMath/Matrix4x4.hpp
        OREMath/Vector4D.cpp
        OREMath/Vector4D.hpp
        OREMath/Vector2D.cpp
        OREMath/Vector2D.hpp
        OREMath/Vector3D.cpp
        OREMath/Vector3D.hpp
        OREMath/GenericMatrix.hpp
)

target_link_libraries(oremath PRIVATE imgui-setup)
target_include_directories(oremath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})