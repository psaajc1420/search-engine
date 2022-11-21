//
// Created by jacob on 11/28/21.
//

#include "tokenizer.h"

void Tokenizer::Split(const char *s, const std::regex &r, std::vector<std::string> &v)
{
  auto rit = std::cregex_token_iterator(s, s + std::strlen(s), r, -1);
  auto rend = std::cregex_token_iterator();
  v.clear();
  while(rit != rend)
  {
    v.push_back(*rit);
    ++rit;
  }
}

void Tokenizer::WordTokenize(std::string &text) {
  std::vector<std::string> tokens;
  StartQuotes(text);
  EndQuotes(text);
  text = std::move(std::regex_replace(text,
                                      std::regex(R"prefix([^A-Za-z']+)prefix"),
                                      R"prefix( "" )prefix"));
  for (char & i : text) {
    if (std::isalpha(i)) {
      i = static_cast<char>(std::tolower(i));
    }
  }
}

std::string Tokenizer::RegexTokenize(std::string &text) {

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
