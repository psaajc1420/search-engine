cmake_minimum_required(VERSION 3.20)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)

include_directories(lib)

add_executable(search_engine main.cpp map.h parser.cc tokenizer.cc tokenizer.h)

add_executable(test_stemmer test_stemmer.cc)