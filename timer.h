//
// Created by Jacob Cadena on 12/13/22.
//

#ifndef SEARCH_ENGINE__TIMER_H_
#define SEARCH_ENGINE__TIMER_H_

#include <chrono>
#include <ctime>

using std::chrono::time_point;
using std::chrono::system_clock;
using std::chrono::duration_cast;
using std::chrono::nanoseconds;

class Timer {
 public:
  Timer() : is_running_{false} {}
  void Start();
  void Stop();
  double ElapsedNanoSeconds();
  double ElapsedSeconds();
 private:
  time_point<system_clock> start_time_;
  time_point<system_clock> end_time_;
  bool is_running_;
};

#endif //SEARCH_ENGINE__TIMER_H_
