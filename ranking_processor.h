//
// Created by Jacob Cadena on 12/16/22.
//

#ifndef SEARCH_ENGINE_RANKING_PROCESSOR_H_
#define SEARCH_ENGINE_RANKING_PROCESSOR_H_

#include "document.h"
#include "unordered_map.h"
#include "map.h"
#include "index_handler.h"

#include <cmath>
#include <string>
#include <vector>

class RankingProcessor {
 public:
  RankingProcessor() : index_handler_{nullptr} {}
  void SetIndexHandler(IndexHandler*&);
  void AddRelevancy(const UnorderedMap<size_t, size_t>&);
  void CalculateCumulativeRelevancy(std::vector<Document>&);
  static bool RankDocumentByWeight(const Document&, const Document&);
 private:
  IndexHandler* index_handler_;
  UnorderedMap<size_t, std::vector<double>> tf_idf_matrix_;
};

#endif //SEARCH_ENGINE_RANKING_PROCESSOR_H_
