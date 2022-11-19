//
// Created by Jacob Cadena on 11/20/21.
//

#ifndef SEARCH_ENGINE_PARSER_H_
#define SEARCH_ENGINE_PARSER_H_

#include "map.h"
#include "tokenizer.h"
#include "unordered_map.h"

#include <cstdio>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>
#include <unordered_set>
#include <vector>

#include "porter2_stemmer/porter2_stemmer.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

class Parser {

 public:
  Parser() {
    id_to_filename_map_ = UnorderedMap<int, std::string>(500000);
  }
  void Traverse(const std::string &);
  void OpenStream(const std::string &, int);
  void Parse(std::string &, int);
  void ReadStopWords(const std::string &);

  Map<std::string, std::vector<int>> word_articles_map_;
  UnorderedMap<int, std::string> id_to_filename_map_;
 private:
  std::unordered_set<std::string> stop_words_;
  constexpr static char delimiter_[] = " ";
  Tokenizer tokenizer_;

  int num_words{0};

};



#endif //SEARCH_ENGINE__PARSER_H_
