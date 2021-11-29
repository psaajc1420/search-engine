//
// Created by jacob on 11/28/21.
//

#include "tokenizer.h"
void Tokenizer::Tokenize(std::string &text) {
  StartQuotes(text);
}
void Tokenizer::StartQuotes(std::string &text) {
  std::regex_replace(text, std::regex(str_converter_.to_bytes(
                         L"([«“‘„]|[`]+)")),
                     R"prefix( \1 )prefix");
  std::regex_replace(text,
                     std::regex(R"prefix("^\"")prefix"),
                     R"prefix(``)prefix");
  std::regex_replace(text,
                     std::regex(R"prefix("(``)")prefix"),
                     R"prefix( \1 )prefix");
  std::regex_replace(text,
                     std::regex(R"prefix("([ ([{<])("|'{2})")prefix"),
                     R"prefix(\1 ``)prefix");
}
void Tokenizer::EndQuotes() {

}
void Tokenizer::Punctuation() {

}
