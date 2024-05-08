#ifndef SIMPLESERVER_HEADER_REQUESTQUEUE_H_
#define SIMPLESERVER_HEADER_REQUESTQUEUE_H_

#include <condition_variable>
#include <queue>
#include <mutex>

#include "request.h"

class RequestQueue {
 private:
  std::queue<Request> queue_;
  std::mutex mutex_;
  std::condition_variable cond_var_;
  bool stop_ = false;

 public:
  void push(Request request);
  bool pop(Request& request);
  void stop();
};

#endif // SIMPLESERVER_HEADER_REQUESTQUEUE_H_
