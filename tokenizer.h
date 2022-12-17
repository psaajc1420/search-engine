//
// Created by jacob on 11/28/21.
//

#ifndef SEARCH_ENGINE_TOKENIZER_H_
#define SEARCH_ENGINE_TOKENIZER_H_

#include <codecvt>
#include <locale>
#include <regex>
#include <vector>
#include <sstream>

class Tokenizer {
 public:
  static void RegexTokenize(std::string &);
  static void WordTokenize(std::string &);
 private:
  static void StartQuotes(std::string &);
  static void EndQuotes(std::string &);
  static void Punctuation(std::string &);
};

#endif //SEARCH_ENGINE_TOKENIZER_H_
