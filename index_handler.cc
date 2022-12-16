//
// Created by Jacob Cadena on 12/6/22.
//

#include "index_handler.h"

void IndexHandler::AddDocument(int id, std::string &filename) {
  id_to_filename_map_[id] = filename;
}

std::string IndexHandler::GetDocument(int id) {
  return id_to_filename_map_[id];
}

size_t IndexHandler::GetNumDocuments() const {
  return id_to_filename_map_.Size();
}

void IndexHandler::AddWordToIndex(const std::string &key, std::string &word, int id) {
  indexes_[key]->Add(word, id);
}

void IndexHandler::AddIndex(const std::string &key, Index *index) {
  indexes_[key] = index;
}

const Index *IndexHandler::GetIndex(const std::string &key) {
  return indexes_[key];
}

void IndexHandler::ClearIndexes() {
  for (auto it = indexes_.Begin(); it != indexes_.End(); ++it) {
    it->second->Clear();
  }
  id_to_filename_map_.Clear();
}

void IndexHandler::ReadIndexes() {
  for (auto it = indexes_.Begin(); it != indexes_.End(); ++it) {
    it->second->Read();
  }
}

void IndexHandler::WriteIndexes() {
  for (auto it = indexes_.Begin(); it != indexes_.End(); ++it) {
    it->second->Write();
  }
}

void IndexHandler::ReadDocuments() {
  using std::string;
  std::ifstream ifw(document_file_);

  if (!ifw) {
    std::cerr << "File not found!" << std::endl;
    exit(1);
  }

  rapidjson::Document document;
  rapidjson::IStreamWrapper isw(ifw);
  document.ParseStream(isw);

  for (auto &doc_info_itr: document["docs"].GetArray()) {
    for (auto & doc_info: doc_info_itr.GetObject()) {
      std::string doc_id_str = doc_info.name.GetString();
      std::string path = doc_info.value.GetString();
      int id = stoi(doc_id_str);
      id_to_filename_map_[id] = path;
    }
  }

  ifw.close();
}

void IndexHandler::WriteDocuments() {
  using rapidjson::OStreamWrapper;
  using rapidjson::Writer;

  std::ofstream ofw(document_file_);

  if (!ofw) {
    std::cerr << "File not found!" << std::endl;
    exit(1);
  }

  OStreamWrapper osw(ofw);
  Writer<OStreamWrapper> writer(osw);
  writer.StartObject();
  writer.Key("docs");
  writer.StartArray();
  for (auto it = id_to_filename_map_.Begin(); it != id_to_filename_map_.End(); ++it) {
    writer.StartObject();
    writer.Key(std::to_string(it->first).c_str());
    writer.String(it->second.c_str());
    writer.EndObject();
  }
  writer.EndArray();
  writer.EndObject();
  ofw.close();
}

