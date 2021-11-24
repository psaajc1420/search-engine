//
// Created by Jacob Cadena on 11/20/21.
//

#ifndef SEARCH_ENGINE__PARSER_H_
#define SEARCH_ENGINE__PARSER_H_

#include "map.h"

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstring>
#include <unordered_set>
#include <dirent.h>

#include "olestem/stemming/english_stem.h"

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

class Parser {

 public:
  inline void Traverse(const std::string &);
  inline void OpenStream(const std::string &);
  inline void Parse(std::string&, const std::string &);
  inline void ReadStopWords(const std::string &);
  inline void RemoveCharacters(char* token);
  static inline void Transform(char* token);
  static inline std::string StemWord(const std::string &);

  static inline std::wstring s2ws(const std::string &);
  static inline std::string ws2s(const std::wstring &);

  Map<std::string, std::vector<std::string>> word_articles_map;
//    Map<std::string, std::unordered_set<std::string>> word_articles_map;
 private:
  std::unordered_set<std::string> stop_words;
  constexpr static char delimit[] = " \t\r\n\v\f \",.;:~`''!?@#$%^&*()_+*-/\\={}[]|1234567890";

//  char readBuffer[60000];
};


void Parser::OpenStream(const std::string &filename) {
  std::ifstream ifw(filename);
//  FILE *fp = fopen(filename.c_str(), "r");
  rapidjson::Document document;
  rapidjson::IStreamWrapper isw(ifw);

//  rapidjson::FileReadStream isw(fp, readBuffer, sizeof(readBuffer));
  document.ParseStream(isw);
//  fclose(fp);
  std::string&& text = document["text"].GetString();
  Parse(text, filename);
  ifw.close();
}

void Parser::Parse(std::string& text, const std::string &filename) {

  std::unordered_set<std::string> seen_words;

  char *input = text.data();
  char *token = strtok(input, delimit);
  while (token != nullptr) {
    RemoveCharacters(token);
    if (stop_words.find(token) == stop_words.end()) {
      auto it = word_articles_map.Find(token);
      if (it == word_articles_map.End()) {
        auto token_it = word_articles_map.Insert(token);
        (*token_it).emplace_back(filename);
      } else {
        if (seen_words.find(token) == seen_words.end()) {
          (*it).emplace_back(filename);
        }
      }
      seen_words.insert(token);
    }
    token = strtok(nullptr, delimit);
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
    stop_words.insert(line);
  }

  infile.close();
}

void Parser::RemoveCharacters(char* token) {
  unsigned int i = 0, j = 0;
  while (token[i]) {
    if (std::isalpha(token[i])) {
      token[j++] = static_cast<char>(tolower(token[i]));
    }
    i++;
  }
  token[j] = '\0';
}


void Parser::Transform(char* token) {
  unsigned int i = 0, j = 0;
  while (token[i]){
    if (std::isalpha(token[i])) {
      token[j++]  = static_cast<char>(tolower(token[i]));
    } else {
      token[j++] = ' ';
    }
    i++;
  }
  token[j] = '\0';
}

std::string Parser::StemWord(const std::string &token) {
  stemming::english_stem<> StemEnglish;
  const std::string &ANSIWord(token);
  std::wstring word = s2ws(ANSIWord);
  StemEnglish(word);
  return std::move(ws2s(word));
}

std::wstring Parser::s2ws(const std::string &str) {
  using convert_typeX = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_typeX, wchar_t> converterX;

  return std::move(converterX.from_bytes(str));
}

std::string Parser::ws2s(const std::wstring &wstr) {
  using convert_typeX = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_typeX, wchar_t> converterX;

  return std::move(converterX.to_bytes(wstr));
}


#endif //SEARCH_ENGINE__PARSER_H_
