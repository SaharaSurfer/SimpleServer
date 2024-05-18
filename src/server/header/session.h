#ifndef SIMPLESERVER_HEADER_SESSION_H_
#define SIMPLESERVER_HEADER_SESSION_H_

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <utility>
#include <boost/asio.hpp>

#include "handlers_includer.h"
#include "bookstore_database.h"
#include "request_handler_interaface.h"

typedef std::unique_ptr<RequestHandlerInterface> request_ptr;

class Session : public std::enable_shared_from_this<Session> {
 private:
  boost::asio::ip::tcp::socket socket_;
  boost::asio::streambuf buffer_;
  BookstoreDatabase& db_;
  std::unordered_map<std::string, request_ptr> handlers_;
  
  int user_id_ = -1;

  void bind_handlers();
  std::vector<std::string> break_by_spaces(const std::string& data);

 public:
  Session(boost::asio::io_context& io_context, BookstoreDatabase& db)
    : socket_(io_context), db_(db) {};

  ~Session() { std::cerr << "Session closed" << std::endl; }

  boost::asio::ip::tcp::socket& socket();

  void send_welcome_message();
  void do_read();
  void process_request(const std::string& type, const std::vector<std::string>& params);

  void start();
  void stop();
};

#endif // SIMPLESERVER_HEADER_SESSION_H_
