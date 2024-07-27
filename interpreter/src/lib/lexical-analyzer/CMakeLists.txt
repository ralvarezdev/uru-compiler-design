add_library(lexical_analyzer STATIC
        lexical_analyzer.cpp
        lexical_analyzer.hpp)

target_include_directories(lexical_analyzer PRIVATE ${CMAKE_CURRENT_LIST_DIR})