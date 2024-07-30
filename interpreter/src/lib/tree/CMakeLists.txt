add_library(tree STATIC
        parse_tree_node.cpp
        parse_tree_node.hpp
        syntax_tree_node.cpp
        syntax_tree_node.hpp
)

target_include_directories(tree PRIVATE ${CMAKE_CURRENT_LIST_DIR})