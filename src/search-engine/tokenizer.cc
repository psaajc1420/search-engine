//
// Created by jacob on 11/28/21.
//

#include "tokenizer.h"

void Tokenizer::WordTokenize(std::string &text) {
  text = std::regex_replace(text, std::regex(R"prefix([^\w\s]+)prefix"), R"prefix( "" )prefix");
  for (char &c: text) {
    if (isalnum(c)) {
      if (isalpha(c)) {
        c = static_cast<char>(tolower(c));
      }
    } else {
      c = ' ';
    }

  }
}

void Tokenizer::RegexTokenize(std::string &text) {

  StartQuotes(text);
  Punctuation(text);

  text = std::regex_replace(text, std::regex(R"prefix([\]\[(\)\{\}\<\>])prefix"),
                            R"prefix( $& )prefix");

  text = std::regex_replace(text, std::regex(R"prefix(--)prefix"), R"prefix( -- )prefix");

  text = " " + text + " ";
  EndQuotes(text);

}

void Tokenizer::StartQuotes(std::string &text) {
  text = std::regex_replace(text, std::regex(R"prefix(([«“‘„]|[`]+))prefix"),
                            R"prefix( $1 )prefix");
  text = std::regex_replace(text, std::regex(R"prefix(^\")prefix"),
                            R"prefix(``)prefix");
  text = std::regex_replace(text, std::regex(R"prefix((``))prefix"),
                            R"prefix( $1 )prefix");
  text = std::regex_replace(text, std::regex(R"prefix(([ \(\[{<])(\"|\'{2}))prefix"),
                            R"prefix($1 `` )prefix");
}

void Tokenizer::EndQuotes(std::string &text) {

  text = std::regex_replace(text, std::regex(R"prefix(([»”’]))prefix"),
                            R"prefix( $1 )prefix");

  text = std::regex_replace(text, std::regex(R"prefix(")prefix"),
                            R"prefix( '' )prefix");

  text = std::regex_replace(text, std::regex(R"prefix((\S)(\'\'))prefix"),
                            R"prefix($1 $2 )prefix");

  text = std::regex_replace(text, std::regex(R"prefix(([^' ])('[sS]|'[mM]|'[dD]|') )prefix"),
                            R"prefix($1 $2 )prefix");

  text = std::regex_replace(text, std::regex(R"prefix(([^' ])('ll|'LL|'re|'RE|'ve|'VE|n't|N'T))prefix"),
                            R"prefix($1 $2 )prefix");
}

void Tokenizer::Punctuation(std::string &text) {

  text = std::regex_replace(text, std::regex(R"prefix(([^\.])(\.)([\]\)}>"\'' "»”’ " r"]*)\s*$)prefix"),
                            R"prefix($1 $2 $3 )prefix");

  text = std::regex_replace(text, std::regex(R"prefix(([:,])([^\d]))prefix"),
                            R"prefix( $1 $2)prefix");

  text = std::regex_replace(text, std::regex(R"prefix(([:,])$)prefix"),
                            R"prefix( $1 )prefix");

  text = std::regex_replace(text, std::regex(R"(\.{2,})"), R"prefix( $& )prefix");

  text = std::regex_replace(text, std::regex(R"prefix([;@#$%&])prefix"),
                            R"prefix( $& )prefix");

  text = std::regex_replace(text, std::regex(R"prefix(([^\.])(\.)([\]\)}>"\']*)\s*$)prefix"),
                            R"prefix($1 $2$3 )prefix");

  text = std::regex_replace(text, std::regex(R"prefix([?!])prefix"),
                            R"prefix( $& )prefix");

  text = std::regex_replace(text, std::regex(R"prefix(([^'])' )prefix"),
                            R"prefix($1 ' )prefix");

  text = std::regex_replace(text, std::regex(R"([*])"),
                            R"prefix( $& )prefix");
}
