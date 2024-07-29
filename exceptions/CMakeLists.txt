add_library(exceptions STATIC
        expressionExceptions.hpp expression_exception.cpp)

target_include_directories(exceptions PRIVATE ${CMAKE_CURRENT_LIST_DIR})