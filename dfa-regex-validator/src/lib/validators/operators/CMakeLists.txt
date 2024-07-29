add_library(operators SHARED
        operators.cpp
        operators.hpp)

target_include_directories(operators PRIVATE ${CMAKE_CURRENT_LIST_DIR})