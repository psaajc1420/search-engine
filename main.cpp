#include "map.h"
#include "parser.h"

#include <string>

int main(int argc, char *argv[]) {
  Parser parser;
  parser.ReadStopWords("");
  parser.Traverse(argv[1]);

  parser.word_articles_map_.Inorder();

  auto it = parser.word_articles_map_.Find(argv[2]);

  std::cout << parser.word_articles_map_.Size() << std::endl;

  std::vector<std::string> sorted_files((*it).size());
  std::copy((*it).begin(), (*it).end(), sorted_files.begin());
  std::sort(sorted_files.begin(), sorted_files.end());
  for (auto & filename : sorted_files) {
    std::cout << filename << std::endl;
  }

  return 0;
}
