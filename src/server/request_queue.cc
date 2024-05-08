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

bool RequestQueue::pop(Request& request) {
  std::unique_lock<std::mutex> lock(mutex_);
  
  // Wait until the queue is not empty or the stop flag is set
  cond_var_.wait(lock, [this] { return stop_ || !queue_.empty(); });
      
  if (queue_.empty() && stop_) {
    return false;
  }

  request = std::move(queue_.front());
  queue_.pop();
  return true;
}
