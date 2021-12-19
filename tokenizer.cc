//
// Created by jacob on 11/28/21.
//

#include "tokenizer.h"

std::string Tokenizer::WordTokenize(std::string &text) {
  std::string new_text;
  for (int i = 0; i < text.length(); i++) {
    if (std::isalpha(text[i])) {
      new_text += static_cast<char>(std::tolower(text[i]));
    } else {
      if (text[i] == ' ' || text[i] == '\'') {
        new_text += text[i];
      } else {
        new_text += " ";
        new_text += text[i];
        new_text += " ";
      }
    }
  }

  return std::move(new_text);
}

std::string Tokenizer::Tokenize(std::string &text) {

  StartQuotes(text);
  Punctuation(text);

  text = std::move(std::regex_replace(text,
                                      std::regex(
                                          R"prefix([\]\[(\)\{\}\<\>])prefix"),
                                      R"prefix( $& )prefix"));

  text = std::move(std::regex_replace(text,
                                      std::regex(R"prefix(--)prefix"),
                                      R"prefix( -- )prefix"));

  text = std::move(" " + text + " ");
  EndQuotes(text);

  return text;
}

void Tokenizer::StartQuotes(std::string &text) {
  text = std::move(std::regex_replace(text,
                                      std::regex(R"prefix(([«“‘„]|[`]+))prefix"),
                                      R"prefix( $1 )prefix"));
  text = std::move(std::regex_replace(text,
                                      std::regex(R"prefix(^\")prefix"),
                                      R"prefix(``)prefix"));
  text = std::move(std::regex_replace(text,
                                      std::regex(R"prefix((``))prefix"),
                                      R"prefix( $1 )prefix"));
  text = std::move(std::regex_replace(text,
                                      std::regex(
                                          R"prefix(([ \(\[{<])(\"|\'{2}))prefix"),
                                      R"prefix($1 `` )prefix"));
//  text = std::move(std::regex_replace(text,
//                                      std::regex(
//                                          R"prefix((?i)(\')(?!re|ve|ll|m|t|s|d|n)(\w)\b)prefix"),
//                                      R"prefix($1 $2)prefix"));
}

void Tokenizer::EndQuotes(std::string &text) {

  text = std::move(std::regex_replace(text,
                                      std::regex(R"prefix(([»”’]))prefix"),
                                      R"prefix( $1 )prefix"));

  text = std::move(std::regex_replace(text,
                                      std::regex(R"prefix(")prefix"),
                                      R"prefix( '' )prefix"));

  text = std::move(std::regex_replace(text,
                                      std::regex(R"prefix((\S)(\'\'))prefix"),
                                      R"prefix($1 $2 )prefix"));

  text = std::move(std::regex_replace(text,
                                      std::regex(
                                          R"prefix(([^' ])('[sS]|'[mM]|'[dD]|') )prefix"),
                                      R"prefix($1 $2 )prefix"));

  text = std::move(std::regex_replace(text,
                                      std::regex(
                                          R"prefix(([^' ])('ll|'LL|'re|'RE|'ve|'VE|n't|N'T))prefix"),
                                      R"prefix($1 $2 )prefix"));
}

void Tokenizer::Punctuation(std::string &text) {

  text = std::move(std::regex_replace(text,
                                      std::regex(
                                          R"prefix(([^\.])(\.)([\]\)}>"\'' "»”’ " r"]*)\s*$)prefix"),
                                      R"prefix($1 $2 $3 )prefix"));

  text = std::move(std::regex_replace(text,
                                      std::regex(R"prefix(([:,])([^\d]))prefix"),
                                      R"prefix( $1 $2)prefix"));
  text = std::move(std::regex_replace(text,
                                      std::regex(R"prefix(([:,])$)prefix"),
                                      R"prefix( $1 )prefix"));

  text = std::move(std::regex_replace(text,
                                      std::regex(R"(\.{2,})"),
                                      R"prefix( $& )prefix"));

  text = std::move(std::regex_replace(text,
                                      std::regex(R"prefix([;@#$%&])prefix"),
                                      R"prefix( $& )prefix"));

  text = std::move(std::regex_replace(text,
                                      std::regex(
                                          R"prefix(([^\.])(\.)([\]\)}>"\']*)\s*$)prefix"),
                                      R"prefix($1 $2$3 )prefix"));

  text = std::move(std::regex_replace(text,
                                      std::regex(R"prefix([?!])prefix"),
                                      R"prefix( $& )prefix"));

  text = std::move(std::regex_replace(text,
                                      std::regex(R"prefix(([^'])' )prefix"),
                                      R"prefix($1 ' )prefix"));

  text = std::move(std::regex_replace(text,
                                      std::regex(R"([*])"),
                                      R"prefix( $& )prefix"));
}

std::string Tokenizer::RegexTokenize(std::string &text) {

  return std::move(
      std::regex_replace(text,
                         std::regex(R"prefix(([^a-zA-Z]+))prefix"),
                         R"prefix( $1 )prefix"));

}
