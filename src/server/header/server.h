#ifndef SIMPLESERVER_HEADER_SERVER_H_
#define SIMPLESERVER_HEADER_SERVER_H_

#include <atomic>
#include <vector>
#include <utility>
#include <boost/asio.hpp>

#include "bookstore_database.h"

class Server {
 private:
  boost::asio::io_context io_context_;
  boost::asio::ip::tcp::acceptor acceptor_;
  BookstoreDatabase& db_;

  std::atomic<bool> stop_{false};
  boost::asio::thread_pool pool_;

  void begin_accept_connections();
  void set_up_acceptor(unsigned short port);

 public:
  Server(unsigned short port, BookstoreDatabase& db);
  ~Server();
  void run();
  void stop();
};

#endif // SIMPLESERVER_HEADER_SERVER_H_
