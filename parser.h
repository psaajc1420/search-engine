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
  Parser() = default;
  inline void Traverse(const std::string &);
  inline void OpenStream(const std::string &, int);
  inline void Parse(std::string &, int);
  inline void ReadStopWords(const std::string &);

  Map<std::string, std::vector<int>> word_articles_map_;
  Map<int, std::string> id_to_word_map_;
 private:
  std::unordered_set<std::string> stop_words_;
  constexpr static char delimiter_[] = " ";
  Tokenizer tokenizer_;

  int num_words{0};
};

void Parser::OpenStream(const std::string &filename, int id) {
  std::ifstream ifw(filename);
  rapidjson::Document document;
  rapidjson::IStreamWrapper isw(ifw);

  document.ParseStream(isw);
  std::string &&text = document["text"].GetString();
  Parse(text, id);
  ifw.close();
}

void Parser::Parse(std::string &text, int id) {

  std::unordered_set<std::string> seen_words;
  text = tokenizer_.WordTokenize(text);

  char *input = text.data();
  char *token_c_str = strtok(input, delimiter_);
  while (token_c_str != nullptr) {
    std::string token = token_c_str;
    if (stop_words_.find(token) == stop_words_.end()) {
      Porter2Stemmer::stem(token);
      Porter2Stemmer::trim(token);

      auto it = word_articles_map_.Find(token);
      if (it == word_articles_map_.End()) {
        word_articles_map_[token] = {id};
        num_words++;
      } else {
        if (seen_words.find(token) == seen_words.end()) {
          it->second.emplace_back(id);
        }
      }
      seen_words.insert(token);
    }
    token_c_str = strtok(nullptr, delimiter_);
  }
}

void Parser::Traverse(const std::string &directory_path) {
  using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

  int counter = 1;
  for (auto const &dir_entry: recursive_directory_iterator{directory_path}) {
    if (counter % 10000 == 0) {
      std::cout << "Files searched: " << counter << std::endl;
    }

    if (strcmp(dir_entry.path().extension().c_str(), ".json") == 0) {
      OpenStream(dir_entry.path(), counter);
      id_to_word_map_[counter] = dir_entry.path();
      counter++;
    }
  }
}

void Parser::ReadStopWords(const std::string &directory) {
  std::ifstream infile("stop_words_english_unix.txt");

  std::string line;
  while (getline(infile, line)) {
    stop_words_.insert(line);
  }

  infile.close();
}

#endif //SEARCH_ENGINE__PARSER_H_
