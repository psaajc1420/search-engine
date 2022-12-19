//
// Created by Jacob Cadena on 11/20/21.
//

#ifndef SEARCH_ENGINE_PARSER_H_
#define SEARCH_ENGINE_PARSER_H_

#include "index_handler.h"
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
#include <utility>
#include <vector>

#include "porter2_stemmer/porter2_stemmer.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

class Parser {

 public:
  Parser() : index_handler_{nullptr}, document_id_{0}, fast_parsing_{true} {}
  explicit Parser(IndexHandler *index_handler,
                  const std::string& stop_words_filename = "stop_words.txt",
                  bool fast_parsing=true)
      : index_handler_{index_handler}, document_id_{0}, fast_parsing_{fast_parsing} {
    stop_words_ = ReadStopWords(stop_words_filename);
  }
  void Traverse(const std::string &);
  static std::unordered_set<std::string> ReadStopWords(const std::string &);

 private:
  void OpenStream(std::string &);
  size_t Parse(std::string &);

  IndexHandler *index_handler_;
  std::unordered_set<std::string> stop_words_;
  size_t document_id_;
  bool fast_parsing_;
};

#endif //SEARCH_ENGINE_PARSER_H_
