//
// Created by Jacob Cadena on 12/14/22.
//

#include "query_processor.h"

void QueryProcessor::Run() {
  std::string query = Enter();
  Parse(query);
  Execute();
  DisplayResults();
}

void QueryProcessor::Parse(std::string& query) {

  auto stop_words = Parser::ReadStopWords(stop_words_filename_);
  Tokenizer::WordTokenize(query);
  std::cout << query << std::endl;

  std::stringstream ss(query);
  std::string word;

  while (ss >> word) {
    if (stop_words.find(word) == stop_words.end())
      parsed_query_.push(word);
  }
}

std::string QueryProcessor::Enter() {
  std::cout << "Enter query: ";
  std::cin.ignore();
  std::string query;
  getline(std::cin, query);

  return query;
}

void QueryProcessor::Execute() {

  while (!parsed_query_.empty()) {
    std::string word = parsed_query_.front();
    parsed_query_.pop();

    size_t person_filename_found = word.find("PERSON:");
    size_t org_filename_found = word.find("ORG:");
    if (word.size() > 1 && word[0] == '-') {
      std::string not_word = word.substr(1);
      NotQuery(not_word);
    } else if (person_filename_found != std::string::npos) {
      std::string person = word.substr(7);
      AndQuery(person, "persons");
    } else if (org_filename_found != std::string::npos) {
      std::string person = word.substr(4);
      AndQuery(person, "orgs");
    } else {
      AndQuery(word, "words");
    }
  }
}

void QueryProcessor::DisplayResults() {
  using std::cout;
  using std::endl;
  if (results_.empty()) {
    cout << "Sorry no results found. Try a different query" << endl << endl;
  } else {
    cout << "Search Results: " << endl;
    for (int i = 0; i < 15 && i < results_.size(); i++) {
      cout << "\t" << i + 1 << ". " << results_.at(i) << endl;
    }
    cout << endl;
  }
}

void QueryProcessor::AndQuery(std::string& word, const std::string &index_key) {
  Porter2Stemmer::stem(word);
  const Index* index = index_handler_->GetIndex(index_key);
  auto word_it = index->Find(word);

  if (word_it == index->GetMap().CEnd()) {
    std::cout << "No documents found for " << word << std::endl;
  } else {
    std::vector<std::string> docs;
    for (auto it = word_it->second.CBegin(); it != word_it->second.CEnd(); ++it) {
      docs.push_back(index_handler_->GetDocument(it->first));
    }
    std::sort(docs.begin(), docs.end());

    if (results_.empty()) {
      results_ = docs;
    } else {
      std::set_intersection(results_.begin(), results_.end(),
                            docs.begin(), docs.end(), results_.begin());
    }
  }
}

void QueryProcessor::NotQuery(std::string& word) {
  Porter2Stemmer::stem(word);
  const Index* index = index_handler_->GetIndex("words");
  auto word_it = index->Find(word);

  if (word_it == index->GetMap().CEnd()) {
    std::cout << "No documents found for " << word << std::endl;
  } else {
    std::vector<std::string> docs;
    for (auto it = word_it->second.CBegin(); it != word_it->second.CEnd(); ++it) {
      docs.push_back(index_handler_->GetDocument(it->first));
    }
    std::sort(docs.begin(), docs.end());

    if (results_.empty()) {
      results_ = docs;
    } else {
      std::set_difference(results_.begin(), results_.end(),
                          docs.begin(), docs.end(), results_.begin());
    }
  }
}


