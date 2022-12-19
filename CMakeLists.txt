cmake_minimum_required(VERSION 3.20)
project(search_engine)

set(CMAKE_CXX_STANDARD 17)

include_directories(lib)
include_directories(lib/porter2_stemmer)
include_directories(lib/podofo/include)

# set important directories to variables
set(search_engine_bin_dir "${CMAKE_SOURCE_DIR}/bin")
set(search_engine_build_include_dirs "${CMAKE_SOURCE_DIR}/src")

set(test_files
    src/search-engine/map_test.cc
    src/search-engine/unordered_map_test.cc
)
set(source_files
    src/search-engine/parser.cc
    src/search-engine/tokenizer.cc
    src/search-engine/interface.cc
    src/search-engine/index.cc
    src/search-engine/index_handler.cc
    src/search-engine/timer.cc
    src/search-engine/query_processor.cc
    src/search-engine/document.cc
    src/search-engine/ranking_processor.cc
)

include_directories(${search_engine_build_include_dirs})

add_library(porter_stemmer_lib STATIC lib/porter2_stemmer/porter2_stemmer.cpp)
target_include_directories(porter_stemmer_lib PUBLIC ${PROJECT_SOURCE_DIR}/lib/porter2_stemmer)
add_executable(search_engine src/search-engine/main.cc ${source_files})
target_link_libraries(search_engine PRIVATE porter_stemmer_lib)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
  ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
  EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(tests ${test_files})
target_link_libraries(tests gtest_main)

set_target_properties(tests
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${search_engine_bin_dir}"
)

set_target_properties(search_engine
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${search_engine_bin_dir}"
)

