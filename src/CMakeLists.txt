cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(lox.cpp)

add_executable(the_executable
    #ASTPrinter.cpp
    #Environment.cpp
    Expr.cpp
    #GenerateAST.cpp
    #Interpreter.cpp
    Lox.cpp
    #LoxCallable.cpp
    #LoxClass.cpp
    #LoxFunction.cpp
    #LoxInstance.cpp
    #Parser.cpp
    #Resolver.cpp
    #Return.cpp
    #RuntimeError.cpp
    #Scanner.cpp
    #Stmt.cpp
    Token.cpp
    main.cpp)

target_link_libraries(the_executable
    fmtd)
