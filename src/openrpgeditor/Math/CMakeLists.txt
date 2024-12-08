add_library(
  oremath STATIC
  OREMath/Color.cpp
  OREMath/Color.hpp
  OREMath/GenericMatrix.hpp
  OREMath/Matrix4x4.cpp
  OREMath/Matrix4x4.hpp
  OREMath/Point.cpp
  OREMath/Point.hpp
  OREMath/Rect.cpp
  OREMath/Rect.hpp
  OREMath/Size.cpp
  OREMath/Size.hpp
  OREMath/Vector2D.cpp
  OREMath/Vector2D.hpp
  OREMath/Vector3D.cpp
  OREMath/Vector3D.hpp
  OREMath/Vector4D.cpp
  OREMath/Vector4D.hpp)

target_link_libraries(oremath PRIVATE imgui-setup)
target_include_directories(oremath PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
