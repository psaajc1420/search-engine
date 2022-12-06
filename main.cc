#include "map.h"
#include "parser.h"
#include <string>

using namespace std;

int main(int argc, char *argv[]) {

  Parser parser;
  parser.ReadStopWords(argv[1]);
  parser.Traverse(argv[2]);


  std::string word = argv[2];
//  Porter2Stemmer::stem(word);
//  Porter2Stemmer::trim(word);
//  auto it = parser.word_articles_map_.Find(word);

//  std::cout << "Number of articles found: " << it->second.size() << std::endl;

//  for (auto file_index: it->second) {
//    std::cout << parser.id_to_filename_map_.Find(file_index)->second << std::endl;
//  }

//  for (auto words_it = parser.word_articles_map_.Begin();
//       words_it != parser.word_articles_map_.End(); ++words_it) {
//    cout << words_it->first << endl;
//  }

  std::cout << parser.word_articles_map_.Size() << std::endl;


  return 0;
}
