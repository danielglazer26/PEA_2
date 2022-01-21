cmake_minimum_required(VERSION 3.20)
project(Etap2)

set(CMAKE_CXX_STANDARD 14)

add_executable(Etap2 main.cpp Source/TabuSearch.cpp Header/TabuSearch.h Source/Matrix.cpp Header/Matrix.h Source/LoadFromFile.cpp Header/LoadFromFile.h Source/SimulatedAnnealing.cpp Header/SimulatedAnnealing.h Tests/Test.cpp Tests/Test.h)
