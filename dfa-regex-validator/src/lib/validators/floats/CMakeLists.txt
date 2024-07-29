add_library(floats SHARED
        floats.cpp
        floats.hpp)

target_include_directories(floats PRIVATE ${CMAKE_CURRENT_LIST_DIR})