cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(Interpreter LANGUAGES CXX)

add_executable(${PROJECT_NAME} ./src/main.cpp)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib/lexical-analyzer)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src/lib/syntax-analyzer)

add_library(exceptions SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/../exceptions/expressionException.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../exceptions/expressionException.cpp
)

add_library(validators SHARED
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/validators.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/validators.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/digits/digits.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/digits/digits.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/operators/operators.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/operators/operators.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/floats/floats.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/floats/floats.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/letters/letters.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/letters/letters.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/integers/integers.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/integers/integers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/words/words.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/words/words.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/identifiers/identifiers.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/../dfa-regex-validator/src/lib/validators/identifiers/identifiers.cpp
)

# Link Libraries
target_link_libraries(${PROJECT_NAME}
        PRIVATE exceptions
        PRIVATE validators
        PRIVATE lexical_analyzer
        PRIVATE syntax_analyzer
)