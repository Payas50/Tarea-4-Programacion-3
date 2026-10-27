cmake_minimum_required(VERSION 4.0)
project(Tarea_5)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

add_executable(Ejercicio1_Singleton
        Ejercicio1_Singleton.cpp)
target_link_libraries(Ejercicio1_Singleton
        PRIVATE Threads::Threads)

add_executable(Ejercicio2_AbstractFactory
        Ejercicio2_AbstractFactory.cpp)

add_executable(Ejercicio3_Composite
        Ejercicio3_Composite.cpp)

add_executable(Ejercicio4_Iterator
        Ejercicio4_Iterator.cpp)