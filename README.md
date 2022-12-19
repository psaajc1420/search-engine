Search Engine
======================

Search Engine is an elementary implementation of a search engine using
AVL Trees and hash tables to create inverted file indices, basic query,
ranking func along with a simple gui.

.. role:: cmake(code)
:language: cmake

.. role:: cmd(code)
:language: bash

.. role:: cpp(code)
:language: cpp

Getting Started
---------------
.. code:: cpp

    #include <glog/logging.h>
    
    int main(int argc, char* argv[]) {
        // Initialize Google’s logging library.
        google::InitGoogleLogging(argv[0]);
        
        // ...
        LOG(INFO) << "Found " << num_cookies << " cookies";
    }

Building from Source
--------------------

CMake
~~~~~

search engine also supports CMake that can be used to build the project on a wide
range of platforms. If you don’t have CMake installed already, you can
download it for from CMake’s `official
website <http://www.cmake.org>`__.

CMake works by generating native makefiles or build projects that can be
used in the compiler environment of your choice. You can either build
search engine with CMake as a standalone project or it can be incorporated into
an existing CMake build for another project.

Building search engine with CMake
^^^^^^^^^^^^^^^^^^^^^^^^

When building glog as a standalone project, on Unix-like systems with
GNU Make as build tool, the typical workflow is:

1. Get the source code and change to it. e.g., cloning with git:

  .. code:: bash

     git clone https://github.com/psaajc1420/search-engine.git
     cd search-engine 
     
2. Run CMake to configure the build tree.

  .. code:: bash

     cmake -S . -B build -G "Unix Makefiles"

  CMake provides different generators, and by default will pick the most
  relevant one to your environment. If you need a specific version of Visual
  Studio, use :cmd:`cmake . -G <generator-name>`, and see :cmd:`cmake --help`
  for the available generators. Also see :cmd:`-T <toolset-name>`, which can
  be used to request the native x64 toolchain with :cmd:`-T host=x64`.

3. Afterwards, generated files can be used to compile the project.

  .. code:: bash

     cmake --build build
