#include "map.h"
#include "parser.h"

#include <string>
#include <ctime>
#include <cstdlib>

int main(int argc, char *argv[]) {

  Parser parser;
  parser.ReadStopWords("");
  parser.Traverse("/home/jacob/data");


  auto it = parser.word_articles_map.Find("zurich");

  for (auto vec_it = (*it).begin(); vec_it != (*it).end(); ++vec_it) {
    std::cout << *vec_it << std::endl;
  }

  return 0;
}
