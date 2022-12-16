//
// Created by Jacob Cadena on 12/6/22.
//

#ifndef SEARCH_ENGINE_INTERFACE_H_
#define SEARCH_ENGINE_INTERFACE_H_

#include "index_handler.h"
#include "parser.h"
#include "query_processor.h"
#include "timer.h"

#include <iostream>
#include <vector>

class Interface {

  struct TimingInfo {
    TimingInfo() = default;
    double parse_time_;
    double query_time_;
    double persistence_time_;
  };
 public:
  Interface() : index_handler_{nullptr}, timing_info_{{}} {}
  ~Interface() { delete index_handler_; }
  explicit Interface(std::vector<std::string> &filenames)
      : index_handler_{nullptr}, filenames_{filenames}, timing_info_{{}} {}
  void Run();
 private:
  static int Menu();
  void CreateIndex();
  void ReadPersistence();
  void WritePersistence();
  void Query();
  void GetStats();
  void ClearIndex();

  IndexHandler* index_handler_;
  std::vector<std::string> filenames_;
  std::vector<std::string> keys_;
  Timer timer_;
  TimingInfo timing_info_;
};

#endif //SEARCH_ENGINE_INTERFACE_H_
