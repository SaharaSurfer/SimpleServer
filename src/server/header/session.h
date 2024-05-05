#ifndef SIMPLESERVER_HEADER_SESSION_H_
#define SIMPLESERVER_HEADER_SESSION_H_

#include <string>

#include <boost/asio.hpp>

#include "bookstore_database.h"

using namespace boost::asio;

class Session {
 private:
  ip::tcp::socket socket_;
  streambuf data_;
  BookstoreDatabase& database_;

  void receive_request();
  void handle_request();

 public:
  Session(ip::tcp::socket socket, BookstoreDatabase& database)  
    : socket_(std::move(socket)), database_(database) {}

  void start() { receive_request(); };
};

#endif // SIMPLESERVER_HEADER_SESSION_H_