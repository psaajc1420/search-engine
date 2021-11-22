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


#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

class Parser {

 public:
  void Traverse(const std::string&);
  void OpenStream(const std::string&);
  void Parse(std::string, std::string);
  void ReadStopWords(const std::string&);
  void RemoveCharacters(char*);

  Map<std::string, std::vector<std::string>> word_articles_map;

 private:
  std::unordered_set<std::string> stop_words;
  std::ifstream ifs;
};

#endif //SEARCH_ENGINE__PARSER_H_
