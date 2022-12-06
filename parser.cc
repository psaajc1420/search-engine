//
// Created by Jacob Cadena on 11/20/21.
//

#include "parser.h"


void Parser::OpenStream(const std::string &filename, int id) {
  std::ifstream ifw(filename);
  rapidjson::Document document;
  rapidjson::IStreamWrapper isw(ifw);

  document.ParseStream(isw);
  std::string&& text = document["text"].GetString();
  Parse(text, id);
  ifw.close();
}

void Parser::Parse(std::string &text, int id) {
  std::unordered_set<std::string> seen_words(1000);

  std::string new_text = tokenizer_.WordTokenize(text);
  std::stringstream ss(new_text);

  std::string token;
  while (ss >> token) {
    Porter2Stemmer::stem(token);
    if (stop_words_.find(token) == stop_words_.end()) {
      if (seen_words.find(token) == seen_words.end())
        word_articles_map_[token].emplace_back(id);

      seen_words.insert(token);
    }
  }
}

void Parser::Traverse(const std::string &directory_path) {
  using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

  int counter = 1;
  for (auto const &dir_entry: recursive_directory_iterator{directory_path}) {
    if (counter % 10000 == 0) {
      std::cout << "Files searched: " << counter << std::endl;
    }

    if (strcmp(dir_entry.path().extension().c_str(), ".json") == 0) {
      OpenStream(dir_entry.path(), counter);
      id_to_filename_map_[counter] = dir_entry.path();
      counter++;
    }
  }
}

void Parser::ReadStopWords(const std::string &filename) {
  std::ifstream infile(filename);

  std::string line;
  while (getline(infile, line)) {
    stop_words_.insert(line);
  }
  infile.close();
}


