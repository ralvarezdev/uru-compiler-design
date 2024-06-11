add_library(validators STATIC
expressions.cpp
expressions.h)

target_include_directories(validators PRIVATE ${CMAKE_CURRENT_LIST_DIR})