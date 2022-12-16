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
  std::string RegexTokenize(std::string &);
  static void WordTokenize(std::string &);
 private:
  void StartQuotes(std::string &);
  void EndQuotes(std::string &);
  void Punctuation(std::string &);
};

#endif //SEARCH_ENGINE__TOKENIZER_H_
