add_library(tree STATIC
        nodes.cpp
        nodes.hpp)

target_include_directories(tree PRIVATE ${CMAKE_CURRENT_LIST_DIR})