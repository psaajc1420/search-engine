# Search Engine

Search Engine is an elementary implementation of a search engine using
AVL trees and hash tables to create inverted file indices, basic query and
ranking functionality along with a simple GUI.

## Getting Started

### Dependencies

The dependencies for this project include [RapidJSON](https://github.com/Tencent/rapidjson), 
a library that parses out json files and convert thems to map objects,
and [Porter2_stemmer](https://github.com/smassung/porter2_stemmer) another library that implements
the Porter Stemmer algorithm for stemming words.

Using [RapidJSON](https://github.com/Tencent/rapidjson) is straightforwards as it's a
self-contained library, which means all you need to include are the header files. If you need extra guidance with [RapidJSON](https://rapidjson.org/index.html), you should take a look at
their documentation it is pretty good.

The [Porter Stemmer library](https://github.com/smassung/porter2_stemmer) is pulled from a
github repository that implements stemming instead of other algorithms such as Snowball, Paice-Husk. etc.,

## Building from Source


### CMake


search engine also supports CMake that can be used to build the project on a wide
range of platforms. If you don’t have CMake installed already, you can
download it for from CMake’s [official
website](http://www.cmake.org>)

CMake works by generating native makefiles or build projects that can be
used in the compiler environment of your choice. You can either build
search engine with CMake as a standalone project or it can be incorporated into
an existing CMake build for another project.

### Building search engine with CMake

When building search-engine as a standalone project, on Unix-like systems with
GNU Make as build tool, the typical workflow is:

1. Get the source code and change to it. e.g., cloning with git:

```bash
git clone https://github.com/psaajc1420/search-engine.git
cd search-engine 
```  

2. Create build folder and run CMake to configure the build tree.

```bash
mkdir cmake-build-release && cd cmake-build-release
cmake .. -DCMAKE_BUILD_TYPE=Release && make
```

3. Afterwards, generated files can be used to compile the project.
```bash
./search_engine stop_words.txt /path/to/corpus-data
```

## Project Structure

After building the project, the project structure should look similar
to this:
``` text
├── .gitgnore
├── CMakeLists.txt
├── CMakeLists.txt.in
├── README.md
├── bin
│   ├── docs.json
│   ├── words.json
│   ├── persons.json
│   ├── orgs.json
│   └── search_engine (executable)
├── lib
│   ├── porter2_stemmer
│   └── rapidjson
└── src
    └── main.cpp
```
