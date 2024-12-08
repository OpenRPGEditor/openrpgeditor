# Test cases

add_executable(ResourcesTest $<TARGET_OBJECTS:TestRunner> Resources.spec.cpp)
add_test(NAME ResourcesTest COMMAND ResourcesTest)
target_link_libraries(ResourcesTest PRIVATE doctest Core)
