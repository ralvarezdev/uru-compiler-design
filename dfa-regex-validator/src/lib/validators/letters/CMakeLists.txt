add_library(letters STATIC
        letters.cpp
        letters.hpp)

target_include_directories(letters PRIVATE ${CMAKE_CURRENT_LIST_DIR})