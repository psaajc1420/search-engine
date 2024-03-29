//
// Created by Jacob Cadena on 11/20/21.
//

#include "parser.h"


void Parser::OpenStream(std::string &filename) {
  std::ifstream ifw(filename);
  rapidjson::Document document;
  rapidjson::IStreamWrapper isw(ifw);

  document.ParseStream(isw);
  std::string&& text = document["text"].GetString();
  std::string&& title = document["title"].GetString();
  std::string&& publisher = document["thread"]["site"].GetString();
  std::string&& date_published = document["published"].GetString();
  std::string&& url = document["url"].GetString();

  size_t num_words = Parse(text);
  Document doc(document_id_, title, text, publisher, date_published, filename, url, num_words);
  index_handler_->AddDocument(document_id_, doc);

  for (auto& m: document["entities"]["persons"].GetArray()) {
    std::string&& person = m["name"].GetString();
    index_handler_->AddWordToIndex("persons", person, document_id_);
  }

  for (auto& m: document["entities"]["organizations"].GetArray()) {
    std::string&& org = m["name"].GetString();
    index_handler_->AddWordToIndex("orgs", org, document_id_);
  }

  ifw.close();
}

size_t Parser::Parse(std::string &text) {

  if (fast_parsing_) {
    Tokenizer::WordTokenize(text);
  } else {
    Tokenizer::RegexTokenize(text);
  }

  std::stringstream ss(text);
  size_t num_words = 0;

  std::string token;
  while (ss >> token) {
    if (stop_words_.find(token) == stop_words_.end()) {
      Porter2Stemmer::stem(token);
      index_handler_->AddWordToIndex("words", token, document_id_);
    }
    ++num_words;
  }

  return num_words;
}

void Parser::Traverse(const std::string &directory_path) {
  using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;

  document_id_ = 1;
  for (auto const &dir_entry: recursive_directory_iterator{directory_path}) {
    if (document_id_ % 10000 == 0) {
      std::cout << "Files searched: " << document_id_ << std::endl;
    }

    if (strcmp(dir_entry.path().extension().c_str(), ".json") == 0) {
      std::string filename = dir_entry.path();
      OpenStream(filename);
      ++document_id_;
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


