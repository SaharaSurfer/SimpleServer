#include "request_queue.h"

#include <condition_variable>
#include <queue>
#include <mutex>

#include "request.h"

void RequestQueue::push(Request request) {
  {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(std::move(request));
  }

  // Notify one waiting thread that a new request is available
  cond_var_.notify_one();
}
