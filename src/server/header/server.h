#ifndef SIMPLESERVER_HEADER_SERVER_H_
#define SIMPLESERVER_HEADER_SERVER_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "bookstore_database.h"
#include "session.h"

using namespace boost::asio;

class Server {
 private:
  io_context io_context_;
  ip::tcp::acceptor acceptor_;
  BookstoreDatabase database_;

  void accept_connection();

 public:
  Server(int port, int num_threads, std::string db_connection_data)
      : acceptor_(io_context_, ip::tcp::endpoint(ip::tcp::v4(), port)),
        database_(db_connection_data) {
    accept_connection();
  }
};

#endif // SIMPLESERVER_HEADER_SERVER_H_
