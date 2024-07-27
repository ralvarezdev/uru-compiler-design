add_library(syntax_analyzer STATIC
        syntax_analyzer.cpp
        syntax_analyzer.hpp)

target_include_directories(syntax_analyzer PRIVATE ${CMAKE_CURRENT_LIST_DIR})