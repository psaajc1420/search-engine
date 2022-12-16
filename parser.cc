//
// Created by Jacob Cadena on 11/20/21.
//

#include "parser.h"


void Parser::OpenStream(std::string &filename, int id) {
  std::ifstream ifw(filename);
  rapidjson::Document document;
  rapidjson::IStreamWrapper isw(ifw);

  document.ParseStream(isw);
  std::string&& text = document["text"].GetString();

  index_handler_->AddDocument(counter_, filename);

  Parse(text, id);

  for (auto& m: document["entities"]["persons"].GetArray()) {
    std::string person = m["name"].GetString();
    index_handler_->AddWordToIndex("persons", person, id);
  }

  for (auto& m: document["entities"]["organizations"].GetArray()) {
    std::string org = m["name"].GetString();
    index_handler_->AddWordToIndex("orgs", org, id);
  }

  ifw.close();
}

void Parser::Parse(std::string &text, int id) {
  std::unordered_set<std::string> seen_words(1000);

  Tokenizer::WordTokenize(text);
  std::stringstream ss(text);

  std::string token;
  while (ss >> token) {
    Porter2Stemmer::stem(token);
    if (stop_words_.find(token) == stop_words_.end()) {
      if (seen_words.find(token) == seen_words.end()) {
        index_handler_->AddWordToIndex("words", token, id);
      }
      seen_words.insert(token);
    }
  }
}

void Parser::Traverse(const std::string &directory_path) {
  using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

  counter_ = 1;
  for (auto const &dir_entry: recursive_directory_iterator{directory_path}) {
    if (counter_ % 10000 == 0) {
      std::cout << "Files searched: " << counter_ << std::endl;
    }

    if (strcmp(dir_entry.path().extension().c_str(), ".json") == 0) {
      std::string filename = dir_entry.path();
      OpenStream(filename, counter_);
      counter_++;
    }
  }
}

std::unordered_set<std::string> Parser::ReadStopWords(const std::string &filename) {
  std::ifstream infile(filename);

  std::string line;
  std::unordered_set<std::string> stop_words;
  while (getline(infile, line)) {
    stop_words.insert(line);
  }
  infile.close();

  return stop_words;
}


