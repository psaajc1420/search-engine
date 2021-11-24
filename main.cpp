#include "map.h"
#include "parser.h"

#include <string>


int main(int argc, char *argv[]) {

  Parser parser;
  parser.ReadStopWords("");
  parser.Traverse("/home/jacob/data/2018_01_112b52537b67659ad3609a234388c50a");
//  parser.Traverse("/home/jacob/data/");
//  parser.word_articles_map.Inorder();

  auto it = parser.word_articles_map.Find("zurich");


//  auto it = parser.word_articles_map.Find("zurich");
//
  std::cout << parser.word_articles_map.Size() << std::endl;
//
  std::vector<std::string> sorted_files((*it).size());
  std::copy((*it).begin(), (*it).end(), sorted_files.begin());
  std::sort(sorted_files.begin(), sorted_files.end());
  for (auto & filename : sorted_files) {
    std::cout << filename << std::endl;
  }

  return 0;
}
