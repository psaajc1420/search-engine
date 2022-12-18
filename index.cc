//
// Created by Jacob Cadena on 12/6/22.
//

#include "index.h"

void Index::Add(std::string &word, size_t id) {
  index_[word][id]++;
}

size_t Index::Size() const {
  return index_.Size();
}

void Index::Clear() {
  index_.Clear();
}

void Index::Read() {
  using std::string;
  std::ifstream ifw(filename_);

  if (!ifw) {
    std::cerr << "File not found!" << std::endl;
    exit(1);
  }

  rapidjson::Document doc_words;
  rapidjson::IStreamWrapper isw(ifw);

  doc_words.ParseStream(isw);

  for (auto &word_docs_itr: doc_words["word_docs"].GetArray()) {
    for (auto &itr_word: word_docs_itr.GetObject()) {
      std::string word = itr_word.name.GetString();
      for (auto &doc_obj_itr: itr_word.value.GetArray()) {
        for (auto &itr_doc: doc_obj_itr.GetObject()) {
          std::string doc_id_str = itr_doc.name.GetString();
          std::string doc_occurrence_str = itr_doc.value.GetString();
          int id = std::stoi(doc_id_str);
          int occurrence = std::stoi(doc_occurrence_str);
          index_[word][id] = occurrence;
        }
      }
    }
  }
  ifw.close();
}

void Index::Write() {
  using rapidjson::OStreamWrapper;
  using rapidjson::Writer;

  std::ofstream ofw(filename_);

  if (!ofw) {
    std::cerr << "File not found!" << std::endl;
    exit(1);
  }

  OStreamWrapper osw(ofw);
  Writer<OStreamWrapper> writer(osw);
  writer.StartObject();
  writer.Key("word_docs");
  writer.StartArray();
  for (auto it = index_.Begin(); it != index_.End(); ++it) {
    writer.StartObject();
    writer.Key(it->first.c_str());
    writer.StartArray();
    for (auto docs_it = it->second.Begin(); docs_it != it->second.End(); ++docs_it) {
      std::string doc_id = std::to_string(docs_it->first);
      std::string occurrence = std::to_string(docs_it->second);
      writer.StartObject();
      writer.Key(doc_id.c_str());
      writer.String(occurrence.c_str());
      writer.EndObject();
    }
    writer.EndArray();
    writer.EndObject();
  }
  writer.EndArray();
  writer.EndObject();
  ofw.close();

}

Index::InvertedMapIterator Index::Find(const std::string &word) const {
  return index_.Find(word);
}

Index::ConstInvertedMap &Index::GetMap() const {
  return index_;
}


