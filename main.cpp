#include "map.h"
#include "parser.h"

#include <string>
#include <ctime>
#include <cstdlib>


int main(int argc, char* argv[]) {

  srand(time(nullptr));
  Map<int, int> test_map;

//  test_map.Insert(1);
//  test_map.Insert(2);
//  test_map.Insert(3);
//  test_map.Insert(3);
//  test_map.Insert(4);
//  test_map.Insert(2);
//  test_map.Insert(2);
//  test_map.Insert(1);


  for (int i = 0; i < 1000; i++) {
    int random_num = rand() % 100000;
    test_map.Insert(random_num);
  }

  test_map.Inorder();
//  Parser parser;
//  parser.ReadStopWords("");

//  parser.Traverse("/home/jacob/data/");

//  parser.Traverse("/home/jacob/data/2018_01_112b52537b67659ad3609a234388c50a");
//
//  parser.word_articles_map.Inorder();
//
//  auto it = parser.word_articles_map.Find("accounts");
//  std::cout << it->first << std::endl;
//
//  for (auto vec_it = it->second.begin(); vec_it != it->second.end(); ++vec_it) {
//    std::cout << *vec_it << std::endl;
//  }

  return 0;
}
