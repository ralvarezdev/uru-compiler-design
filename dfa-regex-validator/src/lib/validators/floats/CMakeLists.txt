add_library(floats STATIC
        floats.cpp
        floats.hpp)

target_include_directories(floats PRIVATE ${CMAKE_CURRENT_LIST_DIR})