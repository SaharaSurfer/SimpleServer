#ifndef SIMPLESERVER_HEADER_SESSION_H_
#define SIMPLESERVER_HEADER_SESSION_H_

#include <utility>
#include <boost/asio.hpp>

#include "bookstore_database.h"

class Session {
 private:
  boost::asio::ip::tcp::socket socket_;
  boost::asio::streambuf input_buffer_;
  BookstoreDatabase& db_;
  
  int user_id_ = -1;
  int current_page_ = 0;

  std::vector<std::string> break_by_spaces(const std::string& data);
  std::string join_strings(const std::vector<std::string>& data);

 public:
  Session(boost::asio::ip::tcp::socket socket, BookstoreDatabase& db)
    : socket_(std::move(socket)), db_(db) {};

  void start();
  void stop();
};

#endif // SIMPLESERVER_HEADER_SESSION_H_
