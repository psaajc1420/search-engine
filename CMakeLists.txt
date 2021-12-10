cmake_minimum_required(VERSION 3.20)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)

include_directories(lib)
include_directories(lib/porter2_stemmer)

add_executable(search_engine
  main.cpp map.h
  parser.cc parser.h
  tokenizer.cc tokenizer.h
  lib/porter2_stemmer/porter2_stemmer.cpp
  lib/porter2_stemmer/porter2_stemmer.h
)

add_executable(test_stemmer test_stemmer.cc)