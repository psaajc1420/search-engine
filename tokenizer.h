//
// Created by jacob on 11/28/21.
//

#ifndef SEARCH_ENGINE__TOKENIZER_H_
#define SEARCH_ENGINE__TOKENIZER_H_

#include <regex>
#include <vector>

class Tokenizer {
 public:
  void Tokenize();
  void StartQuotes();
  void EndQuotes();
  void Punctuation();

 private:


};

#endif //SEARCH_ENGINE__TOKENIZER_H_
