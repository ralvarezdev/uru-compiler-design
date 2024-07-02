add_library(validators STATIC
        validators.hpp
        validators.cpp)

target_include_directories(validators PRIVATE ${CMAKE_CURRENT_LIST_DIR})