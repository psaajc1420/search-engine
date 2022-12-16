//
// Created by Jacob Cadena on 12/14/22.
//

#ifndef SEARCH_ENGINE_QUERY_PROCESSOR_H_
#define SEARCH_ENGINE_QUERY_PROCESSOR_H_

#include "document.h"
#include "index_handler.h"
#include "parser.h"

#include <iostream>
#include <queue>
#include <set>
#include <string>
#include <sstream>
#include <utility>
#include <vector>

#include "porter2_stemmer/porter2_stemmer.h"


class QueryProcessor {
 public:
  QueryProcessor() : index_handler_{nullptr} {}
  explicit QueryProcessor(IndexHandler* index_handler,
                          std::string stop_words_filename = "stop_words.txt")
      : index_handler_{index_handler}, stop_words_filename_{std::move(stop_words_filename)} {}
  void Run();
 private:
  void Parse(std::string&);
  void Execute();
  void DisplayResults();
  void AndQuery(std::string&, const std::string&);
  void NotQuery(std::string&);
  static std::string Enter();

  std::string stop_words_filename_;
  std::queue<std::string> parsed_query_;
  std::vector<Document> results_;
  IndexHandler *index_handler_;
};

#endif //SEARCH_ENGINE_QUERY_PROCESSOR_H_
