add_library(tokens STATIC
        tokens.cpp
        tokens.hpp)

target_include_directories(tokens PRIVATE ${CMAKE_CURRENT_LIST_DIR})