//
// Created by Jacob Cadena on 12/16/22.
//

#include "ranking_processor.h"

void RankingProcessor::SetIndexHandler(IndexHandler *&index_handler) {
  index_handler_ = index_handler;
}

void RankingProcessor::AddRelevancy(const UnorderedMap<int, int> & doc_map) {
  auto total_num_documents = static_cast<double>(index_handler_->GetNumDocuments());
  auto num_documents = static_cast<double>(doc_map.Size());
  double idf = log(total_num_documents / (1.0 + num_documents)) + 1.0;
  for (auto it = doc_map.CBegin(); it != doc_map.CEnd(); ++it) {
    size_t num_words = index_handler_->GetDocument(it->first).GetNumWords();
    auto ftd = static_cast<double>(it->second);
    auto sum_ft_prime_d = static_cast<double>(num_words);
    double tf = ftd / sum_ft_prime_d;
    tf_idf_matrix_[it->first].push_back(tf*idf);
  }
}

void RankingProcessor::CalculateCumulativeRelevancy(std::vector<Document>& docs) {
  for (auto& doc : docs) {
    auto vec = tf_idf_matrix_[doc.GetId()];
    if (!vec.empty()) {
      double prod = 1.0;
      for (auto weight : vec) {
        prod *= weight;
      }
      doc.SetWeight(prod);
    }
  }
}

bool RankingProcessor::RankDocumentByWeight(const Document& lhs, const Document& rhs) {
  return lhs.GetWeight() > rhs.GetWeight();
}
