add_library(reserved_words SHARED
        reserved_words.cpp
        reserved_words.hpp)

target_include_directories(reserved_words PRIVATE ${CMAKE_CURRENT_LIST_DIR})