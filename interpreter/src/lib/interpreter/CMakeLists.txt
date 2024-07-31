add_library(interpreter STATIC
        syntax_tree_interpreter.cpp
        syntax_tree_interpreter.hpp
)

target_include_directories(interpreter PRIVATE ${CMAKE_CURRENT_LIST_DIR})