#ifndef SIMPLESERVER_HEADER_SERVER_H_
#define SIMPLESERVER_HEADER_SERVER_H_

#include <vector>
#include <thread>
#include <utility>
#include <boost/asio.hpp>

#include "request_queue.h"
#include "bookstore_database.h"

class Server {
 private:
  boost::asio::io_context io_context_;
  boost::asio::ip::tcp::acceptor acceptor_;
  BookstoreDatabase& db_;

  RequestQueue request_queue_;
  std::vector<std::thread> worker_threads_;
  bool stop_ = false;

  void begin_accept_connections();
  void set_up_acceptor(unsigned short port);
  void worker_thread();

 public:
  Server(unsigned short port, BookstoreDatabase& db);
};

#endif // SIMPLESERVER_HEADER_SERVER_H_
