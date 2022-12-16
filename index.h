//
// Created by Jacob Cadena on 12/6/22.
//

#ifndef SEARCH_ENGINE_INDEX_H_
#define SEARCH_ENGINE_INDEX_H_

#include "map.h"
#include "unordered_map.h"

#include <iostream>
#include <fstream>
#include <string>
#include <utility>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"

class Index {

  using InvertedMap = Map<std::string, UnorderedMap<int, int>>;
  using InvertedMapIterator = Map<std::string, UnorderedMap<int, int>>::ConstIterator;
  using ConstInvertedMap = const Map<std::string, UnorderedMap<int, int>>;
 public:
  Index() = default;
  explicit Index(std::string filename) : filename_{std::move(filename)} {}
  void Add(std::string&, int);
  void Read();
  void Write();
  void Clear();
  [[nodiscard]] ConstInvertedMap& GetMap() const;
  [[nodiscard]] InvertedMapIterator Find(const std::string&) const;
  [[nodiscard]] size_t Size() const;
 private:
  std::string filename_;
  InvertedMap index_;
};

#endif //SEARCH_ENGINE_INDEX_H_
