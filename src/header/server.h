#ifndef SIMPLESERVER_HEADER_SERVER_H_
#define SIMPLESERVER_HEADER_SERVER_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "bookstore_database.h"

using namespace boost::asio;

class Server {
 private:
  io_context io_context_;
  ip::tcp::acceptor acceptor_;
  ip::tcp::socket socket_;
  boost::thread_group thread_pool;
  BookstoreDatabase database_{std::getenv("DATABASE_URL")};

 public:
  Server(int port, int num_threads)
      : acceptor_(io_context_, ip::tcp::endpoint(ip::tcp::v4(), port)),
        socket_(io_context_) {
    for (int i = 0; i < num_threads; ++i) {
      thread_pool.create_thread([this]() { io_context_.run(); });
    }
  }
};

#endif // SIMPLESERVER_HEADER_SERVER_H_
