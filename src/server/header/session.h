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

  void read_request();

 public:
  Session(boost::asio::ip::tcp::socket socket, BookstoreDatabase& db)
    : socket_(std::move(socket)), db_(db) {};

};

#endif // SIMPLESERVER_HEADER_SESSION_H_
