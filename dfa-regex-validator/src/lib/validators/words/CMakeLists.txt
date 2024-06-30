add_library(words STATIC
        words.cpp
        words.hpp)

target_include_directories(words PRIVATE ${CMAKE_CURRENT_LIST_DIR})