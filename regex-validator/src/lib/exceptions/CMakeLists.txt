add_library(exceptions STATIC
expressionsExceptions.cpp)

target_include_directories(exceptions PRIVATE ${CMAKE_CURRENT_LIST_DIR})