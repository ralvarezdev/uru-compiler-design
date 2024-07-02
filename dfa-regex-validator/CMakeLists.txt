cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(DFARegexValidator LANGUAGES CXX)

add_executable(${PROJECT_NAME} ./src/main.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/digits)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/operators)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/floats)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/letters)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/integers)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/words)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/identifiers)

add_library(exceptions SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/../exceptions/expressionException.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../exceptions/expressionException.cpp
)

# Link Libraries
target_link_libraries(${PROJECT_NAME}
        PRIVATE exceptions
        PRIVATE validators
        PRIVATE digits
        PRIVATE operators
        PRIVATE floats
        PRIVATE letters
        PRIVATE integers
        PRIVATE words
        PRIVATE identifiers
)
