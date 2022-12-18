//
// Created by Jacob Cadena on 12/6/22.
//

#ifndef SEARCH_ENGINE_INDEX_HANDLER_H_
#define SEARCH_ENGINE_INDEX_HANDLER_H_

#include "document.h"
#include "index.h"
#include "map.h"
#include "unordered_map.h"

#include <string>
#include <utility>
#include <vector>

#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "rapidjson/ostreamwrapper.h"
#include "rapidjson/writer.h"

class IndexHandler {
 public:
  IndexHandler() = default;
  explicit IndexHandler(std::string document_file)
      : document_file_{std::move(document_file)},
        id_to_filename_map_{UnorderedMap<size_t, Document>(500000)} {}
  ~IndexHandler() {
    for (auto it = indexes_.Begin(); it != indexes_.End(); ++it) {
      delete it->second;
    }
  }
  void AddIndex(const std::string&, Index*);
  const Index* GetIndex(const std::string&);
  void ClearIndexes();
  void ReadIndexes();
  void WriteIndexes();
  void AddWordToIndex(const std::string&, std::string&, size_t);
  void AddDocument(size_t, Document&);
  Document GetDocument(size_t);
  [[nodiscard]] size_t GetNumDocuments() const;
  void ReadDocuments();
  void WriteDocuments();

 private:
  std::string document_file_;
  UnorderedMap<size_t, Document> id_to_filename_map_;
  UnorderedMap<std::string, Index *> indexes_;
};

#endif //SEARCH_ENGINE_INDEX_HANDLER_H_
