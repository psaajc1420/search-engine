#include "map.h"
#include "parser.h"

#include <string>


int main(int argc, char* argv[]) {
  Parser parser;
  parser.ReadStopWords("");
  parser.Traverse("/Users/jcadena/data/2018_01_112b52537b67659ad3609a234388c50a");

//  auto it = parser.word_articles_map.Find("accounts");
//  std::cout << it->first << std::endl;
  auto it = parser.word_articles_map.find("accounts");
  std::cout << it->first << std::endl;

  return 0;
}
