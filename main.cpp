#include "map.h"
#include "parser.h"
#include "porter2_stemmer.h"
#include "unordered_map.h"

#include <string>

int main(int argc, char *argv[]) {

  Parser parser;
  parser.ReadStopWords("");
  parser.Traverse(argv[1]);

  std::cout << parser.word_articles_map_.Size() << std::endl;

  std::string word = argv[2];
  Porter2Stemmer::stem(word);
  Porter2Stemmer::trim(word);
  auto it = parser.word_articles_map_.Find(word);

  std::cout << "Number of articles found: " << (*it).second.size() << std::endl;

  for (auto file_index : (*it).second) {
    std::cout << (*parser.id_to_word_map_.Find(file_index)).second << std::endl;
  }

  return 0;
}
