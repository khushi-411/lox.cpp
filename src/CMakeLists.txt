cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(lox.cpp)

add_executable(the_executable
    Lox.cpp
    Token.cpp
    #Scanner.cpp
    #GenerateAST.cpp
    Expr.cpp
    ASTPrinter.cpp
    main.cpp)

target_link_libraries(the_executable
    fmtd)
