cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(DFARegexValidator LANGUAGES CXX)

add_executable(${PROJECT_NAME} ./src/main.cpp)

add_library(exceptions SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/../exceptions/expressionException.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../exceptions/expressionException.cpp
)

add_library(validators SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/validators.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/validators.cpp
)

add_library(digits SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/digits/digits.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/digits/digits.cpp
)

add_library(operators SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/operators/operators.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/operators/operators.cpp
)

add_library(floats SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/floats/floats.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/floats/floats.cpp
)

add_library(integers SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/integers/integers.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/integers/integers.cpp
)

add_library(letters SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/letters/letters.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/letters/letters.cpp
)

add_library(words SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/words/words.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/words/words.cpp
)

add_library(identifiers SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/identifiers/identifiers.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/validators/identifiers/identifiers.cpp
)

# Link Libraries
target_link_libraries(operators
        PRIVATE validators
)

target_link_libraries(digits
        PRIVATE validators
)

target_link_libraries(integers
        PRIVATE validators
        PRIVATE digits
)

target_link_libraries(floats
        PRIVATE validators
        PRIVATE digits
        PRIVATE integers
)

target_link_libraries(letters
        PRIVATE validators
)

target_link_libraries(words
        PRIVATE validators
        PRIVATE letters
)

target_link_libraries(identifiers
        PRIVATE validators
        PRIVATE letters
        PRIVATE digits
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
