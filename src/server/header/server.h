#ifndef SIMPLESERVER_HEADER_SERVER_H_
#define SIMPLESERVER_HEADER_SERVER_H_

#include <utility>
#include <boost/asio.hpp>

#include "session.h"
#include "bookstore_database.h"

class Server {
 private:
  boost::asio::io_context io_context_;
  boost::asio::ip::tcp::acceptor acceptor_;
  BookstoreDatabase& db_;

  void begin_accept_connections();
  void set_up_acceptor(unsigned short port);

 public:
  Server(unsigned short port, BookstoreDatabase& db);
};

#endif // SIMPLESERVER_HEADER_SERVER_H_
