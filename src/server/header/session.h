#ifndef SIMPLESERVER_HEADER_SESSION_H_
#define SIMPLESERVER_HEADER_SESSION_H_

#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <utility>
#include <boost/asio.hpp>

#include "request_queue.h"
#include "handlers_includer.h"
#include "bookstore_database.h"
#include "request_handler_interaface.h"

typedef std::unique_ptr<RequestHandlerInterface> request_ptr;

class Session : public std::enable_shared_from_this<Session> {
 private:
  boost::asio::ip::tcp::socket socket_;
  BookstoreDatabase& db_;
  RequestQueue& request_queue_;
  std::unordered_map<std::string, request_ptr> handlers_;
  
  int user_id_ = -1;

  void bind_handlers();
  std::vector<std::string> break_by_spaces(const std::string& data);

 public:
  Session(boost::asio::ip::tcp::socket socket, BookstoreDatabase& db, RequestQueue& queue)
    : socket_(std::move(socket)), db_(db), request_queue_(queue) {};

  void send_welcome_message();
  std::vector<std::string> read_request();
  void process_request(const std::string& type, const std::vector<std::string>& params);

  void start();
  void stop();
};

#endif // SIMPLESERVER_HEADER_SESSION_H_
