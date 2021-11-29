//
// Created by Jacob Cadena on 11/20/21.
//

#ifndef SEARCH_ENGINE__PARSER_H_
#define SEARCH_ENGINE__PARSER_H_

#include "map.h"
#include "tokenizer.h"

#include <codecvt>
#include <locale>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <regex>
#include <unordered_set>
#include <dirent.h>

#include "olestem/stemming/english_stem.h"

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

class Parser {

  using convert_t = std::codecvt_utf8<wchar_t>;
 public:
  inline void Traverse(const std::string &);
  inline void OpenStream(const std::string &);
  inline void Parse(std::string &, const std::string &);
  inline void ReadStopWords(const std::string &);

  static inline std::string StemWord(const std::string &);

  Map<std::string, std::vector<std::string>> word_articles_map_;
 private:
  std::unordered_set<std::string> stop_words_;
  constexpr static char delimiter_[] = " \t\r\n\v\f";
  static std::wstring_convert<convert_t, wchar_t> str_converter_;
  Tokenizer tokenizer_;
};

void Parser::OpenStream(const std::string &filename) {
  std::ifstream ifw(filename);
  rapidjson::Document document;
  rapidjson::IStreamWrapper isw(ifw);

  document.ParseStream(isw);
  std::string &&text = document["text"].GetString();
  Parse(text, filename);
  ifw.close();
}

void Parser::Parse(std::string &text, const std::string &filename) {

  std::unordered_set<std::string> seen_words;
  tokenizer_.Tokenize(text);
  char* input = text.data();
  char *token = strtok(input, delimiter_);
  while (token != nullptr) {
    if (stop_words_.find(token) == stop_words_.end()) {

      auto it = word_articles_map_.Find(token);
      if (it == word_articles_map_.End()) {
        auto token_it = word_articles_map_.Insert(token);
        (*token_it).emplace_back(filename);
      } else {
        if (seen_words.find(token) == seen_words.end()) {
          (*it).emplace_back(filename);
        }
      }
      seen_words.insert(token);
    }
    token = strtok(nullptr, delimiter_);
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

      OpenStream(dir_entry.path());
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


std::string Parser::StemWord(const std::string &token) {
  stemming::english_stem<> StemEnglish;
  const std::string& ansi_word(token);
  std::wstring word = std::move(str_converter_.from_bytes(ansi_word));
  StemEnglish(word);
  return std::move(str_converter_.to_bytes(word));
}

#endif //SEARCH_ENGINE__PARSER_H_
