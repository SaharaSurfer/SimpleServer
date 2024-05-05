#include "header/server.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "header/bookstore_database.h"

using namespace boost::asio;

void Server::accept_connection() {
  while (true) {
    ip::tcp::socket socket = acceptor_.accept(io_context_);
    std::make_shared<Session>(std::move(socket), database_)->start();
  }
}
