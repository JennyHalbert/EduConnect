cmake_minimum_required(VERSION 4.0)
project(EduConnect)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(EduConnect
        completed_alg.cpp
        Request.cpp
        Request.h
        Student.cpp
        Student.h
        Tutor.cpp
        Tutor.h
        User.cpp
        User.h)
