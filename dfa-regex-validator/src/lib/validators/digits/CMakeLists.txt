add_library(digits SHARED
        digits.cpp
        digits.hpp)

target_include_directories(digits PRIVATE ${CMAKE_CURRENT_LIST_DIR})