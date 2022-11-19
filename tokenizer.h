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
  using convert_t = std::codecvt_utf8<wchar_t>;

 public:
  std::string RegexTokenize(std::string &);
  void WordTokenize(std::string &);
  void StartQuotes(std::string &);
  void EndQuotes(std::string &);
  void Punctuation(std::string &);

 private:
  std::wstring_convert<convert_t, wchar_t> str_converter_;
};

#endif //SEARCH_ENGINE__TOKENIZER_H_
