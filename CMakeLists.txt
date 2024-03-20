cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(lox.cpp)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(LOXCPP_ROOT ${PROJECT_SOURCE_DIR})
set(LOXCPP_SRCS_DIR "${LOXCPP_ROOT}/src/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")

set(LOXCPP_SRCS)
list(APPEND LOXCPP_SRCS
    ${LOXCPP_SRCS_DIR}/ASTPrinter.cpp
    ${LOXCPP_SRCS_DIR}/Environment.cpp
    ${LOXCPP_SRCS_DIR}/Expr.cpp
    ${LOXCPP_SRCS_DIR}/GenerateAST.cpp
    ${LOXCPP_SRCS_DIR}/Interpreter.cpp
    ${LOXCPP_SRCS_DIR}/Lox.cpp
    ${LOXCPP_SRCS_DIR}/LoxClass.cpp
    ${LOXCPP_SRCS_DIR}/LoxFunction.cpp
    ${LOXCPP_SRCS_DIR}/LoxInstance.cpp
    ${LOXCPP_SRCS_DIR}/Parser.cpp
    ${LOXCPP_SRCS_DIR}/Resolver.cpp
    ${LOXCPP_SRCS_DIR}/Return.cpp
    ${LOXCPP_SRCS_DIR}/RuntimeError.cpp
    ${LOXCPP_SRCS_DIR}/Scanner.cpp
    ${LOXCPP_SRCS_DIR}/Stmt.cpp
    ${LOXCPP_SRCS_DIR}/Token.cpp
    ${LOXCPP_SRCS_DIR}/main.cpp
)

add_library(${PROJECT_NAME} SHARED ${LOXCPP_SRCS})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wno-unused-function)

target_include_directories(${PROJECT_NAME} PUBLIC ${LOXCPP_SRCS_DIR})
