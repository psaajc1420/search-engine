//
// Created by Jacob Cadena on 12/13/22.
//

#include "timer.h"

void Timer::Start() {
  start_time_ = system_clock::now();
  is_running_ = true;
}

void Timer::Stop() {
  end_time_ = system_clock::now();
  is_running_ = false;
}

double Timer::ElapsedNanoSeconds() {
  time_point<system_clock> end_time;
  end_time = is_running_ ? system_clock::now() : end_time_;
  long long time = duration_cast<nanoseconds>(end_time-start_time_).count();
  return static_cast<double>(time);
}

double Timer::ElapsedSeconds() {
  return ElapsedNanoSeconds() / 1e9;
}