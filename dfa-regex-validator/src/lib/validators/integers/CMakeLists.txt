add_library(integers STATIC
        integers.cpp
        integers.hpp)

target_include_directories(integers PRIVATE ${CMAKE_CURRENT_LIST_DIR})