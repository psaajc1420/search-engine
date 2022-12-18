//
// Created by Jacob Cadena on 12/6/22.
//

#include "index_handler.h"

void IndexHandler::AddDocument(size_t id, Document &document) {
  id_to_filename_map_[id] = document;
}

Document IndexHandler::GetDocument(size_t id) {
  return id_to_filename_map_[id];
}

size_t IndexHandler::GetNumDocuments() const {
  return id_to_filename_map_.Size();
}

void IndexHandler::AddWordToIndex(const std::string &key, std::string &word, size_t id) {
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
    for (auto &doc_info_pair: doc_info_itr.GetObject()) {
      std::string doc_id_str = doc_info_pair.name.GetString();
      int id = stoi(doc_id_str);
      Document doc;
      auto doc_info = doc_info_pair.value.GetObject();
      std::string &&text = doc_info["text"].GetString();
      std::string &&title = doc_info["title"].GetString();
      std::string &&publisher = doc_info["publisher"].GetString();
      std::string &&date_published = doc_info["date-published"].GetString();
      std::string &&file_location = doc_info["file-location"].GetString();
      std::string &&url = doc_info["url"].GetString();
      std::string &&num_words_str = doc_info["num-words"].GetString();
      int num_words = stoi(num_words_str);
      id_to_filename_map_[id] = Document(id, title, text, publisher,
                                         date_published, file_location, url, num_words);
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
    writer.StartObject();
    writer.Key("id");
    writer.String(std::to_string(it->second.GetId()).c_str());
    writer.Key("title");
    writer.String(it->second.GetTitle().c_str());
    writer.Key("text");
    writer.String(it->second.GetText().c_str());
    writer.Key("publisher");
    writer.String(it->second.GetPublisher().c_str());
    writer.Key("date-published");
    writer.String(it->second.GetDatePublished().c_str());
    writer.Key("file-location");
    writer.String(it->second.GetFileLocation().c_str());
    writer.Key("url");
    writer.String(it->second.GetUrl().c_str());
    writer.Key("num-words");
    writer.String(std::to_string(it->second.GetNumWords()).c_str());
    writer.EndObject();
    writer.EndObject();
  }
  writer.EndArray();
  writer.EndObject();
  ofw.close();
}

