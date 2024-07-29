add_library(identifiers SHARED
        identifiers.cpp
        identifiers.hpp)

target_include_directories(identifiers PRIVATE ${CMAKE_CURRENT_LIST_DIR})