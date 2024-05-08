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

  void send_welcome_message();
  void read_request();
  void process_request();

 public:
  Session(boost::asio::ip::tcp::socket socket, BookstoreDatabase& db)
    : socket_(std::move(socket)), db_(db) {};

};

#endif // SIMPLESERVER_HEADER_SESSION_H_
